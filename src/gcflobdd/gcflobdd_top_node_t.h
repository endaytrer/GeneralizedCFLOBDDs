#ifndef G_CFLOBDD_TOP_NODE_T_GUARD
#define G_CFLOBDD_TOP_NODE_T_GUARD


#include <iostream>
#include <fstream>
#include "../utils/ref_ptr.h"
#include "../utils/assignment.h"
#include "connectionT.h"
#include "gcflobdd_node.h"
#include "../utils/bool_op.h"
#include "../grammar/grammar.h"

namespace G_CFL_OBDD {

//********************************************************************
// CFLOBDDTopNode
//********************************************************************

template <typename T>
class G_CFLOBDDTopNodeT {
 public:
  G_CFLOBDDTopNodeT(G_CFLOBDDNode *n, ReturnMapHandle<T> &mapHandle);                // Constructor
  G_CFLOBDDTopNodeT(G_CFLOBDDNodeHandle &nodeHandle, ReturnMapHandle<T> &mapHandle); // Constructor
  ~G_CFLOBDDTopNodeT();                                   // Destructor

  unsigned int Hash(unsigned int modsize);
  bool operator!= (const G_CFLOBDDTopNodeT<T> & C);          // Overloaded !=
  bool operator== (const G_CFLOBDDTopNodeT<T> & C);          // Overloaded ==
  static unsigned int const maxLevel;
  unsigned int level;
  ConnectionT<ReturnMapHandle<T>> rootConnection;                           // A single Connection
  std::shared_ptr<Grammar> grammar;
  RefCounter count;
  void DeallocateMemory();


 private:
  static Hashset<G_CFLOBDDTopNodeT<T> > *computedCache;       // TEMPORARY: should be HashCache
  G_CFLOBDDTopNodeT();                                    // Default constructor (hidden)
  G_CFLOBDDTopNodeT(const G_CFLOBDDTopNodeT<T> &n);             // Copy constructor (hidden)
  G_CFLOBDDTopNodeT& operator= (const G_CFLOBDDTopNodeT<T> &n); // Overloaded = (hidden)
 public:
  std::ostream& print(std::ostream & out = std::cout) const;
  void CountNodesAndEdges(Hashset<G_CFLOBDDNodeHandle>* visitedNodes, Hashset<G_CFLOBDDReturnMapBody>* visitedEdges,
	  unsigned int& nodeCount, unsigned int& edgeCount);

  void CountPaths(Hashset<G_CFLOBDDNodeHandle>* visitedNodes);
  void PrintYield(std::ostream & out = std::cout) const;
  bool FindOneSatisfyingAssignment(SH_OBDD::Assignment * &assignment);

  // Usage: G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
  typedef ref_ptr<G_CFLOBDDTopNodeT<T>> G_CFLOBDDTopNodeTRefPtr;
};

template <typename T>
std::ostream& operator<< (std::ostream & out, const G_CFLOBDDTopNodeT<T> &d);

template <typename T> 
typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    ApplyAndReduce(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n1,
		typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n2,
		BoolOp op
	);
template <typename T> 
  typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    ApplyAndReduce(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n1,
        typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n2,
        T(*func)(T, T)
	);
template <typename T> 
  typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    ApplyAndReduce(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n1,
        typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n2,
        typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n3,
        BoolOp3 op
	);
template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    MkPlusTopNode(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f,
                typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g
	);
template<typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    operator+(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f,
	typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g);

template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    MkExorTopNode(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f,
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g
    );

template<typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    operator^(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f, typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g);

template <typename T, typename T1>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    MkLeftScalarTimesTopNode(T1 c, typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g);

template<typename T, typename T1>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    operator*(T1 c, typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g);

template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    MkRightScalarTimesTopNode(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f, int c);

template<typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    operator*(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f, int c);

template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    MkTimesTopNode(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f,
                typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g);

template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    operator*(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f, typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g);

} // namespace G_CFL_OBDD

#endif
