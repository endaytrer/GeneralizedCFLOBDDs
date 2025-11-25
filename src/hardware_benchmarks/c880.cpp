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

namespace c880_helpers {

    std::tuple<std::vector<std::string>, std::string, int> grammar0Productions() {
        return make_tuple(std::vector<std::string> {
            "S 7 -> S 6 S 6", // 128
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
            "S 8 -> S 7 S 5 S 1", // 60
            "S 7 -> S 5 S 6", // 42
            "S 6 -> S 4 S 5", // 26
            "S 5 -> S 3 S 4", // 16
            "S 4 -> S 2 S 3", // 10
            "S 3 -> S 1 S 2", // 6
            "S 2 -> S 1 S 1", // 4
            "S 1 -> S 0 S 0", // 2
            "S 0 -> a"
        }, "S 8", 8);
    }

    std::tuple<std::vector<std::string>, std::string, int> grammar2Productions() {
        return make_tuple(std::vector<std::string> {
            "S 7 -> A 6 S 3 S 4", // 60
            "A 6 -> A 5 S 2", // 36
            "A 5 -> S 4 S 4", // 32
            "S 4 -> S 3 S 3", // 16
            "S 3 -> S 2 S 2", // 8
            "S 2 -> S 1 S 1", // 4
            "S 1 -> S 0 S 0", // 2
            "S 0 -> a"
        }, "S 7", 7);
    }

    std::tuple<std::vector<std::string>, std::string, int> grammar3Productions() {
        return make_tuple(std::vector<std::string> {
            "S 1 -> S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0 S 0",
            "S 0 -> a"
        }, "S 1", 1);
    }

    std::tuple<std::vector<std::string>, std::string, int> grammar4Productions() {
        return make_tuple(std::vector<std::string> {
            "S 2 -> S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1 S 1",
            "S 1 -> S 0 S 0", // 2
            "S 0 -> a"
        }, "S 2", 2);
    }


} // namespace c880_helpers

