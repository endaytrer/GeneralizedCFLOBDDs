#ifndef G_CFLOBDD_TOP_NODE_INT_GUARD
#define G_CFLOBDD_TOP_NODE_INT_GUARD

#include "../gcflobdd/gcflobdd_top_node_t.h"

namespace G_CFL_OBDD {

typedef G_CFLOBDDTopNodeT<int> G_CFLOBDDTopNode;
typedef G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr G_CFLOBDDTopNodeIntRefPtr;

// G_CFLOBDDTopNode-creation operations --------------------------------------
extern G_CFLOBDDTopNodeIntRefPtr MkTrueTop(int level, const std::shared_ptr<Grammar>& grammar);                    // Representation of \x.true
extern G_CFLOBDDTopNodeIntRefPtr MkFalseTop(int level, const std::shared_ptr<Grammar>& grammar);                   // Representation of \x.false
extern G_CFLOBDDTopNodeIntRefPtr MkDistinction(unsigned int i, int level, const std::shared_ptr<Grammar>& grammar);  // Representation of \x.x_i
extern G_CFLOBDDTopNodeIntRefPtr MkParityTop(int level, const std::shared_ptr<Grammar>& grammar);                  // Representation of parity function

// Unary operations on CFLOBDDTopNodes --------------------------------------
extern G_CFLOBDDTopNodeIntRefPtr MkNot(G_CFLOBDDTopNodeIntRefPtr f);               // \f.!f

// Binary operations on CFLOBDDTopNodes --------------------------------------
extern G_CFLOBDDTopNodeIntRefPtr MkAnd(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);          // \f.\g.(f && g)
extern G_CFLOBDDTopNodeIntRefPtr MkNand(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);         // \f.\g.!(f && g)
extern G_CFLOBDDTopNodeIntRefPtr MkOr(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);           // \f.\g.(f || g)
extern G_CFLOBDDTopNodeIntRefPtr MkNor(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);          // \f.\g.!(f || g)
extern G_CFLOBDDTopNodeIntRefPtr MkIff(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);          // \f.\g.(f == g)
extern G_CFLOBDDTopNodeIntRefPtr MkExclusiveOr(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);  // \f.\g.(f != g)
extern G_CFLOBDDTopNodeIntRefPtr MkImplies(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);      // \f.\g.(!f || g)
extern G_CFLOBDDTopNodeIntRefPtr MkMinus(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);        // \f.\g.(f && !g)
extern G_CFLOBDDTopNodeIntRefPtr MkQuotient(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);     // \f.\g.(!g || f)
extern G_CFLOBDDTopNodeIntRefPtr MkNotQuotient(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);  // \f.\g.(g && !f)
extern G_CFLOBDDTopNodeIntRefPtr MkFirst(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);        // \f.\g.f
extern G_CFLOBDDTopNodeIntRefPtr MkNotFirst(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);     // \f.\g.!f
extern G_CFLOBDDTopNodeIntRefPtr MkSecond(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);       // \f.\g.g
extern G_CFLOBDDTopNodeIntRefPtr MkNotSecond(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);    // \f.\g.!g

extern G_CFLOBDDTopNodeIntRefPtr MkPlus(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);       // \f.\g.(f + g)
extern G_CFLOBDDTopNodeIntRefPtr MkTimes(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g);       // \f.\g.(f * g)

// Ternary operations on CFLOBDDTopNodes ------------------------------------

// \a.\b.\c.(a && b) || (!a && c)
extern G_CFLOBDDTopNodeIntRefPtr MkIfThenElse(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g, G_CFLOBDDTopNodeIntRefPtr h);

extern G_CFLOBDDTopNodeIntRefPtr MkRestrict(G_CFLOBDDTopNodeIntRefPtr n, unsigned int i, bool val);

// \a.\b.\c.(b && !a) || (c && !a) || (b && c)
extern G_CFLOBDDTopNodeIntRefPtr MkNegMajority(G_CFLOBDDTopNodeIntRefPtr f, G_CFLOBDDTopNodeIntRefPtr g, G_CFLOBDDTopNodeIntRefPtr h);

extern G_CFLOBDDTopNodeIntRefPtr MkExists(G_CFLOBDDTopNodeIntRefPtr f, unsigned int i);              // \f. exists x_i : f
extern G_CFLOBDDTopNodeIntRefPtr MkForall(G_CFLOBDDTopNodeIntRefPtr f, unsigned int i);              // \f. forall x_i : f

} // namespace CFL_OBDD

#endif
