#ifndef CROSS_PRODUCT_CACHE_UTILS_H
#define CROSS_PRODUCT_CACHE_UTILS_H

#include <iostream>
#include "../gcflobdd/gcflobdd_node.h"
#include "cross_product.h"

namespace G_CFL_OBDD {

template <typename T>
class PairProductKey {

 public:
  PairProductKey(T nodeHandle1, T nodeHandle2); // Constructor
  unsigned int Hash(unsigned int modsize) const;
  PairProductKey& operator= (const PairProductKey& p);  // Overloaded assignment
  bool operator!= (const PairProductKey& p);        // Overloaded !=
  bool operator== (const PairProductKey& p) const;      // Overloaded ==
  T NodeHandle1() const { return nodeHandle1; }      // Access function
  T NodeHandle2() const { return nodeHandle2; }      // Access function
  std::ostream& print(std::ostream & out) const;
  ~PairProductKey(){}
  struct PairProductKey_Hash {
	 public:
		 size_t operator()(const PairProductKey& nh) const {
            auto h = nh;
            return h.Hash(HASHBASE);
		 }
	 };
  
  struct PairProductKey_Equal {
	 public:
		 bool operator()(const PairProductKey& nh1, const PairProductKey& nh2) const {
            auto h1 = nh1;
            auto h2 = nh2;
			      return h1 == h2;
		 }
	 };
 private:
  T nodeHandle1;
  T nodeHandle2;
  PairProductKey();                                 // Default constructor (hidden)
};

template <typename T>
std::ostream& operator<< (std::ostream & out, const PairProductKey<T> &p);

//***************************************************************
// PairProductMemo
//***************************************************************

template <typename T>
class PairProductMemo {

 public:
  PairProductMemo();                                 // Default constructor
  PairProductMemo(T nodeHandle, PairProductMapHandle pairProductMapHandle); // Constructor
  PairProductMemo& operator= (const PairProductMemo& p);  // Overloaded assignment
  bool operator!= (const PairProductMemo& p);        // Overloaded !=
  bool operator== (const PairProductMemo& p);        // Overloaded ==

  T nodeHandle;
  PairProductMapHandle pairProductMapHandle;
};

} // namespace G_CFL_OBDD

#endif // CROSS_PRODUCT_CACHE_UTILS_H