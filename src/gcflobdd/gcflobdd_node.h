#ifndef G_CFLOBDD_NODE_GUARD
#define G_CFLOBDD_NODE_GUARD

namespace G_CFL_OBDD {

// Node classes declared in this file --------------------------------
class G_CFLOBDDNode;
class G_CFLOBDDInternalNode;   //  : public G_CFLOBDDNode
class G_CFLOBDDLeafNode;       //  : public G_CFLOBDDNode
class G_CFLOBDDForkNode;       //  : public G_CFLOBDDLeafNode
class G_CFLOBDDDontCareNode;   //  : public G_CFLOBDDLeafNode
class G_CFLOBDDNodeHandle;
}

#include <iostream>
#include <fstream>
#include <cstdint>

#include "../utils/list_T.h"
#include "../utils/list_TPtr.h"
#include "../utils/intpair.h"
#include "return_map_T.h"
#include "../grammar/grammar.h"
#include <unordered_map>

//using namespace boost::multiprecision;

namespace G_CFL_OBDD {
	typedef ReturnMapBody<int> G_CFLOBDDReturnMapBody;
	typedef ReturnMapHandle<int> G_CFLOBDDReturnMapHandle;
}
#include "connectionT.h"
#include "connectionListT.h"
namespace G_CFL_OBDD {
  typedef ConnectionListT<G_CFLOBDDReturnMapHandle> ConnectionList;
	typedef ConnectionT<G_CFLOBDDReturnMapHandle> Connection;
}
#include "reduction_map.h"
#include "../utils/hash.h"
#include "../utils/hashset.h"
#include "../utils/ref_ptr.h"

namespace G_CFL_OBDD {
  class G_CFLOBDDNodeHandle;
  class G_CFLOBDDNode;
  class G_CFLOBDDInternalNode;
  class G_CFLOBDDLeafNode;
  class G_CFLOBDDForkNode;
  class G_CFLOBDDDontCareNode;
}

// namespace G_CFL_OBDD {
//   struct G_CFLOBDDNodeHash {
//   public:
//     size_t operator()(const std::shared_ptr<G_CFLOBDDNode>& c) const;
//   };

//   struct G_CFLOBDDNodeEqual {
//   public:
//     bool operator()(const std::shared_ptr<G_CFLOBDDNode>& a, const std::shared_ptr<G_CFLOBDDNode>& b) const;
//   };
// }

namespace G_CFL_OBDD {
  class NoDistinctionCacheKey {
  public:
      NoDistinctionCacheKey(unsigned int level, std::shared_ptr<GrammarNode>& grammar);
      unsigned int Hash(unsigned int modsize) const;
      NoDistinctionCacheKey& operator= (const NoDistinctionCacheKey& p);  // Overloaded assignment
      bool operator!= (const NoDistinctionCacheKey& p) const;        // Overloaded !=
      bool operator== (const NoDistinctionCacheKey& p) const;        // Overloaded ==
      unsigned int Level() const { return level; }      // Access function
      std::shared_ptr<GrammarNode> Grammar() const { return grammar; } // Access function

      struct NoDistinctionCacheKey_Hash {
      public:
          size_t operator()(const NoDistinctionCacheKey& c) const {
              return c.Hash(997);
          }
      };
      struct NoDistinctionCacheKey_Equal {
      public:
          bool operator()(const NoDistinctionCacheKey& a, const NoDistinctionCacheKey& b) const {
              return (a == b);
          }
      };
      private:
      unsigned int level;
      std::shared_ptr<GrammarNode> grammar;
      NoDistinctionCacheKey();                                 // Default constructor (hidden)
  };
}


namespace G_CFL_OBDD {

//********************************************************************
// G_CFLOBDDNodeHandle
//********************************************************************

class G_CFLOBDDNodeHandle {
#define G_CFLOBDD_NODE_HANDLE_GUARD
 public:
  G_CFLOBDDNodeHandle();                                        // Default constructor
  G_CFLOBDDNodeHandle(G_CFLOBDDNode *n);                          // Constructor
  G_CFLOBDDNodeHandle(const G_CFLOBDDNodeHandle &nh);              // Copy constructor
  ~G_CFLOBDDNodeHandle();                                       // Destructor
  unsigned int Hash(unsigned int modsize) const;
  bool operator!= (const G_CFLOBDDNodeHandle &nh) const;              // Overloaded !=
  bool operator== (const G_CFLOBDDNodeHandle &nh) const;              // Overloaded ==
  G_CFLOBDDNodeHandle & operator= (const G_CFLOBDDNodeHandle &nh); // assignment

