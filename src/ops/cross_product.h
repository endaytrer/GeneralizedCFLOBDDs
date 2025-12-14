#ifndef CROSS_PRODUCT_CFL_GUARD
#define CROSS_PRODUCT_CFL_GUARD


#include "../utils/intpair.h"
#include "../gcflobdd/gcflobdd_node.h"

// ********************************************************************
// 2-Way Cross Product
// ********************************************************************

// Classes and types declared in this file ---------------------
namespace G_CFL_OBDD {
class PairProductMapHandle;
class PairProductMapBody;
// class PairProductKey;
}

//***************************************************************
// PairProductMapHandle
//***************************************************************
namespace G_CFL_OBDD {
class PairProductMapHandle {
 public:
  PairProductMapHandle();                               // Default constructor
  ~PairProductMapHandle();                              // Destructor
  PairProductMapHandle(const PairProductMapHandle &r);             // Copy constructor
  PairProductMapHandle& operator= (const PairProductMapHandle &r); // Overloaded assignment
  bool operator!= (const PairProductMapHandle &r);      // Overloaded !=
  bool operator== (const PairProductMapHandle &r);      // Overloaded ==
  unsigned int Hash(unsigned int modsize) const;
  unsigned int Size() const;
  intpair& operator[](unsigned int i);                       // Overloaded []
  void AddToEnd(const intpair& p);
  void Extend(const PairProductMapHandle& other);
  bool Member(intpair& p);
  int Lookup(intpair& p);
  void Canonicalize();
  PairProductMapHandle Flip();                          // Create map with reversed entries
  PairProductMapBody* mapContents;
};

//***************************************************************
// PairProductMapBody
//***************************************************************

class PairProductMapBody {//: public List<intpair> {

  friend void PairProductMapHandle::Canonicalize();

 public:
  PairProductMapBody();    // Constructor
  void IncrRef();
  void DecrRef();
  unsigned int Hash(unsigned int modsize) const;
  unsigned int refCount;         // reference-count value
  void setHashCheck();
  void AddToEnd(const intpair& y);          // Override AddToEnd
  std::vector<intpair> mapArray;
  bool operator==(const PairProductMapBody &p) const;
  intpair& operator[](unsigned int i);                       // Overloaded []
  unsigned int Size() const;
  unsigned int hashCheck;
 public:
  bool isCanonical;              // Is this PairProductMapBody in *canonicalPairProductMapBodySet?
    // struct PPHash {
    //     size_t operator()(const std::weak_ptr<PairProductMapBody>& p) const {
    //         return p.lock()->Hash(997);
    //     }
    // };
    // struct PPEqual {
    //     bool operator()(const std::weak_ptr<PairProductMapBody>& a, const std::weak_ptr<PairProductMapBody>& b) const {
    //         auto sp_a = a.lock();
    //         auto sp_b = b.lock();
    //         if (!sp_a || !sp_b) return false; // treat expired as unequal
    //         return *sp_a == *sp_b;
    //     }
    // };
  // static std::unordered_set<std::weak_ptr<PairProductMapBody>, PairProductMapBody::PPHash, PairProductMapBody::PPEqual> canonicalPairProductMapBodySet;
  static Hashset<PairProductMapBody> *canonicalPairProductMapBodySet;

};

std::ostream& operator<< (std::ostream & out, const PairProductMapBody &r);
}
//***************************************************************
// PairProductKey
//***************************************************************

namespace G_CFL_OBDD{

// Auxiliary functions -----------------------------------------------
G_CFLOBDDNodeHandle PairProduct(G_CFLOBDDNodeHandle n1,
                              G_CFLOBDDNodeHandle n2,
                              PairProductMapHandle &pairProductMap
                             );
G_CFLOBDDNodeHandle PairProduct(G_CFLOBDDInternalNode *n1,
                              G_CFLOBDDInternalNode *n2,
                              PairProductMapHandle &pairProductMap
                             );


void InitPairProductCache();
void DisposeOfPairProductCache();

}

#endif // CROSS_PRODUCT_CFL_GUARD
