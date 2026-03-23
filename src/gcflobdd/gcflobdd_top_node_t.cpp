#include "gcflobdd_top_node_t.h"
#include "gcflobdd_node.h"
#include "../utils/ref_ptr.h"
#include "../utils/bool_op.h"
#include "../ops/cross_product.h"
#include <unordered_set>
#include <unordered_map>
#include "../ops/gcflobdd_node_ops.h"


namespace G_CFL_OBDD{

    // Initializations of static members ---------------------------------

    template <typename T>
    unsigned int const G_CFLOBDDTopNodeT<T>::maxLevel = G_CFLOBDDMaxLevel;

    template <typename T>
    Hashset<G_CFLOBDDTopNodeT<T>> *G_CFLOBDDTopNodeT<T>::computedCache = new Hashset<G_CFLOBDDTopNodeT<T>>(10000);

    // Constructors/Destructor -------------------------------------------

    template<typename T>
    G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeT(G_CFLOBDDNode *n, ReturnMapHandle<T> &mapHandle)
    {
    #ifdef CFLOBDDTopNodeTDebug
        if (n->level >= 1) {
            G_CFLOBDDInternalNode *in = (G_CFLOBDDInternalNode *)n;
            // Check for inconsistencies between the entries in the return maps of in's BConnections and mapHandle.Size()
            unsigned int bound = mapHandle.Size();
            for (unsigned int i = 0; i < in->numBConnections; i++) {
                for (unsigned int j = 0; j < in->BConnection[i].returnMapHandle.mapContents->mapArray.size(); j++) {
                    if (in->BConnection[i].returnMapHandle.Lookup(j) >= bound) {
                        std::cout << "Inconsistent CFLOBDDTopNodeT construction" << std::endl;
                        std::cout << "numExits = " << n->numExits << std::endl;
                        std::cout << "level = " << n->level << std::endl;
                        std::cout << "bound = " << bound << std::endl;
                        std::cout << "index j = " << j << std::endl;
                        std::cout << "map at j = " << in->BConnection[i].returnMapHandle.Lookup(j) << std::endl;
                        std::cout << "map = " << in->BConnection[i].returnMapHandle << std::endl;
                        abort();
                    }
                }
            }
        }
    #endif

        rootConnection = ConnectionT<ReturnMapHandle<T>>(n, mapHandle);
        level = n->level;
        grammar = std::make_shared<Grammar>();
        grammar->root = n->grammar;
    }

    template<typename T>
    G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeT(G_CFLOBDDNodeHandle &nodeHandle, ReturnMapHandle<T> &mapHandle)
    {
    #ifdef G_CFLOBDDTopNodeTDebug
        if (nodeHandle.handleContents->level >= 1) {
            G_CFLOBDDInternalNode *in = (G_CFLOBDDInternalNode *)nodeHandle.handleContents;
            // Check for inconsistencies between the entries in the return maps of in's BConnections and mapHandle.Size()
            unsigned int bound = mapHandle.Size();
            for (unsigned int i = 0; i < in->numBConnections; i++) {
                for (unsigned int j = 0; j < in->BConnection[i].returnMapHandle.mapContents->mapArray.size(); j++) {
                    if (in->BConnection[i].returnMapHandle.Lookup(j) >= bound) {
                        std::cout << "Inconsistent CFLOBDDTopNodeT construction" << std::endl;
                        std::cout << "numExits = " << nodeHandle.handleContents->numExits << std::endl;
                        std::cout << "level = " << nodeHandle.handleContents->level << std::endl;
                        std::cout << "bound = " << bound << std::endl;
                        std::cout << "index j = " << j << std::endl;
                        std::cout << "map at j = " << in->BConnection[i].returnMapHandle.Lookup(j) << std::endl;
                        std::cout << "map = " << in->BConnection[i].returnMapHandle << std::endl;
                        abort();
                    }
                }
            }
        }
    #endif

        rootConnection = ConnectionT<ReturnMapHandle<T>>(nodeHandle, mapHandle);
        level = nodeHandle.handleContents->level;
        grammar = std::make_shared<Grammar>();
        grammar->root = nodeHandle.handleContents->grammar;
    }