void HardwareBenchmarks::c880(unsigned int grammarChoice) {

    std::tuple<std::vector<std::string>, std::string, int> grammarData;

    switch (grammarChoice) {
        case 0:
            grammarData = c880_helpers::grammar0Productions();
            break;
        case 1:
            grammarData = c880_helpers::grammar1Productions();
            break;
        case 2:
            grammarData = c880_helpers::grammar2Productions();
            break;
        case 3:
            grammarData = c880_helpers::grammar3Productions();
            break;
        case 4:
            grammarData = c880_helpers::grammar4Productions();
            break;
        default:
            throw std::invalid_argument("Invalid grammar choice");
    }

    std::shared_ptr<Grammar> grammar = std::make_shared<Grammar>();
    grammar->constructGrammar(std::get<0>(grammarData), std::get<1>(grammarData));
    grammar->InstallNumVars();
    grammar->updateLevel();

    // Create G_G_CFLOBDD for 880 circuit
    auto start = high_resolution_clock::now();
    
    int coeff = 1;
    int offset = 0;
    int max_level = std::get<2>(grammarData);

    // std::vector<int> var_orders = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    //                                16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
    //                                28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    //                                40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
    //                                52, 53, 54, 55, 56, 57, 58, 59, 60};

    std::vector<int> var_orders = {5, 10, 40, 3, 6, 2, 42, 0, 7, 14, 8, 38, 39, 41, 50, 1, 12, 58, 
        59, 54, 55, 56, 57, 44, 33, 11, 17, 22, 25, 27, 29, 52, 51, 4, 21, 24, 15, 18, 19, 9, 45, 43, 16, 
        20, 23, 26, 28, 30, 53, 34, 32, 35, 36, 37, 46, 48, 47, 31, 49, 13};


    // .inputs 1GAT(0) 8GAT(1) 13GAT(2) 17GAT(3) 26GAT(4) 29GAT(5) 36GAT(6) 42GAT(7) 51GAT(8) 55GAT(9) 59GAT(10) 68GAT(11) 72GAT(12) 73GAT(13) 74GAT(14) 75GAT(15) 80GAT(16) 85GAT(17) 86GAT(18) 87GAT(19) 88GAT(20) 89GAT(21) 90GAT(22) 91GAT(23) 96GAT(24) 101GAT(25) 106GAT(26) 111GAT(27) 116GAT(28) 121GAT(29) 126GAT(30) 130GAT(31) 135GAT(32) 138GAT(33) 143GAT(34) 146GAT(35) 149GAT(36) 152GAT(37) 153GAT(38) 156GAT(39) 159GAT(40) 165GAT(41) 171GAT(42) 177GAT(43) 183GAT(44) 189GAT(45) 195GAT(46) 201GAT(47) 207GAT(48) 210GAT(49) 219GAT(50) 228GAT(51) 237GAT(52) 246GAT(53) 255GAT(54) 259GAT(55) 260GAT(56) 261GAT(57) 267GAT(58) 268GAT(59)

    G_CFLOBDD gat1 = MkProjection(var_orders[0], max_level, grammar);
    G_CFLOBDD gat8 = MkProjection(var_orders[1], max_level, grammar);
    G_CFLOBDD gat13 = MkProjection(var_orders[2], max_level, grammar);
    G_CFLOBDD gat17 = MkProjection(var_orders[3], max_level, grammar);
    G_CFLOBDD gat26 = MkProjection(var_orders[4], max_level, grammar);
    G_CFLOBDD gat29 = MkProjection(var_orders[5], max_level, grammar);
    G_CFLOBDD gat36 = MkProjection(var_orders[6], max_level, grammar);
    G_CFLOBDD gat42 = MkProjection(var_orders[7], max_level, grammar);
    G_CFLOBDD gat51 = MkProjection(var_orders[8], max_level, grammar);
    G_CFLOBDD gat55 = MkProjection(var_orders[9], max_level, grammar);
    G_CFLOBDD gat59 = MkProjection(var_orders[10], max_level, grammar);
    G_CFLOBDD gat68 = MkProjection(var_orders[11], max_level, grammar);
    G_CFLOBDD gat72 = MkProjection(var_orders[12], max_level, grammar);
    G_CFLOBDD gat73 = MkProjection(var_orders[13], max_level, grammar);
    G_CFLOBDD gat74 = MkProjection(var_orders[14], max_level, grammar);
    G_CFLOBDD gat75 = MkProjection(var_orders[15], max_level, grammar);
    G_CFLOBDD gat80 = MkProjection(var_orders[16], max_level, grammar);
    G_CFLOBDD gat85 = MkProjection(var_orders[17], max_level, grammar);
    G_CFLOBDD gat86 = MkProjection(var_orders[18], max_level, grammar);
    G_CFLOBDD gat87 = MkProjection(var_orders[19], max_level, grammar);
    G_CFLOBDD gat88 = MkProjection(var_orders[20], max_level, grammar);
    G_CFLOBDD gat89 = MkProjection(var_orders[21], max_level, grammar);
    G_CFLOBDD gat90 = MkProjection(var_orders[22], max_level, grammar);
    G_CFLOBDD gat91 = MkProjection(var_orders[23], max_level, grammar);
    G_CFLOBDD gat96 = MkProjection(var_orders[24], max_level, grammar);
    G_CFLOBDD gat101 = MkProjection(var_orders[25], max_level, grammar);
    G_CFLOBDD gat106 = MkProjection(var_orders[26], max_level, grammar);
    G_CFLOBDD gat111 = MkProjection(var_orders[27], max_level, grammar);
    G_CFLOBDD gat116 = MkProjection(var_orders[28], max_level, grammar);
    G_CFLOBDD gat121 = MkProjection(var_orders[29], max_level, grammar);
    G_CFLOBDD gat126 = MkProjection(var_orders[30], max_level, grammar);
    G_CFLOBDD gat130 = MkProjection(var_orders[31], max_level, grammar);
    G_CFLOBDD gat135 = MkProjection(var_orders[32], max_level, grammar);
    G_CFLOBDD gat138 = MkProjection(var_orders[33], max_level, grammar);
    G_CFLOBDD gat143 = MkProjection(var_orders[34], max_level, grammar);
    G_CFLOBDD gat146 = MkProjection(var_orders[35], max_level, grammar);
    G_CFLOBDD gat149 = MkProjection(var_orders[36], max_level, grammar);
    G_CFLOBDD gat152 = MkProjection(var_orders[37], max_level, grammar);
    G_CFLOBDD gat153 = MkProjection(var_orders[38], max_level, grammar);
    G_CFLOBDD gat156 = MkProjection(var_orders[39], max_level, grammar);
    G_CFLOBDD gat159 = MkProjection(var_orders[40], max_level, grammar);
    G_CFLOBDD gat165 = MkProjection(var_orders[41], max_level, grammar);
    G_CFLOBDD gat171 = MkProjection(var_orders[42], max_level, grammar);
    G_CFLOBDD gat177 = MkProjection(var_orders[43], max_level, grammar);
    G_CFLOBDD gat183 = MkProjection(var_orders[44], max_level, grammar);
    G_CFLOBDD gat189 = MkProjection(var_orders[45], max_level, grammar);
    G_CFLOBDD gat195 = MkProjection(var_orders[46], max_level, grammar);
    G_CFLOBDD gat201 = MkProjection(var_orders[47], max_level, grammar);
    G_CFLOBDD gat207 = MkProjection(var_orders[48], max_level, grammar);
    G_CFLOBDD gat210 = MkProjection(var_orders[49], max_level, grammar);
    G_CFLOBDD gat219 = MkProjection(var_orders[50], max_level, grammar);
    G_CFLOBDD gat228 = MkProjection(var_orders[51], max_level, grammar);
    G_CFLOBDD gat237 = MkProjection(var_orders[52], max_level, grammar);
    G_CFLOBDD gat246 = MkProjection(var_orders[53], max_level, grammar);
    G_CFLOBDD gat255 = MkProjection(var_orders[54], max_level, grammar);
    G_CFLOBDD gat259 = MkProjection(var_orders[55], max_level, grammar);
    G_CFLOBDD gat260 = MkProjection(var_orders[56], max_level, grammar);
    G_CFLOBDD gat261 = MkProjection(var_orders[57], max_level, grammar);
    G_CFLOBDD gat267 = MkProjection(var_orders[58], max_level, grammar);
    G_CFLOBDD gat268 = MkProjection(var_orders[59], max_level, grammar);

    // The circuit
    cout << "gat269" << endl;
    // G_CFLOBDD gat269 = MkNand(4, gat1.root, gat8.root, gat13.root, gat17.root);
    G_CFLOBDD gat269 = MkNand(MkNand(gat1, gat8), MkNand(gat13, gat17));
    // G_CFLOBDD gat270 = MkNand(4, gat1.root, gat26.root, gat13.root, gat17.root);
    G_CFLOBDD gat270 = MkNand(MkNand(gat1, gat26), MkNand(gat13, gat17));
    // G_CFLOBDD gat273 = MkAnd(3, gat29.root, gat36.root, gat42.root);
    G_CFLOBDD gat273 = MkAnd(gat29, MkAnd(gat36, gat42));
    // G_CFLOBDD gat276 = MkAnd(3, gat1.root, gat26.root, gat51.root);
    G_CFLOBDD gat276 = MkAnd(gat1, MkAnd(gat26, gat51));
    // G_CFLOBDD gat279 = MkNand(4, gat1.root, gat8.root, gat51.root, gat17.root);
    G_CFLOBDD gat279 = MkNand(MkNand(gat1, gat8), MkNand(gat51, gat17));
    // G_CFLOBDD gat280 = MkNand(4, gat1.root, gat8.root, gat13.root, gat55.root);
    G_CFLOBDD gat280 = MkNand(MkNand(gat1, gat8), MkNand(gat13, gat55));
    // G_CFLOBDD gat284 = MkNand(4, gat59.root, gat42.root, gat68.root, gat72.root);
    G_CFLOBDD gat284 = MkNand(MkNand(gat59, gat42), MkNand(gat68, gat72));
    G_CFLOBDD gat285 = MkNand(gat29, gat68);
    // G_CFLOBDD gat286 = MkNand(3, gat59.root, gat68.root, gat74.root);
    G_CFLOBDD gat286 = MkNand(gat59, MkNand(gat68, gat74));
    // G_CFLOBDD gat287 = MkAnd(3, gat29.root, gat75.root, gat80.root);
    G_CFLOBDD gat287 = MkAnd(gat29, MkAnd(gat75, gat80));
    cout << "gat290" << endl;
    // G_CFLOBDD gat290 = MkAnd(3, gat29.root, gat75.root, gat42.root);
    G_CFLOBDD gat290 = MkAnd(gat29, MkAnd(gat75, gat42));
    G_CFLOBDD gat291 = MkAnd(gat29, MkAnd(gat36, gat80));
    G_CFLOBDD gat292 = MkAnd(gat29, MkAnd(gat36, gat42));
    G_CFLOBDD gat293 = MkAnd(gat59, MkAnd(gat75, gat80));
    G_CFLOBDD gat294 = MkAnd(gat59, MkAnd(gat75, gat42));
    G_CFLOBDD gat295 = MkAnd(gat59, MkAnd(gat36, gat80));
    G_CFLOBDD gat296 = MkAnd(gat59, MkAnd(gat36, gat42));
    G_CFLOBDD gat297 = MkAnd(gat85, gat86);
    G_CFLOBDD gat298 = MkOr(gat87, gat88);
    G_CFLOBDD gat301 = MkNand(gat91, gat96);
    cout << "gat302" << endl;
    G_CFLOBDD gat302 = MkOr(gat91, gat96);
    G_CFLOBDD gat303 = MkNand(gat101, gat106);
    G_CFLOBDD gat304 = MkOr(gat101, gat106);
    G_CFLOBDD gat305 = MkNand(gat111, gat116);
    G_CFLOBDD gat306 = MkOr(gat111, gat116);
    G_CFLOBDD gat307 = MkNand(gat121, gat126);
    G_CFLOBDD gat308 = MkOr(gat121, gat126);
    G_CFLOBDD gat309 = MkAnd(gat8, gat138);
    G_CFLOBDD gat310 = MkNot(gat268);
    G_CFLOBDD gat316 = MkAnd(gat51, gat138);
    cout << "gat317" << endl;
    G_CFLOBDD gat317 = MkAnd(gat17, gat138);
    G_CFLOBDD gat318 = MkAnd(gat152, gat138);
    G_CFLOBDD gat319 = MkNand(gat59, gat156);
    G_CFLOBDD gat322 = MkNor(gat17, gat42);
    G_CFLOBDD gat323 = MkAnd(gat17, gat42);
    G_CFLOBDD gat324 = MkNand(gat159, gat165);
    G_CFLOBDD gat325 = MkOr(gat159, gat165);
    G_CFLOBDD gat326 = MkNand(gat171, gat177);
    G_CFLOBDD gat327 = MkOr(gat171, gat177);
    G_CFLOBDD gat328 = MkNand(gat183, gat189);
    cout << "gat329" << endl;
    G_CFLOBDD gat329 = MkOr(gat183, gat189);
    G_CFLOBDD gat330 = MkNand(gat195, gat201);
    G_CFLOBDD gat331 = MkOr(gat195, gat201);
    G_CFLOBDD gat332 = MkAnd(gat210, gat91);
    G_CFLOBDD gat333 = MkAnd(gat210, gat96);
    G_CFLOBDD gat334 = MkAnd(gat210, gat101);
    G_CFLOBDD gat335 = MkAnd(gat210, gat106);
    G_CFLOBDD gat336 = MkAnd(gat210, gat111);
    G_CFLOBDD gat337 = MkAnd(gat255, gat259);
    G_CFLOBDD gat338 = MkAnd(gat210, gat116);
    cout << "gat339" << endl;
    G_CFLOBDD gat339 = MkAnd(gat255, gat260);
    G_CFLOBDD gat340 = MkAnd(gat210, gat121);
    G_CFLOBDD gat341 = MkAnd(gat255, gat267);
    G_CFLOBDD gat342 = MkNot(gat269);
    G_CFLOBDD gat343 = MkNot(gat273);
    G_CFLOBDD gat344 = MkOr(gat270, gat273);
    G_CFLOBDD gat345 = MkNot(gat276);
    G_CFLOBDD gat346 = MkNot(gat276);
    G_CFLOBDD gat347 = MkNot(gat279);
    G_CFLOBDD gat348 = MkNor(gat280, gat284);
    cout << "gat349" << endl;
    G_CFLOBDD gat349 = MkOr(gat280, gat285);
    G_CFLOBDD gat350 = MkOr(gat280, gat286);
    G_CFLOBDD gat351 = MkNot(gat293);
    G_CFLOBDD gat352 = MkNot(gat294);
    G_CFLOBDD gat353 = MkNot(gat295);
    G_CFLOBDD gat354 = MkNot(gat296);
    G_CFLOBDD gat355 = MkNand(gat89, gat298);
    G_CFLOBDD gat356 = MkAnd(gat90, gat298);
    G_CFLOBDD gat357 = MkNand(gat301, gat302);
    G_CFLOBDD gat360 = MkNand(gat303, gat304);
    cout << "gat363" << endl;
    G_CFLOBDD gat363 = MkNand(gat305, gat306);
    G_CFLOBDD gat366 = MkNand(gat307, gat308);
    G_CFLOBDD gat369 = MkNot(gat310);
    G_CFLOBDD gat375 = MkNor(gat322, gat323);
    G_CFLOBDD gat376 = MkNand(gat324, gat325);
    G_CFLOBDD gat379 = MkNand(gat326, gat327);
    G_CFLOBDD gat382 = MkNand(gat328, gat329);
    G_CFLOBDD gat385 = MkNand(gat330, gat331);
    G_CFLOBDD gat388 = gat290;
    G_CFLOBDD gat389 = gat291;
    cout << "gat390" << endl;
    G_CFLOBDD gat390 = gat292;
    G_CFLOBDD gat391 = gat297;
    G_CFLOBDD gat392 = MkOr(gat270, gat343);
    G_CFLOBDD gat393 = MkNot(gat345);
    G_CFLOBDD gat399 = MkNot(gat346);
    G_CFLOBDD gat400 = MkAnd(gat348, gat73);
    G_CFLOBDD gat401 = MkNot(gat349);
    G_CFLOBDD gat402 = MkNot(gat350);
    G_CFLOBDD gat403 = MkNot(gat355);
    G_CFLOBDD gat404 = MkNot(gat357);
    cout << "gat405" << endl;
    G_CFLOBDD gat405 = MkNot(gat360);
    G_CFLOBDD gat406 = MkAnd(gat357, gat360);
    G_CFLOBDD gat407 = MkNot(gat363);
    G_CFLOBDD gat408 = MkNot(gat366);
    G_CFLOBDD gat409 = MkAnd(gat363, gat366);
    G_CFLOBDD gat410 = MkNand(gat347, gat352);
    G_CFLOBDD gat411 = MkNot(gat376);
    G_CFLOBDD gat412 = MkNot(gat379);
    G_CFLOBDD gat413 = MkAnd(gat376, gat379);
    G_CFLOBDD gat414 = MkNot(gat382);
    cout << "gat415" << endl;
    G_CFLOBDD gat415 = MkNot(gat385);
    G_CFLOBDD gat416 = MkAnd(gat382, gat385);
    G_CFLOBDD gat417 = MkAnd(gat210, gat369);
    G_CFLOBDD gat418 = gat342;
    G_CFLOBDD gat419 = gat344;
    G_CFLOBDD gat420 = gat351;
    G_CFLOBDD gat421 = gat353;
    G_CFLOBDD gat422 = gat354;
    G_CFLOBDD gat423 = gat356;
    G_CFLOBDD gat424 = MkNot(gat400);
    cout << "gat425" << endl;
    G_CFLOBDD gat425 = MkAnd(gat404, gat405);
    G_CFLOBDD gat426 = MkAnd(gat407, gat408);
    G_CFLOBDD gat427 = MkAnd(gat319, MkAnd(gat393, gat55));
    G_CFLOBDD gat432 = MkAnd(gat393, MkAnd(gat17, gat287));
    G_CFLOBDD gat437 = MkNand(gat393, MkNand(gat287, gat55));
    G_CFLOBDD gat442 = MkNand(MkNand(gat375, gat59), MkNand(gat156, gat393));
    G_CFLOBDD gat443 = MkNand(gat393, MkNand(gat319, gat17));
    G_CFLOBDD gat444 = MkAnd(gat411, gat412);
    G_CFLOBDD gat445 = MkAnd(gat414, gat415);
    G_CFLOBDD gat446 = gat392;
    cout << "gat447" << endl;
    G_CFLOBDD gat447 = gat399;
    G_CFLOBDD gat448 = gat401;
    G_CFLOBDD gat449 = gat402;
    G_CFLOBDD gat450 = gat403;
    G_CFLOBDD gat451 = MkNot(gat424);
    G_CFLOBDD gat460 = MkNor(gat406, gat425);
    G_CFLOBDD gat463 = MkNor(gat409, gat426);
    G_CFLOBDD gat466 = MkNand(gat442, gat410);
    G_CFLOBDD gat475 = MkAnd(gat143, gat427);
    G_CFLOBDD gat476 = MkAnd(gat310, gat432);
    cout << "gat477" << endl;
    G_CFLOBDD gat477 = MkAnd(gat146, gat427);
    G_CFLOBDD gat478 = MkAnd(gat310, gat432);
    G_CFLOBDD gat479 = MkAnd(gat149, gat427);
    G_CFLOBDD gat480 = MkAnd(gat310, gat432);
    G_CFLOBDD gat481 = MkAnd(gat153, gat427);
    G_CFLOBDD gat482 = MkAnd(gat310, gat432);
    G_CFLOBDD gat483 = MkNand(gat443, gat1);
    G_CFLOBDD gat488 = MkOr(gat369, gat437);
    G_CFLOBDD gat489 = MkOr(gat369, gat437);
    G_CFLOBDD gat490 = MkOr(gat369, gat437);
    cout << "gat491" << endl;
    G_CFLOBDD gat491 = MkOr(gat369, gat437);
    G_CFLOBDD gat492 = MkNor(gat413, gat444);
    G_CFLOBDD gat495 = MkNor(gat416, gat445);
    G_CFLOBDD gat498 = MkNand(gat130, gat460);
    G_CFLOBDD gat499 = MkOr(gat130, gat460);
    G_CFLOBDD gat500 = MkNand(gat463, gat135);
    G_CFLOBDD gat501 = MkOr(gat463, gat135);
    G_CFLOBDD gat502 = MkAnd(gat91, gat466);
    G_CFLOBDD gat503 = MkNor(gat475, gat476);
    G_CFLOBDD gat504 = MkAnd(gat96, gat466);
    cout << "gat505" << endl;
    G_CFLOBDD gat505 = MkNor(gat477, gat478);
    G_CFLOBDD gat506 = MkAnd(gat101, gat466);
    G_CFLOBDD gat507 = MkNor(gat479, gat480);
    G_CFLOBDD gat508 = MkAnd(gat106, gat466);
    G_CFLOBDD gat509 = MkNor(gat481, gat482);
    G_CFLOBDD gat510 = MkAnd(gat143, gat483);
    G_CFLOBDD gat511 = MkAnd(gat111, gat466);
    G_CFLOBDD gat512 = MkAnd(gat146, gat483);
    G_CFLOBDD gat513 = MkAnd(gat116, gat466);
    G_CFLOBDD gat514 = MkAnd(gat149, gat483);
    cout << "gat515" << endl;
    G_CFLOBDD gat515 = MkAnd(gat121, gat466);
    G_CFLOBDD gat516 = MkAnd(gat153, gat483);
    G_CFLOBDD gat517 = MkAnd(gat126, gat466);
    G_CFLOBDD gat518 = MkNand(gat130, gat492);
    G_CFLOBDD gat519 = MkOr(gat130, gat492);
    G_CFLOBDD gat520 = MkNand(gat495, gat207);
    G_CFLOBDD gat521 = MkOr(gat495, gat207);
    G_CFLOBDD gat522 = MkAnd(gat451, gat159);
    G_CFLOBDD gat523 = MkAnd(gat451, gat165);
    G_CFLOBDD gat524 = MkAnd(gat451, gat171);
    cout << "gat525" << endl;
    G_CFLOBDD gat525 = MkAnd(gat451, gat177);
    G_CFLOBDD gat526 = MkAnd(gat451, gat183);
    G_CFLOBDD gat527 = MkNand(gat451, gat189);
    G_CFLOBDD gat528 = MkNand(gat451, gat195);
    G_CFLOBDD gat529 = MkNand(gat451, gat201);
    G_CFLOBDD gat530 = MkNand(gat498, gat499);
    G_CFLOBDD gat533 = MkNand(gat500, gat501);
    G_CFLOBDD gat536 = MkNor(gat309, gat502);
    G_CFLOBDD gat537 = MkNor(gat316, gat504);
    G_CFLOBDD gat538 = MkNor(gat317, gat506);
    cout << "gat539" << endl;
    G_CFLOBDD gat539 = MkNor(gat318, gat508);
    G_CFLOBDD gat540 = MkNor(gat510, gat511);
    G_CFLOBDD gat541 = MkNor(gat512, gat513);
    G_CFLOBDD gat542 = MkNor(gat514, gat515);
    G_CFLOBDD gat543 = MkNor(gat516, gat517);
    G_CFLOBDD gat544 = MkNand(gat518, gat519);
    G_CFLOBDD gat547 = MkNand(gat520, gat521);
    G_CFLOBDD gat550 = MkNot(gat530);
    G_CFLOBDD gat551 = MkNot(gat533);
    G_CFLOBDD gat552 = MkAnd(gat530, gat533);
    cout << "gat553" << endl;
    G_CFLOBDD gat553 = MkNand(gat536, gat503);
    G_CFLOBDD gat557 = MkNand(gat537, gat505);
    G_CFLOBDD gat561 = MkNand(gat538, gat507);
    G_CFLOBDD gat565 = MkNand(gat539, gat509);
    G_CFLOBDD gat569 = MkNand(gat488, gat540);
    G_CFLOBDD gat573 = MkNand(gat489, gat541);
    G_CFLOBDD gat577 = MkNand(gat490, gat542);
    G_CFLOBDD gat581 = MkNand(gat491, gat543);
    G_CFLOBDD gat585 = MkNot(gat544);
    G_CFLOBDD gat586 = MkNot(gat547);
    cout << "gat587" << endl;
    G_CFLOBDD gat587 = MkAnd(gat544, gat547);
    G_CFLOBDD gat588 = MkAnd(gat550, gat551);
    G_CFLOBDD gat589 = MkAnd(gat585, gat586);
    G_CFLOBDD gat590 = MkNand(gat553, gat159);
    G_CFLOBDD gat593 = MkOr(gat553, gat159);
    G_CFLOBDD gat596 = MkAnd(gat246, gat553);
    G_CFLOBDD gat597 = MkNand(gat557, gat165);
    G_CFLOBDD gat600 = MkOr(gat557, gat165);
    G_CFLOBDD gat605 = MkAnd(gat246, gat557);
    G_CFLOBDD gat606 = MkNand(gat561, gat171);
    cout << "gat609" << endl;
    G_CFLOBDD gat609 = MkOr(gat561, gat171);
    G_CFLOBDD gat615 = MkAnd(gat246, gat561);
    G_CFLOBDD gat616 = MkNand(gat565, gat177);
    G_CFLOBDD gat619 = MkOr(gat565, gat177);
    G_CFLOBDD gat624 = MkAnd(gat246, gat565);
    G_CFLOBDD gat625 = MkNand(gat569, gat183);
    G_CFLOBDD gat628 = MkOr(gat569, gat183);
    G_CFLOBDD gat631 = MkAnd(gat246, gat569);
    G_CFLOBDD gat632 = MkNand(gat573, gat189);
    G_CFLOBDD gat635 = MkOr(gat573, gat189);
    cout << "gat640" << endl;
    G_CFLOBDD gat640 = MkAnd(gat246, gat573);
    G_CFLOBDD gat641 = MkNand(gat577, gat195);
    G_CFLOBDD gat644 = MkOr(gat577, gat195);
    G_CFLOBDD gat650 = MkAnd(gat246, gat577);
    G_CFLOBDD gat651 = MkNand(gat581, gat201);
    G_CFLOBDD gat654 = MkOr(gat581, gat201);
    G_CFLOBDD gat659 = MkAnd(gat246, gat581);
    G_CFLOBDD gat660 = MkNor(gat552, gat588);
    G_CFLOBDD gat661 = MkNor(gat587, gat589);
    G_CFLOBDD gat662 = MkNot(gat590);
    cout << "gat665" << endl;
    G_CFLOBDD gat665 = MkAnd(gat593, gat590);
    G_CFLOBDD gat669 = MkNor(gat596, gat522);
    G_CFLOBDD gat670 = MkNot(gat597);
    G_CFLOBDD gat673 = MkAnd(gat600, gat597);
    G_CFLOBDD gat677 = MkNor(gat605, gat523);
    G_CFLOBDD gat678 = MkNot(gat606);
    G_CFLOBDD gat682 = MkAnd(gat609, gat606);
    G_CFLOBDD gat686 = MkNor(gat615, gat524);
    G_CFLOBDD gat687 = MkNot(gat616);
    G_CFLOBDD gat692 = MkAnd(gat619, gat616);
    cout << "gat696" << endl;
    G_CFLOBDD gat696 = MkNor(gat624, gat525);
    G_CFLOBDD gat697 = MkNot(gat625);
    G_CFLOBDD gat700 = MkAnd(gat628, gat625);
    G_CFLOBDD gat704 = MkNor(gat631, gat526);
    G_CFLOBDD gat705 = MkNot(gat632);
    G_CFLOBDD gat708 = MkAnd(gat635, gat632);
    G_CFLOBDD gat712 = MkNor(gat337, gat640);
    G_CFLOBDD gat713 = MkNot(gat641);
    G_CFLOBDD gat717 = MkAnd(gat644, gat641);
    G_CFLOBDD gat721 = MkNor(gat339, gat650);
    cout << "gat722" << endl;
    G_CFLOBDD gat722 = MkNot(gat651);
    G_CFLOBDD gat727 = MkAnd(gat654, gat651);
    G_CFLOBDD gat731 = MkNor(gat341, gat659);
    G_CFLOBDD gat732 = MkNand(gat654, gat261);
    G_CFLOBDD gat733 = MkNand(gat644, MkNand(gat654, gat261));
    G_CFLOBDD gat734 = MkNand(MkNand(gat635, gat644), MkNand(gat654, gat261));
    G_CFLOBDD gat735 = MkNot(gat662);
    G_CFLOBDD gat736 = MkAnd(gat228, gat665);
    G_CFLOBDD gat737 = MkAnd(gat237, gat662);
    G_CFLOBDD gat738 = MkNot(gat670);
    cout << "gat739" << endl;
    G_CFLOBDD gat739 = MkAnd(gat228, gat673);
    G_CFLOBDD gat740 = MkAnd(gat237, gat670);
    G_CFLOBDD gat741 = MkNot(gat678);
    G_CFLOBDD gat742 = MkAnd(gat228, gat682);
    G_CFLOBDD gat743 = MkAnd(gat237, gat678);
    G_CFLOBDD gat744 = MkNot(gat687);
    G_CFLOBDD gat745 = MkAnd(gat228, gat692);
    G_CFLOBDD gat746 = MkAnd(gat237, gat687);
    G_CFLOBDD gat747 = MkNot(gat697);
    G_CFLOBDD gat748 = MkAnd(gat228, gat700);
    cout << "gat749" << endl;
    G_CFLOBDD gat749 = MkAnd(gat237, gat697);
    G_CFLOBDD gat750 = MkNot(gat705);
    G_CFLOBDD gat751 = MkAnd(gat228, gat708);
    G_CFLOBDD gat752 = MkAnd(gat237, gat705);
    G_CFLOBDD gat753 = MkNot(gat713);
    G_CFLOBDD gat754 = MkAnd(gat228, gat717);
    G_CFLOBDD gat755 = MkAnd(gat237, gat713);
    G_CFLOBDD gat756 = MkNot(gat722);
    G_CFLOBDD gat757 = MkNor(gat727, gat261);
    G_CFLOBDD gat758 = MkAnd(gat727, gat261);
    cout << "gat759" << endl;
    G_CFLOBDD gat759 = MkAnd(gat228, gat727);
    G_CFLOBDD gat760 = MkAnd(gat237, gat722);
    G_CFLOBDD gat761 = MkNand(gat644, gat722);
    G_CFLOBDD gat762 = MkNand(gat635, gat713);
    G_CFLOBDD gat763 = MkNand(gat635, MkNand(gat644, gat722));
    G_CFLOBDD gat764 = MkNand(gat609, gat687);
    G_CFLOBDD gat765 = MkNand(gat600, gat678);
    G_CFLOBDD gat766 = MkNand(gat600, MkNand(gat609, gat687));
    G_CFLOBDD gat767 = gat660;
    G_CFLOBDD gat768 = gat661;
    cout << "gat769" << endl;
    G_CFLOBDD gat769 = MkNor(gat736, gat737);
    G_CFLOBDD gat770 = MkNor(gat739, gat740);
    G_CFLOBDD gat771 = MkNor(gat742, gat743);
    G_CFLOBDD gat772 = MkNor(gat745, gat746);
    G_CFLOBDD gat773 = MkNand(MkNand(gat750, gat762), MkNand(gat763, gat734));
    G_CFLOBDD gat777 = MkNor(gat748, gat749);
    G_CFLOBDD gat778 = MkNand(gat753, MkNand(gat761, gat733));
    G_CFLOBDD gat781 = MkNor(gat751, gat752);
    G_CFLOBDD gat782 = MkNand(gat756, gat732);
    G_CFLOBDD gat785 = MkNor(gat754, gat755);
    cout << "gat786" << endl;
    G_CFLOBDD gat786 = MkNor(gat757, gat758);
    G_CFLOBDD gat787 = MkNor(gat759, gat760);
    G_CFLOBDD gat788 = MkNor(gat700, gat773);
    G_CFLOBDD gat789 = MkAnd(gat700, gat773);
    G_CFLOBDD gat790 = MkNor(gat708, gat778);
    G_CFLOBDD gat791 = MkAnd(gat708, gat778);
    G_CFLOBDD gat792 = MkNor(gat717, gat782);
    G_CFLOBDD gat793 = MkAnd(gat717, gat782);
    G_CFLOBDD gat794 = MkAnd(gat219, gat786);
    G_CFLOBDD gat795 = MkNand(gat628, gat773);
    cout << "gat796" << endl;
    G_CFLOBDD gat796 = MkNand(gat795, gat747);
    G_CFLOBDD gat802 = MkNor(gat788, gat789);
    G_CFLOBDD gat803 = MkNor(gat790, gat791);
    G_CFLOBDD gat804 = MkNor(gat792, gat793);
    G_CFLOBDD gat805 = MkNor(gat340, gat794);
    G_CFLOBDD gat806 = MkNor(gat692, gat796);
    G_CFLOBDD gat807 = MkAnd(gat692, gat796);
    G_CFLOBDD gat808 = MkAnd(gat219, gat802);
    G_CFLOBDD gat809 = MkAnd(gat219, gat803);
    G_CFLOBDD gat810 = MkAnd(gat219, gat804);
    cout << "gat811" << endl;
    G_CFLOBDD gat811 = MkNand(MkNand(gat805, gat787), MkNand(gat731, gat529));
    G_CFLOBDD gat812 = MkNand(gat619, gat796);
    G_CFLOBDD gat813 = MkNand(gat609, MkNand(gat619, gat796));
    G_CFLOBDD gat814 = MkNand(MkNand(gat600, gat609), MkNand(gat619, gat796));
    G_CFLOBDD gat815 = MkNand(MkNand(gat738, gat765), MkNand(gat766, gat814));
    G_CFLOBDD gat819 = MkNand(gat741, MkNand(gat764, gat813));
    G_CFLOBDD gat822 = MkNand(gat744, gat812);
    G_CFLOBDD gat825 = MkNor(gat806, gat807);
    G_CFLOBDD gat826 = MkNor(gat335, gat808);
    G_CFLOBDD gat827 = MkNor(gat336, gat809);
    cout << "gat828" << endl;
    G_CFLOBDD gat828 = MkNor(gat338, gat810);
    G_CFLOBDD gat829 = MkNot(gat811);
    G_CFLOBDD gat830 = MkNor(gat665, gat815);
    G_CFLOBDD gat831 = MkAnd(gat665, gat815);
    G_CFLOBDD gat832 = MkNor(gat673, gat819);
    G_CFLOBDD gat833 = MkAnd(gat673, gat819);
    G_CFLOBDD gat834 = MkNor(gat682, gat822);
    G_CFLOBDD gat835 = MkAnd(gat682, gat822);
    G_CFLOBDD gat836 = MkAnd(gat219, gat825);
    G_CFLOBDD gat837 = MkNand(gat826, MkNand(gat777, gat704));
    cout << "gat838" << endl;
    G_CFLOBDD gat838 = MkNand(MkNand(gat827, gat781), MkNand(gat712, gat527));
    G_CFLOBDD gat839 = MkNand(MkNand(gat828, gat785), MkNand(gat721, gat528));
    G_CFLOBDD gat840 = MkNot(gat829);
    G_CFLOBDD gat841 = MkNand(gat815, gat593);
    G_CFLOBDD gat842 = MkNor(gat830, gat831);
    G_CFLOBDD gat843 = MkNor(gat832, gat833);
    G_CFLOBDD gat844 = MkNor(gat834, gat835);
    G_CFLOBDD gat845 = MkNor(gat334, gat836);
    G_CFLOBDD gat846 = MkNot(gat837);
    G_CFLOBDD gat847 = MkNot(gat838);
    cout << "gat848" << endl;
    G_CFLOBDD gat848 = MkNot(gat839);
    G_CFLOBDD gat849 = MkAnd(gat735, gat841);
    G_CFLOBDD gat850 = gat840;
    G_CFLOBDD gat851 = MkAnd(gat219, gat842);
    G_CFLOBDD gat852 = MkAnd(gat219, gat843);
    G_CFLOBDD gat853 = MkAnd(gat219, gat844);
    G_CFLOBDD gat854 = MkNand(gat845, MkNand(gat772, gat696));
    G_CFLOBDD gat855 = MkNot(gat846);
    G_CFLOBDD gat856 = MkNot(gat847);
    G_CFLOBDD gat857 = MkNot(gat848);
    cout << "gat858" << endl;
    G_CFLOBDD gat858 = MkNot(gat849);
    G_CFLOBDD gat859 = MkNor(gat417, gat851);
    G_CFLOBDD gat860 = MkNor(gat332, gat852);
    G_CFLOBDD gat861 = MkNor(gat333, gat853);
    G_CFLOBDD gat862 = MkNot(gat854);
    G_CFLOBDD gat863 = gat855;
    G_CFLOBDD gat864 = gat856;
    G_CFLOBDD gat865 = gat857;
    G_CFLOBDD gat866 = gat858;
    G_CFLOBDD gat867 = MkNand(gat859, MkNand(gat769, gat669));
    cout << "gat868" << endl;
    G_CFLOBDD gat868 = MkNand(gat860, MkNand(gat770, gat677));
    G_CFLOBDD gat869 = MkNand(gat861, MkNand(gat771, gat686));
    G_CFLOBDD gat870 = MkNot(gat862);
    G_CFLOBDD gat871 = MkNot(gat867);
    G_CFLOBDD gat872 = MkNot(gat868);
    G_CFLOBDD gat873 = MkNot(gat869);
    G_CFLOBDD gat874 = gat870;
    G_CFLOBDD gat875 = MkNot(gat871);
    G_CFLOBDD gat876 = MkNot(gat872);
    G_CFLOBDD gat877 = MkNot(gat873);
    cout << "gat878" << endl;
    G_CFLOBDD gat878 = gat875;
    G_CFLOBDD gat879 = gat876;
    G_CFLOBDD gat880 = gat877;

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Duration: " << duration.count() << " ms" << endl;

    std::vector<G_CFLOBDD> outputs = {
        gat388, gat389, gat390, gat391,
        gat418, gat419, gat420, gat421, gat422, gat423,
        gat446, gat447, gat448, gat449, gat450,
        gat767, gat768,
        gat850,
        gat863, gat864, gat865, gat866,
        gat874, gat878, gat879, gat880
    };

    unsigned int nodeCount = 0, edgeCount = 0;
    Hashset<G_CFLOBDDNodeHandle>* visitedNodesDuringGroupCountNodesAndEdges = new Hashset<G_CFLOBDDNodeHandle>;
    Hashset<G_CFLOBDDReturnMapBody>* visitedEdgesDuringGroupCountNodesAndEdges = new Hashset<G_CFLOBDDReturnMapBody>;
    for (int i = 0; i < outputs.size(); i++) {
        outputs[i].GroupCountNodesAndEdges(nodeCount, edgeCount, visitedNodesDuringGroupCountNodesAndEdges, visitedEdgesDuringGroupCountNodesAndEdges);
    }

    for (int i = 0; i < outputs.size(); i++) {
        unsigned int localNodeCount = 0, localEdgeCount = 0;
        outputs[i].CountNodesAndEdges(localNodeCount, localEdgeCount);
        cout << "Output " << i << ": Nodes = " << localNodeCount << ", Edges = " << localEdgeCount << " total = " << localNodeCount + localEdgeCount << endl;
    }

    cout << "Total number of unique nodes: " << nodeCount << endl;
    cout << "Total number of unique edges: " << edgeCount << endl;
    cout << "Total count: " << nodeCount + edgeCount << endl;

}