  // Distinguished G_CFLOBDDNodeHandles -----------------------
     public:
      static G_CFLOBDDNodeHandle G_CFLOBDDForkNodeHandle;
      static G_CFLOBDDNodeHandle G_CFLOBDDDontCareNodeHandle;
      static void InitLeafNodes();
      static std::unordered_map<NoDistinctionCacheKey, G_CFLOBDDNodeHandle, NoDistinctionCacheKey::NoDistinctionCacheKey_Hash, NoDistinctionCacheKey::NoDistinctionCacheKey_Equal> NoDistinctionNode;
	  
  // The data member
    G_CFLOBDDNode* handleContents;

 // Table of canonical nodes -------------------------
    public:
     static Hashset<G_CFLOBDDNode> *canonicalNodeTable;
     void Canonicalize();
     static void GarbageCollectCanonicalNodeTable();

 // Reduce and its associated cache ---------------
    public:
     G_CFLOBDDNodeHandle Reduce(ReductionMapHandle& redMapHandle, unsigned int replacementNumExits, bool forceReduce = false);
     static void InitReduceCache();
     static void DisposeOfReduceCache();

	public:
	 std::ostream& print(std::ostream & out = std::cout) const;

	 struct G_CFLOBDDNodeHandle_Hash {
	 public:
		 size_t operator()(const G_CFLOBDDNodeHandle& c) const {
			//  return ((reinterpret_cast<std::uintptr_t>(c.handleContents.get()) >> 2) % 997);
      return ((reinterpret_cast<std::uintptr_t>(c.handleContents) >> 2) % 997);
		 }
	 };
};

std::ostream& operator<< (std::ostream & out, const G_CFLOBDDNodeHandle &d);


typedef Hashtable<G_CFLOBDDNodeHandle, G_CFLOBDDNodeHandle> G_CFLOBDDNodeMemoTable;
typedef ref_ptr<G_CFLOBDDNodeMemoTable> G_CFLOBDDNodeMemoTableRefPtr;
}


namespace G_CFL_OBDD {
    #define G_CFLOBDD_MAX_LEVEL 259
	unsigned int const G_CFLOBDDMaxLevel = G_CFLOBDD_MAX_LEVEL;

	//********************************************************************
	// CFLReduceKey
	//********************************************************************

	class CFLReduceKey {

	public:
		CFLReduceKey(G_CFLOBDDNodeHandle nodeHandle, ReductionMapHandle redMap); // Constructor
		unsigned int Hash(unsigned int modsize) const;
		CFLReduceKey& operator= (const CFLReduceKey& p);  // Overloaded assignment
		bool operator!= (const CFLReduceKey& p);        // Overloaded !=
		bool operator== (const CFLReduceKey& p);        // Overloaded ==
		G_CFLOBDDNodeHandle NodeHandle() const { return nodeHandle; }      // Access function
		ReductionMapHandle RedMapHandle() const { return redMapHandle; } // Access function
		std::ostream& print(std::ostream & out) const;

	private:
		G_CFLOBDDNodeHandle nodeHandle;
		ReductionMapHandle redMapHandle;
		CFLReduceKey();                                 // Default constructor (hidden)
	};

	std::ostream& operator<< (std::ostream & out, const CFLReduceKey &p);

}

#include "gcflobdd_top_node_t.h"
#include "../grammar/grammar.h"

//********************************************************************
// G_CFLOBDDNode
//********************************************************************

