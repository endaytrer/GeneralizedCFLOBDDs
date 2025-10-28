#ifndef TESTS_H
#define TESTS_H

#include <string>

class Tests {
public:
    static void runTests(std::string testName);
    static void testMkTrueAndFalse();
    static void testMkProjection();
    static void testParity();
    static void testDataVisualization();
    static void testMkNot();
    static void testCrossProduct();
    static void testNand();
    static void testRandomFunction();
    static void testC17();
    static void testC432();
    static void testC880();
    static void testC6288_8();
    static void testC6288_9();
    static void testC6288_10();
    static void testC6288_12();
};

#endif // TESTS_H