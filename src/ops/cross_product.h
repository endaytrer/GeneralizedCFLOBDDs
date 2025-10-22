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
  unsigned int Size();
  intpair& operator[](unsigned int i);                       // Overloaded []
  void AddToEnd(const intpair& p);
  bool Member(intpair& p);
  int Lookup(intpair& p);
  void Canonicalize();
  PairProductMapHandle Flip();                          // Create map with reversed entries
  std::shared_ptr<PairProductMapBody> mapContents;
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
  unsigned int Size();
  unsigned int hashCheck;
 public:
  bool isCanonical;              // Is this PairProductMapBody in *canonicalPairProductMapBodySet?
    struct PPHash {
        size_t operator()(const std::weak_ptr<PairProductMapBody>& p) const {
            return p.lock()->Hash(997);
        }
    };
    struct PPEqual {
        bool operator()(const std::weak_ptr<PairProductMapBody>& a, const std::weak_ptr<PairProductMapBody>& b) const {
            auto sp_a = a.lock();
            auto sp_b = b.lock();
            if (!sp_a || !sp_b) return false; // treat expired as unequal
            return *sp_a == *sp_b;
        }
    };
  static std::unordered_set<std::weak_ptr<PairProductMapBody>, PairProductMapBody::PPHash, PairProductMapBody::PPEqual> canonicalPairProductMapBodySet;

};

std::ostream& operator<< (std::ostream & out, const PairProductMapBody &r);
}
//***************************************************************
// PairProductKey
//***************************************************************

namespace G_CFL_OBDD{

class PairProductKey {

 public:
  PairProductKey(G_CFLOBDDNodeHandle nodeHandle1, G_CFLOBDDNodeHandle nodeHandle2); // Constructor
  unsigned int Hash(unsigned int modsize) const;
  PairProductKey& operator= (const PairProductKey& p);  // Overloaded assignment
  bool operator!= (const PairProductKey& p);        // Overloaded !=
  bool operator== (const PairProductKey& p) const;      // Overloaded ==
  G_CFLOBDDNodeHandle NodeHandle1() const { return nodeHandle1; }      // Access function
  G_CFLOBDDNodeHandle NodeHandle2() const { return nodeHandle2; }      // Access function
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
  G_CFLOBDDNodeHandle nodeHandle1;
  G_CFLOBDDNodeHandle nodeHandle2;
  PairProductKey();                                 // Default constructor (hidden)
};

std::ostream& operator<< (std::ostream & out, const PairProductKey &p);

//***************************************************************
// PairProductMemo
//***************************************************************

class PairProductMemo {

 public:
  PairProductMemo();                                 // Default constructor
  PairProductMemo(G_CFLOBDDNodeHandle nodeHandle, PairProductMapHandle pairProductMapHandle); // Constructor
  PairProductMemo& operator= (const PairProductMemo& p);  // Overloaded assignment
  bool operator!= (const PairProductMemo& p);        // Overloaded !=
  bool operator== (const PairProductMemo& p);        // Overloaded ==

  G_CFLOBDDNodeHandle nodeHandle;
  PairProductMapHandle pairProductMapHandle;
};

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
