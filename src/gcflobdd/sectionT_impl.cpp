#include "sectionT.cpp"

namespace G_CFL_OBDD {
    template class  SectionT<ReturnMapHandle<int>>;
	template std::ostream& operator<< (std::ostream & out, const SectionT<ReturnMapHandle<int>> &c);
}