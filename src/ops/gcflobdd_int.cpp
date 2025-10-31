#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include "gcflobdd_int.h"
#include "../gcflobdd/gcflobdd_node.h"
#include "../utils/bool_op.h"
#include "gcflobdd_top_node_int.h"
#include "../gcflobdd/gcflobdd_top_node_t.h"

using namespace G_CFL_OBDD;

//********************************************************************
// G_CFLOBDD_T specializations
//********************************************************************


#include "../gcflobdd/gcflobdd_t.h"

// Constructors/Destructor -------------------------------------------

// Default constructor
template<>
G_CFLOBDD_T<int>::G_CFLOBDD_T()
{
    auto grammar = std::make_shared<Grammar>();
    root = MkTrueTop(-1, grammar);
}


namespace G_CFL_OBDD {

template class G_CFLOBDD_T<int>;    // aka G_CFLOBDD

// G_CFLOBDD-creation operations --------------------------------------

// Create representation of \x.true
G_CFLOBDD MkTrue(int level, const std::shared_ptr<Grammar>& grammar)
{
  return G_CFLOBDD(MkTrueTop(level, grammar));
}

// Create representation of \x.false
G_CFLOBDD MkFalse(int level, const std::shared_ptr<Grammar>& grammar)
{
  return G_CFLOBDD(MkFalseTop(level, grammar));
}

// Create representation of \x.x_i
G_CFLOBDD MkProjection(unsigned int i, int level, const std::shared_ptr<Grammar>& grammar)
{
  return G_CFLOBDD(MkDistinction(i, level, grammar));
}

} // namespace G_CFL_OBDD

namespace G_CFL_OBDD {

// Create representation of parity function
G_CFLOBDD MkParity(int level, const std::shared_ptr<Grammar>& grammar)
{
  return G_CFLOBDD(MkParityTop(level, grammar));
}

// Unary operations on G_CFLOBDDs --------------------------------------

// Implements \f.!f
G_CFLOBDD MkNot(G_CFLOBDD f)
{
  return G_CFLOBDD(MkNot(f.root));
}

// Binary operations on G_CFLOBDDs --------------------------------------

// \f.\g.(f && g)
G_CFLOBDD MkAnd(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);   
    return G_CFLOBDD(MkAnd(f.root, g.root));
}

// \f.\g.!(f && g)
G_CFLOBDD MkNand(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    // return MkNot(MkAnd(f.root, g.root));
    return G_CFLOBDD(MkNand(f.root, g.root));
}

// \f.\g.(f || g)
G_CFLOBDD MkOr(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    return G_CFLOBDD(MkOr(f.root, g.root));
}

// \f.\g.!(f || g)
G_CFLOBDD MkNor(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    // return MkNot(MkOr(f.root, g.root));
    return G_CFLOBDD(MkNor(f.root, g.root));
}

// \f.\g.(f == g)
G_CFLOBDD MkIff(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    return G_CFLOBDD(MkIff(f.root, g.root));
}

// \f.\g.(f != g)
G_CFLOBDD MkExclusiveOr(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    return G_CFLOBDD(MkExclusiveOr(f.root, g.root));
}

// \f.\g.(!f || g)
G_CFLOBDD MkImplies(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    return G_CFLOBDD(MkImplies(f.root, g.root));
}

// \f.\g.(f && !g)
G_CFLOBDD MkMinus(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    return G_CFLOBDD(MkMinus(f.root, g.root));
}

// \f.\g.(!g || f)
G_CFLOBDD MkQuotient(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    return G_CFLOBDD(MkQuotient(f.root, g.root));
}

// \f.\g.(g && !f)
G_CFLOBDD MkNotQuotient(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    return G_CFLOBDD(MkNotQuotient(f.root, g.root));
}

// \f.\g.f
G_CFLOBDD MkFirst(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    return G_CFLOBDD(MkFirst(f.root, g.root));
}

// \f.\g.!f
G_CFLOBDD MkNotFirst(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
    return G_CFLOBDD(MkNotFirst(f.root, g.root));
}

// \f.\g.g
G_CFLOBDD MkSecond(G_CFLOBDD f, G_CFLOBDD g)
{
  return G_CFLOBDD(MkSecond(f.root, g.root));
}

// \f.\g.!g
G_CFLOBDD MkNotSecond(G_CFLOBDD f, G_CFLOBDD g)
{
  return G_CFLOBDD(MkNotSecond(f.root, g.root));
}

// \f.\g.(f + g)
G_CFLOBDD MkPlus(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
	return G_CFLOBDD(MkPlus(f.root, g.root));
}

// \f.\g.(f * g)
G_CFLOBDD MkTimes(G_CFLOBDD f, G_CFLOBDD g)
{
    assert(f.root->level == g.root->level);
    assert(f.root->grammar == g.root->grammar);
	return G_CFLOBDD(MkTimes(f.root, g.root));
}

// // N-ary operations on CFLOBDDs -----------------------------------

// \f1. ... \fk.(f1 && ... && fk)
// CFLOBDD  MkAnd(int N, ...)
// {
// 	CFLOBDD  temp;

// 	assert(N >= 2);
// 	va_list ap;
// 	va_start(ap, N);
// 	temp = va_arg(ap, CFLOBDD);
// 	for (int k = 1; k < N; k++) {
// 		temp = MkAnd(temp, va_arg(ap, CFLOBDD));
// 	}
// 	va_end(ap);
// 	return temp;
// }

