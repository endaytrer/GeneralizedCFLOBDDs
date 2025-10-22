#ifndef G_CFLOBDD_NODE_OPS_GUARD
#define G_CFLOBDD_NODE_OPS_GUARD

#include <iostream>
#include <fstream>
#include "../gcflobdd/gcflobdd_node.h"
#include "../gcflobdd/return_map_T.h"
#include <unordered_map>

namespace G_CFL_OBDD {

extern G_CFLOBDDNodeHandle MkNoDistinction(unsigned int level, std::shared_ptr<GrammarNode>& grammar);
extern G_CFLOBDDNodeHandle MkDistinction(unsigned int level, unsigned int i, std::shared_ptr<GrammarNode>& grammar);
extern G_CFLOBDDNodeHandle Restrict(G_CFLOBDDNodeHandle g, unsigned int i, bool val, G_CFLOBDDReturnMapHandle &MapHandle);
extern G_CFLOBDDNodeHandle Restrict(G_CFLOBDDInternalNode *g, unsigned int i, bool val, G_CFLOBDDReturnMapHandle &MapHandle);
extern G_CFLOBDDNodeHandle MkParity(unsigned int level, std::shared_ptr<GrammarNode>& grammar);

}

#endif // G_CFLOBDD_NODE_OPS_GUARD