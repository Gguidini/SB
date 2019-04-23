// This file has Unit Tests for the File_reader class.
// Tests are created using the Catch framework (header file)

#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include "catch_framework.hpp"
// Lib to test
#include "../src/file_reader.cpp"

TEST_CASE("Test that Class creation is ok", "[file_reader]") {

    SECTION("Blank class creation"){
        File_reader reader = File_reader();

        REQUIRE(reader.size() == 0);
        REQUIRE(reader.is_loaded() == false);
        REQUIRE(reader.done() == false);
        REQUIRE(reader.references() == "");
    }

    SECTION("Creating class with reference"){
        File_reader reader = File_reader("assets/test_FileReader.txt");

        REQUIRE(reader.size() == 0);
        REQUIRE(reader.is_loaded() == false);
        REQUIRE(reader.done() == false);
        REQUIRE(reader.references() == "assets/test_FileReader.txt");
    }
}

TEST_CASE("Testing load_file function", "[file_reader]") {

    SECTION("Check if load_file loads properly."){
        File_reader reader = File_reader();
        // Loading file with no reference - FAIL
        REQUIRE_FALSE(reader.load_file());
        REQUIRE(reader.is_loaded() == false);
        // Loading file passing wrong reference - FAIL
        REQUIRE_FALSE(reader.load_file("batata.fail"));
        REQUIRE(reader.is_loaded() == false);
        // Loading file created with wrong reference - FAIL
        reader = File_reader("creating_batata.fail");
        REQUIRE_FALSE(reader.load_file());
        REQUIRE(reader.is_loaded() == false);
        // Loading file passing correct reference - PASS
        REQUIRE(reader.load_file("assets/test_FileReader.txt"));
        REQUIRE(reader.references() == "assets/test_FileReader.txt");
        REQUIRE(reader.is_loaded() == true);
        // Loading file created with corret reference - PASS
        reader = File_reader("assets/test_FileReader.txt");
        REQUIRE(reader.load_file());
        REQUIRE(reader.is_loaded() == true);
    }
}

TEST_CASE("Testing if load_file() annotates lines correctly", "[file_reader]") {
    // Creater reader. Loads file.
    File_reader reader = File_reader("assets/test_FileReader.txt");
    reader.load_file();
    // Making sure it's ok to proceed
    REQUIRE(reader.is_loaded() == true);
    REQUIRE(reader.size() > 0);
    REQUIRE(reader.done() == false);
    // Tests first line.
    std::string line;
    line = reader.next_line();
    REQUIRE(line == "THIS LINE SHOULD GO COMPLETE.");
    REQUIRE(reader.current_line_number() == 1);
    // Tests second line - blank line compression
    line = reader.next_line();
    REQUIRE(line == "This line's number must be 4.");
    REQUIRE(reader.current_line_number() == 4);
    // Third line - Trimming left
    line = reader.next_line();
    REQUIRE(line == "Check for trimming left.");
    REQUIRE(line.size() == 24);
    REQUIRE(reader.current_line_number() == 5);
    // Fourth line - Trimming right
    line = reader.next_line();
    REQUIRE(line == "Check for trimming right.");
    REQUIRE(line.size() == 25);
    REQUIRE(reader.current_line_number() == 6);
    // Fifth line - removes comments
    line = reader.next_line();
    REQUIRE(line == "Removes comments? ");
    // Remove lines that are only comments
    line = reader.next_line();
    REQUIRE(line == "The number of this line must be 10.");
    REQUIRE(reader.current_line_number() == 10);
    // Now reading must be done
    REQUIRE(reader.done() == true);
}