#ifndef G_CFLOBDD_T_GUARD
#define G_CFLOBDD_T_GUARD

#include <iostream>
#include <fstream>
#include "../utils/ref_ptr.h"
#include "gcflobdd_top_node_t.h"

namespace G_CFL_OBDD {

//********************************************************************
// G_CFLOBDD_T
//********************************************************************

// Representation of a Boolean function
template <typename T>
class G_CFLOBDD_T {
 public:
  G_CFLOBDD_T();      // Default constructor (rep. of \a.true)
  G_CFLOBDD_T(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n); // Constructor
  G_CFLOBDD_T(const G_CFLOBDD_T &d);                            // Copy constructor
  ~G_CFLOBDD_T();                                             // Destructor
  unsigned int Hash(unsigned int modsize);
  bool operator!= (const G_CFLOBDD_T & C) const;          // Overloaded !=
  bool operator== (const G_CFLOBDD_T & C) const;          // Overloaded ==
  G_CFLOBDD_T& operator= (const G_CFLOBDD_T &c);       // assignment
  ref_ptr<G_CFLOBDDTopNodeT<T>> root;

  void CountNodesAndEdges(unsigned int &nodeCount, unsigned int &edgeCount);
  void CountPaths();
  void GroupCountNodesAndEdges(unsigned int &nodeCount, unsigned int &edgeCount, Hashset<G_CFLOBDDNodeHandle>* visitedNodesDuringGroupCountNodesAndEdges,
	  Hashset<G_CFLOBDDReturnMapBody>* visitedEdgesDuringGroupCountNodesAndEdges);
  void PrintYield(std::ostream & out = std::cout) const;

