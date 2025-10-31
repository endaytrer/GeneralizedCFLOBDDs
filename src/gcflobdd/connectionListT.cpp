#include "connectionListT.h"

namespace G_CFL_OBDD {

//***************************************************************
// ConnectionListTBody
//***************************************************************

// Constructor
template <typename T>
ConnectionListTBody<T>::ConnectionListTBody()
  : refCount(0), isCanonical(false)
{
}

template <typename T>
ConnectionListTBody<T>::ConnectionListTBody(unsigned int capacity)
	: refCount(0), isCanonical(false)
{
	connections.reserve(capacity);
}

template <typename T>
ConnectionListTBody<T>::~ConnectionListTBody()
{
}

template <typename T>
void ConnectionListTBody<T>::IncrRef()
{
  refCount++;    // Warning: Saturation not checked
}

template <typename T>
void ConnectionListTBody<T>::DecrRef()
{
  if (--refCount == 0) {    // Warning: Saturation not checked
    if (isCanonical) {
      ConnectionListTHandle<T>::canonicalConnectionListTBodySet->DeleteEq(this);
    }
    delete this;
  }
}

template <typename T>
bool ConnectionListTBody<T>::operator==(const ConnectionListTBody<T> &o) const
{
	if (connections.size() != o.connections.size())
	{
		return false;
	} else {
	  for (unsigned i = 0; i < connections.size(); i++)
	  {
		  if (connections[i] != o.connections[i])
		  {
			  return false;
		  }
	  }
	}
	return true;
}

// Overloaded []
template <typename T>
ConnectionT<T>& ConnectionListTBody<T>::operator[](unsigned int i)
{
	return connections[i];
}

template <typename T>
unsigned int ConnectionListTBody<T>::Hash(unsigned int modsize) const
{
  unsigned int hvalue = 0;
  for (unsigned int j = 0; j < connections.size(); j++) {
    hvalue = (997 * hvalue + connections[j].Hash(modsize)) % modsize;
  }
  return hvalue;
}


template <typename T>
std::ostream& operator<< (std::ostream & out, const ConnectionListTBody<T> &r)
{
  unsigned int indentation_level = 5;
  for (unsigned int i = 0; i < indentation_level; i++) {  // Indentation
      out << "  ";
  }
	out << "{CLB<T>: \n";
	size_t last = r.connections.size() - 1;
	for (size_t i = 0; i <= last; ++i) {
		out << r.connections[i];
        if (i != last) 
            out << ", ";
    }
  for (unsigned int i = 0; i < indentation_level; i++) {  // Indentation
      out << "  ";
  }
	out << " CLB<T>}\n";
    return out;
}

//***************************************************************
// ReturnMapHandle
//***************************************************************

// Initializations of static members ---------------------------------
template <typename T> Hashset<ConnectionListTBody<T>> *ConnectionListTHandle<T>::canonicalConnectionListTBodySet = new Hashset<ConnectionListTBody<T>>(HASHSET_NUM_BUCKETS);

// Default constructor
template <typename T>
ConnectionListTHandle<T>::ConnectionListTHandle()
  :  connectionList(new ConnectionListTBody<T>)
{
  connectionList->IncrRef();
}

template <typename T>
ConnectionListTHandle<T>::ConnectionListTHandle(unsigned int capacity)
	: connectionList(new ConnectionListTBody<T>(capacity))
{
	connectionList->IncrRef();
}

// Destructor
template <typename T>
ConnectionListTHandle<T>::~ConnectionListTHandle()
{
  connectionList->DecrRef();
}

// Copy constructor
template <typename T>
ConnectionListTHandle<T>::ConnectionListTHandle(const ConnectionListTHandle<T> &r)
  :  connectionList(r.connectionList)
{
  connectionList->IncrRef();
}

// Overloaded assignment
template <typename T>
ConnectionListTHandle<T>& ConnectionListTHandle<T>::operator= (const ConnectionListTHandle<T> &r)
{
  if (this != &r)      // don't assign to self!
  {
    ConnectionListTBody<T> *temp = connectionList;
    connectionList = r.connectionList;
    connectionList->IncrRef();
    temp->DecrRef();
  }
  return *this;        
}

// Overloaded !=
template <typename T>
bool ConnectionListTHandle<T>::operator!=(const ConnectionListTHandle<T> &r) const
{
  return (connectionList != r.connectionList);
}

// Overloaded ==
template <typename T>
bool ConnectionListTHandle<T>::operator==(const ConnectionListTHandle<T> &r) const
{
  return (connectionList == r.connectionList);
  // return (*connectionList == *r.connectionList);
}

// Overloaded []
template <typename T>
ConnectionT<T>& ConnectionListTHandle<T>::operator[](unsigned int i)
{
	return (*(this->connectionList))[i];
}

// print
template <typename T>
std::ostream& ConnectionListTHandle<T>::print(std::ostream & out) const
{
  out << *connectionList << std::endl;
  return out;
}

template <typename T>
std::ostream& operator<< (std::ostream & out, const ConnectionListTHandle<T> &r)
{
  r.print(out);
  return(out);
}

template <typename T>
unsigned int ConnectionListTHandle<T>::Hash(unsigned int modsize) const
{
	assert(connectionList->isCanonical);
	return ((unsigned int) reinterpret_cast<uintptr_t>(connectionList) >> 2) % modsize;
  // return connectionList->Hash(modsize);
}

template <typename T>
unsigned int ConnectionListTHandle<T>::Size()
{
	return connectionList->connections.size();
}

template <typename T>
void ConnectionListTHandle<T>::AddConnection(ConnectionT<T>& y)
{
    assert(connectionList->refCount <= 1);
    connectionList->connections.push_back(y);
}

template <typename T>
ConnectionT<T> ConnectionListTHandle<T>::Lookup(int x)
{
	return connectionList->connections[x];
}

template <typename T>
long int ConnectionListTHandle<T>::LookupInv(ConnectionT<T>& y)
{
  for (unsigned int i = 0; i < connectionList->connections.size(); i++){
      if (connectionList->connections[i] == y)
          return i;
  }
  return -1;
}

template <typename T>
void ConnectionListTHandle<T>::Canonicalize()
{ 
    ConnectionListTBody<T> *answerContents;

    if (!connectionList->isCanonical) {
        unsigned int hash = canonicalConnectionListTBodySet->GetHash(connectionList);
        answerContents = canonicalConnectionListTBodySet->Lookup(connectionList, hash);
        if (answerContents == NULL) {
            canonicalConnectionListTBodySet->Insert(connectionList, hash);
            connectionList->isCanonical = true;
        }
        else {
            answerContents->IncrRef();
            connectionList->DecrRef();
            connectionList = answerContents;
        }
    }
}

}