#ifndef REDUCTION_MAP_GUARD
#define REDUCTION_MAP_GUARD

#include <iostream>
#include <fstream>
#include <unordered_set>
#include "../utils/hashset.h"
#include "../utils/intpair.h"

class ReductionMapHandle;
class ReductionMapBody;


//***************************************************************
// ReductionMapHandle
//***************************************************************

class ReductionMapHandle {
 public:
  ReductionMapHandle();                               // Default constructor
  ~ReductionMapHandle();                              // Destructor
  ReductionMapHandle(const ReductionMapHandle &r);             // Copy constructor
  ReductionMapHandle(unsigned int capacity);
  ReductionMapHandle& operator= (const ReductionMapHandle &r); // Overloaded assignment
  bool operator!= (const ReductionMapHandle &r) const;      // Overloaded !=
  bool operator== (const ReductionMapHandle &r) const;      // Overloaded ==
  unsigned int Hash(unsigned int modsize) const;
  unsigned int Size() const;
  void AddToEnd(int y);
  int& operator[](unsigned int i);                       // Overloaded []
  int Lookup(int x) const;
  intpair Lookup(intpair& x) const;
  int LookupInv(int y) const;
  void Canonicalize();
  ReductionMapBody *mapContents;
  static Hashset<ReductionMapBody> *canonicalReductionMapBodySet;
  std::ostream& print(std::ostream & out = std::cout) const;
};

std::ostream& operator<< (std::ostream & out, const ReductionMapHandle &r);
extern std::size_t hash_value(const ReductionMapHandle& val);

//***************************************************************
// ReductionMapBody
//***************************************************************

class ReductionMapBody {//: public List<int> {

  friend void ReductionMapHandle::Canonicalize();

 public:
  ReductionMapBody();    // Constructor
  ReductionMapBody(unsigned int capacity);
  void IncrRef();
  void DecrRef();
  unsigned int Hash(unsigned int modsize) const;
  void setHashCheck();
  void AddToEnd(int y);          // Override AddToEnd
  unsigned int refCount;         // reference-count value
  bool isIdentityMap;            // Is this ReductionMapBody an identity map?
  std::vector<int> mapArray;
  bool operator==(const ReductionMapBody &o) const;
  int& operator[](unsigned int i);                       // Overloaded []
  unsigned int Size() const;

 //protected:
  unsigned int hashCheck;
  bool isCanonical;              // Is this ReductionMapBody in *canonicalReductionMapBodySet?

  struct PointerHash {
  public:
	  size_t operator()(const ReductionMapBody* r){
		  return r->hashCheck;
	  }
  };

  struct PointerEqual {
  public:
	  bool operator()(const ReductionMapBody* r1, const ReductionMapBody* r2){
		  return (r1->hashCheck == r2->hashCheck);
	  };
  };

};

std::ostream& operator<< (std::ostream & out, const ReductionMapBody &r);
//static std::unordered_set<ReductionMapBody *, ReductionMapBody::PointerHash, ReductionMapBody::PointerEqual> canonicalReductionMapBodySet;
#endif