 public:
	std::ostream& print(std::ostream & out = std::cout) const;
    static constexpr unsigned int maxLevel = G_CFLOBDDMaxLevel;
};

template <typename T>
std::ostream& operator<< (std::ostream & out, const G_CFLOBDD_T<T> &d);

// Default constructor must be provided by each specialization XXX
// template<>
// CFLOBDD_T<XXX>::CFLOBDD_T()
// {
// 	root = default XXX value, e.g., for int, we use MkTrueTop();
// }

template<typename T>
G_CFLOBDD_T<T>::G_CFLOBDD_T(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n)
{
	root = n;
}

// Copy constructor
template<typename T>
G_CFLOBDD_T<T>::G_CFLOBDD_T(const G_CFLOBDD_T<T> &c)
{
	root = c.root;
}

template<typename T>
G_CFLOBDD_T<T>::~G_CFLOBDD_T()
{
}

// Operations -----------------------------------------------------

// Hash
template<typename T>
unsigned int G_CFLOBDD_T<T>::Hash(unsigned int modsize)
{
	return root->Hash(modsize);
}

// Overloaded !=
template<typename T>
bool G_CFLOBDD_T<T>::operator!= (const G_CFLOBDD_T<T> & C) const
{
	return *root != *C.root;
}

// Overloaded ==
template<typename T>
bool G_CFLOBDD_T<T>::operator== (const G_CFLOBDD_T<T> & C) const
{
	return *root == *C.root;
}

// Overloaded assignment
template<typename T>
G_CFLOBDD_T<T> & G_CFLOBDD_T<T>::operator= (const G_CFLOBDD_T<T> &c)
{
	if (this != &c)      // don't assign to self!
	{
		root = c.root;
	}
	return *this;
}


// print
template<typename T>
std::ostream& G_CFLOBDD_T<T>::print(std::ostream & out) const
{
	out << *root << std::endl;
	return out;
}

template<typename T>
std::ostream& operator<< (std::ostream & out, const G_CFLOBDD_T<T> &d)
{
	d.print(out);
	return(out);
}

template<typename T>
void G_CFLOBDD_T<T>::PrintYield(std::ostream & out) const
{
	root->PrintYield(out);
}

// Linear operations -----------------------------------------------

// Pointwise addition: \f.\g.(f + g)
template<typename T>
G_CFLOBDD_T<T> operator+(G_CFLOBDD_T<T> f, G_CFLOBDD_T<T> g)
{
	return G_CFLOBDD_T<T>(MkPlusTopNode<T>(f.root, g.root));
}

// Pointwise addition: \f.\g.(f ^ g)
template<typename T>
G_CFLOBDD_T<T> operator^(G_CFLOBDD_T<T> f, G_CFLOBDD_T<T> g)
{
	return G_CFLOBDD_T<T>(MkExorTopNode<T>(f.root, g.root));
}

// Left scalar-multiplication: \c:unsigned int.\g.(c * g)
template<typename T>
G_CFLOBDD_T<T> operator*(unsigned int c, G_CFLOBDD_T<T> g)
{
	return G_CFLOBDD_T<T>(MkLeftScalarTimesTopNode(c, g.root));
}

// Left scalar-multiplication: \c:int.\g.(c * g)
template<typename T>
G_CFLOBDD_T<T> operator*(int c, G_CFLOBDD_T<T> g)
{
	return G_CFLOBDD_T<T>(MkLeftScalarTimesTopNode<int, T>(c, g.root));
}

// Left scalar-multiplication: \c:double.\g.(c * g)
template<typename T>
G_CFLOBDD_T<T> operator*(double c, G_CFLOBDD_T<T> g)
{
	return G_CFLOBDD_T<T>(MkLeftScalarTimesTopNode(c, g.root));
}

// Left scalar-multiplication: \c:unsigned int.\g.(c * g)
template<typename T, typename T1>
G_CFLOBDD_T<T> operator*(T1 c, G_CFLOBDD_T<T> g)
{
	return G_CFLOBDD_T<T>(MkLeftScalarTimesTopNode<T>(c, g.root));
}

// Right scalar-multiplication: \f.\c:int.(f * c)
template<typename T>
G_CFLOBDD_T<T> operator*(G_CFLOBDD_T<T> f, int c)
{
	return G_CFLOBDD_T<T>(MkRightScalarTimesTopNode<T>(f.root, c));
}

// Pointwise multiplication: \f.\g.(f * g) -------------------------------------------
template<typename T>
G_CFLOBDD_T<T> operator*(G_CFLOBDD_T<T> f, G_CFLOBDD_T<T> g)
{
	return G_CFLOBDD_T<T>(MkTimesTopNode<T>(f.root, g.root));
}

template<typename T>
void G_CFLOBDD_T<T>::CountNodesAndEdges(unsigned int &nodeCount, unsigned int &edgeCount)
{
	Hashset<G_CFLOBDDNodeHandle> *visitedNodes = new Hashset<G_CFLOBDDNodeHandle>(HASH_NUM_BUCKETS);
	Hashset<G_CFLOBDDReturnMapBody> *visitedEdges = new Hashset<G_CFLOBDDReturnMapBody>(HASH_NUM_BUCKETS);
	nodeCount = 0;
	edgeCount = 0;
	root->CountNodesAndEdges(visitedNodes, visitedEdges, nodeCount, edgeCount);
	//delete visitedEdges;
}

template<typename T>
void G_CFLOBDD_T<T>::GroupCountNodesAndEdges(unsigned int &nodeCount, unsigned int &edgeCount, Hashset<G_CFLOBDDNodeHandle>* visitedNodesDuringGroupCountNodesAndEdges,
							Hashset<G_CFLOBDDReturnMapBody>* visitedEdgesDuringGroupCountNodesAndEdges)
{
	root->CountNodesAndEdges(visitedNodesDuringGroupCountNodesAndEdges,
							visitedEdgesDuringGroupCountNodesAndEdges,
                           nodeCount, edgeCount);
}

template<typename T>
void G_CFLOBDD_T<T>::CountPaths()
{
	Hashset<G_CFLOBDDNodeHandle> *visitedNodes = new Hashset<G_CFLOBDDNodeHandle>;
	root->CountPaths(visitedNodes);
	delete visitedNodes;
}


} // namespace G_CFL_OBDD

#endif
