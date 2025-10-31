#include "tests.h"
#include <iostream>
#include "grammar/grammar.h"
#include "gcflobdd/gcflobdd_t.h"
#include "ops/gcflobdd_int.h"
#include "visualization/visualize.h"
#include "hardware_benchmarks/hardware_tests.h"
#include "ops/cross_product.h"
#include <chrono>
using namespace G_CFL_OBDD;
using namespace std;
using namespace std::chrono;

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

std::shared_ptr<Grammar> generateBalancedSampleGrammar() {
    std::vector<std::string> productions = {
        "S 3 -> S 2 S 2",
        "S 2 -> S 1 S 1",
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

void Tests::testNand() {
    std::shared_ptr<Grammar> grammar = generateBalancedSampleGrammar();
    G_CFLOBDD x0 = MkProjection(0, 3, grammar);
    G_CFLOBDD x1 = MkProjection(2, 3, grammar);
    G_CFLOBDD x0_nand_x1 = MkNand(x0, x1);
    PrintCFLOBDD(x0_nand_x1);
}

void Tests::testRandomFunction() {
	std::cout << "Random start..." << std::endl;

    std::vector<std::string> productions = {
        // "S 13 -> S 12 S 12 S 12", // 1594323
        "S 12 -> S 11 S 11 S 11", // 531441
        "S 11 -> S 10 S 10 S 10", // 59049
        "S 10 -> S 9 S 9 S 9", // 59049
        "S 9 -> S 8 S 8 S 8", // 19683
        "S 8 -> S 7 S 7 S 7", // 6561
        "S 7 -> S 6 S 6 S 6", // 2187
        "S 6 -> S 5 S 5 S 5", // 729
        "S 5 -> S 4 S 4 S 4", // 243
        "S 4 -> S 3 S 3 S 3", // 81
        "S 3 -> S 2 S 2 S 2", // 27
        "S 2 -> S 1 S 1 S 1", // 9
        "S 1 -> S 0 S 0 S 0", // 3
        "S 0 -> a"
    };

    // std::vector<std::string> productions = {
    //     // "S 21 -> S 20 S 20", // 2097152
    //     "S 20 -> S 19 S 19", // 1048576
    //     "S 19 -> S 18 S 18", // 524288
    //     "S 18 -> S 17 S 17", // 262144
    //     "S 17 -> S 16 S 16", // 131072
    //     "S 16 -> S 15 S 15", // 65536
    //     "S 15 -> S 14 S 14", // 32768
    //     "S 14 -> S 13 S 13", // 16384
    //     "S 13 -> S 12 S 12", // 8192
    //     "S 12 -> S 11 S 11", // 4096
    //     "S 11 -> S 10 S 10", // 2048
    //     "S 10 -> S 9 S 9", // 1024
    //     "S 9 -> S 8 S 8", // 512
    //     "S 8 -> S 7 S 7", // 256
    //     "S 7 -> S 6 S 6", // 128
    //     "S 6 -> S 5 S 5", // 64
    //     "S 5 -> S 4 S 4", // 32
    //     "S 4 -> S 3 S 3", // 16
    //     "S 3 -> S 2 S 2", // 8
    //     "S 2 -> S 1 S 1", // 4
    //     "S 1 -> S 0 S 0", // 2
    //     "S 0 -> a"
    // };

    std::shared_ptr<Grammar> grammar = std::make_shared<Grammar>();
    grammar->constructGrammar(productions, "S 12");
    grammar->InstallNumVars();
    grammar->updateLevel();

	auto start = high_resolution_clock::now();
	unsigned int numVars = std::pow(3, 12); // 531441
	unsigned int level = 12;
	std::vector<G_CFLOBDD> vars;
	for (unsigned int i = 0; i < numVars; i++) {
        if (i % 10000 == 0) {
            std::cout << "Creating projection for variable " << i << " / " << numVars << std::endl;
        }
		vars.push_back(MkProjection(i, level, grammar));
	}

	G_CFLOBDD F = MkTrue(level, grammar);
	for (unsigned int i = 0; i < numVars / 3; i++) {
        if (i % 10000 == 0) {
            std::cout << "Processing variable set " << i << " / " << (numVars / 3) << std::endl;
        }
		unsigned int a = 3 * i;
		unsigned int b = 3 * i + 1;;
		unsigned int c = 3 * i + 2;
		G_CFLOBDD A = vars[a];
		G_CFLOBDD B = vars[b];
		G_CFLOBDD C = vars[c];
		G_CFLOBDD A_and_B = MkAnd(A, B);
		G_CFLOBDD Not_A = MkNot(A);
		G_CFLOBDD Not_A_and_C = MkAnd(Not_A, C);
		G_CFLOBDD A_and_B_or_Not_A_and_C = MkOr(A_and_B, Not_A_and_C);
		F = MkAnd(F, A_and_B_or_Not_A_and_C);
	}

	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start);
	std::cout << "Duration: " << duration.count() << " ms" << std::endl;

	unsigned int nodeCount = 0, edgeCount = 0;

	F.CountNodesAndEdges(nodeCount, edgeCount);
	std::cout << "nodeCount: " << nodeCount << " edgeCount: " << edgeCount << " totalCount: " << (nodeCount + edgeCount) << std::endl;

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
    InitPairProductCache();
    G_CFLOBDDNodeHandle::InitReduceCache();
}

void ClearUp() {
    G_CFLOBDDNodeHandle::DisposeOfReduceCache();
    DisposeOfPairProductCache();
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
    } else if (testName == "testNand") {
        testNand();
    } else if (testName == "testRandomFunction") {
        testRandomFunction();
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
    ClearUp();
    // Add calls to other test functions here
}