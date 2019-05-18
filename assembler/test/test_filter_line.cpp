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
    // 0th line - section
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    REQUIRE(token[0] == std::make_pair(std::string("SECTION"), SEC));
    // 1st line - empty
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 1);
    REQUIRE(token[0] == std::make_pair(std::string("\n"), ENDL));
    // 2nd line - comment line
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 1);
    REQUIRE(token[0] == std::make_pair(std::string("\n"), ENDL));    
    // 3rd line - comment line with identation
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 1);
    REQUIRE(token[0] == std::make_pair(std::string("\n"), ENDL));
    // 4th line - label token
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 2);
    REQUIRE(token[0].second == LABEL);
    // 5th line - just text
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 4);
    REQUIRE(token[0].first == "coisas");
    REQUIRE(token[1].first == "sem");
    REQUIRE(token[2].first == "sentido");
    // 6th line - Label + EQU
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    REQUIRE(token[0].second == LABEL);
    REQUIRE(token[1].second == EQU);
    // 7th line - IFs
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 4);
    token.pop_back(); // remove o token \n
    for(auto pair : token){
        REQUIRE(pair.second == IF);
    }
    // 8th line - Label + MACRO
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    REQUIRE(token[0].second == LABEL);
    REQUIRE(token[1].second == MACRO);
    // 9th line - Opcodes
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 7);
    token.pop_back(); // remove token \n
    for(std::pair<std::string, int> p : token){
        REQUIRE(p.second == OP);
    }
    // 10th line - COPY
    getline(test_file, line);
    token = proc._filter_line(line);
    REQUIRE(token.size() == 3);
    REQUIRE(token[0].second == OP);
    REQUIRE(token[1].first == "A,B");
}

TEST_CASE("Testing filter lines for EQU"){
    Pre_processor proc("assets/test_PreProcessor_equ.txt");
    std::ifstream test_file("assets/test_PreProcessor_equ.txt");
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

TEST_CASE("Testing detection of lexical errors", "[error]"){
    Pre_processor proc("assets/test_filter_lines_error.txt");
    proc.run();
    REQUIRE(proc.get_done());
    std::vector<Error> errs = proc.get_errors();
    REQUIRE(errs.size() == 5);
    for(int i = 0; i < 5; i++){
        REQUIRE(errs[i].get_code() == LEX_ERR);
        REQUIRE(errs[i].get_line() == (5 + i));
        std::cout << errs[i] << std::endl;
    }
}