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
    std::string __input_name;
    std::string __output_name;
    std::vector<int> __object_code;
    bool __done;

public:
    // Constructors
    Processor();
    Processor(std::vector<Token> tokens, std::unordered_map<std::string, Symbol> symbol_table, std::string input_name);
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
Processor::Processor(std::vector<Token> tokens, std::unordered_map<std::string, Symbol> symbol_table, std::string input_name){
    __tokens = tokens;
    __symbol_table = symbol_table;
    __errs = std::vector<Error>();
    __input_name = input_name;
    __output_name = input_name.substr(0, input_name.size() - 4) + ".obj";;
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
        fd << std::to_string(value) + " ";
    }
    fd.close();
    return __output_name;
}

std::vector<int> Processor::run(){
    std::cout << "tabela de symbles = " << std::endl;
    for ( auto it = __symbol_table.begin(); it != __symbol_table.end(); ++it )
        std::cout << it->first << ":" << it->second.can_jump() << " " << it->second.get_value() << std::endl;

    std::unordered_map<std::string, Instruction> instructions = table_of_instructions();

    std::cout << "tabela de opcodes = " << std::endl;
    for ( auto it = instructions.begin(); it != instructions.end(); ++it )
        std::cout << it->first << ":" << it->second.mnemonic() <<  std::endl;

    int curr_line = 1;
    for(int i = 0; i < (int)__tokens.size(); i++){
        Token &pair = __tokens[i];
        std::cout << "TOKEN = " << pair.first << std::endl;
        if(pair.first == "\n"){
            curr_line++;
        } else if(instructions.count(pair.first)){
            Instruction current_instruction = instructions[pair.first];
            std::cout << "add o opcode " << current_instruction.opcode() << " que tem nome = " << current_instruction.mnemonic() << std::endl;
            __object_code.push_back(current_instruction.opcode());

            if(current_instruction.operands() == 1){
                i++;
                pair = __tokens[i];
                std::cout << "TOKEN2 [1]= " << pair.first << std::endl;
                if(__symbol_table.count(pair.first)){

                    Symbol curr_symbol = __symbol_table[pair.first];

                    if(curr_symbol.is_vector()){
                        int offset;
                        __errs.push_back(Error(SEM_ERR, curr_line, "Parâmetro " + pair.first + " é um array e não possui OFFSET", __input_name));
                    } else if(!curr_symbol.is_vector() && __tokens[i+1].first == "+"){
                        __errs.push_back(Error(SEM_ERR, curr_line, "Parâmetro " + pair.first + " NÃO é um array e está com OFFSET", __input_name));
                    }

                    std::cout << "add o valor " << __symbol_table[pair.first].get_value() << " que tem nome = " << pair.first << std::endl;
                    __object_code.push_back(__symbol_table[pair.first].get_value());
                }
                else{
                    __errs.push_back(Error(SEM_ERR, curr_line, "Parâmetro " + pair.first + " inválido", __input_name));
                    __object_code.push_back(0);
                }
                while(pair.first != "\n"){
                    i++;
                    pair = __tokens[i];
                }
                i--;
            }

            for(int j = 0; j < current_instruction.operands(); j++){
            }
        } else if(pair.first == "SPACE"){
            __object_code.push_back(0);
        } else if(pair.first == "CONST"){
            i++;
            pair = __tokens[i];
            std::string err;
            int value = Utils::digit_value(pair.first, err);
            if(err == "")
                __object_code.push_back(value);
        } else if(pair.first == "SECTION"){
            i++;
        } else if(pair.second != LABEL){
            __errs.push_back(Error(SEM_ERR, curr_line, "Comando " + pair.first + " inválido", __input_name));
        }
    }

    
    __done = true;
    return __object_code;
}


#endif
