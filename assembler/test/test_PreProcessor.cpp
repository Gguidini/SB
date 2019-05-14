// Unit test for Pre_processor.cpp
// Assuming that program is correct.
// No error handlin

// Pre_processor's responsibilities and tasks:
//
//  1. Read file removing comments, blank lines, and unnecessary enters.
//  2. Identify MACRO diretive.
//  3. Every time MACRO is found, create a new entry in MNT and copy all lines between MACRO and ENDMACRO
//     to new entry in MDT. Associate MNT and MDT entries.
//  4. Identify EQU directive.
//  5. Associate EQU label to defined value.
//  6. Identify IF
//  7. Process IF: verify if argument is 1. If it is one, process next line. If it's zero, skip next line.
//  8. Generate .txt file with code after pre processing. Name is the same of original file with extension
//     .pre (e.g. input.pre).
//  9. Pass vector of strings (the pre processed file) to assembler.
// 10. Verify that file passed as reference exists
// 11. Verify that file can be opened
// 12. Verify that file's extension is .txt

#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include <iostream>
#include <fstream>

#include "catch_framework.hpp"
// Lib to test
#include "../src/pre_processor.cpp"

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
            Pre_processor proc("assets/test_PreProcessor.txt");
        }());
        Pre_processor proc("assets/test_PreProcessor.txt");
        REQUIRE(proc.get_input_name() == "assets/test_PreProcessor.txt");
        REQUIRE(proc.get_buffer().size() == 0);
        REQUIRE(proc.get_file_pointer().is_open());
        REQUIRE(proc.get_output_name() == "assets/test_PreProcessor.pre");
        REQUIRE(proc.get_done() == false);
    }
    
    SECTION("Verifiy that I cannot create a Pre_processor object with an unacceptable file"){
        // FIXME: Fix test to throw a custom expection when creating class
        REQUIRE_THROWS([&](){
            Pre_processor proc("unnacceptable_file.txt");
            if(!proc.get_file_pointer().is_open()){
                throw 1;
            }
        }());
    }
    
    SECTION("Verify that I cannot create a Pre_processor object without a file") {
        // FIXME: Fix test to throw a custom expection when creating class
        REQUIRE_THROWS([&](){
            Pre_processor proc = Pre_processor();
            if(proc.get_input_name() == ""){
                throw 1;
            }
        }());
    }

    SECTION("Verify that file's extension is .txt"){
        // FIXME: Fix test to throw a custom expection when creating class
        REQUIRE_THROWS([&](){
            Pre_processor proc("assets/test_PreProcessor.pdf");
            std::string name = proc.get_input_name();
            name = name.substr (name.size() - 4,4);
            if(name != ".txt"){
                throw 1;
            } 
        }());
    }
}

TEST_CASE("Generates output file", "[pre_processor]"){
    Pre_processor proc("assets/test_PreProcessor_comment.txt");
    proc.run();
    std::string out_name = proc.generate_output();
    std::ifstream out(out_name);
    REQUIRE(out.is_open());
    REQUIRE(proc.get_done());
}

TEST_CASE("Orignal file parsing test", "[pre_processor]") {

    SECTION("Removes empty lines") {
        Pre_processor proc("assets/test_PreProcessor_empty_spaces.txt");
        std::vector<std::string> lines = proc.run();
        std::string out_name = proc.generate_output();
        lines = read_file(out_name);
        std::vector<std::string> expected{
            "Non Empty Line 1",
            "Non Empty Line 2",
            "Non Empty Line 3",
            "Non Empty Line 4",
            "Non Empty Line 5"
        };
        REQUIRE(lines.size() == expected.size());
        for(int i = 0; i < (int)expected.size(); i++){
            REQUIRE(lines[i] == expected[i]);
        }
    }

    SECTION("Removes comments and comment lines") {
        Pre_processor proc("assets/test_PreProcessor_comment.txt");
        std::vector<std::string> lines = proc.run();
        std::string out_name = proc.generate_output();
        lines = read_file(out_name);
        std::vector<std::string> expected{
            "Non Empty Line 1",
            "Non Empty Line 2",
            "Non Empty Line 3",
            "Non Empty Line 4",
            "Non Empty Line 5"
        };
        REQUIRE(lines.size() == expected.size());
        for(int i = 0; i < (int)expected.size(); i++){
            REQUIRE(lines[i] == expected[i]);
        }
    }
}

TEST_CASE("Test MACRO processing", "[directives]") {

    SECTION("Macros WITHOUT parameters"){
        Pre_processor proc("assets/test_PreProcessor_macro.txt");
        std::vector<std::string> lines = proc.run();
        std::string out_name = proc.generate_output();
        lines = read_file(out_name);
        std::vector<std::string> expected{
            "HELLOW WORLD",
            "COPY X,Y",
            "COPY A,TEMP",
            "COPY B,A",
            "COPY TEMP,B",
            "GOOD BYE WORLD",
            "COPY A,TEMP",
            "COPY B,A",
            "COPY TEMP,B",
        };
        REQUIRE(lines.size() == expected.size());
        for(int i = 0; i < (int)expected.size(); i++){
            REQUIRE(lines[i] == expected[i]);
        }
    }

    SECTION("Macros WITH parameters"){
        Pre_processor proc("assets/test_PreProcessor_macro_parameters.txt");
        std::vector<std::string> lines = proc.run();
        std::string out_name = proc.generate_output();
        lines = read_file(out_name);
        std::vector<std::string> expected{
            "HELLOW WORLD",
            "COPY A, B",
            "COPY A, Z",
            "COPY B, A",
            "COPY Z, B",
            "GOOD BYE WORLD",
            "COPY B, Z",
            "COPY A, B",
            "COPY Z, A"
        };
        REQUIRE(lines.size() == expected.size());
        for(int i = 0; i < (int)expected.size(); i++){
            REQUIRE(lines[i] == expected[i]);
        }
    }
    
}

TEST_CASE("Test EQU processing", "[directives]") {
    Pre_processor proc("assets/test_PreProcessor_equ.txt");
    std::vector<std::string> lines = proc.run();
    std::string out_name = proc.generate_output();
    lines = read_file(out_name);
    std::vector<std::string> expected{
        "N: SPACE 1",
        "ADD N+1",
        "SUB N+10",
        "K: CONST 10"
    };
    REQUIRE(lines.size() == expected.size());
    for(int i = 0; i < (int)expected.size(); i++){
        REQUIRE(lines[i] == expected[i]);
    }
}

TEST_CASE("Test IF processing", "[directives]") {
    Pre_processor proc("assets/test_PreProcessor_if.txt"); 
    std::vector<std::string> lines = proc.run();
    std::string out_name = proc.generate_output();
    lines = read_file(out_name);
    std::vector<std::string> expected{
        "This line is ok",
        "This line has to be in the program",
        "This line is ok"
    };
    REQUIRE(lines.size() == expected.size());
    for(int i = 0; i < (int)expected.size(); i++){
        REQUIRE(lines[i] == expected[i]);   
    }
}