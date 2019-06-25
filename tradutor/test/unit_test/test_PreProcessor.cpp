// Unit test for Pre_processor.cpp
// Assuming that program is correct.
// No error handlin

#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include <iostream>
#include <fstream>

#include "catch_framework.hpp"
// Lib to test
#include "../../src/pre_processor.cpp"

std::vector<std::string> read_file(std::string file){
    std::vector<std::string> out;
    std::ifstream fd(file);
    if(!fd.is_open()){
        throw("Output File not Found");
    }
    std::string line;
    while(getline(fd, line)){
        out.push_back(line);
    }

    return out;
}

TEST_CASE("Testing class creation", "[pre_processor]") {

    SECTION("Verifiy that I can create a Pre_processor object with an acceptable file") {
        REQUIRE_NOTHROW( [&](){
            Pre_processor proc("assets/pre_processor/test_PreProcessor_comment.txt");
        }());
        Pre_processor proc("assets/pre_processor/test_PreProcessor_comment.txt");
        REQUIRE(proc.get_input_name() == "assets/pre_processor/test_PreProcessor_comment.txt");
        REQUIRE(proc.get_buffer().size() == 0);
        REQUIRE(proc.get_file_pointer().is_open());
        REQUIRE(proc.get_output_name() == "assets/pre_processor/test_PreProcessor_comment.pre");
        REQUIRE(proc.get_done() == false);
    }
    
    SECTION("Verifiy that I cannot create a Pre_processor object with an unacceptable file"){
        REQUIRE_THROWS([&](){
            Pre_processor proc("unnacceptable_file.txt");
        }());
    }

    SECTION("Verify that file's extension is .txt"){
        REQUIRE_THROWS([&](){
            Pre_processor proc("assets/pre_processor/test_PreProcessor.pdf");
        }());
    }
}

TEST_CASE("Generates output file", "[pre_processor]"){
    Pre_processor proc("assets/pre_processor/test_PreProcessor_comment.txt");
    proc.run();
    std::string out_name = proc.generate_output();
    std::ifstream out(out_name);
    REQUIRE(out.is_open());
    REQUIRE(proc.get_done());
}

TEST_CASE("Orignal file parsing test", "[pre_processor]") {

    SECTION("Removes empty lines") {
        Pre_processor proc("assets/pre_processor/test_PreProcessor_empty_spaces.txt");
        std::vector<std::string> lines;
        proc.run();
        std::string out_name = proc.generate_output();
        lines = read_file(out_name);
        std::vector<std::string> expected{
            "NON EMPTY LINE 1",
            "NON EMPTY LINE 2",
            "NON EMPTY LINE 3",
            "NON EMPTY LINE 4",
            "NON EMPTY LINE 5"
        };
        REQUIRE(lines.size() == expected.size());
        for(int i = 0; i < (int)expected.size(); i++){
            REQUIRE(lines[i] == expected[i]);
        }
    }

    SECTION("Removes comments and comment lines") {
        Pre_processor proc("assets/pre_processor/test_PreProcessor_comment.txt");
        std::vector<std::string> lines;
        proc.run();
        std::string out_name = proc.generate_output();
        lines = read_file(out_name);
        std::vector<std::string> expected{
            "NON EMPTY LINE 1",
            "NON EMPTY LINE 2",
            "NON EMPTY LINE 3",
            "NON EMPTY LINE 4",
            "NON EMPTY LINE 5"
        };
        REQUIRE(lines.size() == expected.size());
        for(int i = 0; i < (int)expected.size(); i++){
            REQUIRE(lines[i] == expected[i]);
        }
    }
}

TEST_CASE("Test EQU processing", "[directives]") {
    Pre_processor proc("assets/pre_processor/test_PreProcessor_equ.txt");
    std::vector<std::string> lines;
    proc.run();
    std::string out_name = proc.generate_output();
    lines = read_file(out_name);
    std::vector<std::string> expected{
        "N: SPACE 1",
        "ADD N + 1",
        "SUB N + 10",
        "K: CONST 10"
    };
    REQUIRE(lines.size() == expected.size());
    for(int i = 0; i < (int)expected.size(); i++){
        REQUIRE(lines[i] == expected[i]);
    }
}

TEST_CASE("Test IF processing", "[directives]") {
    Pre_processor proc("assets/pre_processor/test_PreProcessor_if.txt"); 
    std::vector<std::string> lines;
    proc.run();
    std::string out_name = proc.generate_output();
    lines = read_file(out_name);
    std::vector<std::string> expected{
        "THIS LINE IS OK",
        "THIS LINE HAS TO BE IN THE PROGRAM",
        "THIS LINE HAS TO BE IN THE PROGRAM",
        "THIS LINE IS OK"
    };
    REQUIRE(lines.size() == expected.size());
    for(int i = 0; i < (int)expected.size(); i++){
        REQUIRE(lines[i] == expected[i]);   
    }
}