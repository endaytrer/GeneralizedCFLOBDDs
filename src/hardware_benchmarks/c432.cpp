#include "hardware_tests.h"
#include <iostream>
#include "../grammar/grammar.h"
#include "../gcflobdd/gcflobdd_t.h"
#include "../ops/gcflobdd_int.h"
#include "../ops/gcflobdd_node_ops.h"
#include <chrono>

using namespace G_CFL_OBDD;
using namespace std;
using namespace HardwareBenchmarks;
using namespace std::chrono;

namespace c432_helpers {

    std::tuple<std::vector<std::string>, std::string, int> grammar0Productions() {
        return make_tuple(std::vector<std::string> {
            "S 6 -> S 5 S 5", // 64
            "S 5 -> S 4 S 4", // 32
            "S 4 -> S 3 S 3", // 16
            "S 3 -> S 2 S 2", // 8
            "S 2 -> S 1 S 1", // 4
            "S 1 -> S 0 S 0", // 2
            "S 0 -> a"
        }, "S 6", 6);
    }

    std::tuple<std::vector<std::string>, std::string, int> grammar1Productions() {
        return make_tuple(std::vector<std::string> {
        "S 35 -> S 0 S 34",
        "S 34 -> S 0 S 33",
        "S 33 -> S 0 S 32",
        "S 32 -> S 0 S 31",
        "S 31 -> S 0 S 30",
        "S 30 -> S 0 S 29",
        "S 29 -> S 0 S 28",
        "S 28 -> S 0 S 27",
        "S 27 -> S 0 S 26",
        "S 26 -> S 0 S 25",
        "S 25 -> S 0 S 24",
        "S 24 -> S 0 S 23",
        "S 23 -> S 0 S 22",
        "S 22 -> S 0 S 21",
        "S 21 -> S 0 S 20",
        "S 20 -> S 0 S 19",
        "S 19 -> S 0 S 18",
        "S 18 -> S 0 S 17",
        "S 17 -> S 0 S 16",
        "S 16 -> S 0 S 15",
        "S 15 -> S 0 S 14",
        "S 14 -> S 0 S 13",
        "S 13 -> S 0 S 12",
        "S 12 -> S 0 S 11",
        "S 11 -> S 0 S 10",
        "S 10 -> S 0 S 9",
        "S 9 -> S 0 S 8",
        "S 8 -> S 0 S 7",
        "S 7 -> S 0 S 6",
        "S 6 -> S 0 S 5",
        "S 5 -> S 0 S 4",
        "S 4 -> S 0 S 3",
        "S 3 -> S 0 S 2",
        "S 2 -> S 0 S 1",
        "S 1 -> S 0 S 0",
        "S 0 -> a"
        }, "S 35", 35);
    }

    std::tuple<std::vector<std::string>, std::string, int> grammar2Productions() {
        return make_tuple(std::vector<std::string> {
            // over 36 bits as fibonacci numbers
            "S 7 -> S 6 S 4", // 26 7
            "S 6 -> S 4 S 5", // 26 6
            "S 5 -> S 3 S 4", // 16 5
            "S 4 -> S 2 S 3", // 10 4
            "S 3 -> S 1 S 2", // 6 3
            "S 2 -> S 1 S 1", // 4 2
            "S 1 -> S 0 S 0", // 2 1
            "S 0 -> a"
        }, "S 7", 7);
    }

    std::tuple<std::vector<std::string>, std::string, int> grammar3Productions() {
        return make_tuple(std::vector<std::string> {
            // over 36 bits as fibonacci numbers
            "S 7 -> S 4 S 6", // 26 7
            "S 6 -> S 5 S 4", // 26 6
            "S 5 -> S 3 S 4", // 16 5
            "S 4 -> S 2 S 3", // 10 4
            "S 3 -> S 1 S 2", // 6 3
            "S 2 -> S 1 S 1", // 4 2
            "S 1 -> S 0 S 0", // 2 1
            "S 0 -> a"
        }, "S 7", 7);
    }

    std::tuple<std::vector<std::string>, std::string, int> grammar4Productions() {
        return make_tuple(std::vector<std::string> {
            "S 4 -> S 3 S 3 S 3", // 36 4
            "S 3 -> S 2 S 2 S 2", // 12 3
            "S 2 -> S 1 S 1", // 4 2
            "S 1 -> S 0 S 0", // 2 1
            "S 0 -> a" // 1
        }, "S 4", 4);
    }

