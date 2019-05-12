// Unit tests for PreProcessor::_filter_line().
// This function parses a string breaking it in an array of tokens.

#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include <iostream>
#include <fstream>

#include "catch_framework.hpp"
// Lib to test
#include "../src/pre_processor.cpp"

TEST_CASE("Filter lines correctly", "[pre_processor]"){

    Pre_processor proc("assets/test_filter_lines.txt");
    std::ifstream test_file("assets/test_filter_lines.txt");
    std::vector<std::pair<std::string, int>> token;
    std::string line;
    // 1st line - empty
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 0);
    // 2nd line - comment line
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 0);
    // 3rd line - comment line with identation
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 0);
    // 4th line - label token
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 1);
    REQUIRE(token[0].second == LABEL);
    // 5th line - just text
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    REQUIRE(token[0].first == "coisas");
    REQUIRE(token[1].first == "sem");
    REQUIRE(token[2].first == "sentido");
    // 6th line - Label + EQU
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 2);
    REQUIRE(token[0].second == LABEL);
    REQUIRE(token[1].second == EQU);
    // 7th line - IFs
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    for(auto pair : token){
        REQUIRE(pair.second == IF);
    }
    // 8th line - Label + MACRO
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 2);
    REQUIRE(token[0].second == LABEL);
    REQUIRE(token[1].second == MACRO);
}

TEST_CASE("Testing filter lines for EQU"){
    Pre_processor proc("assets/test_PreProcessor_equ.txt");
    std::ifstream test_file("assets/test_PreProcessor_equ.txt");
    std::vector<std::pair<std::string, int>> token;
    std::string line;
    // 1st line - EQU define
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    REQUIRE(token[0] == std::make_pair(std::string("TESTE"), LABEL));
    REQUIRE(token[1] == std::make_pair(std::string("EQU"), EQU));
    REQUIRE(token[2] == std::make_pair(std::string("1"), -1));
    // 2nd line - EQU define
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    REQUIRE(token[0] == std::make_pair(std::string("OUTRO"), LABEL));
    REQUIRE(token[1] == std::make_pair(std::string("EQU"), EQU));
    REQUIRE(token[2] == std::make_pair(std::string("10"), -1));
    // 3rd line - EQU use
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    REQUIRE(token[0] == std::make_pair(std::string("N"), LABEL));
    REQUIRE(token[1] == std::make_pair(std::string("SPACE"), -1));
    REQUIRE(token[2] == std::make_pair(std::string("TESTE"), -1));
}