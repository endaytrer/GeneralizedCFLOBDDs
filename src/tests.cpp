#include "tests.h"
#include <iostream>
#include "grammar/grammar.h"
#include "gcflobdd/gcflobdd_t.h"
#include "ops/gcflobdd_int.h"
#include "visualization/visualize.h"
#include "hardware_benchmarks/hardware_tests.h"
using namespace G_CFL_OBDD;
using namespace std;

std::shared_ptr<Grammar> generateSampleGrammar() {
    std::vector<std::string> productions = {
        "S 3 -> S 0 S 2",
        "S 2 -> S 0 S 1",
        "S 1 -> S 0 S 0",
        "S 0 -> a"
    };

    Grammar grammar;
    grammar.constructGrammar(productions, "S 3");
    grammar.InstallNumVars();
    grammar.updateLevel();
    return std::make_shared<Grammar>(grammar);
}

void Tests::testMkTrueAndFalse() {
    std::shared_ptr<Grammar> grammar = generateSampleGrammar();
    G_CFLOBDD trueFunc = MkTrue(3, grammar);
    G_CFLOBDD falseFunc = MkFalse(3, grammar);

    cout << "True Function:" << endl;
    PrintCFLOBDD(trueFunc);
    cout << "False Function:" << endl;
    PrintCFLOBDD(falseFunc);

    std::cout << "trueFunc == falseFunc nodeHandle: " 
              << (*(trueFunc.root->rootConnection.entryPointHandle) == *(falseFunc.root->rootConnection.entryPointHandle) ? "True" : "False") << std::endl;
}

void Tests::testMkProjection() {
    std::shared_ptr<Grammar> grammar = generateSampleGrammar();
    G_CFLOBDD projFunc = MkProjection(2, 3, grammar);

    cout << "Projection Function (x_2):" << endl;
    PrintCFLOBDD(projFunc);
    unsigned int nodeCount = 0, edgeCount = 0;
    projFunc.CountNodesAndEdges(nodeCount, edgeCount);
    std::cout << "Node Count: " << nodeCount << ", Edge Count: " << edgeCount << std::endl;
}

void Tests::testParity() {
    std::shared_ptr<Grammar> grammar = generateSampleGrammar();
    G_CFLOBDD parityFunc = MkParity(3, grammar);

    cout << "Parity Function:" << endl;
    PrintCFLOBDD(parityFunc);
}

void Tests::testDataVisualization() {
    std::shared_ptr<Grammar> grammar = generateSampleGrammar();
    G_CFLOBDD parityFunc = MkParity(3, grammar);

    cout << "Visualizing Parity Function:" << endl;
    Visualization::visualizeGCFLOBDD(parityFunc);
}

void Tests::testMkNot() {
    std::shared_ptr<Grammar> grammar = generateSampleGrammar();
    G_CFLOBDD trueFunc = MkTrue(3, grammar);
    G_CFLOBDD notTrueFunc = MkNot(trueFunc);
    G_CFLOBDD falseFunc = MkFalse(3, grammar);

    cout << "Not True Function:" << endl;
    PrintCFLOBDD(notTrueFunc);

    cout << "False Function:" << endl;
    PrintCFLOBDD(falseFunc);

    std::cout << "notTrueFunc == falseFunc: " 
              << (notTrueFunc == falseFunc ? "True" : "False") << std::endl;
}

void Tests::testCrossProduct() {
    std::shared_ptr<Grammar> grammar = generateSampleGrammar();
    G_CFLOBDD x0 = MkProjection(0, 3, grammar);
    G_CFLOBDD x1 = MkProjection(1, 3, grammar);
    G_CFLOBDD x2 = MkProjection(2, 3, grammar);
    G_CFLOBDD x0_and_x1 = MkAnd(x0, x1);
    G_CFLOBDD x0_and_x1_and_x2 = MkAnd(x0_and_x1, x2);
    G_CFLOBDD x1_and_x2 = MkAnd(x1, x2);
    G_CFLOBDD x0_and_x1_x2_p = MkAnd(x0, x1_and_x2);
    std::cout << "x0_and_x1_x2_p == x1_and_x0_and_x2: " 
              << (x0_and_x1_x2_p == x0_and_x1_and_x2 ? "True" : "False") << std::endl;

    unsigned int nodeCount = 0, edgeCount = 0;
    x0_and_x1_and_x2.CountNodesAndEdges(nodeCount, edgeCount);
    std::cout << "Node Count: " << nodeCount << ", Edge Count: " << edgeCount << std::endl;

    nodeCount = 0; edgeCount = 0;
    x0_and_x1_x2_p.CountNodesAndEdges(nodeCount, edgeCount);
    std::cout << "Node Count: " << nodeCount << ", Edge Count: " << edgeCount << std::endl;

    G_CFLOBDDNodeHandle::GarbageCollectCanonicalNodeTable();

}

void Tests::testC17() {
    HardwareBenchmarks::c17();
}

void Tests::testC432() {
    HardwareBenchmarks::c432();
}

void Tests::testC880() {
    HardwareBenchmarks::c880();
}

void Tests::testC6288_8() {
    HardwareBenchmarks::c6288_8();
}

void Tests::testC6288_12() {
    HardwareBenchmarks::c6288_12();
}

void Tests::testC6288_9() {
    HardwareBenchmarks::c6288_9();
}

void Tests::testC6288_10() {
    HardwareBenchmarks::c6288_10();
}

void RunInit() {
    G_CFLOBDDNodeHandle::InitLeafNodes();
    G_CFLOBDDNodeHandle::InitReduceCache();
}

void Tests::runTests(std::string testName) {

    RunInit();
    if (testName == "testMkTrueAndFalse") {
        testMkTrueAndFalse();
    } else if (testName == "testMkProjection") {
        testMkProjection();
    } else if (testName == "testParity") {
        testParity();
    } else if (testName == "testDataVisualization") {
        testDataVisualization();
    } else if (testName == "testMkNot") {
        testMkNot();
    } else if (testName == "testCrossProduct") {
        testCrossProduct();
    } else if (testName == "testC17") {
        testC17();
    } else if (testName == "testC432") {
        testC432();
    } else if (testName == "testC880") {
        testC880();
    } else if (testName == "testC6288_8") {
        testC6288_8();
    } else if (testName == "testC6288_9") {
        testC6288_9();
    } else if (testName == "testC6288_10") {
        testC6288_10();
    } else if (testName == "testC6288_12") {
        testC6288_12();
    } else {
        std::cout << "Unknown test name: " << testName << std::endl;
    }
    // Add calls to other test functions here
}