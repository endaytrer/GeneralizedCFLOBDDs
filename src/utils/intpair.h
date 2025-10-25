#ifndef INTPAIR_GUARD
#define INTPAIR_GUARD

#include <iostream>
#include <fstream>

class intpair {
 public:
  intpair();                              // Default constructor
  intpair(const int i1, const int i2);    // Constructor
  intpair& operator= (const intpair& p);  // Overloaded assignment
  intpair operator! ();
  bool operator!= (const intpair& p) const;     // Overloaded !=
  bool operator== (const intpair& p);     // Overloaded ==
  int First() const { return first; }     // Access function
  int Second() const { return second; }   // Access function
  struct intpair_hash {
	  size_t operator()(const intpair& p) const {
		  return 117 * (p.First() + 1) + p.Second();
	  }
  };
  struct intpair_equal {
	  size_t operator()(const intpair& p1, const intpair& p2) const {
		  return (p1.First() == p2.First()) && (p1.Second() == p2.Second());
	  }
  };
 private:
  int first;
  int second;
};

bool operator==(const intpair& lhs, const intpair& rhs);
std::ostream& operator<< (std::ostream & out, const intpair &p);

#endif
