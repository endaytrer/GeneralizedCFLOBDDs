#include "gcflobdd_top_node_t.cpp"

namespace G_CFL_OBDD
{
    template class G_CFLOBDDTopNodeT<int>;

    template std::ostream& operator<< (std::ostream & out, const G_CFLOBDDTopNodeT<int> &d);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr ApplyAndReduce<int>(
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr n1,
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr n2,
        BoolOp op);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr ApplyAndReduce<int>(
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr n1,
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr n2,
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr n3,
        BoolOp3 op);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr MkPlusTopNode<int>(
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr f,
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr g);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr operator+<int>(
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr f, 
	    G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr g);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr MkExorTopNode<int>(
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr f,
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr g);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr operator^<int>(
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr f, 
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr g);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr MkLeftScalarTimesTopNode<int, int>(
        int c, G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr g);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr operator*<int, int>(
        int c, G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr g);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr MkRightScalarTimesTopNode<int>(
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr f, int c);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr operator*<int>(
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr f, int c);
    template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr MkTimesTopNode<int>(
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr f,
        G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr g);
    // template G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr operator*<int>(
    //     G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr f,
    //     G_CFLOBDDTopNodeT<int>::G_CFLOBDDTopNodeTRefPtr g);

} // namespace G_CFL_OBDD
