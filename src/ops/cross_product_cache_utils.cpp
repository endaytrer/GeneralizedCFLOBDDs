#include "cross_product_cache_utils.h"
#include "cross_product.h"
#include <iostream>

using namespace G_CFL_OBDD;

//***************************************************************
// PairProductKey
//***************************************************************

// Constructor
template <typename T>
PairProductKey<T>::PairProductKey(T nodeHandle1, T nodeHandle2)
  :  nodeHandle1(nodeHandle1), nodeHandle2(nodeHandle2)
{
}

// Hash
template <typename T>
unsigned int PairProductKey<T>::Hash(unsigned int modsize) const
{
  unsigned int hvalue = 0;
  hvalue = (997 * nodeHandle1.Hash(modsize) + nodeHandle2.Hash(modsize)) % modsize;
  return hvalue;
}

// print
template <typename T>
std::ostream& PairProductKey<T>::print(std::ostream & out) const
{
  out << "(" << nodeHandle1 << ", " << nodeHandle2 << ")";
  return out;
}

template <typename T>
std::ostream& operator<< (std::ostream & out, const PairProductKey<T> &p)
{
  p.print(out);
  return(out);
}

template <typename T>
PairProductKey<T>& PairProductKey<T>::operator= (const PairProductKey<T>& i)
{
  if (this != &i)      // don't assign to self!
  {
    nodeHandle1 = i.nodeHandle1;
    nodeHandle2 = i.nodeHandle2;
  }
  return *this;        
}

// Overloaded !=
template <typename T>
bool PairProductKey<T>::operator!=(const PairProductKey<T>& p)
{
  return (nodeHandle1 != p.nodeHandle1) || (nodeHandle2 != p.nodeHandle2);
}

// Overloaded ==
template <typename T>
bool PairProductKey<T>::operator==(const PairProductKey<T>& p) const
{
  return (nodeHandle1 == p.nodeHandle1) && (nodeHandle2 == p.nodeHandle2);
}

//***************************************************************
// PairProductMemo
//***************************************************************

// Default constructor
template <typename T>
PairProductMemo<T>::PairProductMemo()
  : nodeHandle(T()), pairProductMapHandle(PairProductMapHandle())
{
}

// Constructor
template <typename T>
PairProductMemo<T>::PairProductMemo(T nodeHandle, PairProductMapHandle pairProductMapHandle)
  :  nodeHandle(nodeHandle), pairProductMapHandle(pairProductMapHandle)
{
}

template <typename T>
std::ostream& operator<< (std::ostream & out, const PairProductMemo<T> &p)
{
  out << "(" << p.nodeHandle << ", " << p.pairProductMapHandle << ")";
  return(out);
}

template <typename T>
PairProductMemo<T>& PairProductMemo<T>::operator= (const PairProductMemo<T>& i)
{
  if (this != &i)      // don't assign to self!
  {
    nodeHandle = i.nodeHandle;
    pairProductMapHandle = i.pairProductMapHandle;
  }
  return *this;        
}

// Overloaded !=
template <typename T>
bool PairProductMemo<T>::operator!=(const PairProductMemo<T>& p)
{
  return (nodeHandle != p.nodeHandle) || (pairProductMapHandle != p.pairProductMapHandle);
}

// Overloaded ==
template <typename T>
bool PairProductMemo<T>::operator==(const PairProductMemo<T>& p)
{
  return (nodeHandle == p.nodeHandle) && (pairProductMapHandle == p.pairProductMapHandle);
}