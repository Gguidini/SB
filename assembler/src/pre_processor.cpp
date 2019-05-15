// The pre processor classes.
// TODO: Update file description

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
// Dependencies
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "../lib/opcodes.h"

// defines for Sections
//#define TEXT 1
//#define DATA 0

// defines token symbols
#define LABEL 0
#define MACRO 1
#define EQU 2
#define IF 3
#define ENDMACRO 5
#define OP 4
#define ENDL 11

// The pre processor class.
// Receives the original file to process,
// Outputs a new file, with preprocessor directives handled.
class Pre_processor {
    // Private attributes
    // General information
    std::vector<std::string> __buffer;  // Saves lines after processing
    std::string __input_name;           // Name of input file
    std::string __output_name;          // Name of output file
    std::ifstream __file_pointer;       // Input file pointer
    bool __done;                        // If processed the entire file
    
    // Macro values
    int __macro_id;
    std::unordered_map<std::string, std::pair<int,int> > __MNT;
    std::unordered_map<int, std::vector<std::pair<std::string, int>>> __MDT;

    // EQU values
    std::map<std::string, int> __equs;

public:
    // Constructors
    Pre_processor();
    Pre_processor(std::string);
    // Getters
    std::vector<std::string> get_buffer() { return __buffer;}
    std::string get_input_name() {return __input_name;}
    std::string get_output_name() {return __output_name;}
    std::ifstream& get_file_pointer(){return __file_pointer;}
    bool get_done(){return __done;}
    // Run
    std::vector<std::string> run();
    // Generate output
    std::string generate_output();
    // Internal use (public for testing purposes)
    std::vector<std::pair<std::string, int>> _filter_line(std::string&line);    
};

// Pre_processor sem arquivo de referência.
// Nada é inicializado como válido.
Pre_processor::Pre_processor(){
    __buffer = std::vector<std::string>();
    __input_name = "";
    __output_name = "";
    __done = false;
    __macro_id = 0;
}

// Pre_processor com arquivo de referência.
// Arquivo de referência já é aberto.
Pre_processor::Pre_processor(std::string input_name){
    __buffer = std::vector<std::string>();
    __output_name = input_name.substr(0, input_name.size() - 4) + ".pre";
    __done = false;
    __input_name = input_name;
    // Verifica se extensão é .txt
    if(__input_name.substr(__input_name.size() - 4, 4) != ".txt"){
        throw "Arquivo " + __input_name + " não possui extensão correta";
    } 
    __file_pointer.open(input_name);
    // Verifica que conseguiu abrir o arquivo
    if(!__file_pointer.is_open()){
        throw "File " + __input_name + " not Found";
    }
}

// generate_output gera o arquivo .pre.
// Esse arquivo é o arquivo original com todas as diretivas de pre processamentos realizadas.
// Só deve ser executada após a função run().
// As linhas do arquivo estarão no atributo __buffer
// Nome do arquivo de saída estará no atributo __output_name
// Retorna o nome do arquivo de saída gerado, com extensão .pre
std::string Pre_processor::generate_output(){
    if(!__done){
        throw("Tentando gerar output sem pre-processar o arquivo\n");
    }
    
    std::ofstream fd(__output_name);
    if(!fd.is_open()){
        throw("Algum erro aconteceu tentando gerar o arquivo de saída\n");
    }
    for(std::string str : __buffer){
        fd << str;
    }
    fd.close();
    return __output_name;
}

// _filter_line é uma função interna que faz a filtragem das linhas.
// Classificada como publica para podermos testá-la mais facilmente.
// Recebe uma linha de um arquivo e a separa em um vetor de tokens.
// Cada token é um string e uma anotação daquela string.
std::vector<std::pair<std::string, int>> Pre_processor::_filter_line(std::string &line){
    // Return value
    std::vector<std::pair<std::string, int>> next_tokens;
    // Construction of return value
    std::string curr_token_str = "";
    for(int i = 0; i < (int)line.size(); i++){
        // current char
        char c = line[i];
        // Erase comments
        if(c == ';'){
            break;
        }
        // Erase mutiples spaces and ending space
        // Separates tokens (space is used to separate tokens)
        else if(c == ' '){
            // Check if curr token is any of the preprocessing directives
            // And this space is pointing to the end of it
            std::string test = "";
            for (char c: curr_token_str){
                test += toupper(c); // curr_token_str UPPERCASE
            } 
                    
            if(test == "MACRO"){
                next_tokens.push_back(std::make_pair(curr_token_str, MACRO));
                curr_token_str = "";
            } else if(test == "EQU"){
                next_tokens.push_back(std::make_pair(curr_token_str, EQU));
                curr_token_str = "";
            } else if(test == "IF"){
                next_tokens.push_back(std::make_pair(curr_token_str, IF));
                curr_token_str = "";
            } else if(test == "ENDMACRO"){
                next_tokens.push_back(std::make_pair(curr_token_str, ENDMACRO));
                curr_token_str = "";
            } else if(is_opcode(test)){
                next_tokens.push_back(std::make_pair(curr_token_str, OP));
                curr_token_str = "";
            } else if(test.back() != ',' && test != ""){
                // COPY A, B. The ',' is to check if we are in a COPY situation.
                // Otherwise, this is an unidentified token
                next_tokens.push_back(std::make_pair(curr_token_str, -1));
                curr_token_str = "";
            }
            // Jump all spaces until end or next valid char
            while(i < (int)line.size() and line[i] == ' ') i++;
            // If has a valid char that is diff from comment, add space                
            if(i < (int)line.size()){   
                if(line[i] == ';') break;
            }
            // Get current char diff from space
            i--;
        }
        // Catch labels and push that token.
        // Flush curr line and continue
        else if(c == ':'){
            next_tokens.push_back(std::make_pair(curr_token_str, LABEL));
            curr_token_str = "";
            continue;
        }
        // Add normal char to current token
        else if( c >= '!' and c <= '~' ){
            curr_token_str += c;
        }
    }
    
    // Check if last token is any of the directives
    std::string test = "";
    for (char c: curr_token_str){
        test += toupper(c); // curr_token_str UPPERCASE
    } 
      
    if(test == "MACRO"){
        next_tokens.push_back(std::make_pair(curr_token_str, MACRO));
    } else if(test == "ENDMACRO"){
        next_tokens.push_back(std::make_pair(curr_token_str, ENDMACRO));
    } else if(test == "EQU"){
        next_tokens.push_back(std::make_pair(curr_token_str, EQU));
    } else if(test == "IF"){
        next_tokens.push_back(std::make_pair(curr_token_str, IF));
    } else if(is_opcode(test)){
        next_tokens.push_back(std::make_pair(curr_token_str, OP));
    } else if(test != ""){
        next_tokens.push_back(std::make_pair(curr_token_str, -1));
    }

    return next_tokens;
}

