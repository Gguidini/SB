#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include <iostream>
#include <fstream>


#include "catch_framework.hpp"
// Lib to test
#include "../../src/pre_processor.cpp"

TEST_CASE("Identifies EQU errors", "[equ_error]"){
    Pre_processor proc("assets/pre_processor/test_PreProcessorErrors_equ.txt");
    proc.run();
    std::vector<Error> err = proc.get_errors();
    REQUIRE((err.size()) == 5);
    for(int i = 0; i < 5; i++){
        REQUIRE(err[i].get_code() == SYN_ERR);
        REQUIRE(err[i].get_line() == 7 + i);
        std::cout << err[i] << std::endl;
    }

}

TEST_CASE("Identifies IF errors", "[error]"){
    Pre_processor proc("assets/pre_processor/test_PreProcessorErrors_if.txt");
    proc.run();
    std::vector<Error> err = proc.get_errors();
    REQUIRE((err.size()) == 5);
    for(int i = 0; i < 5; i++){
        REQUIRE(err[i].get_code() == SYN_ERR);
        REQUIRE(err[i].get_line() == 11 + i);
        std::cout << err[i] << std::endl;
    }
}

TEST_CASE("Identifies MACRO errors", "[error]"){
    Pre_processor proc("assets/pre_processor/test_PreProcessorErrors_macro.txt");
    proc.run();
    std::vector<Error> errs = proc.get_errors();
    for(Error err : errs){
        std::cout << err << std::endl;
    }
    REQUIRE((errs.size()) == 8);

    REQUIRE(errs[0].get_code() == LEX_ERR);
    REQUIRE(errs[0].get_line() == 1);

    REQUIRE(errs[1].get_code() == SYN_ERR);
    REQUIRE(errs[1].get_line() == 8);

    REQUIRE(errs[2].get_code() == SYN_ERR);
    REQUIRE(errs[2].get_line() == 11);

    REQUIRE(errs[3].get_code() == SYN_ERR);
    REQUIRE(errs[3].get_line() == 15);

    REQUIRE(errs[4].get_code() == SYN_ERR);
    REQUIRE(errs[4].get_line() == 20);

    REQUIRE(errs[5].get_code() == LEX_ERR);
    REQUIRE(errs[5].get_line() == 20);

    REQUIRE(errs[6].get_code() == LEX_ERR);
    REQUIRE(errs[6].get_line() == 25);

    REQUIRE(errs[7].get_code() == SEM_ERR);
    REQUIRE(errs[7].get_line() == 31);
}
