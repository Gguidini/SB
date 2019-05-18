// The pre processor classes.
#ifndef PROCESSOR_H
#define PROCESSOR_H
// Dependencies
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
// Local dependencies
#include "../lib/opcodes.h"
#include "../lib/utils.h"
#include "errors.cpp"
#include "symbols.cpp"

// defines token symbols
#define LABEL 0
#define MACRO 1
#define EQU 2
#define IF 3
#define ENDMACRO 5
#define OP 4
#define DIR 6     // SPACE or CONST
#define ENDL 11

typedef std::pair<std::string, int> Token;

// The pre processor class.
// Receives tokens from first pass and symbles table,
// Outputs a new file, with opcodes handled.
class Processor {
    // Private attributes
    // General information
    std::vector<Token> __tokens;
    std::unordered_map<std::string, Symbol> __symbol_table;
    std::vector<Error> __errs;
    std::string __output_name;
    std::vector<int> __object_code;
    bool __done;

public:
    // Constructors
    Processor();
    Processor(std::vector<Token> tokens, std::unordered_map<std::string, Symbol> symbol_table, std::string output_name);
    // Getters
    std::vector<Token> get_tokens() {return __tokens;}
    std::unordered_map<std::string, Symbol> get_symbol_table(){return __symbol_table;}
    std::vector<Error> get_errors(){return __errs;}
    std::string get_output_name() {return __output_name;}
    std::vector<int> get_object_code(){return __object_code;}
    bool get_done(){return __done;}
    // Run
    std::vector<int> run();
    // Generate output
    std::string generate_output();
};

// Processor sem arquivo de referência.
// Nada é inicializado como válido.
Processor::Processor(){
    __output_name = "";
    __done = false;
    __errs = std::vector<Error>();
    __object_code = std::vector<int>();
}

// Processor com arquivo de referência.
// Arquivo de referência já é aberto.
Processor::Processor(std::vector<Token> tokens, std::unordered_map<std::string, Symbol> symbol_table, std::string output_name){
    __tokens = tokens;
    __symbol_table = symbol_table;
    __errs = std::vector<Error>();
    __output_name = output_name;
    __done = false;
}

std::string Processor::generate_output(){
    if(!__done){
        throw("Tentando gerar output sem pre-processar o arquivo\n");
    }
    
    std::ofstream fd(__output_name);
    if(!fd.is_open()){
        throw("Algum erro aconteceu tentando gerar o arquivo de saída\n");
    }
    for(int value : __object_code){
        std::cout << "INSERINDO O " << std::to_string(value) << " NO ARQUIVO " << __output_name << std::endl;
        fd << std::to_string(value) + " ";
    }
    fd.close();
    return __output_name;
}
/*
    int get_value() {return __addrs;}
    bool can_jump() {return __jumpable;}
    bool is_const() {return __constant;}
    bool is_const_zero() {return __const_zero;}
    bool is_vector() {return __vector;}
    int get_vector_size() {return __vector_size;}
*/

std::vector<int> Processor::run(){
    int curr_addres = 1;
    std::cout << "tabela de symbles = " << std::endl;
    for ( auto it = __symbol_table.begin(); it != __symbol_table.end(); ++it )
        std::cout << it->first << ":" << it->second.can_jump() << " " << it->second.get_value() << std::endl;

    std::unordered_map<std::string, Instruction> instructions = table_of_instructions();

    std::cout << "tabela de opcodes = " << std::endl;
    for ( auto it = instructions.begin(); it != instructions.end(); ++it )
        std::cout << it->first << ":" << it->second.mnemonic() <<  std::endl;

    for(int i = 0; i < __tokens.size(); i++){
        Token &pair = __tokens[i];
        // Eh instrução
        if(instructions.count(pair.first)){
            Instruction current_instruction = instructions[pair.first];
            std::cout << "add o opcode " << current_instruction.opcode() << " que tem nome = " << current_instruction.mnemonic() << std::endl;
            __object_code.push_back(current_instruction.opcode());
            for(int j = 0; j < current_instruction.operands(); j++){
                i++;
                pair = __tokens[i];
                std::cout << "add o valor " << __symbol_table[pair.first].get_value() << " que tem nome = " << pair.first << std::endl;
                __object_code.push_back(__symbol_table[pair.first].get_value());
            }
        }
    }

    
    __done = true;
    return __object_code;
}


#endif
