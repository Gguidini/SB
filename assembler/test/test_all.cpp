// Execute all tests

#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include <iostream>
#include <fstream>

#include "catch_framework.hpp"

// Test files
#include "test_opcodes.cpp"
#include "test_filter_line.cpp"
#include "test_first_pass.cpp"
#include "test_second_pass.cpp"
#include "test_PreProcessor.cpp"
#include "test_PreProcessor_errors.cpp"