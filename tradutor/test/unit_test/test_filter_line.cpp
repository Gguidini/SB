// Unit tests for PreProcessor::_filter_line().
// This function parses a string breaking it in an array of tokens.

#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include <iostream>
#include <fstream>

#include "catch_framework.hpp"
// Lib to test
#include "../../src/pre_processor.cpp"

TEST_CASE("Filter lines correctly", "[pre_processor]"){

    Pre_processor proc("assets/pre_processor/test_process_line.txt");
    std::ifstream test_file("assets/pre_processor/test_process_line.txt");
    std::vector<std::pair<std::string, int>> token;
    std::string line;
    // Comment and blank lines are supposed to be blank
    for(int i = 0; i < 4; i++){
        getline(test_file, line);
        token = proc._filter_line(line);
        REQUIRE(token.size() == 1);
        
    }
    // 0th line - section
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    REQUIRE(token[0] == std::make_pair(std::string("SECTION"), SEC));
    // 1st line - Input
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    REQUIRE(token[0] == std::make_pair(std::string("INPUT"), OP));
    // 2nd line - COPY
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 4);
    REQUIRE(token[0] == std::make_pair(std::string("COPY"), OP));
    REQUIRE(token[1] == std::make_pair(std::string("N1"), -1));
    REQUIRE(token[2] == std::make_pair(std::string("N2"), -1));
    // 3rd line - S_INPUT
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 4);
    REQUIRE(token[0] == std::make_pair(std::string("S_INPUT"), OP));
    REQUIRE(token[1] == std::make_pair(std::string("S1"), -1));
    REQUIRE(token[2] == std::make_pair(std::string("4"), -1));
    // 4th line - STOP
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 2);
    // 5th line - CONST
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 4);
    REQUIRE(token[0] == std::make_pair(std::string("N2"), LABEL));
    REQUIRE(token[1] == std::make_pair(std::string("CONST"), DIR));
    REQUIRE(token[2] == std::make_pair(std::string("1"), -1));
}

TEST_CASE("Testing filter lines for EQU"){
    Pre_processor proc("assets/pre_processor/test_PreProcessor_equ.txt");
    std::ifstream test_file("assets/pre_processor/test_PreProcessor_equ.txt");
    std::vector<std::pair<std::string, int>> token;
    std::string line;
    // 1st line - EQU define
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 4);
    REQUIRE(token[0] == std::make_pair(std::string("TESTE"), LABEL));
    REQUIRE(token[1] == std::make_pair(std::string("EQU"), EQU));
    REQUIRE(token[2] == std::make_pair(std::string("1"), -1));
    // 2nd line - EQU define
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 4);
    REQUIRE(token[0] == std::make_pair(std::string("OUTRO"), LABEL));
    REQUIRE(token[1] == std::make_pair(std::string("EQU"), EQU));
    REQUIRE(token[2] == std::make_pair(std::string("10"), -1));
    // 3rd line - EQU use
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 4);
    REQUIRE(token[0] == std::make_pair(std::string("N"), LABEL));
    REQUIRE(token[1] == std::make_pair(std::string("SPACE"), DIR));
    REQUIRE(token[2] == std::make_pair(std::string("TESTE"), -1));
    // 4th line - EQU use
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 5);
    REQUIRE(token[0] == std::make_pair(std::string("ADD"), OP));
    REQUIRE(token[1] == std::make_pair(std::string("N"), -1));
    REQUIRE(token[2] == std::make_pair(std::string("+"), -1));
    REQUIRE(token[3] == std::make_pair(std::string("TESTE"), -1));
}
