#ifndef G_CFLOBDD_INT_GUARD
#define G_CFLOBDD_INT_GUARD

#include <iostream>
#include <fstream>
#include "../gcflobdd/gcflobdd_t.h"

namespace G_CFL_OBDD {

typedef G_CFLOBDD_T<int> G_CFLOBDD;

// CFLOBDD-creation operations --------------------------------------
extern G_CFLOBDD MkTrue(int level, const std::shared_ptr<Grammar>& grammar);                             // Representation of \x.true
extern G_CFLOBDD MkFalse(int level, const std::shared_ptr<Grammar>& grammar);                            // Representation of \x.false
extern G_CFLOBDD MkProjection(unsigned int i, int level, const std::shared_ptr<Grammar>& grammar);         // Representation of \x.x_i
extern G_CFLOBDD MkParity(int level, const std::shared_ptr<Grammar>& grammar);                           // Representation of parity function

// Unary operations on CFLOBDDs --------------------------------------
extern G_CFLOBDD MkNot(G_CFLOBDD f);                     // \f.!f

// Binary operations on CFLOBDDs --------------------------------------
extern G_CFLOBDD MkAnd(G_CFLOBDD f, G_CFLOBDD g);          // \f.\g.(f && g)
extern G_CFLOBDD MkNand(G_CFLOBDD f, G_CFLOBDD g);         // \f.\g.!(f && g)
extern G_CFLOBDD MkOr(G_CFLOBDD f, G_CFLOBDD g);           // \f.\g.(f || g)
extern G_CFLOBDD MkNor(G_CFLOBDD f, G_CFLOBDD g);          // \f.\g.!(f || g)
extern G_CFLOBDD MkIff(G_CFLOBDD f, G_CFLOBDD g);          // \f.\g.(f == g)
extern G_CFLOBDD MkExclusiveOr(G_CFLOBDD f, G_CFLOBDD g);  // \f.\g.(f != g)
extern G_CFLOBDD MkImplies(G_CFLOBDD f, G_CFLOBDD g);      // \f.\g.(!f || g)
extern G_CFLOBDD MkMinus(G_CFLOBDD f, G_CFLOBDD g);        // \f.\g.(f && !g)
extern G_CFLOBDD MkQuotient(G_CFLOBDD f, G_CFLOBDD g);     // \f.\g.(!g || f)
extern G_CFLOBDD MkNotQuotient(G_CFLOBDD f, G_CFLOBDD g);  // \f.\g.(g && !f)
extern G_CFLOBDD MkFirst(G_CFLOBDD f, G_CFLOBDD g);        // \f.\g.f
extern G_CFLOBDD MkNotFirst(G_CFLOBDD f, G_CFLOBDD g);     // \f.\g.!f
extern G_CFLOBDD MkSecond(G_CFLOBDD f, G_CFLOBDD g);       // \f.\g.g
extern G_CFLOBDD MkNotSecond(G_CFLOBDD f, G_CFLOBDD g);    // \f.\g.!g

extern G_CFLOBDD MkPlus(G_CFLOBDD f, G_CFLOBDD g);      // \f.\g.(f + g)
extern G_CFLOBDD MkTimes(G_CFLOBDD f, G_CFLOBDD g);     // \f.\g.(f * g)

// N-ary operations on CFLOBDDs --------------------------------------
extern G_CFLOBDD MkAnd(int N, ...);                    // \f1. ... \fk.(f1 && ... && fk)
extern G_CFLOBDD MkNand(int N, ...);                   // \f1. ... \fk.!(f1 && ... && fk)
extern G_CFLOBDD MkOr(int N, ...);                     // \f1. ... \fk.(f1 || ... || fk)
extern G_CFLOBDD MkNor(int N, ...);                    // \f1. ... \fk.!(f1 || ... || fk)

// Ternary operations on CFLOBDDs --------------------------------------
extern G_CFLOBDD MkIfThenElse(G_CFLOBDD f, G_CFLOBDD g, G_CFLOBDD h);  // \a.\b.\c.(a && b) || (!a && c)
extern G_CFLOBDD MkNegMajority(G_CFLOBDD f, G_CFLOBDD g, G_CFLOBDD h); // \a.\b.\c.(b && !a) || (c && !a) || (b && c)

extern G_CFLOBDD MkRestrict(G_CFLOBDD f, unsigned int i, bool val);  // \f. f | (x_i = val)
extern G_CFLOBDD MkExists(G_CFLOBDD f, unsigned int i);              // \f. exists x_i : f
extern G_CFLOBDD MkForall(G_CFLOBDD f, unsigned int i);              // \f. forall x_i : f
extern G_CFLOBDD MkCompose(G_CFLOBDD f, int i, G_CFLOBDD g);  // \f. f | (x_i = g)

// Other operations on CFLOBDDs ---------------------------------------------------
extern bool DependsOn(G_CFLOBDD f, int i);
extern bool IsPositiveCube(G_CFLOBDD f);
extern bool IsPositiveCubeInt(G_CFLOBDD f, int least);
extern bool SupportSetIs(G_CFLOBDD f, const std::vector<int> &ss);
extern std::vector<int> GetSupportSet(G_CFLOBDD f);

extern void PrintCFLOBDD(G_CFLOBDD f);

} // namespace G_CFL_OBDD


#endif
