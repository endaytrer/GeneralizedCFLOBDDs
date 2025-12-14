#include "hardware_tests.h"
#include "../grammar/grammar.h"
#include "../gcflobdd/gcflobdd_t.h"
#include "../ops/gcflobdd_int.h"
#include "../ops/gcflobdd_node_ops.h"
#include <iostream>
#include <chrono>

using namespace G_CFL_OBDD;
using namespace std;
using namespace HardwareBenchmarks;
using namespace std::chrono;

void HardwareBenchmarks::c17() {
    std::shared_ptr<Grammar> grammar = std::make_shared<Grammar>();
    // std::vector<std::string> productions = {
    //     "S 4 -> S 0 S 3",
    //     "S 3 -> S 0 S 2",
    //     "S 2 -> S 0 S 1",
    //     "S 1 -> S 0 S 0",
    //     "S 0 -> a"
    // };
    // std::vector<std::string> productions = {
    //     "S 1 -> S 0 S 0 S 0 S 0 S 0",
    //     "S 0 -> a"
    // };
    // std::vector<std::string> productions = {
    //     "S 3 -> S 2 S 2", // 8
    //     "S 2 -> S 1 S 1", // 4
    //     "S 1 -> S 0 S 0", // 2
    //     "S 0 -> a"
    // };
    std::vector<std::string> productions = {
        "S 0 -> BDD(5)"
    }; 
    grammar->constructGrammar(productions, "S 0");
    grammar->InstallNumVars();
    grammar->updateLevel();

    // Create G_CFLOBDD for c17 circuit
    auto start = high_resolution_clock::now();

    int max_level = 4;

    G_CFLOBDD gat1 = MkProjection(0, max_level, grammar);
    G_CFLOBDD gat2 = MkProjection(1, max_level, grammar);
    G_CFLOBDD gat3 = MkProjection(2, max_level, grammar);
    G_CFLOBDD gat6 = MkProjection(3, max_level, grammar);
    G_CFLOBDD gat7 = MkProjection(4, max_level, grammar);
    G_CFLOBDD gat10 = MkNand(gat1, gat3);
    G_CFLOBDD gat11 = MkNand(gat3, gat6);
    G_CFLOBDD gat16 = MkNand(gat2, gat11);
    G_CFLOBDD gat19 = MkNand(gat11, gat7);
    G_CFLOBDD gat22 = MkNand(gat10, gat16);
    G_CFLOBDD gat23 = MkNand(gat16, gat19);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    gat22.PrintYield();
    std::cout << "-----------------" << std::endl;
    gat23.PrintYield();

    // gat1.print(std::cout);

    // Hashset<G_CFLOBDDNodeHandle>* visitedNodesDuringGroupCountNodesAndEdges = new Hashset<G_CFLOBDDNodeHandle>(HASH_NUM_BUCKETS);
    // Hashset<G_CFLOBDDReturnMapBody>* visitedEdgesDuringGroupCountNodesAndEdges = new Hashset<G_CFLOBDDReturnMapBody>(HASH_NUM_BUCKETS);
    // // std::vector<G_CFLOBDD> gates = {gat22, gat23};
    // unsigned int totalNodeCount = 0, totalEdgeCount = 0;
    // unsigned int nodeCount = 0, edgeCount = 0;
    // gat22.GroupCountNodesAndEdges(totalNodeCount, totalEdgeCount,
    //     visitedNodesDuringGroupCountNodesAndEdges,
    //     visitedEdgesDuringGroupCountNodesAndEdges);
    // gat23.GroupCountNodesAndEdges(totalNodeCount, totalEdgeCount,
    //     visitedNodesDuringGroupCountNodesAndEdges,
    //     visitedEdgesDuringGroupCountNodesAndEdges);

    // // delete visitedEdgesDuringGroupCountNodesAndEdges;

    // cout << "c17 G_CFLOBDD created in " << duration.count() << " milliseconds." << endl;
    // cout << "Total Node Count: " << totalNodeCount << ", Total Edge Count: " << totalEdgeCount << endl;
    // cout << "Total Size (Nodes + Edges): " << totalNodeCount + totalEdgeCount << endl;

    // nodeCount = 0; edgeCount = 0;
    // gat1.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 1 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;
    // nodeCount = 0; edgeCount = 0;
    // gat2.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 2 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;
    // nodeCount = 0; edgeCount = 0;
    // gat3.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 3 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;
    // nodeCount = 0; edgeCount = 0;
    // gat6.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 6 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;
    // nodeCount = 0; edgeCount = 0;
    // gat7.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 7 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;
    // nodeCount = 0; edgeCount = 0;
    // gat10.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 10 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;
    // nodeCount = 0; edgeCount = 0;
    // gat11.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 11 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;
    // nodeCount = 0; edgeCount = 0;
    // gat16.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 16 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;
    // nodeCount = 0; edgeCount = 0;
    // gat19.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 19 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;
    // nodeCount = 0; edgeCount = 0;
    // gat22.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 22 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;
    // nodeCount = 0; edgeCount = 0;
    // gat23.CountNodesAndEdges(nodeCount, edgeCount);
    // cout << "Gate 23 - Node Count: " << nodeCount << ", Edge Count: " << edgeCount << endl;

}