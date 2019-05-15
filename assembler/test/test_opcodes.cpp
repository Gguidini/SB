// This file has Unit Tests for the opcodes.h lib.
// Tests are created using the Catch framework (header file)

#ifndef CATCH_CONFIG_MAIN   // Only one test file can contain CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN   // So catch will provide the main function
#endif

#include "catch_framework.hpp"
// Lib to test
#include "../lib/opcodes.h"

TEST_CASE( "Class Instruction works as expected", "[instruction]" ) {

    Instruction inst(15, "test instruction", 5, 6);

    REQUIRE(inst.opcode() == 15);
    REQUIRE(inst.mnemonic() == "test instruction");
    REQUIRE(inst.operands() == 5);
    REQUIRE(inst.size() == 6);
}

TEST_CASE("The table of instructions is OK", "[instruction]") {

    std::unordered_map<std::string, Instruction> all_instructions;
    all_instructions =  table_of_instructions();

    REQUIRE(all_instructions.size() == 14);

    SECTION("OPCODES match NAMES"){
        std::vector<std::string> names = {
            "ADD", "SUB", "MULT", "DIV", "JMP", "JMPN", "JMPP", "JMPZ",
            "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP"
        };

        for(int i = 1; i <= 14; i++){
            std::string curr_op = names[i-1];
            REQUIRE(all_instructions[curr_op].mnemonic() == curr_op);
            REQUIRE(all_instructions[curr_op].opcode() == i);
        }
    }

    SECTION("OPERANDS and SIZE are OK"){
        for(auto el : all_instructions){
            std::string inst_name = el.first;
            Instruction inst = el.second;
            if(inst_name == "STOP"){
                REQUIRE(inst.operands() == 0);
                REQUIRE(inst.size() == 1);
            } else if(inst_name == "COPY"){
                REQUIRE(inst.operands() == 2);
                REQUIRE(inst.size() == 3);
            } else{
                REQUIRE(inst.operands() == 1);
                REQUIRE(inst.size() == 2);
            }
        }
    }
}

TEST_CASE("Identifies opcode strings", "[instruction]"){
    std::vector<std::string> tests = {
        "ADD", "SUB", "MULT", "DIV", "JMP", "JMPN", "JMPP",
        "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT",
        "STOP", "NAO", "FALSE"
    };

    for(int i = 0; i < 15; i++){
        if(i < 14){
            REQUIRE(is_opcode(tests[i]) == true);
        } else {
            REQUIRE(is_opcode(tests[i]) == false);
        }
    }
}
