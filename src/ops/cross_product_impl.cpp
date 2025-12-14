#include "cross_product_cache_utils.cpp"
#include "../gcflobdd/gcflobdd_node.h"
#include "../gcflobdd/gcflobdd_bdd_node.h"

namespace G_CFL_OBDD
{
    template class PairProductKey<G_CFLOBDDNodeHandle>;
    template class PairProductMemo<G_CFLOBDDNodeHandle>;

    template class PairProductKey<BDDNodeHandle>;
    template class PairProductMemo<BDDNodeHandle>;

} // namespace G_CFL_OBDD