    template<typename T>
    G_CFLOBDDTopNodeT<T>::~G_CFLOBDDTopNodeT()
    {
    }

    // Hash
    template <typename T>
    unsigned int G_CFLOBDDTopNodeT<T>::Hash(unsigned int modsize)
    {
        return rootConnection.Hash(modsize);
    }

    // Overloaded !=
    template <typename T>
    bool G_CFLOBDDTopNodeT<T>::operator!= (const G_CFLOBDDTopNodeT<T> & C)
    {
        return rootConnection != C.rootConnection;
    }

    // Overloaded ==
    template <typename T>
    bool G_CFLOBDDTopNodeT<T>::operator== (const G_CFLOBDDTopNodeT<T> & C)
    {
        return rootConnection == C.rootConnection;
    }

    // print
    template <typename T>
    std::ostream& G_CFLOBDDTopNodeT<T>::print(std::ostream & out) const
    {
        out << *(rootConnection.entryPointHandle) << std::endl;
        out << rootConnection.returnMapHandle << std::endl;
        return out;
    }

    template <typename T>
    std::ostream& operator<< (std::ostream & out, const G_CFLOBDDTopNodeT<T> &d)
    {
        d.print(out);
        return(out);
    }

    template <typename T>
    void G_CFLOBDDTopNodeT<T>::CountNodesAndEdges(Hashset<G_CFLOBDDNodeHandle>* visitedNodes, Hashset<G_CFLOBDDReturnMapBody>* visitedEdges,
        unsigned int& nodeCount, unsigned int& edgeCount)
    {
        rootConnection.entryPointHandle->handleContents->CountNodesAndEdges(visitedNodes, visitedEdges, nodeCount, edgeCount);
        edgeCount += rootConnection.returnMapHandle.Size();
    }

    template <typename T>
    void G_CFLOBDDTopNodeT<T>::CountPaths(Hashset<G_CFLOBDDNodeHandle>* visitedNodes)
    {
        rootConnection.entryPointHandle->handleContents->CountPaths(visitedNodes);
    }

    template <typename T>
    void G_CFLOBDDTopNodeT<T>::PrintYield(std::ostream & out) const
    {
        std::unordered_map<int, std::vector<std::string>> yield_strings;
        rootConnection.entryPointHandle->handleContents->PrintYield(yield_strings);
        for (const auto& pair : yield_strings) {
            for (const auto& str : pair.second) {
                out << str << " " << rootConnection.returnMapHandle.Lookup(pair.first) << std::endl;
            }
        }
        out << std::endl;
    }

