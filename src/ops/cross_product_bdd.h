#ifndef CROSS_PRODUCT_BDD_GUARD
#define CROSS_PRODUCT_BDD_GUARD

#include "../gcflobdd/gcflobdd_node.h"
#include "../gcflobdd/gcflobdd_bdd_node.h"
#include "cross_product.h"
#include "cross_product_cache_utils.h"
#include <unordered_map>

namespace G_CFL_OBDD {

BDDNodeHandle PairProduct(BDDNodeHandle n1,
                          BDDNodeHandle n2,
                          PairProductMapHandle &pairProductMapHandle,
                          std::unordered_map<PairProductKey<BDDNodeHandle>, BDDNodeHandle, PairProductKey<BDDNodeHandle>::PairProductKey_Hash, PairProductKey<BDDNodeHandle>::PairProductKey_Equal> &pairProductCache
                        );

BDDNodeHandle PairProductBDDInternal(BDDInternalNode *n1,
                          BDDInternalNode *n2,
                          PairProductMapHandle &pairProductMapHandle,
                          std::unordered_map<PairProductKey<BDDNodeHandle>, BDDNodeHandle, PairProductKey<BDDNodeHandle>::PairProductKey_Hash, PairProductKey<BDDNodeHandle>::PairProductKey_Equal> &pairProductCache
                        );

G_CFLOBDDNodeHandle PairProduct(G_CFLOBDDBDDNode* n1,
                                G_CFLOBDDBDDNode* n2,
                                PairProductMapHandle &pairProductMap
                               );


void InitBDDPairProductCache();
void DisposeOfBDDPairProductCache();

}

#endif // CROSS_PRODUCT_BDD_GUARD