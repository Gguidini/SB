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
#include "../src/processor.cpp"


TEST_CASE("Test Int to bynary program", "[second_pass]"){
    Pre_processor pre_proc("assets/final_tests/test_Processor_1.txt");
    std::vector<Token> tokens = pre_proc.run();
    pre_proc.generate_output();
    std::unordered_map<std::string, Symbol> symbols = pre_proc.get_symbol_table();
    std::vector<Error> pre_proc_errs = pre_proc.get_errors();
    REQUIRE((pre_proc_errs.size()) == 0);
    Processor proc(tokens, symbols, "assets/final_tests/test_Processor_1.pre");
    std::vector<int> object_code = proc.run();
    proc.generate_output();
    std::vector<int> answer = {12, 29, 10, 29, 8, 28, 4, 31, 3, 31, 11, 30, 10, 29, 2, 30, 11, 30, 13, 30, 10, 29, 4, 31, 11, 29, 5, 4, 14, 0, 0, 2};
    std::vector<Error> proc_errs = proc.get_errors();
    REQUIRE((proc_errs.size()) == 0);
    REQUIRE((object_code.size()) == (answer.size()));
    for(int i = 0; i < (int)answer.size(); i++){
        REQUIRE(object_code[i] == answer[i]);
    }
}
