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
    std::vector<std::string> productions = {
        "S 4 -> S 0 S 3",
        "S 3 -> S 0 S 2",
        "S 2 -> S 0 S 1",
        "S 1 -> S 0 S 0",
        "S 0 -> a"
    };
    grammar->constructGrammar(productions, "S 4");
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

    unsigned int nodeCount = 0, edgeCount = 0;
    unsigned int totalNodeCount = 0, totalEdgeCount = 0;
    gat22.CountNodesAndEdges(nodeCount, edgeCount);
    totalNodeCount += nodeCount;
    totalEdgeCount += edgeCount;
    nodeCount = 0; edgeCount = 0;
    gat23.CountNodesAndEdges(nodeCount, edgeCount);
    totalNodeCount += nodeCount;
    totalEdgeCount += edgeCount;

    cout << "c17 G_CFLOBDD created in " << duration.count() << " milliseconds." << endl;
    cout << "Total Node Count: " << totalNodeCount << ", Total Edge Count: " << totalEdgeCount << endl;
}