#ifndef TESTS_H
#define TESTS_H

#include <string>

class Tests {
public:
    static void runTests(std::string testName, unsigned int grammarChoice, unsigned int n = 4);
    static void testMkTrueAndFalse();
    static void testMkProjection();
    static void testParity();
    static void testDataVisualization();
    static void testMkNot();
    static void testCrossProduct();
    static void testNand();
    static void testRandomFunction();
    static void testSynFun2();
    static void testSynFun3();
    static void testSynFun4();
    static void testSynFun5();
    static void testSynFun6();
    static void testSynFun7();
    static void testSynFun8();
    static void testSynFun9();
    static void testSynFun10();
    static void testSynFun11();
    static void testC17();
    static void testC432(unsigned int grammarChoice);
    static void testC880(unsigned int grammarChoice);
    static void testC6288_8(unsigned int grammarChoice);
    static void testC6288_9(unsigned int grammarChoice);
    static void testC6288_10(unsigned int grammarChoice);
    static void testC6288_12();
    static void testC6288_16(unsigned int grammarChoice);
    static void testNQueens(unsigned int n, unsigned int grammarChoice);
    static void testBDDGrammar();
};

#endif // TESTS_H