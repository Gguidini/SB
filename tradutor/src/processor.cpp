// Converts imaginary assembly to IA-32 assembly
// Works with the output of Pre_processor

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <unordered_map>
#include <fstream>
#include "../lib/opcodes.h"
#include "../lib/utils.h"

typedef std::pair<std::string, int> Token;

// Processor class.
// Processes a stream of tokens, transforming them into IA-32 code
class Processor {
    std::vector<Token> __token_stream;          // Tokens to process
    std::vector<std::string> __output_stream;   // Processed tokens
    std::string __input_name;                   // Name of input file
    std::string __output_name;                  // Name of output file
    
public:
    // Constructors 
    Processor();
    Processor(std::string input, std::vector<Token> tokens);
    // Process
    std::vector<std::string> run();
    // Generate output
    std::string generate_output();
};

Processor::Processor(){
    __token_stream = std::vector<Token>();
    __output_stream = std::vector<std::string>();
}

Processor::Processor(std::string input, std::vector<Token> tokens) : Processor() {
    __input_name = input;
    __output_name = input.substr(0, input.size() - 4) + ".s";
}

// generate_output copia as linhas de código geradas pelo Processor
// para o arquivo de saída.
std::string Processor::generate_output(){
    std::ofstream out(__output_name);
    for(std::string str : __output_stream){
        out << str << std::endl;
    }
    out.close();
    return __output_name;
}

std::vector<std::string> Processor::run() {
    return std::vector<std::string>();
}
#endif