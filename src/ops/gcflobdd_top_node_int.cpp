#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdarg>

#include "gcflobdd_top_node_int.h"
#include "../gcflobdd/gcflobdd_node.h"
#include "gcflobdd_node_ops.h"
#include "../gcflobdd/return_map_T.h"

//********************************************************************
// G_CFLOBDDTopNode
//********************************************************************

namespace G_CFL_OBDD {

template class G_CFLOBDDTopNodeT<int>;

// G_CFLOBDDTopNode-creation operations --------------------------------------

// Create representation of \x.true
G_CFLOBDDTopNodeIntRefPtr MkTrueTop(int level, const std::shared_ptr<Grammar>& grammar)
{
  G_CFLOBDDTopNodeIntRefPtr v;
  G_CFLOBDDReturnMapHandle m;

  m.AddToEnd(1);  // Map the only exit of the body to 1 (i.e., T)
  m.Canonicalize();
  if (level == -1) {
    auto nodeHandle = MkNoDistinction(G_CFLOBDDTopNode::maxLevel, grammar->root);
    v = new G_CFLOBDDTopNode(nodeHandle, m);
  }
  else {
    auto nodeHandle = MkNoDistinction(level, grammar->root);
    v = new G_CFLOBDDTopNode(nodeHandle, m);
  }
  return v;
}

// Create representation of \x.false
G_CFLOBDDTopNodeIntRefPtr MkFalseTop(int level, const std::shared_ptr<Grammar>& grammar)
{
  G_CFLOBDDTopNodeIntRefPtr v;
  G_CFLOBDDReturnMapHandle m;

  m.AddToEnd(0);  // Map the only exit of the body to 0 (i.e., F)
  m.Canonicalize();
  if (level == -1) {
    auto nodeHandle = MkNoDistinction(G_CFLOBDDTopNode::maxLevel, grammar->root);
    v = new G_CFLOBDDTopNode(nodeHandle, m);
  }
  else {
    auto nodeHandle = MkNoDistinction(level, grammar->root);
    v = new G_CFLOBDDTopNode(nodeHandle, m);
  }
  return v;
}

// Create representation of \x.x_i
G_CFLOBDDTopNodeIntRefPtr MkDistinction(unsigned int i, int level, const std::shared_ptr<Grammar>& grammar)
{
  G_CFLOBDDTopNodeIntRefPtr v;
  G_CFLOBDDNodeHandle tempHandle;
  G_CFLOBDDReturnMapHandle m;

  // assert(i < (1 << G_CFLOBDDTopNode::maxLevel));   // i.e., i < 2**maxLevel
  if (level == -1)
	tempHandle = MkDistinction(G_CFLOBDDTopNode::maxLevel, i, grammar->root);
  else
	  tempHandle = MkDistinction(level, i, grammar->root);
  m.AddToEnd(0);
  m.AddToEnd(1);
  m.Canonicalize();
  v = new G_CFLOBDDTopNode(tempHandle, m);
  return v;
}

// Create representation of parity function
G_CFLOBDDTopNodeIntRefPtr MkParityTop(int level, const std::shared_ptr<Grammar>& grammar)
{
  G_CFLOBDDTopNodeIntRefPtr v;
  G_CFLOBDDNodeHandle tempHandle;
  G_CFLOBDDReturnMapHandle m;

  if (level == -1){
    tempHandle = MkParity(G_CFLOBDDTopNode::maxLevel, grammar->root);
  } else {
    tempHandle = MkParity(level, grammar->root);
  }
  m.AddToEnd(0);
  m.AddToEnd(1);
  m.Canonicalize();
  v = new G_CFLOBDDTopNode(tempHandle, m);
  return v;
}

// Unary operations on CFLOBDDTopNodes --------------------------------------

// Implements \f.!f
G_CFLOBDDTopNodeIntRefPtr MkNot(G_CFLOBDDTopNodeIntRefPtr f)
{
  G_CFLOBDDTopNodeIntRefPtr answer;
  G_CFLOBDDReturnMapHandle m = f->rootConnection.returnMapHandle.Complement();
  answer = new G_CFLOBDDTopNode(*(f->rootConnection.entryPointHandle), m);
  return answer;
}

// Binary operations on CFLOBDDTopNodes ------------------------------------

// \f.\g.(f && g)
G_CFLOBDDTopNodeIntRefPtr MkAnd(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, andOp);
}

// \f.\g.!(f && g)
G_CFLOBDDTopNodeIntRefPtr MkNand(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, nandOp);
}

// \f.\g.(f || g)
G_CFLOBDDTopNodeIntRefPtr MkOr(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, orOp);
}

