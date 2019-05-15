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

// Class that is an instruction.
// Instructions have: OPCODE, NAME, OPERANDS, SIZE
class Instruction {
    int OPCODE;
    std::string NAME;
    int OPERANDS;
    int SIZE;

public:
    Instruction(){
    }

    Instruction(int opcode, std::string name, int operands, int size){
        OPCODE = opcode;
        NAME = name;
        OPERANDS = operands;
        SIZE = size;
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

    int size() {
        return SIZE;
    }
};

// Returns table with all the instructions
std::unordered_map<std::string, Instruction> table_of_instructions(){
    std::unordered_map<std::string, Instruction> table;
    // fill table with all the 14 operations
    table["ADD"] = Instruction(1, "ADD", 1, 2);
    table["SUB"] = Instruction(2, "SUB", 1, 2);
    table["MULT"] = Instruction(3, "MULT", 1, 2);
    table["DIV"] = Instruction(4, "DIV", 1, 2);
    table["JMP"] = Instruction(5, "JMP", 1, 2);
    table["JMPN"] = Instruction(6, "JMPN", 1, 2);
    table["JMPP"] = Instruction(7, "JMPP", 1, 2);
    table["JMPZ"] = Instruction(8, "JMPZ", 1, 2);
    table["COPY"] = Instruction(9, "COPY", 2, 3);
    table["LOAD"] = Instruction(10, "LOAD", 1, 2);
    table["STORE"] = Instruction(11, "STORE", 1, 2);
    table["INPUT"] = Instruction(12, "INPUT", 1, 2);
    table["OUTPUT"] = Instruction(13, "OUTPUT", 1, 2);
    table["STOP"] = Instruction(14, "STOP", 0, 1);
    // return table
    return table;
}

bool is_opcode(std::string test){
    std::vector<std::string> ops = {
        "ADD", "SUB", "MULT", "DIV", "JMP", "JMPN", "JMPP",
        "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP"};
    for(std::string op : ops){
        if(test == op) return true;
    }
    return false;
}

#endif