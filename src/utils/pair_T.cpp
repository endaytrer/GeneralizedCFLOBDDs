#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "pair_T.h"

template <typename T1, typename T2>
Pair_T<T1, T2>::Pair_T()
{
}

template <typename T1, typename T2>
Pair_T<T1, T2>::Pair_T(const T1 i1, const T2 i2)
  :  first(i1), second(i2)
{
}

template <typename T1, typename T2>
std::ostream& operator<< (std::ostream & out, const Pair_T<T1, T2> &p)
{
  out << "(" << p.First() << ", " << p.Second() << ")";
  return(out);
}

template <typename T1, typename T2>
bool operator==(const Pair_T<T1, T2>& lhs, const Pair_T<T1, T2>& rhs)
{
	return (lhs.First() == rhs.First()) && (lhs.Second() == rhs.Second());
}

template <typename T1, typename T2>
Pair_T<T1, T2>& Pair_T<T1, T2>::operator= (const Pair_T<T1, T2>& i)
{
  if (this != &i)      // don't assign to self!
  {
    first = i.first;
    second = i.second;
  }
  return *this;        
}

// Overloaded !=
template <typename T1, typename T2>
bool Pair_T<T1, T2>::operator!=(const Pair_T<T1, T2>& p) const
{
  return (first != p.first) || (second != p.second);
}

// Overloaded ==
template <typename T1, typename T2>
bool Pair_T<T1, T2>::operator==(const Pair_T<T1, T2>& p)
{
  return (first == p.first) && (second == p.second);
}
