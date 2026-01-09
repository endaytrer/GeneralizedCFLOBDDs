#include "pair_T.cpp"
#include "../gcflobdd/gcflobdd_node.h"
#include "../gcflobdd/return_map_T.h"

using namespace G_CFL_OBDD;

// template class Pair_T<int, int>;
template class Pair_T<G_CFLOBDDNodeHandle, ReturnMapHandle<int>>;