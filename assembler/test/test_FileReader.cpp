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

TEST_CASE("Testing load_file function", "[file_reader") {


    SECTION("Check if load_file fails for no reference and wrong reference."){
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