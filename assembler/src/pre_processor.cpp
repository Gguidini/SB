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
#include "../lib/utils.h"
#include "errors.cpp"

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

typedef std::pair<std::string, int> Token;

// create_tagged_token analisa uma string para ver se é um token reconhecido ou não.
// Retorna true se o token for válido (não-vazio), false se for inválido.
// Salva o token criado na variável tok
bool create_tagged_token(std::string str, Token& tok){

    // COPY A, B - Put all arguments in the same token
    if(str.back() == ',' || str == ""){
        return false;
    }

    std::string test = Utils::to_upper(str);
    if(test == "MACRO"){
        tok = std::make_pair(str, MACRO);
    } else if(test == "EQU"){
        tok = std::make_pair(str, EQU);
    } else if(test == "IF"){
        tok = std::make_pair(str, IF);
    } else if(test == "ENDMACRO"){
        tok = std::make_pair(str, ENDMACRO);
    } else if(is_opcode(test)){
        tok = std::make_pair(str, OP);
    } else {
        tok = std::make_pair(str, -1);
    }

    return true;
}

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
    std::vector<Error> __errs;
    
    // Macro values
    int __macro_id;
    std::unordered_map<std::string, std::pair<int,int> > __MNT;
    std::unordered_map<int, std::vector<Token>> __MDT;

    // EQU values
    std::map<std::string, int> __equs;
    void _expand_equs(std::vector<Token> curr_tokens);

    // Expand IFs
    void _expand_ifs(std::vector<Token> curr_tokens);

public:
    // Constructors
    Pre_processor();
    Pre_processor(std::string);
    // Getters
    std::vector<std::string> get_buffer() { return __buffer;}
    std::string get_input_name() {return __input_name;}
    std::string get_output_name() {return __output_name;}
    std::ifstream& get_file_pointer(){return __file_pointer;}
    std::vector<Error> get_errors(){return __errs;}
    bool get_done(){return __done;}
    // Run
    std::vector<std::string> run();
    // Generate output
    std::string generate_output();
    // Internal use (public for testing purposes)
    std::vector<Token> _filter_line(std::string&line);    
};

// Pre_processor sem arquivo de referência.
// Nada é inicializado como válido.
Pre_processor::Pre_processor(){
    __buffer = std::vector<std::string>();
    __input_name = "";
    __output_name = "";
    __done = false;
    __macro_id = 0;
    __errs = std::vector<Error>();
}

