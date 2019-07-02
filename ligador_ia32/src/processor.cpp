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

// Processor class.
// Processes a stream of tokens, transforming them into IA-32 binary code
class Processor {
    // Private attributes
    // General information
    std::string __input_name;
    std::ifstream __file_pointer;       // Input file pointer
    std::vector<std::string> __lines;
    bool __done;

    std::vector<Token> _filter_line(std::string&line, int lst_line = 0);    
    void convert_token_to_bytes(char* binary_code, std::vector<Token> &tokens);
    
public:
    // Constructors 
    Processor();
    Processor(std::string input);
    // Process
    std::pair < char* , char* > run();


};


// Processor sem arquivo de referência.
// Nada é inicializado como válido.
Processor::Processor(){
    __done = false;
}

// Processor com arquivo de referência.
// Arquivo de referência já é aberto.
Processor::Processor(std::string input_name){
    __done = false;
    __input_name = input_name;
    // Verifica se extensão é .txt
    if(__input_name.substr(__input_name.size() - 2, 2) != ".s"){
        throw "Arquivo " + __input_name + " não possui extensão correta (.s)";
    } 
    __file_pointer.open(input_name);
    // Verifica que conseguiu abrir o arquivo
    if(!__file_pointer.is_open()){
        throw "File " + __input_name + " not Found";
    }
}

std::pair < char* , char* > Processor::run(){
    
    std::string line;
    std::pair < char* , char* > binary_code;
    int curr_line = 0;

    while(getline(__file_pointer, line)){
        if(line == "section .data") break;

        std::vector<Token> tokens_to_add = _filter_line(line, curr_line);
        convert_token_to_bytes(binary_code.first, tokens_to_add);
    }

    // TODO: Add our functions to binary code
    
    while(getline(__file_pointer, line)){

        std::vector<Token> tokens_to_add = _filter_line(line, curr_line);
        convert_token_to_bytes(binary_code.second, tokens_to_add);
    }

    return binary_code;

}

std::vector<Token> Processor::_filter_line(std::string &line, int lst_line){

    // TODO implement this function

    return std::vector<Token>();
}

void Processor::convert_token_to_bytes(char* binary_code, std::vector<Token> &tokens){

    // TODO implement this function

}


#endif
