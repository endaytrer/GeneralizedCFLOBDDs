#include <iostream>
#include "tests.h"

int main(int argc, char** argv) {
    
    if (argc < 2) {
        std::cerr << "Please provide a test name to run." << std::endl;
        return 1;
    }
    std::string testName = argv[1];
    unsigned int grammarChoice = 0;
    if ((testName == "testC6288_9" || testName == "testC6288_10" || testName == "testC432" || testName == "testC6288_8" || testName == "testC6288_16" || testName == "testC880" || testName == "testNQueens") && argc >= 3) {
        grammarChoice = std::stoi(argv[2]);
    }
    unsigned int n = 4;
    if (testName == "testNQueens" && argc >= 4) {
        n = std::stoi(argv[3]);
    }
    Tests::runTests(testName, grammarChoice, n);
    return 0;
}