namespace G_CFL_OBDD {

enum G_CFLOBDD_NODEKIND { G_CFLOBDD_INTERNAL, G_CFLOBDD_FORK, G_CFLOBDD_DONTCARE };

class G_CFLOBDDNode {
 public:
  G_CFLOBDDNode();                       // Constructor
  G_CFLOBDDNode(const unsigned int l);   // Constructor
  virtual ~G_CFLOBDDNode();              // Destructor
  virtual G_CFLOBDD_NODEKIND NodeKind() const = 0;
  unsigned int numExits;
  static unsigned int const maxLevel;
  long double *numPathsToExit;
  bool isNumPathsMemAllocated;

  virtual G_CFLOBDDNodeHandle Reduce(ReductionMapHandle& redMapHandle, unsigned int replacementNumExits, bool forceReduce = false) = 0;
  virtual unsigned int Hash(unsigned int modsize) const = 0;

  virtual bool operator!= (const G_CFLOBDDNode & n) const = 0;  // Overloaded !=
  virtual bool operator== (const G_CFLOBDDNode & n) const = 0;  // Overloaded ==
  virtual void IncrRef() = 0;
  virtual void DecrRef() = 0;
  const unsigned int Level() const { return level; }
  const bool IsCanonical() const { return isCanonical; }
  void SetCanonical() { isCanonical = true;  }
  unsigned int GetRefCount(){ return refCount; }
 public:
  virtual std::ostream& print(std::ostream & out = std::cout) const = 0;
  virtual void CountNodesAndEdges(Hashset<G_CFLOBDDNodeHandle>* visitedNodes, Hashset<G_CFLOBDDReturnMapBody>* visitedEdges,
	  unsigned int& nodeCount, unsigned int& edgeCount) = 0;
  virtual void CountPaths(Hashset<G_CFLOBDDNodeHandle>* visitedNodes) = 0;
  virtual void PrintYield(std::vector<std::vector<std::string>>& yield_strings) const = 0;
  const unsigned int level;
  std::shared_ptr<GrammarNode> grammar;
  unsigned int cachedHash_997 = 0;
  bool hash_set_997 = false;
  void SetHashCache(unsigned int modsize,  unsigned int hashValue);
 protected:
  unsigned int refCount;
  bool isCanonical;              // Is this CFLOBDDNode in canonicalNodeTable?

};

std::ostream& operator<< (std::ostream & out, const G_CFLOBDDNode &n);

//********************************************************************
// G_CFLOBDDInternalNode
//********************************************************************

class G_CFLOBDDInternalNode : public G_CFLOBDDNode {
  
//   friend G_CFLOBDDNodeHandle PairProduct(G_CFLOBDDInternalNode *n1, G_CFLOBDDInternalNode *n2, PairProductMapHandle &pairProductMap);
//   friend G_CFLOBDDNodeHandle TripleProduct(G_CFLOBDDInternalNode *n1, G_CFLOBDDInternalNode *n2, G_CFLOBDDInternalNode *n3, TripleProductMapHandle &tripleProductMap);

 public:
  G_CFLOBDDInternalNode(const unsigned int l);   // Constructor
  ~G_CFLOBDDInternalNode();                      // Destructor
  G_CFLOBDD_NODEKIND NodeKind() const { return G_CFLOBDD_INTERNAL; }
  
  G_CFLOBDDNodeHandle Reduce(ReductionMapHandle& redMapHandle, unsigned int replacementNumExits, bool forceReduce = false);
  unsigned int Hash(unsigned int modsize) const;
  bool operator!= (const G_CFLOBDDNode & n) const;        // Overloaded !=
  bool operator== (const G_CFLOBDDNode & n) const;        // Overloaded ==
  void IncrRef();
  void DecrRef();

 public:
  std::ostream& print(std::ostream & out = std::cout) const;
  void CountNodesAndEdges(Hashset<G_CFLOBDDNodeHandle>* visitedNodes, Hashset<G_CFLOBDDReturnMapBody>* visitedEdges,
	  unsigned int& nodeCount, unsigned int& edgeCount);
  void CountPaths(Hashset<G_CFLOBDDNodeHandle>* visitedNodes);
  void PrintYield(std::vector<std::vector<std::string>>& yield_strings) const;

  unsigned int numLayers;
  ConnectionList *connections; // layers 1 ... numLayers

  void InstallPathCounts();

