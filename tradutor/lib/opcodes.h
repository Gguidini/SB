// This header file defines the INstruction class and 
// table_of_instructions() function.
//
//  Instruction class defines an instruction, with
//  OPCODE, NAME, OPERANDS, SIZE
//
//  table_of_instructions() returns an unordered_map
//  with all the instructions indexed by NAME.

#ifndef OPCODES_H
#define OPCODES_H

// Dependencies
#include <unordered_map>
#include <vector>

// Class that is an instruction.
// Instructions have: OPCODE, NAME, OPERANDS, SIZE
class Instruction {
    int OPCODE;
    std::string NAME;
    int OPERANDS;

public:
    Instruction(){
    }

    Instruction(int opcode, std::string name, int operands){
        OPCODE = opcode;
        NAME = name;
        OPERANDS = operands;
    }

    int opcode(){
        return OPCODE;
    }

    std::string mnemonic(){
        return NAME;
    }

    int operands() {
        return OPERANDS;
    }
};

// Returns table with all the instructions
std::unordered_map<std::string, Instruction> table_of_instructions(){
    std::unordered_map<std::string, Instruction> table;
    // fill table with all the 14 operations
    table["ADD"] = Instruction(1, "ADD", 1);
    table["SUB"] = Instruction(2, "SUB", 1);
    table["MULT"] = Instruction(3, "MULT", 1);
    table["DIV"] = Instruction(4, "DIV", 1);
    table["JMP"] = Instruction(5, "JMP", 1);
    table["JMPN"] = Instruction(6, "JMPN", 1);
    table["JMPP"] = Instruction(7, "JMPP", 1);
    table["JMPZ"] = Instruction(8, "JMPZ", 1);
    table["COPY"] = Instruction(9, "COPY", 2);
    table["LOAD"] = Instruction(10, "LOAD", 1);
    table["STORE"] = Instruction(11, "STORE", 1);
    table["INPUT"] = Instruction(12, "INPUT", 1);
    table["OUTPUT"] = Instruction(13, "OUTPUT", 1);
    table["STOP"] = Instruction(14, "STOP", 0);
    table["C_INPUT"] = Instruction(15, "C_INPUT", 1);
    table["C_OUTPUT"] = Instruction(16, "C_OUTPUT", 1);
    table["S_INPUT"] = Instruction(17, "S_INPUT", 2);
    table["S_OUTPUT"] = Instruction(18, "S_OUTPUT", 2);
    table["H_INPUT"] = Instruction(19, "H_INPUT", 1);
    table["H_OUTPUT"] = Instruction(20, "H_OUTPUT", 1);
    // return table
    return table;
}

bool is_opcode(std::string test){
    std::vector<std::string> ops = {
        "ADD", "SUB", "MULT", "DIV", "JMP",
        "JMPN", "JMPP", "JMPZ", "COPY", "LOAD",
        "STORE", "INPUT", "OUTPUT", "STOP", "C_INPUT",
        "C_OUTPUT", "S_INPUT", "S_OUTPUT", "H_INPUT", "H_OUTPUT"
    };
    for(std::string op : ops){
        if(test == op) return true;
    }
    return false;
}

#endif