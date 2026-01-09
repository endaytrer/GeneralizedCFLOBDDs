#include "connectionListT.h"
#include "connectionT.h"
#include <new>

namespace G_CFL_OBDD {


// Default constructor
template <typename Handle>
ConnectionListT<Handle>::ConnectionListT()
{
  numConnections = 0;
  connections = nullptr;
  currentConnectionIndex = 0;
}

template <typename Handle>
ConnectionListT<Handle>::ConnectionListT(unsigned int capacity)
{
  connections = new ConnectionT<Handle>[capacity];
  // void* raw = ::operator new[](capacity * sizeof(ConnectionT<Handle>));
  // connections = reinterpret_cast<ConnectionT<Handle>*>(raw);
  // connections.reserve(capacity);
  numConnections = capacity;
  // numConnections = 0; 
  currentConnectionIndex = 0;
}

// Copy constructor
template <typename Handle>
ConnectionListT<Handle>::ConnectionListT(const ConnectionListT<Handle> &C)
{
  if (C.numConnections == 0) {
    connections = nullptr;
    numConnections = 0;
    currentConnectionIndex = 0;
    return;
  }
  connections = new ConnectionT<Handle>[C.numConnections];
  for (unsigned int i = 0; i < C.currentConnectionIndex; i++) {
    connections[i] = C.connections[i];
  }
  numConnections = C.numConnections;
  currentConnectionIndex = C.currentConnectionIndex;
}

// Destructor
template <typename Handle>
ConnectionListT<Handle>::~ConnectionListT()
{
  delete [] connections;
  connections = nullptr;
  numConnections = 0;
  currentConnectionIndex = 0;
}

template <typename Handle>
void ConnectionListT<Handle>::Reserve(unsigned int capacity)
{
  if (connections != nullptr) {
    delete [] connections;
  }
  connections = new ConnectionT<Handle>[capacity];
  // void* raw = ::operator new[](capacity * sizeof(ConnectionT<Handle>));
  // connections = reinterpret_cast<ConnectionT<Handle>*>(raw);
  numConnections = capacity;
  currentConnectionIndex = 0;
}

// Hash
template <typename Handle>
unsigned int ConnectionListT<Handle>::Hash(unsigned int modsize) const
{
	unsigned int hvalue = 0;
	for (unsigned int i = 0; i < currentConnectionIndex; i++) {
		hvalue = (997 * connections[i].Hash(modsize) + hvalue) % modsize;
	}
	return hvalue;
}

// Overloaded =
template <typename Handle>
ConnectionListT<Handle>& ConnectionListT<Handle>::operator= (const ConnectionListT<Handle>& C)
{
	if (this != &C)      // don't assign to self!
	{
    if (C.numConnections == 0) {
      connections = nullptr;
      numConnections = 0;
      currentConnectionIndex = 0;
      return *this;
    }
    delete [] connections;
    connections = new ConnectionT<Handle>[C.numConnections];
    // void* raw = ::operator new[](C.numConnections * sizeof(ConnectionT<Handle>));
    // connections = reinterpret_cast<ConnectionT<Handle>*>(raw);
		for (unsigned int i = 0; i < C.currentConnectionIndex; i++) {
			connections[i] = C.connections[i];
		}
		numConnections = C.numConnections;
		currentConnectionIndex = C.currentConnectionIndex;
	}
	return *this;
}

// Overloaded !=
template <typename Handle>
bool ConnectionListT<Handle>::operator!= (const ConnectionListT<Handle> & C) const
{
  if (numConnections != C.numConnections) return true;
  if (currentConnectionIndex != C.currentConnectionIndex) return true;
  for (unsigned int i = 0; i < currentConnectionIndex; i++) {
    if (connections[i] != C.connections[i]) return true;
  }
	return false;
}

// Overloaded ==
template <typename Handle>
bool ConnectionListT<Handle>::operator== (const ConnectionListT<Handle> & C) const
{
  if (numConnections != C.numConnections) return false;
  if (currentConnectionIndex != C.currentConnectionIndex) return false;
  for (unsigned int i = 0; i < currentConnectionIndex; i++) {
    if (connections[i] != C.connections[i]) return false;
  }
  return true;
}

// print
template <typename Handle>
std::ostream& ConnectionListT<Handle>::print(std::ostream & out) const
{
	for (unsigned int i = 0; i < currentConnectionIndex; i++) {
		connections[i].print(out);
        out << "\n";
	}
  return out;
}

template <typename Handle>
void ConnectionListT<Handle>::AddConnection(ConnectionT<Handle>& y)
{
  connections[currentConnectionIndex] = y;
  currentConnectionIndex++;
  // connections.push_back(y);
}

template <typename Handle>
ConnectionT<Handle> ConnectionListT<Handle>::Lookup(int x)
{
  if (x < 0 || static_cast<unsigned int>(x) >= currentConnectionIndex) {
      throw std::out_of_range("Lookup index out of range");
  }
  return connections[x];
}

template <typename Handle>
long int ConnectionListT<Handle>::LookupInv(ConnectionT<Handle>& y)
{
  for (unsigned int i = 0; i < currentConnectionIndex; i++){
      if (connections[i] == y) {
          return static_cast<long int>(i);
      }
  }
  return -1; // Not found
}

template <typename Handle>
ConnectionT<Handle>& ConnectionListT<Handle>::operator[](unsigned int i)
{
    // assert(i < currentConnectionIndex);
    return connections[i];
}

template <typename Handle>
unsigned int ConnectionListT<Handle>::Size()
{
    return currentConnectionIndex;
}

template <typename Handle>	
std::ostream& operator<< (std::ostream & out, const ConnectionListT<Handle> &c) {
  c.print(out);
  return out;
}


} // namespace G_CFL_OBDD
