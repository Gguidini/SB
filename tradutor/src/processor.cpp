// Converts imaginary assembly to IA-32 assembly
// Works with the output of Pre_processor

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
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
    int size = __token_stream.size();
    std::vector<std::string> output_lines;
    // Usada somente para DIRs
    std::string line_with_label;
    int curr_section = -1;
    for(int i = 0; i < size; i++){
        Token curr = __token_stream[i];
        // Triagem dos tokens diferentes
        switch (curr.second){
            // Section TEXT or DATA
            case SEC:
                if(__token_stream[i+1].first == "TEXT"){
                    curr_section = SEC_TEXT;
                    output_lines.push_back("section .text");
                    output_lines.push_back("global _start");
                    output_lines.push_back("_start:");
                } else {
                    curr_section = SEC_DATA;
                    output_lines.push_back("section .data");
                }
                // Pula o token DATA ou TEXT
                i++;
                break;
            // Diretiva SPACE ou CONST
            case DIR:
                // Pega a Label que foi colocada anteriormente
                line_with_label = output_lines.back();
                output_lines.pop_back();
                // Adiciona a diretiva
                if(curr.first == "SPACE"){
                    line_with_label += " dd 0";
                    if(__token_stream[i+1].second != ENDL){
                        // SPACE <size> <\n>
                        std::string err = "";
                        int value = Utils::digit_value(__token_stream[i+1].first, err);
                        for(int k = 1; k < value; k++){
                            line_with_label += ", 0";
                        }
                        // Pula token <size>
                        i++;
                    }
                } else {
                    // CONST <value>
                    line_with_label += " dd " + __token_stream[i+1].first;
                    // Pula token <value>
                    i++;
                }
                output_lines.push_back(line_with_label);
                break;
            case LABEL:
                if(curr_section = SEC_DATA){
                    output_lines.push_back(curr.first);
                } else {
                    output_lines.push_back(curr.first + ":");
                }
                break;
            case OP:
            // "ADD", "SUB", "MULT", "DIV", "JMP",
            // "JMPN", "JMPP", "JMPZ", "COPY", "LOAD",
            // "STORE", "INPUT", "OUTPUT", "STOP", "C_INPUT",
            // "C_OUTPUT", "S_INPUT", "S_OUTPUT", "H_INPUT", "H_OUTPUT"
                // TODO: adicionar a tradução de instruções
                std::string line_to_add = "";
                if(curr.first == "ADD" || curr.first == "SUB"){
                    line_to_add = curr.first == "ADD" ? "add eax, " : "sub eax, ";
                    int j = 0;
                    while(__token_stream[i+j].second != ENDL){
                        // Argumento, possivelmente com + IMEDIATO
                        line_to_add += __token_stream[i+j].first;
                        j++;
                    }
                    // Pula até ENDL
                    i += j;
                } else if(curr.first == "MULT"){
                
                }
                break;
        }
    }
}
#endif