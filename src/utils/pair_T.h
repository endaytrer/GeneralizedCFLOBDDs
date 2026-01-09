#ifndef PAIR_T_GUARD
#define PAIR_T_GUARD

#include <iostream>
#include <fstream>

template <typename T1, typename T2>
class Pair_T {
 public:
  Pair_T();                              // Default constructor
  Pair_T(const T1 i1, const T2 i2);    // Constructor
  Pair_T& operator= (const Pair_T& p);  // Overloaded assignment
  bool operator!= (const Pair_T& p) const;     // Overloaded !=
  bool operator== (const Pair_T& p);     // Overloaded ==
  T1 First() const { return first; }     // Access function
  T2 Second() const { return second; }   // Access function
  struct pair_hash {
	  size_t operator()(const Pair_T<T1, T2>& p) const {
		  return 117 * (p.First().Hash(997) + 1) + p.Second().Hash(997);
	  }
  };
  struct pair_equal {
	  size_t operator()(const Pair_T<T1, T2>& p1, const Pair_T<T1, T2>& p2) const {
		  return (p1.First() == p2.First()) && (p1.Second() == p2.Second());
	  }
  };
 private:
  T1 first;
  T2 second;
};

template <typename T1, typename T2>
bool operator==(const Pair_T<T1, T2>& lhs, const Pair_T<T1, T2>& rhs);
template <typename T1, typename T2>
std::ostream& operator<< (std::ostream & out, const Pair_T<T1, T2>& p);

#endif