// run processes de original file.
// Saves processed lines in __buffer. Returns this value.
std::vector<std::string> Pre_processor::run(){
    
    std::vector<std::string> processed_file;
    std::string line;

    while(getline(__file_pointer,line)){
        bool has_equ = false;
        bool has_macro = false;
        bool has_if = false;
        /*
        Add flag "linha cheia"
        while(!linha cheia){
            Pega tokens
        }
          Linha cheia --> Tem uma diretiva || Tem uma operação 
        
        */
        std::vector<std::pair<std::string, int>> tokens = _filter_line(line);
        if(tokens.size() == 0) continue;    // Empty line
        tokens.push_back(std::make_pair("\n", 11));
        
        for(int i = 0; i < (int) tokens.size(); i++){
            std::pair<std::string, int>& pair = tokens[i];
            if(pair.second == EQU){
                has_equ = true;
            } else if(pair.second == MACRO){
                has_macro = true;
            } else if(pair.second == IF){
                has_if = true;
            } else if(pair.second == -1){
                // Prováveis LABELS em uso
                if(__equs.find(pair.first) != __equs.end()){
                    // Expand EQU
                    pair.first = std::to_string(__equs[pair.first]);
                }else if(__MNT.count(pair.first)){
                    int label_id = __MNT[pair.first].second;
                    tokens[i] = __MDT[label_id][0];
                    tokens.insert(tokens.begin() + i + 1, __MDT[label_id].begin() + 1, __MDT[label_id].end());
                }
            }
        }
        // EXPAND MACROS
        if(has_macro){
            int parameters = 0;
            if(tokens.size() == 3){
                parameters = 1;
                for(int i = 0; i < (int) tokens[2].first.size(); i++) if(tokens[2].first[i] == ',') parameters++;
            }
            __MNT[tokens[0].first] = std::make_pair(parameters, ++__macro_id);
            getline(__file_pointer,line);
            tokens = _filter_line(line);
            while(tokens[0].second != ENDMACRO){
                if(tokens[(int)tokens.size()-1].second != ENDL){
                    tokens.push_back(std::make_pair("\n", 11));
                }
                __MDT[__macro_id].insert(__MDT[__macro_id].end(), tokens.begin(), tokens.end());
                getline(__file_pointer,line);
                tokens = _filter_line(line);
            }
            __MDT[__macro_id].pop_back();
            continue;
        }

        // EXPAND IF - IF <value> <\n>
        if(has_if){
            // FIXME: Verificar se argumento do IF é um número
            if( tokens.size() == 3){
                if(tokens[1].first != "0"){
                    // Inclui próxima linha, mas não esta
                    continue;
                } else {
                    // Não inclui nem esta nem a próxima linha
                    getline(__file_pointer,line);
                    continue;
                }
            } else {
                // TODO: IF errors
            }
        }
        // EXPAND EQU - LABEL: EQU VALUE <\n>
        if(has_equ){
            // FIXME: Verificar se argumento do EQU é um número
            if( tokens.size() == 4 && 
                tokens[0].second == LABEL &&
                tokens[1].second == EQU &&
                tokens[2].second == -1){
                // EQU usada corretamente
                __equs[tokens[0].first] = stoi(tokens[2].first);
                // EQU def doesn't go for processing
                continue;
            } else {
                // TODO: Tratamento de erros para EQU
            }

        }
        // TODO: Talvez devemos considerar retornar direto o vetor de tokens
        // Transforma o vetor de tokens em string
        int s = tokens.size();
        std::string processed_line = tokens[0].first;
        if(tokens[0].second == LABEL){
            processed_line += ":";
        }
        for(int i = 1; i < s; i++){
            if(tokens[i].second == ENDL or tokens[i-1].second == ENDL){
                processed_line += tokens[i].first;
            }
            else{
                processed_line += " " + tokens[i].first;
            }

            if(tokens[i].second == LABEL){
                processed_line += ":";
            }
        }
        processed_file.push_back(processed_line);
    }

    __done = true;
    __buffer = processed_file;
    return processed_file;
}

#endif