#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "intpair.h"

intpair::intpair()
  :  first(0), second(0)
{
}

intpair::intpair(const int i1, const int i2)
  :  first(i1), second(i2)
{
}

std::ostream& operator<< (std::ostream & out, const intpair &p)
{
  out << "(" << p.First() << ", " << p.Second() << ")";
  return(out);
}

bool operator==(const intpair& lhs, const intpair& rhs)
{
	return (lhs.First() == rhs.First()) && (lhs.Second() == rhs.Second());
}

intpair& intpair::operator= (const intpair& i)
{
  if (this != &i)      // don't assign to self!
  {
    first = i.first;
    second = i.second;
  }
  return *this;        
}

intpair intpair::operator! ()
{
	int newFirst = !this->First();
	int newSecond = !this->Second();
	return intpair(newFirst,newSecond);
}

// Overloaded !=
bool intpair::operator!=(const intpair& p) const
{
  return (first != p.first) || (second != p.second);
}

// Overloaded ==
bool intpair::operator==(const intpair& p)
{
  return (first == p.first) && (second == p.second);
}