    template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    // ApplyAndReduce -----------------------------------------------------
    ApplyAndReduce(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n1,
                typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n2,
                BoolOp op
                )
    {
        // Perform 2-way cross product of n1 and n2
        PairProductMapHandle MapHandle;
        G_CFLOBDDNodeHandle n = PairProduct(*(n1->rootConnection.entryPointHandle),
            *(n2->rootConnection.entryPointHandle),
            MapHandle);

        // unsigned int nCount = 0, eCount = 0;
        // Hashset<G_CFLOBDDNodeHandle>* visitedNodes = new Hashset<G_CFLOBDDNodeHandle>(1000);
        // Hashset<G_CFLOBDDReturnMapBody>* visitedEdges = new Hashset<G_CFLOBDDReturnMapBody>(1000);
        // n.handleContents->CountNodesAndEdges(visitedNodes, visitedEdges, nCount, eCount);
        // delete visitedNodes;
        // delete visitedEdges;
        // std::cout << "After PairProduct: Nodes = " << nCount << ", Edges = " << eCount << std::endl;

        // Create returnMapHandle from MapHandle: Fold the pairs in MapHandle by applying
        // [n1->rootConnection.returnMapHandle, n2->rootConnection.returnMapHandle]
        // (component-wise) to each pair.
        ReturnMapHandle<T> returnMapHandle;
        std::unordered_map<T, unsigned int> reduction_map;
        ReductionMapHandle reductionMapHandle (MapHandle.Size());
        unsigned int iterator = 0;
        while (iterator < MapHandle.Size()){
            T c1, c2;
            int first, second;
            first = MapHandle[iterator].First();
            second = MapHandle[iterator].Second();
            c1 = n1->rootConnection.returnMapHandle.Lookup(first);
            c2 = n2->rootConnection.returnMapHandle.Lookup(second);
            T val = op[c1][c2];
            if (reduction_map.find(val) == reduction_map.end()){
                returnMapHandle.AddToEnd(val);
                reduction_map.insert(std::make_pair(val, returnMapHandle.Size() - 1));
                reductionMapHandle.AddToEnd(returnMapHandle.Size() - 1);
            }
            else{
                reductionMapHandle.AddToEnd(reduction_map[val]);
            }
            iterator++;
        }
        returnMapHandle.Canonicalize();
        reductionMapHandle.Canonicalize();

        G_CFLOBDDNodeHandle reduced_n = n.Reduce(reductionMapHandle, returnMapHandle.Size());

        // nCount = 0; eCount = 0;
        // Hashset<G_CFLOBDDNodeHandle>* visitedNodes1 = new Hashset<G_CFLOBDDNodeHandle>(1000);
        // Hashset<G_CFLOBDDReturnMapBody>* visitedEdges1 = new Hashset<G_CFLOBDDReturnMapBody>(1000);
        // reduced_n.handleContents->CountNodesAndEdges(visitedNodes1, visitedEdges1, nCount, eCount);
        // delete visitedNodes1;
        // delete visitedEdges1;
        // std::cout << "After Reduce: Nodes = " << nCount << ", Edges = " << eCount << std::endl;

        // Create and return G_CFLOBDDTopNode
        return(new G_CFLOBDDTopNodeT<T>(reduced_n, returnMapHandle));
    }


    template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    ApplyAndReduce(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n1,
                typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n2,
                T(*func)(T, T)
                )
    {
        // Perform 2-way cross product of n1 and n2
        PairProductMapHandle MapHandle;
        G_CFLOBDDNodeHandle n = PairProduct(*(n1->rootConnection.entryPointHandle),
                *(n2->rootConnection.entryPointHandle),
                MapHandle);

        // Create returnMapHandle from MapHandle: Fold the pairs in MapHandle by applying
        // [n1->rootConnection.returnMapHandle, n2->rootConnection.returnMapHandle]
        // (component-wise) to each pair.
        
        ReturnMapHandle<T> returnMapHandle;

        std::unordered_map<T, unsigned int> reduction_map;
        ReductionMapHandle reductionMapHandle;
        unsigned int iterator = 0;
        while (iterator < MapHandle.Size()){
            T c1, c2;
            int first, second;
            first = MapHandle[iterator].First();
            second = MapHandle[iterator].Second();
            c1 = n1->rootConnection.returnMapHandle.Lookup(first);
            c2 = n2->rootConnection.returnMapHandle.Lookup(second);
            T val = (*func)(c1, c2);
            if (reduction_map.find(val) == reduction_map.end()){
                returnMapHandle.AddToEnd(val);
                reduction_map.insert(std::make_pair(val, returnMapHandle.Size() - 1));
                reductionMapHandle.AddToEnd(returnMapHandle.Size() - 1);
            }
            else{
                reductionMapHandle.AddToEnd(reduction_map[val]);
            }
            iterator++;
        }

            returnMapHandle.Canonicalize();
            reductionMapHandle.Canonicalize();

            G_CFLOBDDNodeHandle reduced_n = n.Reduce(reductionMapHandle, returnMapHandle.Size());
            return(new G_CFLOBDDTopNodeT<T>(reduced_n, returnMapHandle));
    }


    template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    ApplyAndReduce(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n1,
                typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n2,
                typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr n3,
                BoolOp3 op
                )
    {
        // TODO: Implement this function
        std::cout << "ApplyAndReduce with 3 arguments not implemented" << std::endl;
        abort();
        return nullptr;
    }

    // \f.\g.(f + g)
    template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    MkPlusTopNode(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f,
                typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g
                )
    {
        return ApplyAndReduce<T>(f, g, PlusFunc);
    }

