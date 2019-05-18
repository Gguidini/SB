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
#include <vector>

#include "catch_framework.hpp"
// Lib to test
#include "../src/pre_processor.cpp"


TEST_CASE("Test SECTION DATA", "[second_pass]"){
    Pre_processor pre_proc("assets/test_second_pass.txt");
    std::vector<Token> tokens = pre_proc.run();
    std::unordered_map<std::string, Symbol> symbols = pre_proc.get_symbol_table();
    std::vector<Error> errs = pre_proc.get_errors();
    REQUIRE((errs.size()) == 0);
    Processor proc(tokens, symbols);
    proc.run();
    errs = proc.get_errors();
    REQUIRE((errs.size()) == 15);
    for(Error err : errs){
       std::cout << err << std::endl;
    }
    for(Error err : errs){
        REQUIRE(err.get_code() == SEM_ERR);
    }
}
