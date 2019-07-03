// The pre processor classes.
#ifndef PROCESSOR_H
#define PROCESSOR_H
// Dependencies
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>

// Local dependencies
#include "linker.cpp"   // To use some defines

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
    std::map<std::string, long> __symbols;
    bool __done;

    std::vector<Token> _filter_line(std::string&line);    
    int convert_token_to_bytes(std::vector<char> binary_code, std::vector<Token> &tokens, bool size_only = false);
    void __count_symbols_addrs();
    
public:
    // Constructors 
    Processor();
    Processor(std::string input);
    // Process
    std::pair<std::vector<char>, std::vector<char>> run();


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

std::pair<std::vector<char>, std::vector<char>> Processor::run(){
    
    // Primeiramente calcula o valor dos símbolos
    __count_symbols_addrs();
    // Faz a tradução do código
    std::string line;
    std::pair<std::vector<char>, std::vector<char>> binary_code;
    int curr_line, max_line = __lines.size();
    bool text_sec = true;
    // Aproveita as linhas salvas em memória para acelerar o processamento
    for(curr_line = 0; curr_line < max_line; curr_line++){
        line = __lines[curr_line];
        if(line == "section .data"){
            text_sec = false;
            continue;
        } else if(line == "section .text"){
            text_sec = true;
            continue;
        }

        std::vector<Token> tokens_to_add = _filter_line(line);
        std::vector<char>& curr_bin = text_sec ? binary_code.first : binary_code.second;
        convert_token_to_bytes(curr_bin, tokens_to_add);
        curr_line++;
    }
    

       // Testing linker with hello world example
    std::vector<char> data = {
        '\x48', '\x65', '\x6C', '\x6C', '\x6F',   // msg: db   'Hello, World!', 10
        '\x2C', '\x20', '\x57', '\x6F', '\x72',
        '\x6C', '\x64', '\x21', '\x0A'
    };
    std::vector<char> text = { 
        '\xB8', '\x04', '\x00', '\x00', '\x00',   // mov eax, 4                   
        '\xBB', '\x01', '\x00', '\x00', '\x00',   // mov ebx, 1                   
        '\xB9', '\x00', '\xa0', '\x04', '\x08',   // mov ecx, msg                 
        '\xBA', '\x0E', '\x00', '\x00', '\x00',   // mov edx, 14                  
        '\xCD', '\x80',                           // int 0x80                     
        '\xB8', '\x01', '\x00', '\x00', '\x00',   // mov eax, 1      
        '\xBB', '\x00', '\x00', '\x00', '\x00',   // mov ebx, 0              
        '\xCD', '\x80'                            // int 0x80                     
    };
    std::pair<std::vector<char>, std::vector<char>> binary = std::make_pair(text, data);
    return binary;
}

std::vector<Token> Processor::_filter_line(std::string &line){
    // TODO implement this function

    return std::vector<Token>();
}

// Converte Tokens para codigo de maquina e os adiciona à binary_code se size_only == false.
// Retorna o tamanho, em bytes, que Tokens ocupam em código máquina se size_only == true.
// Default é size_only = false.
// Retorno é sempre o tamanho em bytes de Tokens em código máquina
int Processor::convert_token_to_bytes(std::vector<char> binary_code, std::vector<Token> &tokens, bool size_only){
    // TODO implement this function
    return 0;
}

// Faz uma passagem pelo arquivo descobrindo o valor de cada símbolo
// E cria uma tabela de símbolos
void Processor::__count_symbols_addrs() {
    long text_curr_addrs = 0, data_curr_addrs = 0;
    bool sec_text = true;
    std::string line;
    while(getline(__file_pointer, line)){
        // Saves line in memory
        __lines.push_back(line);
        // Breaks line
        std::vector<Token> tokens_to_add = _filter_line(line);
        // Labels estão sempre no início das linhas
        if(tokens_to_add[0].second == LABEL){
            __symbols[tokens_to_add[0].first] = sec_text ? 
            (text_curr_addrs + MY_TEXT_INIT_ADDRS) : 
            (data_curr_addrs + MY_DATA_INIT_ADDRS);
        }
        // Counts address of line
        if(sec_text){
            text_curr_addrs += convert_token_to_bytes(std::vector<char>(), tokens_to_add, true);
        } else {
            data_curr_addrs += convert_token_to_bytes(std::vector<char>(), tokens_to_add, true);
        }

    }

}
#endif