    std::tuple<std::vector<std::string>, std::string, int> grammar5Productions() {
        return make_tuple(std::vector<std::string> {
            "S 3 -> S 2 S 2 S 2 S 2 S 2 S 2 S 2 S 2 S 2", // 8 3
            "S 2 -> S 1 S 1", // 4 2
            "S 1 -> S 0 S 0", // 2 1
            "S 0 -> a" // 1
        }, "S 3", 3);
    }

    std::tuple<std::vector<std::string>, std::string, int> grammar6Productions() {
        return make_tuple(std::vector<std::string> {
            "S 1 -> S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0", // 36
            "S 0 -> a"
        }, "S 1", 1);
    }

    std::tuple<std::vector<std::string>, std::string, int> grammar7Productions() {
        return make_tuple(std::vector<std::string> {
            "S 6 -> S 5 S 2", // 36 6
            "S 5 -> S 4 S 4", // 32 5
            "S 4 -> S 3 S 3", // 16 4
            "S 3 -> S 2 S 2", // 8 3
            "S 2 -> S 1 S 1", // 4 2
            "S 1 -> S 0 S 0", // 2 1
            "S 0 -> a" // 1
        }, "S 6", 6);
    }
} // namespace c432_helpers

void HardwareBenchmarks::c432(unsigned int grammarChoice) {

    std::tuple<std::vector<std::string>, std::string, int> grammarData;

    switch (grammarChoice) {
        case 0:
            grammarData = c432_helpers::grammar0Productions();
            break;
        case 1:
            grammarData = c432_helpers::grammar1Productions();
            break;
        case 2:
            grammarData = c432_helpers::grammar2Productions();
            break;
        case 3:
            grammarData = c432_helpers::grammar3Productions();
            break;
        case 4:
            grammarData = c432_helpers::grammar4Productions();
            break;
        case 5:
            grammarData = c432_helpers::grammar5Productions();
            break;
        case 6:
            grammarData = c432_helpers::grammar6Productions();
            break;
        case 7:
            grammarData = c432_helpers::grammar7Productions();
            break;
        default:
            throw std::invalid_argument("Invalid grammar choice");
    }

    std::shared_ptr<Grammar> grammar = std::make_shared<Grammar>();
    grammar->constructGrammar(std::get<0>(grammarData), std::get<1>(grammarData));
    grammar->InstallNumVars();
    grammar->updateLevel();

    auto start = high_resolution_clock::now();

    int coeff = 1;
    int offset = 0;
    int max_level = std::get<2>(grammarData);

    // std::vector<int> var_order = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    // 							  16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    // 							  26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

    std::vector<int> var_order = {0, 1, 2, 4, 3, 5, 6, 8, 7, 9, 10, 12, 11, 13, 14, 16,
                                15, 17, 18, 20, 19, 21, 22, 24, 23, 25,
                                26, 28, 27, 29, 30, 32, 31, 33, 34, 35};    
    // std::vector<int> var_order = {0, 1, 18, 2, 27, 3, 19, 4, 28, 5, 20, 6, 29, 7, 21, 8, 30, 9, 22, 10, 31, 11, 23, 12, 32, 13, 24, 14, 33, 15, 25, 16, 34, 17, 26};

    // Inputs
    // G_CFLOBDD gat1 = MkProjection(coeff*0 + offset, max_level, grammar);
    // G_CFLOBDD gat4 = MkProjection(coeff*1 + offset, max_level, grammar);
    // G_CFLOBDD gat11 = MkProjection(coeff*2 + offset, max_level, grammar);
    // G_CFLOBDD gat17 = MkProjection(coeff*3 + offset, max_level, grammar);
    // G_CFLOBDD gat24 = MkProjection(coeff*4 + offset, max_level, grammar);
    // G_CFLOBDD gat30 = MkProjection(coeff*5 + offset, max_level, grammar);
    // G_CFLOBDD gat37 = MkProjection(coeff*6 + offset, max_level, grammar);
    // G_CFLOBDD gat43 = MkProjection(coeff*7 + offset, max_level, grammar);
    // G_CFLOBDD gat50 = MkProjection(coeff*8 + offset, max_level, grammar);
    // G_CFLOBDD gat56 = MkProjection(coeff*9 + offset, max_level, grammar);
    // G_CFLOBDD gat63 = MkProjection(coeff*10 + offset, max_level, grammar);
    // G_CFLOBDD gat69 = MkProjection(coeff*11 + offset, max_level, grammar);
    // G_CFLOBDD gat76 = MkProjection(coeff*12 + offset, max_level, grammar);
    // G_CFLOBDD gat82 = MkProjection(coeff*13 + offset, max_level, grammar);
    // G_CFLOBDD gat89 = MkProjection(coeff*14 + offset, max_level, grammar);
    // G_CFLOBDD gat95 = MkProjection(coeff*15 + offset, max_level, grammar);
    // G_CFLOBDD gat102 = MkProjection(coeff*16 + offset, max_level, grammar);
    // G_CFLOBDD gat108 = MkProjection(coeff*17 + offset, max_level, grammar);
    // G_CFLOBDD gat8 = MkProjection(coeff*18 + offset, max_level, grammar);
    // G_CFLOBDD gat21 = MkProjection(coeff*19 + offset, max_level, grammar);
    // G_CFLOBDD gat34 = MkProjection(coeff*20 + offset, max_level, grammar);
    // G_CFLOBDD gat47 = MkProjection(coeff*21 + offset, max_level, grammar);
    // G_CFLOBDD gat60 = MkProjection(coeff*22 + offset, max_level, grammar);
    // G_CFLOBDD gat73 = MkProjection(coeff*23 + offset, max_level, grammar);
    // G_CFLOBDD gat86 = MkProjection(coeff*24 + offset, max_level, grammar);
    // G_CFLOBDD gat99 = MkProjection(coeff*25 + offset, max_level, grammar);
    // G_CFLOBDD gat112 = MkProjection(coeff*26 + offset, max_level, grammar);
    // G_CFLOBDD gat14 = MkProjection(coeff*27 + offset, max_level, grammar);
    // G_CFLOBDD gat27 = MkProjection(coeff*28 + offset, max_level, grammar);
    // G_CFLOBDD gat40 = MkProjection(coeff*29 + offset, max_level, grammar);
    // G_CFLOBDD gat53 = MkProjection(coeff*30 + offset, max_level, grammar);
    // G_CFLOBDD gat66 = MkProjection(coeff*31 + offset, max_level, grammar);
    // G_CFLOBDD gat79 = MkProjection(coeff*32 + offset, max_level, grammar);
    // G_CFLOBDD gat92 = MkProjection(coeff*33 + offset, max_level, grammar);
    // G_CFLOBDD gat105 = MkProjection(coeff*34 + offset, max_level, grammar);
    // G_CFLOBDD gat115 = MkProjection(coeff*35 + offset, max_level, grammar);

    G_CFLOBDD gat1 =  MkProjection(var_order[0], max_level, grammar);
    G_CFLOBDD gat4 =  MkProjection(var_order[1], max_level, grammar);
    G_CFLOBDD gat8 =  MkProjection(var_order[2], max_level, grammar);
    G_CFLOBDD gat11 =  MkProjection(var_order[3], max_level, grammar);
    G_CFLOBDD gat14 =  MkProjection(var_order[4], max_level, grammar);
    G_CFLOBDD gat17 =  MkProjection(var_order[5], max_level, grammar);
    G_CFLOBDD gat21 =  MkProjection(var_order[6], max_level, grammar);
    G_CFLOBDD gat24 =  MkProjection(var_order[7], max_level, grammar);
    G_CFLOBDD gat27 =  MkProjection(var_order[8], max_level, grammar);
    G_CFLOBDD gat30 =  MkProjection(var_order[9], max_level, grammar);
    G_CFLOBDD gat34 =  MkProjection(var_order[10], max_level, grammar);
    G_CFLOBDD gat37 =  MkProjection(var_order[11], max_level, grammar);
    G_CFLOBDD gat40 =  MkProjection(var_order[12], max_level, grammar);
    G_CFLOBDD gat43 =  MkProjection(var_order[13], max_level, grammar);
    G_CFLOBDD gat47 =  MkProjection(var_order[14], max_level, grammar);
    G_CFLOBDD gat50 =  MkProjection(var_order[15], max_level, grammar);
    G_CFLOBDD gat53 =  MkProjection(var_order[16], max_level, grammar);
    G_CFLOBDD gat56 =  MkProjection(var_order[17], max_level, grammar);
    G_CFLOBDD gat60 =  MkProjection(var_order[18], max_level, grammar);
    G_CFLOBDD gat63 =  MkProjection(var_order[19], max_level, grammar);
    G_CFLOBDD gat66 =  MkProjection(var_order[20], max_level, grammar);
    G_CFLOBDD gat69 =  MkProjection(var_order[21], max_level, grammar);
    G_CFLOBDD gat73 =  MkProjection(var_order[22], max_level, grammar);
    G_CFLOBDD gat76 =  MkProjection(var_order[23], max_level, grammar);
    G_CFLOBDD gat79 =  MkProjection(var_order[24], max_level, grammar);
    G_CFLOBDD gat82 =  MkProjection(var_order[25], max_level, grammar);
    G_CFLOBDD gat86 =  MkProjection(var_order[26], max_level, grammar);
    G_CFLOBDD gat89 =  MkProjection(var_order[27], max_level, grammar);
    G_CFLOBDD gat92 =  MkProjection(var_order[28], max_level, grammar);
    G_CFLOBDD gat95 =  MkProjection(var_order[29], max_level, grammar);
    G_CFLOBDD gat99 =  MkProjection(var_order[30], max_level, grammar);
    G_CFLOBDD gat102 =  MkProjection(var_order[31], max_level, grammar);
    G_CFLOBDD gat105 =  MkProjection(var_order[32], max_level, grammar);
    G_CFLOBDD gat108 =  MkProjection(var_order[33], max_level, grammar);
    G_CFLOBDD gat112 =  MkProjection(var_order[34], max_level, grammar);
    G_CFLOBDD gat115 =  MkProjection(var_order[35], max_level, grammar);

    // Outputs: gat223,gat329,gat370,gat421,gat430,gat431,gat432;
    
    // The circuit
    cout << "Starting circuit" << endl;
    cout << "gat118" << endl;
    G_CFLOBDD gat118 = MkNot(gat1);
    cout << "gat119" << endl;
    G_CFLOBDD gat119 = MkNot(gat4);
    cout << "gat122" << endl;
    G_CFLOBDD gat122 = MkNot(gat11);
    cout << "gat123" << endl;
    G_CFLOBDD gat123 = MkNot(gat17);
    cout << "gat126" << endl;
    G_CFLOBDD gat126 = MkNot(gat24);
    cout << "gat127" << endl;
    G_CFLOBDD gat127 = MkNot(gat30);
    cout << "gat130" << endl;
    G_CFLOBDD gat130 = MkNot(gat37);
    cout << "gat131" << endl;
    G_CFLOBDD gat131 = MkNot(gat43);
    cout << "gat134" << endl;
    G_CFLOBDD gat134 = MkNot(gat50);
    cout << "gat135" << endl;
    G_CFLOBDD gat135 = MkNot(gat56);
    cout << "gat138" << endl;
    G_CFLOBDD gat138 = MkNot(gat63);
    cout << "gat139" << endl;
    G_CFLOBDD gat139 = MkNot(gat69);
    cout << "gat142" << endl;
    G_CFLOBDD gat142 = MkNot(gat76);
    cout << "gat143" << endl;
    G_CFLOBDD gat143 = MkNot(gat82);
    cout << "gat146" << endl;
    G_CFLOBDD gat146 = MkNot(gat89);
    cout << "gat147" << endl;
    G_CFLOBDD gat147 = MkNot(gat95);
    cout << "gat150" << endl;
    G_CFLOBDD gat150 = MkNot(gat102);
    cout << "gat151" << endl;
    G_CFLOBDD gat151 = MkNot(gat108);
    cout << "gat154" << endl;
    G_CFLOBDD gat154 = MkNand(gat118, gat4);
    cout << "gat157" << endl;
    G_CFLOBDD gat157 = MkNor(gat8, gat119);
    cout << "gat158" << endl;
    G_CFLOBDD gat158 = MkNor(gat14, gat119);
    cout << "gat159" << endl;
    G_CFLOBDD gat159 = MkNand(gat122, gat17);
    cout << "gat162" << endl;
    G_CFLOBDD gat162 = MkNand(gat126, gat30);
    cout << "gat165" << endl;
    G_CFLOBDD gat165 = MkNand(gat130, gat43);
    cout << "gat168" << endl;
    G_CFLOBDD gat168 = MkNand(gat134, gat56);
    cout << "gat171" << endl;
    G_CFLOBDD gat171 = MkNand(gat138, gat69);
    cout << "gat174" << endl;
    G_CFLOBDD gat174 = MkNand(gat142, gat82);
    cout << "gat177" << endl;
    G_CFLOBDD gat177 = MkNand(gat146, gat95);
    cout << "gat180" << endl;
    G_CFLOBDD gat180 = MkNand(gat150, gat108);
    cout << "gat183" << endl;
    G_CFLOBDD gat183 = MkNor(gat21, gat123);
    cout << "gat184" << endl;
    G_CFLOBDD gat184 = MkNor(gat27, gat123);
    cout << "gat185" << endl;
    G_CFLOBDD gat185 = MkNor(gat34, gat127);
    cout << "gat186" << endl;
    G_CFLOBDD gat186 = MkNor(gat40, gat127);
    cout << "gat187" << endl;
    G_CFLOBDD gat187 = MkNor(gat47, gat131);
    cout << "gat188" << endl;
    G_CFLOBDD gat188 = MkNor(gat53, gat131);
    cout << "gat189" << endl;
    G_CFLOBDD gat189 = MkNor(gat60, gat135);
    cout << "gat190" << endl;
    G_CFLOBDD gat190 = MkNor(gat66, gat135);
    cout << "gat191" << endl;
    G_CFLOBDD gat191 = MkNor(gat73, gat139);
    cout << "gat192" << endl;
    G_CFLOBDD gat192 = MkNor(gat79, gat139);
    cout << "gat193" << endl;
    G_CFLOBDD gat193 = MkNor(gat86, gat143);
    cout << "gat194" << endl;
    G_CFLOBDD gat194 = MkNor(gat92, gat143);
    cout << "gat195" << endl;
    G_CFLOBDD gat195 = MkNor(gat99, gat147);
    cout << "gat196" << endl;
    G_CFLOBDD gat196 = MkNor(gat105, gat147);
    cout << "gat197" << endl;
    G_CFLOBDD gat197 = MkNor(gat112, gat151);
    cout << "gat198" << endl;
    G_CFLOBDD gat198 = MkNor(gat115, gat151);
    cout << "gat199" << endl;
    // G_CFLOBDD gat199 = MkAnd(9, gat154.root, gat159.root, gat162.root, gat165.root, gat168.root, gat171.root, gat174.root, gat177.root, gat180.root);
    G_CFLOBDD gat199 = MkAnd(MkAnd(MkAnd(gat154, gat159), MkAnd(gat162, MkAnd(gat165, gat168))), MkAnd(MkAnd(gat171, gat174), MkAnd(gat177, gat180)));
    cout << "gat203" << endl;
    G_CFLOBDD gat203 = MkNot(gat199);
    cout << "gat213" << endl;
    G_CFLOBDD gat213 = MkNot(gat199);
    cout << "gat223" << endl;
    G_CFLOBDD gat223 = MkNot(gat199);
    cout << "gat224" << endl;
    G_CFLOBDD gat224 = MkExclusiveOr(gat203, gat154);
    cout << "gat227" << endl;
    G_CFLOBDD gat227 = MkExclusiveOr(gat203, gat159);
    cout << "gat230" << endl;
    G_CFLOBDD gat230 = MkExclusiveOr(gat203, gat162);
    cout << "gat233" << endl;
    G_CFLOBDD gat233 = MkExclusiveOr(gat203, gat165);
    cout << "gat236" << endl;
    G_CFLOBDD gat236 = MkExclusiveOr(gat203, gat168);
    cout << "gat239" << endl;
    G_CFLOBDD gat239 = MkExclusiveOr(gat203, gat171);
    cout << "gat242" << endl;
    G_CFLOBDD gat242 = MkNand(gat1, gat213);
    cout << "gat243" << endl;
    G_CFLOBDD gat243 = MkExclusiveOr(gat203, gat174);
    cout << "gat246" << endl;
    G_CFLOBDD gat246 = MkNand(gat213, gat11);
    cout << "gat247" << endl;
    G_CFLOBDD gat247 = MkExclusiveOr(gat203, gat177);
    cout << "gat250" << endl;
    G_CFLOBDD gat250 = MkNand(gat213, gat24);
    cout << "gat251" << endl;
    G_CFLOBDD gat251 = MkExclusiveOr(gat203, gat180);
    cout << "gat254" << endl;
    G_CFLOBDD gat254 = MkNand(gat213, gat37);
    cout << "gat255" << endl;
    G_CFLOBDD gat255 = MkNand(gat213, gat50);
    cout << "gat256" << endl;
    G_CFLOBDD gat256 = MkNand(gat213, gat63);
    cout << "gat257" << endl;
    G_CFLOBDD gat257 = MkNand(gat213, gat76);
    cout << "gat258" << endl;
    G_CFLOBDD gat258 = MkNand(gat213, gat89);
    cout << "gat259" << endl;
    G_CFLOBDD gat259 = MkNand(gat213, gat102);
    cout << "gat260" << endl;
    G_CFLOBDD gat260 = MkNand(gat224, gat157);
    cout << "gat263" << endl;
    G_CFLOBDD gat263 = MkNand(gat224, gat158);
    cout << "gat264" << endl;
    G_CFLOBDD gat264 = MkNand(gat227, gat183);
    cout << "gat267" << endl;
    G_CFLOBDD gat267 = MkNand(gat230, gat185);
    cout << "gat270" << endl;
    G_CFLOBDD gat270 = MkNand(gat233, gat187);
    cout << "gat273" << endl;
    G_CFLOBDD gat273 = MkNand(gat236, gat189);
    cout << "gat276" << endl;
    G_CFLOBDD gat276 = MkNand(gat239, gat191);
    cout << "gat279" << endl;
    G_CFLOBDD gat279 = MkNand(gat243, gat193);
    cout << "gat282" << endl;
    G_CFLOBDD gat282 = MkNand(gat247, gat195);
    cout << "gat285" << endl;
    G_CFLOBDD gat285 = MkNand(gat251, gat197);
    cout << "gat288" << endl;
    G_CFLOBDD gat288 = MkNand(gat227, gat184);
    cout << "gat289" << endl;
    G_CFLOBDD gat289 = MkNand(gat230, gat186);
    cout << "gat290" << endl;
    G_CFLOBDD gat290 = MkNand(gat233, gat188);
    cout << "gat291" << endl;
    G_CFLOBDD gat291 = MkNand(gat236, gat190);
    cout << "gat292" << endl;
    G_CFLOBDD gat292 = MkNand(gat239, gat192);
    cout << "gat293" << endl;
    G_CFLOBDD gat293 = MkNand(gat243, gat194);
    cout << "gat294" << endl;
    G_CFLOBDD gat294 = MkNand(gat247, gat196);
    cout << "gat295" << endl;
    G_CFLOBDD gat295 = MkNand(gat251, gat198);
    cout << "gat296" << endl;
    // G_CFLOBDD gat296 = MkAnd(9, gat260.root, gat264.root, gat267.root, gat270.root, gat273.root, gat276.root, gat279.root, gat282.root, gat285.root);
    G_CFLOBDD gat296 = MkAnd(MkAnd(MkAnd(gat260, gat264), MkAnd(gat267, gat270)), MkAnd(MkAnd(gat273, gat276), MkAnd(MkAnd(gat279, gat282), gat285)));
    cout << "gat300" << endl;
    G_CFLOBDD gat300 = MkNot(gat263);
    cout << "gat301" << endl;
    G_CFLOBDD gat301 = MkNot(gat288);
    cout << "gat302" << endl;
    G_CFLOBDD gat302 = MkNot(gat289);
    cout << "gat303" << endl;
    G_CFLOBDD gat303 = MkNot(gat290);
    cout << "gat304" << endl;
    G_CFLOBDD gat304 = MkNot(gat291);
    cout << "gat305" << endl;
    G_CFLOBDD gat305 = MkNot(gat292);
    cout << "gat306" << endl;
    G_CFLOBDD gat306 = MkNot(gat293);
    cout << "gat307" << endl;
    G_CFLOBDD gat307 = MkNot(gat294);
    cout << "gat308" << endl;
    G_CFLOBDD gat308 = MkNot(gat295);
    cout << "gat309" << endl;
    G_CFLOBDD gat309 = MkNot(gat296);
    cout << "gat319" << endl;
    G_CFLOBDD gat319 = MkNot(gat296);
    cout << "gat329" << endl;
    G_CFLOBDD gat329 = MkNot(gat296);
    cout << "gat330" << endl;
    G_CFLOBDD gat330 = MkExclusiveOr(gat309, gat260);
    cout << "gat331" << endl;
    G_CFLOBDD gat331 = MkExclusiveOr(gat309, gat264);
    cout << "gat332" << endl;
    G_CFLOBDD gat332 = MkExclusiveOr(gat309, gat267);
    cout << "gat333" << endl;
    G_CFLOBDD gat333 = MkExclusiveOr(gat309, gat270);
    cout << "gat334" << endl;
    G_CFLOBDD gat334 = MkNand(gat8, gat319);
    cout << "gat335" << endl;
    G_CFLOBDD gat335 = MkExclusiveOr(gat309, gat273);
    cout << "gat336" << endl;
    G_CFLOBDD gat336 = MkNand(gat319, gat21);
    cout << "gat337" << endl;
    G_CFLOBDD gat337 = MkExclusiveOr(gat309, gat276);
    cout << "gat338" << endl;
    G_CFLOBDD gat338 = MkNand(gat319, gat34);
    cout << "gat339" << endl;
    G_CFLOBDD gat339 = MkExclusiveOr(gat309, gat279);
    cout << "gat340" << endl;
    G_CFLOBDD gat340 = MkNand(gat319, gat47);
    cout << "gat341" << endl;
    G_CFLOBDD gat341 = MkExclusiveOr(gat309, gat282);
    cout << "gat342" << endl;
    G_CFLOBDD gat342 = MkNand(gat319, gat60);
    cout << "gat343" << endl;
    G_CFLOBDD gat343 = MkExclusiveOr(gat309, gat285);
    cout << "gat344" << endl;
    G_CFLOBDD gat344 = MkNand(gat319, gat73);
    cout << "gat345" << endl;
    G_CFLOBDD gat345 = MkNand(gat319, gat86);
    cout << "gat346" << endl;
    G_CFLOBDD gat346 = MkNand(gat319, gat99);
    cout << "gat347" << endl;
    G_CFLOBDD gat347 = MkNand(gat319, gat112);
    cout << "gat348" << endl;
    G_CFLOBDD gat348 = MkNand(gat330, gat300);
    cout << "gat349" << endl;
    G_CFLOBDD gat349 = MkNand(gat331, gat301);
    cout << "gat350" << endl;
    G_CFLOBDD gat350 = MkNand(gat332, gat302);
    cout << "gat351" << endl;
    G_CFLOBDD gat351 = MkNand(gat333, gat303);
    cout << "gat352" << endl;
    G_CFLOBDD gat352 = MkNand(gat335, gat304);
    cout << "gat353" << endl;
    G_CFLOBDD gat353 = MkNand(gat337, gat305);
    cout << "gat354" << endl;
    G_CFLOBDD gat354 = MkNand(gat339, gat306);
    cout << "gat355" << endl;
    G_CFLOBDD gat355 = MkNand(gat341, gat307);
    cout << "gat356" << endl;
    G_CFLOBDD gat356 = MkNand(gat343, gat308);
    cout << "gat357" << endl;
    // G_CFLOBDD gat357 = MkAnd(9, gat348.root, gat349.root, gat350.root, gat351.root, gat352.root, gat353.root, gat354.root, gat355.root, gat356.root);
    G_CFLOBDD gat357 = MkAnd(MkAnd(MkAnd(gat348, gat349), MkAnd(gat350, gat351)), MkAnd(MkAnd(gat352, gat353), MkAnd(MkAnd(gat354, gat355), gat356)));
    cout << "gat360" << endl;
    G_CFLOBDD gat360 = MkNot(gat357);
    cout << "gat370" << endl;
    G_CFLOBDD gat370 = MkNot(gat357);
    cout << "gat371" << endl;
    G_CFLOBDD gat371 = MkNand(gat14, gat360);
    cout << "gat372" << endl;
    G_CFLOBDD gat372 = MkNand(gat360, gat27);
    cout << "gat373" << endl;
    G_CFLOBDD gat373 = MkNand(gat360, gat40);
    cout << "gat374" << endl;
    G_CFLOBDD gat374 = MkNand(gat360, gat53);
    cout << "gat375" << endl;
    G_CFLOBDD gat375 = MkNand(gat360, gat66);
    cout << "gat376" << endl;
    G_CFLOBDD gat376 = MkNand(gat360, gat79);
    cout << "gat377" << endl;
    G_CFLOBDD gat377 = MkNand(gat360, gat92);
    cout << "gat378" << endl;
    G_CFLOBDD gat378 = MkNand(gat360, gat105);
    cout << "gat379" << endl;
    G_CFLOBDD gat379 = MkNand(gat360, gat115);
    cout << "gat380" << endl;
    G_CFLOBDD gat380 = MkNand(MkNand(gat4, gat242), MkNand(gat334, gat371));
    cout << "gat381" << endl;
    G_CFLOBDD gat381 = MkNand(MkNand(gat246, gat336), MkNand(gat372, gat17));
    cout << "gat386" << endl;
    G_CFLOBDD gat386 = MkNand(MkNand(gat250, gat338), MkNand(gat373, gat30));
    cout << "gat393" << endl;
    G_CFLOBDD gat393 = MkNand(MkNand(gat254, gat340), MkNand(gat374, gat43));
    cout << "gat399" << endl;
    G_CFLOBDD gat399 = MkNand(MkNand(gat255, gat342), MkNand(gat375, gat56));
    cout << "gat404" << endl;
    G_CFLOBDD gat404 = MkNand(MkNand(gat256, gat344), MkNand(gat376, gat69));
    cout << "gat407" << endl;
    G_CFLOBDD gat407 = MkNand(MkNand(gat257, gat345), MkNand(gat377, gat82));
    cout << "gat411" << endl;
    G_CFLOBDD gat411 = MkNand(MkNand(gat258, gat346), MkNand(gat378, gat95));
    cout << "gat414" << endl;
    G_CFLOBDD gat414 = MkNand(MkNand(gat259, gat347), MkNand(gat379, gat108));
    cout << "gat415" << endl;
    G_CFLOBDD gat415 = MkNot(gat380);
    cout << "gat416" << endl;
    // G_CFLOBDD gat416 = MkAnd(8, gat381.root, gat386.root, gat393.root, gat399.root, gat404.root, gat407.root, gat411.root, gat414.root);
    G_CFLOBDD gat416 = MkAnd(MkAnd(MkAnd(gat381, gat386), MkAnd(gat393, gat399)), MkAnd(MkAnd(gat404, gat407), MkAnd(gat411, gat414)));
    cout << "gat417" << endl;
    G_CFLOBDD gat417 = MkNot(gat393);
    cout << "gat418" << endl;
    G_CFLOBDD gat418 = MkNot(gat404);
    cout << "gat419" << endl;
    G_CFLOBDD gat419 = MkNot(gat407);
    cout << "gat420" << endl;
    G_CFLOBDD gat420 = MkNot(gat411);
    cout << "gat421" << endl;
    G_CFLOBDD gat421 = MkNor(gat415, gat416);
    cout << "gat422" << endl;
    G_CFLOBDD gat422 = MkNand(gat386, gat417);
    cout << "gat425" << endl;
    G_CFLOBDD gat425 = MkNand(MkNand(gat386, gat393), MkNand(gat418, gat399));
    cout << "gat428" << endl;
    G_CFLOBDD gat428 = MkNand(gat399, MkNand(gat393, gat419));
    cout << "gat429" << endl;
    G_CFLOBDD gat429 = MkNand(MkNand(gat386, gat393), MkNand(gat407, gat420));
    cout << "gat430" << endl;
    G_CFLOBDD gat430 = MkNand(MkNand(gat381, gat386), MkNand(gat422, gat399));
    cout << "gat431" << endl;
    G_CFLOBDD gat431 = MkNand(MkNand(gat381, gat386), MkNand(gat425, gat428));
    cout << "gat432" << endl;
    G_CFLOBDD gat432 = MkNand(MkNand(gat381, gat422), MkNand(gat425, gat429));
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);  
    
    std::vector<G_CFLOBDD> outputs = {gat223, gat329, gat370, gat421, gat430, gat431, gat432};

    unsigned int nodeCount = 0, edgeCount = 0;
    Hashset<G_CFLOBDDNodeHandle>* visitedNodesDuringGroupCountNodesAndEdges = new Hashset<G_CFLOBDDNodeHandle>;
    Hashset<G_CFLOBDDReturnMapBody>* visitedEdgesDuringGroupCountNodesAndEdges = new Hashset<G_CFLOBDDReturnMapBody>;
    for (int i = 0; i < outputs.size(); i++) {
        outputs[i].GroupCountNodesAndEdges(nodeCount, edgeCount, visitedNodesDuringGroupCountNodesAndEdges, visitedEdgesDuringGroupCountNodesAndEdges);
    }

    for (int i = 0; i < outputs.size(); i++) {
        unsigned int localNodeCount = 0, localEdgeCount = 0;
        outputs[i].CountNodesAndEdges(localNodeCount, localEdgeCount);
        cout << "Output " << i << " has " << localNodeCount << " nodes and " << localEdgeCount << " edges and total of " << (localNodeCount + localEdgeCount) << endl;
    }
    
    cout << "C432 took " << duration.count() << " milliseconds." << endl;
    cout << "Total nodes in outputs: " << nodeCount << endl;
    cout << "Total edges in outputs: " << edgeCount << endl;
    cout << "Total count: " << nodeCount + edgeCount << endl;


}