// // \f1. ... \fk.!(f1 && ... && fk)
// CFLOBDD  MkNand(int N, ...)
// {
// 	CFLOBDD  temp;

// 	assert(N >= 2);
// 	va_list ap;
// 	va_start(ap, N);
// 	temp = va_arg(ap, CFLOBDD);
// 	for (int k = 1; k < N; k++) {
// 		temp = MkAnd(temp, va_arg(ap, CFLOBDD));
// 	}
// 	va_end(ap);
// 	return MkNot(temp);
// }

// // \f1. ... \fk.(f1 || ... || fk)
// CFLOBDD  MkOr(int N, ...)
// {
// 	CFLOBDD  temp;

// 	assert(N >= 2);
// 	va_list ap;
// 	va_start(ap, N);
// 	temp = va_arg(ap, CFLOBDD);
// 	for (int k = 1; k < N; k++) {
// 		temp = MkOr(temp, va_arg(ap, CFLOBDD));
// 	}
// 	va_end(ap);
// 	return temp;
// }

// // \f1. ... \fk.!(f1 || ... || fk)
// CFLOBDD  MkNor(int N, ...)
// {
// 	CFLOBDD  temp;

// 	assert(N >= 2);
// 	va_list ap;
// 	va_start(ap, N);
// 	temp = va_arg(ap, CFLOBDD);
// 	for (int k = 1; k < N; k++) {
// 		temp = MkOr(temp, va_arg(ap, CFLOBDD));
// 	}
// 	va_end(ap);
// 	return MkNot(temp);
// }

// Ternary operations on CFLOBDDs --------------------------------------

// \a.\b.\c.(a && b) || (!a && c)
G_CFLOBDD MkIfThenElse(G_CFLOBDD f, G_CFLOBDD g, G_CFLOBDD h)
{
  return G_CFLOBDD(MkIfThenElse(f.root, g.root, h.root));
}

// \a.\b.\c.(b && !a) || (c && !a) || (b && c)
G_CFLOBDD MkNegMajority(G_CFLOBDD f, G_CFLOBDD g, G_CFLOBDD h)
{
  return G_CFLOBDD(MkNegMajority(f.root, g.root, h.root));
}

// \f. f | (x_i = val)
G_CFLOBDD MkRestrict(G_CFLOBDD f, unsigned int i, bool val)
{
  return G_CFLOBDD(MkRestrict(f.root, i, val));
}

// \f. exists x_i : f
G_CFLOBDD MkExists(G_CFLOBDD f, unsigned int i)
{
  return G_CFLOBDD(MkExists(f.root, i));
}

// \f. forall x_i : f
G_CFLOBDD MkForall(G_CFLOBDD f, unsigned int i)
{
  return G_CFLOBDD(MkForall(f.root, i));
}

// Other operations on CFLOBDDs --------------------------------------

bool DependsOn(G_CFLOBDD f, int i)
{
	// // horrible performance!
	// if (MkRestrict(f, i, false) == f) {
	// 	assert(MkRestrict(f, i, true) == f); // or this algorithm is bad
	// 	return false;
	// }
	// assert(MkRestrict(f, i, true) != f);
	return true;
}

bool IsPositiveCube(G_CFLOBDD f)
{
	// return IsPositiveCubeInt(f, 0);
    return false; // Temporary
}

bool IsPositiveCubeInt(G_CFLOBDD f, int least)
{
	// base case:
	if (f == MkFalse(f.root->level, f.root->grammar)) return false;
	if (f == MkTrue(f.root->level, f.root->grammar)) return true;

	// recursive step:
	// for (int xi = least; xi < G_CFLOBDD::maxLevel; xi++) {
	// 	if (DependsOn(f, xi)) {
	// 		if (MkRestrict(f, xi, false) != MkFalse())
	// 			return false;
	// 		if (!IsPositiveCubeInt(MkRestrict(f, xi, true), xi + 1))
	// 			return false;
	// 	}
	// }
	return true;
}

bool SupportSetIs(G_CFLOBDD f, const std::vector<int> &ss)
{
	assert(&ss != NULL);
	// std::vector<int> *truess = GetSupportSet(f);
	// bool rc = (ss == *truess);
	// delete truess;
	// return rc;
    return false; // Temporary
}

std::vector<int> GetSupportSet(G_CFLOBDD f)
// Returns a new, sorted-in-increasing-order array
// containing the support set (the set of VarNums
// corresponding to indices of projection functions upon which bdd
// depends -- for (x0+x1*x3), it would be [0, 1, 3]).
//
// Delete it when you are done.
{
	// Decent algorithm: find the index associated with each fork node on EVERY path.  kind of ugly, but better than this quickie:

	std::vector<int> vec;
	// for (unsigned int i = 0; i < G_CFLOBDD::maxLevel; i++) {
	// 	if (DependsOn(f, i)) {
	// 		vec.push_back(i);
	// 	}
	// }
	// assert(vec != NULL);
	return vec;
}

void PrintCFLOBDD(G_CFLOBDD f)
{
  std::cout << f << std::endl;
}	

} // namespace G_CFL_OBDD
