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
#include "../../src/pre_processor.cpp"

TEST_CASE("Symbol table creation", "[fst_pass]"){
    Pre_processor proc("assets/first_pass/test_symbol_table.txt");
    proc.run();
    std::unordered_map<std::string, Symbol> The_table = proc.get_symbol_table();
    REQUIRE(The_table.size() == 6);
    // FOR label
    REQUIRE(The_table["FOR"].get_value() == 0);
    REQUIRE(The_table["FOR"].can_jump());
    // OUT label
    REQUIRE(The_table["OUT"].get_value() == 5);
    REQUIRE(The_table["OUT"].can_jump());
    // N label
    REQUIRE(The_table["N"].get_value() == 7);
    REQUIRE_FALSE(The_table["N"].can_jump());
    REQUIRE_FALSE(The_table["N"].is_const());
    REQUIRE_FALSE(The_table["N"].is_vector());
    // ONE label
    REQUIRE(The_table["ONE"].get_value() == 8);
    REQUIRE(The_table["ONE"].is_const());
    REQUIRE_FALSE(The_table["ONE"].is_const_zero());
    REQUIRE_FALSE(The_table["ONE"].can_jump());
    REQUIRE_FALSE(The_table["ONE"].is_vector());
    // ZERO label
    REQUIRE(The_table["ZERO"].get_value() == 9);
    REQUIRE(The_table["ZERO"].is_const());
    REQUIRE(The_table["ZERO"].is_const_zero());
    REQUIRE_FALSE(The_table["ZERO"].can_jump());
    REQUIRE_FALSE(The_table["ZERO"].is_vector());
    // VEC label
    REQUIRE(The_table["VEC"].get_value() == 10);
    REQUIRE(The_table["VEC"].is_vector());
    REQUIRE(The_table["VEC"].get_vector_size() == 4);
    REQUIRE_FALSE(The_table["VEC"].is_const());
    REQUIRE_FALSE(The_table["VEC"].can_jump());
}

TEST_CASE("Test SECTION detection", "[section]"){
    Pre_processor proc("assets/first_pass/test_section_error.txt");
    proc.run();
    std::vector<Error> errs = proc.get_errors();

    for(Error err : errs){
        std::cout << err << std::endl;
    }
    REQUIRE(errs.size() == 6);
    // Section DATA before text
    REQUIRE(errs[0].get_code() == SEM_ERR);
    REQUIRE(errs[0].get_line() == 1);
    // Section UNDEFINED
    REQUIRE(errs[1].get_code() == SYN_ERR);
    REQUIRE(errs[1].get_line() == 2);
    // Redefinition of section data
    REQUIRE(errs[2].get_code() == SEM_ERR);
    REQUIRE(errs[2].get_line() == 3);
    // Section DATA before text
    REQUIRE(errs[3].get_code() == SEM_ERR);
    REQUIRE(errs[3].get_line() == 3);
    // Section with no parameter
    REQUIRE(errs[4].get_code() == SYN_ERR);
    REQUIRE(errs[4].get_line() == 4);
    // Section text missing
    REQUIRE(errs[5].get_code() == SEM_ERR);
}

TEST_CASE("Test labels", "[first_pass]"){
    Pre_processor proc("assets/first_pass/test_labels.txt");
    proc.run();
    std::vector<Error> errs = proc.get_errors();
    for(Error err : errs){
       std::cout << err << std::endl;
    }
    REQUIRE((errs.size()) == 14);
    for(Error err : errs){
        REQUIRE(err.get_code() == SEM_ERR);
    }
}

TEST_CASE("Test symbol table for next pass"){
    Pre_processor proc("assets/second_pass/test_Processor_1.txt");
    proc.run();
    std::unordered_map<std::string, Symbol> symbols = proc.get_symbol_table();
    std::vector<Error> errs = proc.get_errors();
    for(Error err : errs){
        std::cout << err << std::endl;
    }
    REQUIRE(symbols.size() == 5);
    REQUIRE(symbols["WHILE"].get_value() == 4);
    REQUIRE(symbols["FIM"].get_value() == 28);
    REQUIRE(symbols["N"].get_value() == 29);
    REQUIRE(symbols["AUX"].get_value() == 30);
    REQUIRE(symbols["DOIS"].get_value() == 31);

}

