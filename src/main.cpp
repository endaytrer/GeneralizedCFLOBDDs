#include <iostream>
#include "tests.h"

int main(int argc, char** argv) {
    
    if (argc < 2) {
        std::cerr << "Please provide a test name to run." << std::endl;
        return 1;
    }
    std::string testName = argv[1];
    Tests::runTests(testName);
    return 0;
}