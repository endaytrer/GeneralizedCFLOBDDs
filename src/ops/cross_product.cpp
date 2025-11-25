#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unordered_map>
#include "../gcflobdd/gcflobdd_node.h"
#include "../ops/gcflobdd_node_ops.h"
#include "../utils/intpair.h"
#include "cross_product.h"

using namespace G_CFL_OBDD;

// ********************************************************************
// 2-Way Cross Product
// ********************************************************************

//***************************************************************
// PairProductMapBody
//***************************************************************

// Initializations of static members ---------------------------------
Hashset<PairProductMapBody> *PairProductMapBody::canonicalPairProductMapBodySet = new Hashset<PairProductMapBody>(HASHSET_NUM_BUCKETS);
// std::unordered_set<std::weak_ptr<PairProductMapBody>, PairProductMapBody::PPHash, PairProductMapBody::PPEqual> PairProductMapBody::canonicalPairProductMapBodySet;

// Constructor
PairProductMapBody::PairProductMapBody()
  : refCount(0), isCanonical(false)
{
}

void PairProductMapBody::IncrRef()
{
  refCount++;    // Warning: Saturation not checked
}

void PairProductMapBody::DecrRef()
{
  if (--refCount == 0) {    // Warning: Saturation not checked
    if (isCanonical) {
      PairProductMapBody::canonicalPairProductMapBodySet->DeleteEq(this);
    }
    delete this;
  }
}

unsigned int PairProductMapBody::Hash(unsigned int modsize) const
{
  unsigned int hvalue = 0;

  for (unsigned int i = 0; i < mapArray.size(); i++){
	  hvalue = (997*hvalue + (unsigned int)97*mapArray[i].First() + (unsigned int)mapArray[i].Second()) % modsize;
  }

  return hvalue;
}

void PairProductMapBody::setHashCheck()
{
	unsigned int hvalue = 0;

	for (auto &i : mapArray) {
		hvalue = (117 * (hvalue + 1) + (int)(97 * i.First()) + i.Second());
	}
	hashCheck = hvalue;
}

void PairProductMapBody::AddToEnd(const intpair& y)
{
	mapArray.push_back(y);
}

bool PairProductMapBody::operator==(const PairProductMapBody &o) const
{
	if (mapArray.size() != o.mapArray.size())
		return false;

	for (unsigned int i = 0; i < mapArray.size(); i++){
		if (mapArray[i] != o.mapArray[i])
			return false;
	}
	return true;
}
intpair& PairProductMapBody::operator[](unsigned int i){                       // Overloaded []
	return mapArray[i];
}

unsigned int PairProductMapBody::Size(){
	return (unsigned int)mapArray.size();
}


namespace G_CFL_OBDD {
std::ostream& operator<< (std::ostream & out, const PairProductMapBody &r)
{
  //out << (List<int>&)r;
	for (unsigned int i = 0; i < r.mapArray.size(); i++)
	{
		out << r.mapArray[i] << " ";
	}
  return(out);
}
}
//***************************************************************
// PairProductMapHandle
//***************************************************************


// Default constructor
PairProductMapHandle::PairProductMapHandle()
  :  mapContents(new PairProductMapBody)
{
  mapContents->IncrRef();
}

// Destructor
PairProductMapHandle::~PairProductMapHandle()
{
  mapContents->DecrRef();
}

// Copy constructor
PairProductMapHandle::PairProductMapHandle(const PairProductMapHandle &r)
  :  mapContents(r.mapContents)
{
  mapContents->IncrRef();
}

// Overloaded assignment
PairProductMapHandle& PairProductMapHandle::operator= (const PairProductMapHandle &r)
{
  if (this != &r)      // don't assign to self!
  {
    PairProductMapBody *temp = mapContents;
    mapContents = r.mapContents;
    mapContents->IncrRef();
    temp->DecrRef();
  }
  return *this;
}

// Overloaded !=
bool PairProductMapHandle::operator!=(const PairProductMapHandle &r)
{
  return (mapContents != r.mapContents);
}

// Overloaded ==
bool PairProductMapHandle::operator==(const PairProductMapHandle &r)
{
  return (mapContents == r.mapContents);
}

std::ostream& operator<< (std::ostream & out, const PairProductMapHandle &r)
{
  out << "[" << *r.mapContents << "]";
  return(out);
}

unsigned int PairProductMapHandle::Hash(unsigned int modsize) const
{
  return ((unsigned int) reinterpret_cast<uintptr_t>(mapContents) >> 2) % modsize;
}

unsigned int PairProductMapHandle::Size()
{
  return mapContents->Size();
}

intpair& PairProductMapHandle::operator[](unsigned int i)
{
	return mapContents->mapArray[i];
}

