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

    void get_endline(int &i);

    std::string get_full_line(int &i);
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
        throw("Tentando gerar output sem processar o arquivo\n");
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

void Processor::get_endline(int &i){
    Token pair = __tokens[i];
    while(pair.first != "\n"){
        i++;
        pair = __tokens[i];
    }
    i--;
}

std::string Processor::get_full_line(int &i){
    std::string ret = "";
    
    Token pair = __tokens[i];
    while(pair.first != "\n"){
        ret += pair.first;
        i++;
        pair = __tokens[i];
    }
    i--;
    return ret;
}


std::vector<int> Processor::run(){
    std::unordered_map<std::string, Instruction> instructions = table_of_instructions();
    int curr_line = 1;
    for(int i = 0; i < (int)__tokens.size(); i++){
        Token &pair = __tokens[i];

        // End line, just need to increment line count variable
        if(pair.first == "\n"){
            curr_line++;

          // If token are in Instruction table  
        } else if(instructions.count(pair.first)){

            // Object of Instruction class
            Instruction current_instruction = instructions[pair.first];

            // Push that opcode
            __object_code.push_back(current_instruction.opcode());

            // Check if that instruction have 1 or 2 parameters
            if(current_instruction.operands() == 1){

                // Get parameter
                i++;
                pair = __tokens[i];

                if(pair.first == "\n"){
                    __errs.push_back(Error(SYN_ERR, curr_line, "Instrução " + current_instruction.mnemonic() + " está faltando parâmetro", __input_name));
                    continue;
                }
               
                // Offset to sum with array (if needed)
                int offset = 0;

                // If that parameter is in symbol tabel
                if(__symbol_table.count(pair.first)){
                    if(__tokens[i+1].first != "\n" and __tokens[i+1].first != "+"){
                        __errs.push_back(Error(SYN_ERR, curr_line, "Instrução " + current_instruction.mnemonic() + " com mais de 1 parâmetro", __input_name));
                    }
                    // Get curr symble object
                    Symbol curr_symbol = __symbol_table[pair.first];
                    std::string curr_symbol_name = pair.first;

                    // Checks if is an array
                    if(curr_symbol.is_vector()){

                        // If if a JMP, cannot use ARRAY, only label
                        if((current_instruction.mnemonic() == "JMP" or current_instruction.mnemonic() == "JMPN" or current_instruction.mnemonic() == "JMPP" or current_instruction.mnemonic() == "JMPZ") and !curr_symbol.can_jump()){
                            __errs.push_back(Error(SEM_ERR, curr_line, "Não se pode pular para uma label fora da SECTION TEXT", __input_name));

                        // Checks if have offset
                        } else if(__tokens[i+1].first == "+"){
                            
                            // Get offset
                            i += 2;
                            pair = __tokens[i];
                            std::string err;

                            offset = Utils::digit_value(pair.first, err);

                            // If have error, check if this not number is a instruction, label or invalid number
                            if(err != ""){
                                if(instructions.count(pair.first)){
                                    __errs.push_back(Error(SEM_ERR, curr_line, "OFFSET não pode ser uma instrução" , __input_name));    
                                } else if (__symbol_table.count(pair.first)){
                                    __errs.push_back(Error(SEM_ERR, curr_line, "OFFSET não pode ser uma LABEL" , __input_name));    
                                } else{
                                    __errs.push_back(Error(LEX_ERR, curr_line, err , __input_name));
                                }
                            }

                            // Seg fault test
                            if(offset > curr_symbol.get_vector_size()){
                                __errs.push_back(Error(SEM_ERR, curr_line, "Seg fault, OFFSET " + pair.first + " superior ao tamanho do array", __input_name));
                                offset = 0;
                            }
                        }
                    
                    // Not array
                    } else{

                        // Have + sign of array
                        if(__tokens[i+1].first == "+"){
                            i += 2;
                            if(__tokens[i+1].first != "\n"){
                                __errs.push_back(Error(SYN_ERR, curr_line, "Instrução " + current_instruction.mnemonic() + " com mais de 1 parâmetro", __input_name));
                            }
                                                       
                            get_endline(i);
                            
                            __errs.push_back(Error(SEM_ERR, curr_line, "Parâmetro " + pair.first + " NÃO é um array", __input_name));
                        } else{
                            i++;
                            pair = __tokens[i];
                            if(pair.first != "\n"){
                                __errs.push_back(Error(SYN_ERR, curr_line, "Instrução " + current_instruction.mnemonic() + " com mais de 1 parâmetro", __input_name));
                            }
                            if(current_instruction.mnemonic() == "DIV" and curr_symbol.is_const_zero()){
                                __errs.push_back(Error(SEM_ERR, curr_line, "Divisão por zero", __input_name));
                            } 
                            if(current_instruction.mnemonic() == "STORE" and curr_symbol.is_const()){
                                __errs.push_back(Error(SEM_ERR, curr_line, "Não se pode alterar o valor de um CONST", __input_name));
                            }
                            if((current_instruction.mnemonic() == "JMP" or current_instruction.mnemonic() == "JMPN" or current_instruction.mnemonic() == "JMPP" or current_instruction.mnemonic() == "JMPZ") and !curr_symbol.can_jump()){
                                __errs.push_back(Error(SEM_ERR, curr_line, "Não se pode pular para uma label fora da SECTION TEXT", __input_name));
                            }
                            if((current_instruction.mnemonic() != "JMP" and current_instruction.mnemonic() != "JMPN" and current_instruction.mnemonic() != "JMPP" and current_instruction.mnemonic() != "JMPZ") and curr_symbol.can_jump()){
                                __errs.push_back(Error(SEM_ERR, curr_line, "Instrução " + current_instruction.mnemonic() + " está utilizando uma label dentro da SECTION TEXT", __input_name));
                            }
                        }
                    }

                    // Push label value + offset (array case)
                    __object_code.push_back(__symbol_table[curr_symbol_name].get_value() + offset);

                    // Check if have more than 1 parameter 
                }
                // Invalid parameter, check if is an insctrction or not defined label
                else{
                    if(instructions.count(pair.first)){
                        __errs.push_back(Error(SEM_ERR, curr_line, "Parâmetro não pode ser uma instrução (" + pair.first + ")" , __input_name));    
                    } else{
                        __errs.push_back(Error(SEM_ERR, curr_line, "Parâmetro " + pair.first + " não definido", __input_name));
                    }
                    // Push 0 to continue the tests
                    __object_code.push_back(0);
                }
               
                // Ignore the rest of the line (case array not valid, then ignore all + OFFSET and that stuff)
                get_endline(i);
                
            
            // Instruction with 2 parameters
            } else if(current_instruction.operands() == 2){

                // Get first parameter
                i++;
                pair = __tokens[i];
                std::string old = pair.first;

                if(pair.first == "\n"){
                    __errs.push_back(Error(SYN_ERR, curr_line, "Instrução " + current_instruction.mnemonic() + " está sem parâmetros", __input_name));
                    continue;
                }
                

                // Array offset
                int offset = 0;
                // Checks if that parameter is in symbol table
                if(__symbol_table.count(pair.first)){

                    // Get current symbol object
                    Symbol curr_symbol = __symbol_table[pair.first];
                    std::string curr_symbol_name = pair.first;

                    // Checks if is an array
                    if(curr_symbol.is_vector()){
                        if(__tokens[i+1].first == "+"){
                            i += 2;
                            pair = __tokens[i];
                            std::string err;
                            offset = Utils::digit_value(pair.first, err);
                            if(err != ""){
                                if(instructions.count(pair.first)){
                                    __errs.push_back(Error(SEM_ERR, curr_line, "OFFSET não pode ser uma instrução" , __input_name));    
                                } else if (__symbol_table.count(pair.first)){
                                    __errs.push_back(Error(SEM_ERR, curr_line, "OFFSET não pode ser uma LABEL" , __input_name));    
                                } else{
                                    __errs.push_back(Error(LEX_ERR, curr_line, err , __input_name));
                                }
                            }
                            if(offset > curr_symbol.get_vector_size()){
                                __errs.push_back(Error(SEM_ERR, curr_line, "Seg fault, OFFSET " + pair.first + " superior ao tamanho do array", __input_name));
                                offset = 0;
                            }
                        }
                    } else{
                        if(__tokens[i+1].first == "+"){
                            __errs.push_back(Error(SEM_ERR, curr_line, "Parâmetro " + pair.first + " NÃO é um array", __input_name));
                            i += 2;
                            pair = __tokens[i];
                        }
                    }

                    __object_code.push_back(__symbol_table[curr_symbol_name].get_value() + offset);


                } else{
                    __errs.push_back(Error(SEM_ERR, curr_line, "Parâmetro " + pair.first + " não definido", __input_name));
                    __object_code.push_back(0);
                    if(__tokens[i+1].first == "+"){
                        i += 2;
                        pair = __tokens[i];
                    }
                }
                i++;
                pair = __tokens[i];
                offset = 0;
                if(pair.first == "\n"){
                    __errs.push_back(Error(SYN_ERR, curr_line, "Instrução " + current_instruction.mnemonic() + " está faltando o segundo parâmetro", __input_name));
                    continue;
                }

                if(__symbol_table.count(pair.first)){

                    Symbol curr_symbol = __symbol_table[pair.first];
                    std::string curr_symbol_name = pair.first;

                    if(curr_symbol.is_vector()){
                        if(__tokens[i+1].first == "+"){
                            i += 2;
                            pair = __tokens[i];
                            std::string err;
                            offset = Utils::digit_value(pair.first, err);
                            if(err != ""){
                                if(instructions.count(pair.first)){
                                    __errs.push_back(Error(SEM_ERR, curr_line, "OFFSET não pode ser uma instrução" , __input_name));    
                                } else if (__symbol_table.count(pair.first)){
                                    __errs.push_back(Error(SEM_ERR, curr_line, "OFFSET não pode ser uma LABEL" , __input_name));    
                                } else{
                                    __errs.push_back(Error(LEX_ERR, curr_line, err , __input_name));
                                }
                            }
                            if(offset > curr_symbol.get_vector_size()){
                                __errs.push_back(Error(SEM_ERR, curr_line, "Seg fault, OFFSET " + pair.first + " superior ao tamanho do array", __input_name));
                                offset = 0;
                            }
                        }

                    } else{
                        if(__tokens[i+1].first == "+"){
                            
                            get_endline(i);
                            
                            __errs.push_back(Error(SEM_ERR, curr_line, "Parâmetro " + pair.first + " NÃO é um array", __input_name));
                        }
                    }
                    __object_code.push_back(__symbol_table[curr_symbol_name].get_value() + offset);

                    if(__tokens[i+1].first != "\n"){
                        __errs.push_back(Error(SYN_ERR, curr_line, "Instrução " + current_instruction.mnemonic() + " com mais de 1 parâmetro", __input_name));
                        get_endline(i);
                    }
                } else{
                    __errs.push_back(Error(SEM_ERR, curr_line, "Parâmetro " + pair.first + " não definido", __input_name));
                    __object_code.push_back(0);
                    while(pair.first != "\n"){
                        i++;
                        pair = __tokens[i];
                    }
                    i--;
                }
            }
        
        // If is a SPACE token
        } else if(pair.first == "SPACE"){

            // Check if have parameter (array size)
            if(__tokens[i+1].first != "\n"){
                // Get next token
                i++;
                pair = __tokens[i];

                // If have more token, then have more than necessary 
                if(__tokens[i+1].first != "\n"){
                    __errs.push_back(Error(SYN_ERR, curr_line, "Instrução SPACE com mais de 1 parâmetro", __input_name));

                    // Ignore all the parameters in that line
                    while(pair.first != "\n"){
                        i++;
                        pair = __tokens[i];
                    }
                    i--;

                // Have only one parameter
                } else{
                    
                    std::string err;
                    // Transform that string into value
                    int value = Utils::digit_value(pair.first, err);

                    // If have error, check if this not number is a instruction, label or invalid number
                    if(err != ""){
                        if(instructions.count(pair.first)){
                            __errs.push_back(Error(SEM_ERR, curr_line, "SPACE não pode ser uma instrução" , __input_name));    
                        } else if (__symbol_table.count(pair.first)){
                            __errs.push_back(Error(SEM_ERR, curr_line, "SPACE não pode ser uma LABEL" , __input_name));    
                        } else{
                            __errs.push_back(Error(LEX_ERR, curr_line, "SPACE " + pair.first + " inválido", __input_name));
                        }
                    // Valid number
                    } else {
                        // Space can't have <= 0 space
                        if(value <= 0){
                            __errs.push_back(Error(SEM_ERR, curr_line, "Não se pode alocar valores <= 0 no SPACE", __input_name));
                        // Store quantity of space for that array
                        } else {
                            for(int qnt = 0; qnt < value; qnt++){
                                __object_code.push_back(0);
                            }
                        }
                    }
                }
            // Not array variable, need only 1 space
            } else{
                __object_code.push_back(0);
            }
        // Const Directive
        } else if(pair.first == "CONST"){
            // Get next parameter
            i++;
            pair = __tokens[i];
            std::string err;
            int value = Utils::digit_value(pair.first, err);

            // If have error, check if this not number is a instruction, label or invalid number
            if(err != ""){
                if(instructions.count(pair.first)){
                    __errs.push_back(Error(SEM_ERR, curr_line, "CONST não pode ser uma instrução" , __input_name));    
                } else if (__symbol_table.count(pair.first)){
                    __errs.push_back(Error(SEM_ERR, curr_line, "CONST não pode ser uma LABEL" , __input_name));    
                } else{
                    __errs.push_back(Error(LEX_ERR, curr_line, "CONST " + pair.first + " inválido", __input_name));
                }
            }
            // Store that value
            else {
                __object_code.push_back(value);
            }
            // If not is the end of the line
            if(__tokens[i+1].first != "\n"){
                __errs.push_back(Error(SYN_ERR, curr_line, "Instrução CONST com mais de 1 parâmetro", __input_name));

                // Ignore all the parameters in that line
                 get_endline(i);
            }

        // IGNORE THAT LINE
        } else if(pair.first == "SECTION"){
            i++;
        // If is not instruction, space, const and label, then is an invalid instruction
        } else if(pair.second != LABEL){
            __errs.push_back(Error(SEM_ERR, curr_line, "Instrução " + pair.first + " inválida", __input_name));

            // Ignore that line
            while(pair.first != "\n"){
                i++;
                pair = __tokens[i];
            }
            i--;
        }
    }

    
    __done = true;
    return __object_code;
}


#endif
