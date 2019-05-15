#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include <iostream>
#include <fstream>


#include "catch_framework.hpp"
// Lib to test
#include "../src/pre_processor.cpp"

TEST_CASE("Identifies EQU errors", "[error]"){
    Pre_processor proc("assets/test_PreProcessorErrors_equ.txt");
    proc.run();
    std::vector<Error> err = proc.get_errors();
    REQUIRE((err.size()) == 5);
    for(int i = 0; i < 5; i++){
        REQUIRE(err[i].get_code() == SYN_ERR);
        REQUIRE(err[i].get_line() == 6 + i);
        std::cout << err[i] << std::endl;
    }

}