void PairProductMapHandle::AddToEnd(const intpair& p)
{
  assert(mapContents->refCount <= 1);
  mapContents->AddToEnd(p);
}

bool PairProductMapHandle::Member(intpair& p)
{
	for (auto& i : mapContents->mapArray){
		if (i == p)
			return true;
	}
	return false;
}

int PairProductMapHandle::Lookup(intpair& p)
{
	for (unsigned int i = 0; i < mapContents->mapArray.size(); i++){
		if (mapContents->mapArray[i] == p)
			return i;
	}
  return -1;
}

void PairProductMapHandle::Canonicalize()
{
  PairProductMapBody *answerContents;
  unsigned int hash = PairProductMapBody::canonicalPairProductMapBodySet->GetHash(mapContents);
  answerContents = PairProductMapBody::canonicalPairProductMapBodySet->Lookup(mapContents, hash);
  if (answerContents == NULL) {
    PairProductMapBody::canonicalPairProductMapBodySet->Insert(mapContents, hash);
    mapContents->isCanonical = true;
  }
  else {
    answerContents->IncrRef();
    mapContents->DecrRef();
    mapContents = answerContents;
  }
}

// Create map with reversed entries
PairProductMapHandle PairProductMapHandle::Flip()
{
  PairProductMapHandle answer;
  for (auto& i : mapContents->mapArray){
	  intpair p(i.Second(), i.First());
	  answer.AddToEnd(p);
  }
  return answer;
}

//***************************************************************
// PairProductKey
//***************************************************************

// Constructor
PairProductKey::PairProductKey(G_CFLOBDDNodeHandle nodeHandle1, G_CFLOBDDNodeHandle nodeHandle2)
  :  nodeHandle1(nodeHandle1), nodeHandle2(nodeHandle2)
{
}

// Hash
unsigned int PairProductKey::Hash(unsigned int modsize) const
{
  unsigned int hvalue = 0;
  hvalue = (997 * nodeHandle1.Hash(modsize) + nodeHandle2.Hash(modsize)) % modsize;
  return hvalue;
}

// print
std::ostream& PairProductKey::print(std::ostream & out) const
{
  out << "(" << nodeHandle1 << ", " << nodeHandle2 << ")";
  return out;
}

template <typename T>
std::ostream& operator<< (std::ostream & out, const PairProductKey &p)
{
  p.print(out);
  return(out);
}

PairProductKey& PairProductKey::operator= (const PairProductKey& i)
{
  if (this != &i)      // don't assign to self!
  {
    nodeHandle1 = i.nodeHandle1;
    nodeHandle2 = i.nodeHandle2;
  }
  return *this;        
}

// Overloaded !=
bool PairProductKey::operator!=(const PairProductKey& p)
{
  return (nodeHandle1 != p.nodeHandle1) || (nodeHandle2 != p.nodeHandle2);
}

// Overloaded ==
bool PairProductKey::operator==(const PairProductKey& p) const
{
  return (nodeHandle1 == p.nodeHandle1) && (nodeHandle2 == p.nodeHandle2);
}

//***************************************************************
// PairProductMemo
//***************************************************************

// Default constructor
PairProductMemo::PairProductMemo()
  :  nodeHandle(G_CFLOBDDNodeHandle()), pairProductMapHandle(PairProductMapHandle())
{
}

// Constructor
PairProductMemo::PairProductMemo(G_CFLOBDDNodeHandle nodeHandle, PairProductMapHandle pairProductMapHandle)
  :  nodeHandle(nodeHandle), pairProductMapHandle(pairProductMapHandle)
{
}

std::ostream& operator<< (std::ostream & out, const PairProductMemo &p)
{
  out << "(" << p.nodeHandle << ", " << p.pairProductMapHandle << ")";
  return(out);
}

PairProductMemo& PairProductMemo::operator= (const PairProductMemo& i)
{
  if (this != &i)      // don't assign to self!
  {
    nodeHandle = i.nodeHandle;
    pairProductMapHandle = i.pairProductMapHandle;
  }
  return *this;        
}

// Overloaded !=
bool PairProductMemo::operator!=(const PairProductMemo& p)
{
  return (nodeHandle != p.nodeHandle) || (pairProductMapHandle != p.pairProductMapHandle);
}

// Overloaded ==
bool PairProductMemo::operator==(const PairProductMemo& p)
{
  return (nodeHandle == p.nodeHandle) && (pairProductMapHandle == p.pairProductMapHandle);
}

// --------------------------------------------------------------------
// PairProduct
//
// Returns a new CFLOBDDNodeHandle, and (in pairProductMapHandle) a descriptor of the
// node's exits
// --------------------------------------------------------------------