// Pre_processor com arquivo de referência.
// Arquivo de referência já é aberto.
Pre_processor::Pre_processor(std::string input_name){
    __buffer = std::vector<std::string>();
    __output_name = input_name.substr(0, input_name.size() - 4) + ".pre";
    __done = false;
    __errs = std::vector<Error>();
    __macro_id = 0;
    __input_name = input_name;
    // Verifica se extensão é .txt
    if(__input_name.substr(__input_name.size() - 4, 4) != ".txt"){
        throw "Arquivo " + __input_name + " não possui extensão correta (.txt)";
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
std::vector<Token> Pre_processor::_filter_line(std::string &line){
    // Return value
    std::vector<Token> next_tokens;
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
        else if(c == ' ' || c == '\t'){
            // Check if curr token is any of the preprocessing directives
            // And this space is pointing to the end of it
            Token tok;
            bool curr_tok_valid = create_tagged_token(curr_token_str, tok);
            if(curr_tok_valid){
                next_tokens.push_back(tok);
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
    Token tok;
    bool curr_tok_valid = create_tagged_token(curr_token_str, tok);
    if(curr_tok_valid){
        next_tokens.push_back(tok);
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
        std::vector<Token> tokens = _filter_line(line);
        if(tokens.size() == 0) continue;    // Empty line
        tokens.push_back(std::make_pair("\n", 11));
        
        for(int i = 0; i < (int) tokens.size(); i++){
            Token &pair = tokens[i];
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
                    // Don't have parameters
                    if(__MNT[pair.first].first == 0){
                        int label_id = __MNT[pair.first].second;
                        tokens[i] = __MDT[label_id][0];
                        tokens.insert(tokens.begin() + i + 1, __MDT[label_id].begin() + 1, __MDT[label_id].end());
                    } else{
                        // Hash map that insert current id and return respective parameter
                        std::unordered_map<char,std::string> parameters;
                        int label_id = __MNT[pair.first].second;
                        // Go to parameters (currently in macro funcion name, not parameters)
                        i++;
                        // Get parameters
                        Token& parameter_token = tokens[i];
                        std::vector<std::string> splited_token = Utils::split(parameter_token.first, ',');

                        for(int i = 0; i < splited_token.size(); i++){
                            parameters['1' + i] = splited_token[i];
                        }

                        // Tokens parsed to generic macro to especifc macro
                        std::vector<Token> fixed_tokens = __MDT[label_id];

                        for(auto &pair : fixed_tokens){
                            // If token don't have #, then this token don't use macro parameter
                            if(pair.first[0] != '#') continue;
                            // New token string
                            std::string new_token;
                            splited_token = Utils::split(pair.first, ',');
                            for(int i = 0; i < splited_token.size(); i++){
                                if(i) new_token += ",";
                                // Transform #1 to respective parameter
                                new_token += parameters[splited_token[i][1]];
                            }
                            // Overwrite generic parameter
                            pair.first = new_token;
                        }

                        // Overwrite macro name token with the first fixed_tokens
                        tokens[i-1] = fixed_tokens[0];
                        // Overwrite parameters token with the second fixed_tokens
                        tokens[i] = fixed_tokens[1];
                        // Insert the remaining macro token
                        tokens.insert(tokens.begin() + i + 1, fixed_tokens.begin() + 2, fixed_tokens.end());
                    }
                }
            }
        }
        // EXPAND MACROS
        if(has_macro){
            // Hash map to order current parameter to generic parameter in MDT
            std::unordered_map< std::string,char> parameters;
            // If have 3 tokens, then will be label, macro and parameters
            if(tokens.size() == 4){
                
                std::string current_parameter;
                std::vector<std::string> splited_token = Utils::split(tokens[2].first, ',');

                for(int i = 0; i < splited_token.size(); i++){
                    current_parameter = splited_token[i];
                    // Remove &
                    current_parameter.erase(current_parameter.begin());
                    // Create hash table with Parameter and current generic ID
                    parameters[current_parameter] = '1' + i;
                }
                
            }
            // Insert macro name into MNT table, with current amount of paramenters and respective MDT id
            __MNT[tokens[0].first] = std::make_pair(parameters.size(), ++__macro_id);

            // Read all next line until ENDMACRO
            getline(__file_pointer,line);
            tokens = _filter_line(line);
            while(tokens[0].second != ENDMACRO){
                tokens.push_back(std::make_pair("\n", 11));
                // Checks if have &parameter, in order to change to the generic in MDT
                for(Token &pair : tokens){
                    // If is not &, then don't need to change
                    if(pair.first[0] != '&') continue;
                    // New generic token with correct parameters                    
                    std::string new_token, current_parameter;
                    // Split tokens by ','
                    std::vector<std::string> splited_token = Utils::split(pair.first, ',');
                    // Iterate through splited token
                    for(int i = 0; i < splited_token.size(); i++){
                        // Add ',' if have more than 1 parameter
                        if(i) new_token += ",";
                        // Get splited parameter
                        current_parameter = splited_token[i];
                        // Erase &
                        current_parameter.erase(current_parameter.begin());
                        // Add # + Current ID
                        new_token = new_token + "#" + parameters[current_parameter];
                    }
                    // New token with generic parameter
                    pair.first = new_token;
                }

                // Append tokens from that macro in MDT table
                __MDT[__macro_id].insert(__MDT[__macro_id].end(), tokens.begin(), tokens.end());

                // Read all next line until ENDMACRO
                getline(__file_pointer,line);
                tokens = _filter_line(line);
            }
            // Remove last duplicated \n token
            __MDT[__macro_id].pop_back();

            // Don't add tokens to final file becaus macro definition don't appear in main final file
            continue;
        }

        // EXPAND IF - IF <value> <\n>
        // IF line not added to 
        else if(has_if){
            _expand_ifs(tokens);
            continue;
        }
        // EXPAND EQU - LABEL: EQU VALUE <\n>
        else if(has_equ){
            _expand_equs(tokens);
            continue;
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

void Pre_processor::_expand_ifs(std::vector<Token> curr_tokens){
    // FIXME: Verificar se argumento do IF é um número
    if( curr_tokens.size() == 3){
        if(curr_tokens[1].first == "0"){
            // Não inclui a próxima linha
            std::string line;
            getline(__file_pointer, line);
        }
    } else {
        // TODO: IF errors
    }
}


void Pre_processor::_expand_equs(std::vector<Token> curr_tokens){
    // FIXME: Verificar se argumento do EQU é um número
    if( curr_tokens.size() == 4 && 
        curr_tokens[0].second == LABEL &&
        curr_tokens[1].second == EQU &&
        curr_tokens[2].second == -1){
        // EQU usada corretamente
        __equs[curr_tokens[0].first] = stoi(curr_tokens[2].first);
    } else {
        // TODO: Tratamento de erros para EQU
    }
}

#endif