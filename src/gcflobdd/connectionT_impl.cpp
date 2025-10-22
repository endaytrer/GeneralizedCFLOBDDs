#include "connectionT.cpp"

namespace G_CFL_OBDD {
    template class  ConnectionT<ReturnMapHandle<int>>;
	template std::ostream& operator<< (std::ostream & out, const ConnectionT<ReturnMapHandle<int>> &c);
}