 private:
  G_CFLOBDDInternalNode();                                         // Default constructor (hidden)
  G_CFLOBDDInternalNode(const G_CFLOBDDInternalNode &n);             // Copy constructor (hidden)
  G_CFLOBDDInternalNode& operator= (const G_CFLOBDDInternalNode &n); // Overloaded = (hidden)
};

//********************************************************************
// G_CFLOBDDLeafNode
//********************************************************************

class G_CFLOBDDLeafNode : public G_CFLOBDDNode {
 public:
  G_CFLOBDDLeafNode();                   // Constructor
  virtual ~G_CFLOBDDLeafNode();          // Destructor
  virtual G_CFLOBDD_NODEKIND NodeKind() const = 0;
  virtual G_CFLOBDDNodeHandle Reduce(ReductionMapHandle& redMapHandle, unsigned int replacementNumExits, bool forceReduce = false) = 0;
  virtual unsigned int Hash(unsigned int modsize) const = 0;
  virtual bool operator!= (const G_CFLOBDDNode & n) const = 0;  // Overloaded !=
  virtual bool operator== (const G_CFLOBDDNode & n) const = 0;  // Overloaded ==
  void IncrRef();
  void DecrRef();

 public:
	virtual std::ostream& print(std::ostream & out = std::cout) const = 0;
  void CountNodesAndEdges(Hashset<G_CFLOBDDNodeHandle>* visitedNodes, Hashset<G_CFLOBDDReturnMapBody>* visitedEdges,
	  unsigned int& nodeCount, unsigned int& edgeCount);
  void CountPaths(Hashset<G_CFLOBDDNodeHandle>* visitedNodes);
  virtual void PrintYield(std::vector<std::vector<std::string>>& yield_strings) const = 0;
};

//********************************************************************
// G_CFLOBDDForkNode
//********************************************************************

class G_CFLOBDDForkNode : public G_CFLOBDDLeafNode {
 public:
  G_CFLOBDDForkNode();                   // Constructor
  ~G_CFLOBDDForkNode();                  // Destructor
  G_CFLOBDD_NODEKIND NodeKind() const { return G_CFLOBDD_FORK; }
  G_CFLOBDDNodeHandle Reduce(ReductionMapHandle& redMapHandle, unsigned int replacementNumExits, bool forceReduce = false);
  unsigned int Hash(unsigned int modsize) const;
  bool operator!= (const G_CFLOBDDNode & n) const;        // Overloaded !=
  bool operator== (const G_CFLOBDDNode & n) const;        // Overloaded ==

 public:
	std::ostream& print(std::ostream & out = std::cout) const;
  void PrintYield(std::vector<std::vector<std::string>>& yield_strings) const;

 private:
  G_CFLOBDDForkNode(const G_CFLOBDDForkNode &n);   // Copy constructor (hidden)
  G_CFLOBDDForkNode& operator= (const G_CFLOBDDForkNode &n); // Overloaded = (hidden)
};

//********************************************************************
// G_CFLOBDDDontCareNode
//********************************************************************

class G_CFLOBDDDontCareNode : public G_CFLOBDDLeafNode {
 public:
  G_CFLOBDDDontCareNode();                   // Constructor
  ~G_CFLOBDDDontCareNode();                  // Destructor
  G_CFLOBDD_NODEKIND NodeKind() const { return G_CFLOBDD_DONTCARE; }
  G_CFLOBDDNodeHandle Reduce(ReductionMapHandle& redMapHandle, unsigned int replacementNumExits, bool forceReduce = false);
  unsigned int Hash(unsigned int modsize) const;
  bool operator!= (const G_CFLOBDDNode & n) const;        // Overloaded !=
  bool operator== (const G_CFLOBDDNode & n) const;        // Overloaded ==

 public:
	std::ostream& print(std::ostream & out = std::cout) const;
  void PrintYield(std::vector<std::vector<std::string>>& yield_strings) const;

 private:
  G_CFLOBDDDontCareNode(const G_CFLOBDDDontCareNode &n);   // Copy constructor (hidden)
  G_CFLOBDDDontCareNode& operator= (const G_CFLOBDDDontCareNode &n); // Overloaded = (hidden)
};

 } // namespace G_CFL_OBDD

#endif // G_CFLOBDD_NODE_GUARD