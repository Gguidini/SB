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


TEST_CASE("Test SECTION DATA", "[second_pass]"){
    Pre_processor pre_proc("assets/test_second_pass_error.txt");
    std::vector<Token> tokens = pre_proc.run();
    std::unordered_map<std::string, Symbol> symbols = pre_proc.get_symbol_table();
    std::vector<Error> errs = pre_proc.get_errors();
    REQUIRE((errs.size()) == 0);
    Processor proc(tokens, symbols, "assets/test_second_pass_error.txt");
    proc.run();
    errs = proc.get_errors();
    for(Error err : errs){
       std::cout << err << std::endl;
    }
    REQUIRE((errs.size()) == 16);

    REQUIRE(errs[0].get_code() == SEM_ERR);
    //std::cout << errs[0] << std::endl;
    REQUIRE(errs[1].get_code() == LEX_ERR);
    //std::cout << errs[1] << std::endl;
    REQUIRE(errs[2].get_code() == SEM_ERR);
    //std::cout << errs[2] << std::endl;
    REQUIRE(errs[3].get_code() == SYN_ERR);
    //std::cout << errs[3] << std::endl;
    REQUIRE(errs[4].get_code() == SEM_ERR);
    //std::cout << errs[4] << std::endl;
    REQUIRE(errs[5].get_code() == SEM_ERR);
    //std::cout << errs[5] << std::endl;
    REQUIRE(errs[6].get_code() == SEM_ERR);
    //std::cout << errs[6] << std::endl;
    REQUIRE(errs[7].get_code() == SEM_ERR);
    //std::cout << errs[7] << std::endl;
    REQUIRE(errs[8].get_code() == SEM_ERR);
    //std::cout << errs[8] << std::endl;
    REQUIRE(errs[9].get_code() == SEM_ERR);
    //std::cout << errs[9] << std::endl;
    REQUIRE(errs[10].get_code() == SEM_ERR);
    //std::cout << errs[10 << std::endl;
    REQUIRE(errs[11].get_code() == SEM_ERR);
    //std::cout << errs[11 << std::endl;
    REQUIRE(errs[12].get_code() == SEM_ERR);
    //std::cout << errs[12 << std::endl;
    REQUIRE(errs[13].get_code() == SEM_ERR);
    //std::cout << errs[13 << std::endl;
    REQUIRE(errs[14].get_code() == SEM_ERR);
    //std::cout << errs[14 << std::endl;
    REQUIRE(errs[15].get_code() == SEM_ERR);
    //std::cout << errs[15 << std::endl;



}
