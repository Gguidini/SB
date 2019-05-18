// Unit tests for functions of Pre_processor that actually do the assembler's first pass
// This includes:
// 1. Creation of symbol table
// 2. Return of token vector of the entire file for processing
// 3. Some error detection

#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "catch_framework.hpp"
// Lib to test
#include "../src/pre_processor.cpp"


TEST_CASE("Test SECTION DATA", "[second_pass]"){
    Pre_processor proc("assets/test_labels.txt");
    proc.run();
    std::vector<Error> errs = proc.get_errors();
    for(Error err : errs){
       std::cout << err << std::endl;
    }
    REQUIRE((errs.size()) == 11);
    for(Error err : errs){
        REQUIRE(err.get_code() == SEM_ERR);
    }
}
