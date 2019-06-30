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
    // Method to copy operands from one assembly to the other
    std::string __copy_all_operands(int idx, int& disp);
    
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
                    // Variaveis necessárias para as funções de input/output
                    output_lines.push_back("__string dd 0,0,0");
                    output_lines.push_back("crlf db 0xd, 0xa");
                    output_lines.push_back("__hex_table db \"0123456789ABCDEF\"");
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
                int j;
                if(curr.first == "ADD" || curr.first == "SUB"){
                    line_to_add = curr.first == "ADD" ? "add eax, [" : "sub eax, [";
                    line_to_add += __copy_all_operands(i, j);
                    line_to_add += "]";
                } else if(curr.first == "MULT"){
                    line_to_add = "mul dword [";
                    line_to_add += __copy_all_operands(i, j);                    
                    line_to_add += "]";
                } else if(curr.first == "DIV"){
                    line_to_add += "sub edx, edx\n";
                    line_to_add += "div dword [";
                    line_to_add += __copy_all_operands(i, j);
                    line_to_add += "]";
                } else if(curr.first == "JMP"){
                    line_to_add += "jmp ";
                    line_to_add += __copy_all_operands(i, j);
                } else if(curr.first == "JMPN" || curr.first == "JMPP" || curr.first == "JMPZ"){
                    line_to_add += "cmp eax, 0\n";
                    if(curr.first == "JMPN"){
                        line_to_add += "jl ";
                    } else if(curr.first == "JMPP"){
                        line_to_add += "jg ";
                    } else {
                        line_to_add += "je ";
                    }
                    line_to_add += __copy_all_operands(i, j);
                } else if(curr.first == "COPY"){
                    // Copia valor do primeiro argumento para ebx
                    line_to_add += "mov ebx, dword [";
                    line_to_add += __token_stream[i+1].first;
                    j = 2; // Inicio do prox argumento
                    if(__token_stream[i+2].first == "+"){
                        line_to_add += " + " + __token_stream[i+3].first;
                        j = 4;
                    }
                    line_to_add += "]\n";
                    // Copia o valor de ebx para o segundo argumento
                    line_to_add += "mov dword [";
                    while(__token_stream[i+j].second != ENDL){
                        line_to_add += __token_stream[i+j].first;
                        j++;
                    }
                    line_to_add += "], ebx";
                } else if(curr.first == "LOAD"){
                    line_to_add += "mov eax, [";
                    line_to_add += __copy_all_operands(i, j);
                    line_to_add += "]";
                } else if(curr.first == "STORE"){
                    line_to_add += "mov dowrd [";
                    line_to_add += __copy_all_operands(i, j);
                    line_to_add += "], eax";
                } else if(curr.first == "INPUT"){
                    line_to_add += "push ";
                    line_to_add += __copy_all_operands(i,j);
                    line_to_add += "\n";
                    line_to_add += "call readInt\n";
                    line_to_add += "add esp, 4";
                } else if(curr.first == "OUTPUT"){
                    line_to_add += "push ";
                    line_to_add += __copy_all_operands(i,j);
                    line_to_add += "\n";
                    line_to_add += "call putInt\n";
                    line_to_add += "add esp, 4";
                } else if(curr.first == "C_INPUT" || curr.first == "C_OUTPUT"){
                    line_to_add += "push ";
                    line_to_add += __copy_all_operands(i,j);
                    line_to_add += "\n";
                    if(curr.first == "H_INPUT"){
                        line_to_add += "call readChar\n";
                    } else {
                        line_to_add += "call putChar\n";
                    }
                    line_to_add += "add esp, 4";
                } else if(curr.first == "H_INPUT" || curr.first == "H_OUTPUT"){
                    line_to_add += "push ";
                    line_to_add += __copy_all_operands(i,j);
                    line_to_add += "\n";
                    if(curr.first == "H_INPUT"){
                        line_to_add += "call readHex\n";
                    } else {
                        line_to_add += "call putHex\n";
                    }
                    line_to_add += "add esp, 4";
                } else if(curr.first == "S_INPUT" || curr.first == "S_OUTPUT"){
                    // Push no primeiro argumento
                    line_to_add += "push ";
                    line_to_add += __token_stream[i+1].first;
                    j = 2; // Inicio do prox argumento
                    if(__token_stream[i+2].first == "+"){
                        line_to_add += " + " + __token_stream[i+3].first;
                        j = 4;
                    }
                    line_to_add += "\n";
                    // Push do segundo argumento
                    line_to_add += "push ";
                    while(__token_stream[i+j].second != ENDL){
                        line_to_add += __token_stream[i+j].first;
                        j++;
                    }
                    line_to_add += "\n";
                    // Chama a função
                    if(curr.first == "S_INPUT"){
                        line_to_add += "call readStr\n";
                    } else {
                        line_to_add += "call putStr\n";
                    }
                    line_to_add += "add esp, 8";
                } else if(curr.first == "STOP"){
                    line_to_add += "mov eax, 1\n";
                    line_to_add += "mov ebx, 0\n";
                    line_to_add += "int 0x80";
                }

                output_lines.push_back(line_to_add);
                // Pula até ENDL
                    i += j;
                break;
        }
    }

    // TODO: adicionar funções de input/output
}

std::string Processor::__copy_all_operands(int idx, int& disp){
    disp = 1;
    std::string operands = "";
    while(__token_stream[idx+disp].second != ENDL){
        // Argumento, possivelmente com + IMEDIATO
        operands += __token_stream[idx+disp].first;
        disp++;
    }
    return operands;
}

#endif