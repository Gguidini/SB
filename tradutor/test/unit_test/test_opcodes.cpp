// This file has Unit Tests for the opcodes.h lib.
// Tests are created using the Catch framework (header file)

#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include "catch_framework.hpp"
// Lib to test
#include "../../lib/opcodes.h"

TEST_CASE( "Create instruction", "[instruction]" ) {
    Instruction inst(15, "test instruction", 5);

    REQUIRE(inst.opcode() == 15);
    REQUIRE(inst.mnemonic() == "test instruction");
    REQUIRE(inst.operands() == 5);
}

TEST_CASE("The table of instructions is OK", "[instruction]") {
    std::unordered_map<std::string, Instruction> all_instructions;
    all_instructions =  table_of_instructions();

    REQUIRE(all_instructions.size() == 20);

    SECTION("OPCODES match NAMES"){
        std::vector<std::string> names = {
            "ADD", "SUB", "MULT", "DIV", "JMP",
            "JMPN", "JMPP", "JMPZ", "COPY", "LOAD",
            "STORE", "INPUT", "OUTPUT", "STOP", "C_INPUT",
            "C_OUTPUT", "S_INPUT", "S_OUTPUT", "H_INPUT", "H_OUTPUT"
        };

        for(int i = 1; i <= 20; i++){
            std::string curr_op = names[i-1];
            REQUIRE(all_instructions[curr_op].mnemonic() == curr_op);
            REQUIRE(all_instructions[curr_op].opcode() == i);
        }
    }
}

TEST_CASE("Identifies opcode strings", "[instruction]"){
    std::vector<std::string> tests = {
        "ADD", "SUB", "MULT", "DIV", "JMP",
        "JMPN", "JMPP", "JMPZ", "COPY", "LOAD",
        "STORE", "INPUT", "OUTPUT", "STOP", "C_INPUT",
        "C_OUTPUT", "S_INPUT", "S_OUTPUT", "H_INPUT", "H_OUTPUT",
        "NAO", "FALSE", "ERROR"
    };

    for(int i = 0; i < 23; i++){
        if(i < 20){
            REQUIRE(is_opcode(tests[i]) == true);
        } else {
            REQUIRE(is_opcode(tests[i]) == false);
        }
    }
}
