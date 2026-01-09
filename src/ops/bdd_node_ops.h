#ifndef BDD_NODE_OPS_H
#define BDD_NODE_OPS_H

#include "../gcflobdd/gcflobdd_bdd_node.h"

namespace G_CFL_OBDD {

extern G_CFLOBDDNodeHandle MkNoDistinction_BDD(unsigned int numVars);
extern G_CFLOBDDNodeHandle MkDistinction_BDD(unsigned int numVars, unsigned int i);
extern G_CFLOBDDNodeHandle MkParity_BDD(unsigned int numVars);

}

#endif // BDD_NODE_OPS_H