    // Pointwise addition: \f.\g.(f + g)
    template<typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    operator+(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f, typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g)
    {
        return MkPlusTopNode<T>(f, g);
    }


    // \f.\g.(f ^ g)
    template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    MkExorTopNode(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f,
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g
    )
    {
        return ApplyAndReduce<T>(f, g, exclusiveOrOp);
    }

    // Pointwise addition: \f.\g.(f ^ g)
    template<typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    operator^(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f, typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g)
    {
        return MkExorTopNode<T>(f, g);
    }

    // Left scalar-multiplication: \c:int.\g.(c * g)
    template <typename T, typename T1>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    MkLeftScalarTimesTopNode(T1 c, typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g)
    {
        if (c == 1) return g;  // Nothing need be performed

        G_CFLOBDDNodeHandle eph;
        typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr ans;
        if (c == 0) {  // Special case
            eph = MkNoDistinction(g->level, g->grammar->root);
            ReturnMapHandle<T> rmh = c * g->rootConnection.returnMapHandle;
            // Perform reduction on eph, with respect to the common elements that rmh maps tssogether
            ReductionMapHandle inducedReductionMapHandle;
            ReturnMapHandle<T> inducedReturnMap;
            rmh.InducedReductionAndReturnMap(inducedReductionMapHandle, inducedReturnMap);
            G_CFLOBDDNodeHandle reduced_eph = eph.Reduce(inducedReductionMapHandle, inducedReturnMap.Size());

            // Create and return G_CFLOBDDTopNode
            return(new G_CFLOBDDTopNodeT<T>(reduced_eph, inducedReturnMap));
        }
        else {
            eph = *(g->rootConnection.entryPointHandle);
            ReturnMapHandle<T> rmh = c * g->rootConnection.returnMapHandle;
            return(new G_CFLOBDDTopNodeT<T>(eph, rmh));
        }
    }



    // Left scalar-multiplication: \c:int.\g.(c * g)
    template<typename T, typename T1>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    operator*(T1 c, typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g)
    {
        return MkLeftScalarTimesTopNode<T, T1>(c, g);
    }

    // Right scalar-multiplication: \f.\c:int.(f * c)
    template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    MkRightScalarTimesTopNode(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f, int c)
    {
        if (c == 1) return f;  // Nothing need be performed

        G_CFLOBDDNodeHandle eph;
        typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr ans;
        if (c == 0) {  // Special case
            eph = MkNoDistinction(f->level, f->grammar->root);
            ReturnMapHandle<T> rmh = f->rootConnection.returnMapHandle * c;

            // Perform reduction on eph, with respect to the common elements that rmh maps together
            ReductionMapHandle inducedReductionMapHandle;
            ReturnMapHandle<T> inducedReturnMap;
            rmh.InducedReductionAndReturnMap(inducedReductionMapHandle, inducedReturnMap);
            G_CFLOBDDNodeHandle reduced_eph = eph.Reduce(inducedReductionMapHandle, inducedReturnMap.Size());

            // Create and return G_CFLOBDDTopNode
            return(new G_CFLOBDDTopNodeT<T>(reduced_eph, inducedReturnMap));
        }
        else {
            eph = *(f->rootConnection.entryPointHandle);
            ReturnMapHandle<T> rmh = f->rootConnection.returnMapHandle * c;
            return(new G_CFLOBDDTopNodeT<T>(eph, rmh));
        }
    }

    // Right scalar-multiplication: \f.\c:int.(f * c)
    template<typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    operator*(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f, int c)
    {
        return MkRightScalarTimesTopNode<T>(f, c);
    }

    // Pointwise multiplication: \f.\g.(f * g) ---------------------------------------
    template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    MkTimesTopNode(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f,
                typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g
                )
    {
        return ApplyAndReduce<T>(f, g, TimesFunc);
    }

    // Pointwise multiplication: \f.\g.(f * g) 
    template <typename T>
    typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr
    operator*(typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr f, typename G_CFLOBDDTopNodeT<T>::G_CFLOBDDTopNodeTRefPtr g)
    {
        return MkTimesTopNode<T>(f, g);
    }

}