static Hashtable<PairProductKey, PairProductMemo> *pairProductCache = NULL;
// static std::unordered_map<PairProductKey, PairProductMemo, PairProductKey::PairProductKey_Hash, PairProductKey::PairProductKey_Equal> pairProductCache;

namespace G_CFL_OBDD {

G_CFLOBDDNodeHandle PairProduct(G_CFLOBDDInternalNode* n1,
                              G_CFLOBDDInternalNode* n2,
                              PairProductMapHandle &pairProductMapHandle
                             )
{
  if (n1 == MkNoDistinction(n1->level, n1->grammar).handleContents) {
    if (n2 == MkNoDistinction(n2->level, n2->grammar).handleContents) {   // ND, ND
      pairProductMapHandle.AddToEnd(intpair(0,0));
      pairProductMapHandle.Canonicalize();
      return G_CFLOBDDNodeHandle(n1);
    }
    else {                                                                        // ND, XX
      for (unsigned int kk = 0; kk < n2->numExits; kk++) {
        pairProductMapHandle.AddToEnd(intpair(0,kk));
      }
      pairProductMapHandle.Canonicalize();
      return G_CFLOBDDNodeHandle(n2);
    }
  }
  else {
    if (n2 == MkNoDistinction(n2->level, n2->grammar).handleContents) {   // XX, ND
      for (unsigned int kk = 0; kk < n1->numExits; kk++) {
        pairProductMapHandle.AddToEnd(intpair(kk,0));
      }
      pairProductMapHandle.Canonicalize();
      return G_CFLOBDDNodeHandle(n1);
    }
    else {                                                                        // XX, XX
      PairProductMapHandle LayerMapHandle;
      unsigned int j;
      unsigned int curExit;
      int b1, b2;

      auto n = new G_CFLOBDDInternalNode(n1->level);
      n->numLayers = n1->numLayers;  // = n2->numLayers
      n->connections = new ConnectionList[n->numLayers];

      // Perform the cross product of the Layer 0 connections
      G_CFLOBDDNodeHandle aHandle = PairProduct(*(n1->connections[0][0].entryPointHandle),
                                                 *(n2->connections[0][0].entryPointHandle),
                                                 LayerMapHandle
                                                );
      // Fill in n->AConnection.returnMapHandle
      // Correctness relies on LayerMapHandle having no duplicates
      G_CFLOBDDReturnMapHandle aReturnHandle (LayerMapHandle.Size());
      for (unsigned int k = 0; k < LayerMapHandle.Size(); k++) {
        aReturnHandle.AddToEnd(k);
      }
      aReturnHandle.Canonicalize();
      auto layer_0_connection = Connection(aHandle, aReturnHandle);
      n->connections[0].Reserve(1);
      n->connections[0].AddConnection(layer_0_connection);
      // n->connections[0].Canonicalize();

      for (unsigned int layer = 1; layer < n->numLayers; layer++) {
        // iterate over LayerMapHandle to get the pairs of BConnections
        PairProductMapHandle newLayerMapHandle;
        std::unordered_map<intpair, int, intpair::intpair_hash, intpair::intpair_equal> tempMap;
        n->connections[layer].Reserve(LayerMapHandle.Size());
        for (auto& it : LayerMapHandle.mapContents->mapArray) {
          Connection n1_connection = n1->connections[layer][it.First()];
          Connection n2_connection = n2->connections[layer][it.Second()];

          PairProductMapHandle tempMapHandle;
          G_CFLOBDDNodeHandle n_handle = PairProduct(*(n1_connection.entryPointHandle),
                                                      *(n2_connection.entryPointHandle),
                                                      tempMapHandle);
          // Fill in n->connections[layer].returnMapHandle
          G_CFLOBDDReturnMapHandle n_returnHandle(tempMapHandle.Size());
          for (unsigned int k = 0; k < tempMapHandle.Size(); k++) {
              auto first = tempMapHandle[k].First();
              auto second = tempMapHandle[k].Second();
              auto adjusted_first = n1_connection.returnMapHandle.Lookup(first);
              auto adjusted_second = n2_connection.returnMapHandle.Lookup(second);
              auto pair_index = intpair(adjusted_first, adjusted_second);
              auto pi_it = tempMap.find(pair_index);
              if (pi_it == tempMap.end()) {
                // Not found
                newLayerMapHandle.AddToEnd(pair_index);
                n_returnHandle.AddToEnd(newLayerMapHandle.Size() - 1);
                // n_returnHandle[k] = newLayerMapHandle.Size() - 1;
                tempMap[pair_index] = newLayerMapHandle.Size() - 1;
              }
              else {
                // Found
                n_returnHandle.AddToEnd(pi_it->second);
                // n_returnHandle[k] = pi_it->second;
              }
          }
          n_returnHandle.Canonicalize();
          auto new_connection = Connection(n_handle, n_returnHandle);
          n->connections[layer].AddConnection(new_connection);
        }
        // n->connections[layer].Canonicalize();
        newLayerMapHandle.Canonicalize();
        LayerMapHandle = newLayerMapHandle;
      }
      n->numExits = LayerMapHandle.Size();
      n->grammar = n1->grammar; // = n2->grammar
      LayerMapHandle.Canonicalize();
      pairProductMapHandle = LayerMapHandle;
#ifdef PATH_COUNTING_ENABLED
      n->InstallPathCounts();
#endif
      return G_CFLOBDDNodeHandle(n);
    }
  }
}

G_CFLOBDDNodeHandle PairProduct(G_CFLOBDDNodeHandle n1,
                                 G_CFLOBDDNodeHandle n2,
                                 PairProductMapHandle &pairProductMapHandle
                                )
{
  PairProductMemo cachedPairProductMemo;

  auto key1 = PairProductKey(n1, n2);
  auto key2 = PairProductKey(n2, n1);

  // bool isCached = pairProductCache.find(key1) != pairProductCache.end();
  bool isCached = pairProductCache->Fetch(key1, cachedPairProductMemo);

  if (isCached) {
    auto memo = cachedPairProductMemo;
    pairProductMapHandle = memo.pairProductMapHandle;
    return memo.nodeHandle;
  }
  else if (pairProductCache->Fetch(key2, cachedPairProductMemo)) {
    pairProductMapHandle = cachedPairProductMemo.pairProductMapHandle.Flip();
    return cachedPairProductMemo.nodeHandle;
  }
  else {
    G_CFLOBDDNodeHandle answer;

    if (n1.handleContents->NodeKind() == G_CFLOBDD_INTERNAL) {
      answer = PairProduct(static_cast<G_CFLOBDDInternalNode*>(n1.handleContents),
                           static_cast<G_CFLOBDDInternalNode*>(n2.handleContents),
                           pairProductMapHandle
                          );
    }
    else if (n1.handleContents->NodeKind() == G_CFLOBDD_FORK) {
      if (n2.handleContents->NodeKind() == G_CFLOBDD_FORK) {                 // G_CFLOBDD_FORK, G_CFLOBDD_FORK
        pairProductMapHandle.AddToEnd(intpair(0,0));
        pairProductMapHandle.AddToEnd(intpair(1,1));
        pairProductMapHandle.Canonicalize();
        answer = n1;
      }
      else { /* n2.handleContents->NodeKind() == G_CFLOBDD_DONTCARE */       // G_CFLOBDD_FORK, G_CFLOBDD_DONTCARE
        pairProductMapHandle.AddToEnd(intpair(0,0));
        pairProductMapHandle.AddToEnd(intpair(1,0));
        pairProductMapHandle.Canonicalize();
        answer = n1;
      }
    }
    else { /* n1.handleContents->NodeKind() == G_CFLOBDD_DONTCARE */
      if (n2.handleContents->NodeKind() == G_CFLOBDD_FORK) {                 // G_CFLOBDD_DONTCARE, G_CFLOBDD_FORK
        pairProductMapHandle.AddToEnd(intpair(0,0));
        pairProductMapHandle.AddToEnd(intpair(0,1));
        pairProductMapHandle.Canonicalize();
        answer = n2;
      }
      else { /* n2.handleContents->NodeKind() == G_CFLOBDD_DONTCARE */       // G_CFLOBDD_DONTCARE, G_CFLOBDD_DONTCARE
        pairProductMapHandle.AddToEnd(intpair(0,0));
        pairProductMapHandle.Canonicalize();
        answer = n1;
      }
    }
    // std::cout << "Input nodes:\n";
    // std::cout << "n1: \n";
    // n1.print(std::cout);
    // std::cout << "\n";
    // std::cout << "n2: \n";
    // n2.print(std::cout);
    // std::cout << "\n";
    // std::cout << "Output node:\n";
    // answer.print(std::cout);
    // std::cout << "\n";
    auto memo = PairProductMemo(answer, pairProductMapHandle);
    pairProductCache->Insert(key1, memo);
    return answer;
  }
}


void InitPairProductCache()
{
  pairProductCache = new Hashtable<PairProductKey, PairProductMemo>(HASHSET_NUM_BUCKETS);
}

void DisposeOfPairProductCache()
{
  // pairProductCache.clear();
  delete pairProductCache;
  pairProductCache = NULL;
}
} // namespace G_CFL_OBDD