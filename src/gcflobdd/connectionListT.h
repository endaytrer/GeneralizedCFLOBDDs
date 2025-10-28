#ifndef CONNECTIONLIST_T_H
#define CONNECTIONLIST_T_H

#include <iostream>
#include <fstream>
#include <vector>
#include "../utils/hashset.h"
#include "connectionT.h"

namespace G_CFL_OBDD {

template <typename T> class ConnectionListTHandle;
template <typename T> class ConnectionListTBody;

//***************************************************************
// ConnectionListTHandle
//***************************************************************

template <typename T>
class ConnectionListTHandle {
 public:
  ConnectionListTHandle();                               // Default constructor
  ConnectionListTHandle(unsigned int capacity);
  ~ConnectionListTHandle();                              // Destructor
  ConnectionListTHandle(const ConnectionListTHandle<T> &r);    // Copy constructor
  ConnectionListTHandle<T>& operator= (const ConnectionListTHandle<T> &r); // Overloaded assignment
  bool operator!= (const ConnectionListTHandle<T> &r) const;      // Overloaded !=
  bool operator== (const ConnectionListTHandle<T> &r) const;      // Overloaded ==
  ConnectionT<T>& operator[](unsigned int i);                       // Overloaded []
  unsigned int Hash(unsigned int modsize) const;
  unsigned int Size();
  void AddConnection(ConnectionT<T>& y);
  ConnectionT<T> Lookup(int x);
  long int LookupInv(ConnectionT<T>& y);
  void Canonicalize();
  ConnectionListTBody<T> *connectionList;
  static Hashset<ConnectionListTBody<T>> *canonicalConnectionListTBodySet;
  std::ostream& print(std::ostream & out = std::cout) const;
};

template <typename T>
std::ostream& operator<< (std::ostream & out, const ConnectionListTHandle<T> &r);

//***************************************************************
// ConnectionListTBody
//***************************************************************

template <typename T>
class ConnectionListTBody {

  friend void ConnectionListTHandle<T>::Canonicalize();
  friend unsigned int ConnectionListTHandle<T>::Hash(unsigned int modsize) const;

 public:
  ConnectionListTBody();    // Constructor
  ConnectionListTBody(unsigned int capacity);    // Constructor
  ~ConnectionListTBody();
  void IncrRef();
  void DecrRef();
  unsigned int Hash(unsigned int modsize) const;
  unsigned int refCount;         // reference-count value
  std::vector<ConnectionT<T>> connections;
  bool operator==(const ConnectionListTBody<T> &o) const;
  ConnectionT<T>& operator[](unsigned int i);                       // Overloaded []

 protected:
  bool isCanonical;              // Is this ConnectionListTBody in *canonicalConnectionListTBodySet?

};
}
#endif // CONNECTIONLIST_T_H