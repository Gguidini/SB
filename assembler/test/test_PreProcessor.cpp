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

TEST_CASE("Testing class creation", "[pre_processor]") {

    SECTION("Verifiy that I can create a Pre_processor object with an acceptable file") {
        REQUIRE_NOTHROW(Pre_processor proc("assets/test_PreProcessor.txt"));
        Pre_processor proc("assets/test_PreProcessor.txt");
        REQUIRE(proc.references() == "assets/test_PreProcessor.txt");
        REQUIRE(proc.buffer().size() == 0);
        REQUIRE(proc.file_pointer().is_open());
        REQUIRE(proc.output_file_name() == "assets/test_PreProcessor.pre");
        REQUIRE(proc.is_done() == false);
    }
    
    SECTION("Verifiy that I cannot create a Pre_processor object with an unacceptable file"){
        REQUIRE_THROWS_AS(Pre_processor proc("unnacceptable_file.txt"), InvalidFileException);
    }
    
    SECTION("Verify that I cannot create a Pre_processor object without a file") {
        REQUIRE_THROWS_AS(Pre_processor proc(), NoFileException);
    }

    SECTION("Verify that file's extension is .txt"){
        REQUIRE_THROWS_AS(Pre_processor proc("assets/test_PreProcessor.pdf"), InvalidFileException);
    }
}

TEST_CASE("Generates output file", "[pre_processor]"){
    Pre_processor proc("assets/test_PreProcessor_output_creation.txt");
    proc.run();
    proc.generate_output();
    ifstream out(proc.output_file_name());
    REQUIRE(out.is_open());
}

TEST_CASE("Orignal file parsing test", "[pre_processor]") {

    SECTION("Removes empty lines") {
        Pre_processor proc("assets/test_PreProcessor_empty_spaces.txt");
        std::vector<std::string> lines = proc.run();
        REQUIRE(lines.size() == 5);
        for(int i = 0; i < 5; i++)
            REQUIRE(lines[i] == ("Non Empty Line " + to_string(i + 1)));
    }

    SECTION("Removes comments and comment lines") {
        Pre_processor proc("assets/test_PreProcessor_comment.txt");
        std::vector<std::string> lines = proc.run();
        REQUIRE(lines.size() == 5);
        for(int i = 0; i < 5; i++)
            REQUIRE(lines[i] == ("Non Empty Line " + to_string(i + 1)));
    }
}

TEST_CASE("Test MACRO processing", "[directives]") {

    SECTION("Macros WITHOUT parameters"){
        Pre_processor proc("assets/test_PreProcessor_macro.txt");
        std::vector<std::string> lines = proc.run();
        std::vector<std::string> expected{
            "HELLOW WORLD",
            "COPY A, B",
            "COPY A, TEMP",
            "COPY B, A",
            "COPY TEMP, B",
            "GOOD BYE WORLD",
            "COPY A, TEMP",
            "COPY B, A",
            "COPY TEMP, B"
        };
        REQUIRE(lines.size() == expected.size());
        for(int i = 0; i < lines.size(); i++)
            REQUIRE(lines[i] == expected[i]);
    }

    SECTION("Macros WITH parameters"){
Pre_processor proc("assets/test_PreProcessor_macro_parameters.txt");
        std::vector<std::string> lines = proc.run();
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
        for(int i = 0; i < lines.size(); i++)
            REQUIRE(lines[i] == expected[i]);
    }
    
}

TEST_CASE("Test EQU processing", "[directives]") {
    Pre_processor proc("assets/test_PreProcessor_equ.txt");
}

TEST_CASE("Test IF processing", "[directives]") {
    Pre_processor proc("assets/test_PreProcessor_if.txt");    
}