// \f.\g.!(f || g)
G_CFLOBDDTopNodeIntRefPtr MkNor(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, norOp);
}

// \f.\g.(f == g)
G_CFLOBDDTopNodeIntRefPtr MkIff(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, iffOp);
}

// \f.\g.(f != g)
G_CFLOBDDTopNodeIntRefPtr MkExclusiveOr(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, exclusiveOrOp);
}

// \f.\g.(!f || g)
G_CFLOBDDTopNodeIntRefPtr MkImplies(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, impliesOp);
}

// \f.\g.(f && !g)
G_CFLOBDDTopNodeIntRefPtr MkMinus(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, minusOp);
}

// \f.\g.(!g || f)
G_CFLOBDDTopNodeIntRefPtr MkQuotient(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, quotientOp);
}

// \f.\g.(g && !f)
G_CFLOBDDTopNodeIntRefPtr MkNotQuotient(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, notQuotientOp);
}

// \f.\g.f
G_CFLOBDDTopNodeIntRefPtr MkFirst(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr )
{
  return f;
}

// \f.\g.!f
G_CFLOBDDTopNodeIntRefPtr MkNotFirst(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr )
{
  return MkNot(f);
}

// \f.\g.g
G_CFLOBDDTopNodeIntRefPtr MkSecond(G_CFLOBDDTopNodeIntRefPtr , G_CFLOBDDTopNodeIntRefPtr g)
{
  return g;
}

// \f.\g.!g
G_CFLOBDDTopNodeIntRefPtr MkNotSecond(G_CFLOBDDTopNodeIntRefPtr , G_CFLOBDDTopNodeIntRefPtr g)
{
  return MkNot(g);
}


// \f.\g.(f + g)
G_CFLOBDDTopNodeIntRefPtr MkPlus(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, PlusFunc);
}

// \f.\g.(f * g)
G_CFLOBDDTopNodeIntRefPtr MkTimes(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g)
{
	return ApplyAndReduce<int>(f, g, TimesFunc);
}

// Ternary operations on CFLOBDDTopNodes ------------------------------------

// \a.\b.\c.(a && b) || (!a && c)
G_CFLOBDDTopNodeIntRefPtr MkIfThenElse(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g, G_CFLOBDDTopNodeIntRefPtr h)
{
	return ApplyAndReduce<int>(f, g, h, ifThenElseOp);
}

// \a.\b.\c.(b && !a) || (c && !a) || (b && c)
G_CFLOBDDTopNodeIntRefPtr MkNegMajority(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g, G_CFLOBDDTopNodeIntRefPtr h)
{
	return ApplyAndReduce<int>(f, g, h, negMajorityOp);
}

G_CFLOBDDTopNodeIntRefPtr MkRestrict(G_CFLOBDDTopNodeIntRefPtr n, unsigned int i, bool val)
{
	// G_CFLOBDDReturnMapHandle MapHandle;
	// G_CFLOBDDNodeHandle g = Restrict(*(n->rootConnection.entryPointHandle), i, val,
	// 	MapHandle);

	// // Create returnMapHandle from MapHandle
	// G_CFLOBDDReturnMapHandle returnMapHandle;
	// unsigned MapSize = MapHandle.mapContents->mapArray.size();
	// for (unsigned sBI = 0; sBI < MapSize; sBI++)
	// {
	// 	int d = MapHandle.mapContents->mapArray[sBI];
	// 	int c = n->rootConnection.returnMapHandle.Lookup(d);
	// 	returnMapHandle.AddToEnd(c);
	// }
	// returnMapHandle.Canonicalize();

	// // Create and return G_CFLOBDDTopNode
	// return(new G_CFLOBDDTopNode(g, returnMapHandle));
    abort(); // Not implemented
}

// Create representation of \f . exists x_i : f
G_CFLOBDDTopNodeIntRefPtr MkExists(G_CFLOBDDTopNodeIntRefPtr f, unsigned int i)
{
//   G_CFLOBDDTopNodeIntRefPtr tempTrue = MkRestrict(f, i, true);
//   G_CFLOBDDTopNodeIntRefPtr tempFalse = MkRestrict(f, i, false);
//   return MkOr(tempTrue, tempFalse);
    abort(); // Not implemented
}

// Create representation of \f . forall x_i : f
G_CFLOBDDTopNodeIntRefPtr MkForall(G_CFLOBDDTopNodeIntRefPtr f, unsigned int i)
{
//   G_CFLOBDDTopNodeIntRefPtr tempTrue = MkRestrict(f, i, true);
//   G_CFLOBDDTopNodeIntRefPtr tempFalse = MkRestrict(f, i, false);
//   return MkAnd(tempTrue, tempFalse);
    abort(); // Not implemented
}


}
