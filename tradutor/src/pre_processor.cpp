// The pre processor classes.
// Pre_processor executes the pre processing of program,
// that is, identifies and expands EQUs, IFs and MACROs.
// This class also performs the first pass in the assembly process,
// That is, creates the symbol table for the program.
// The process continues to the processor and output is generated only if no errors are found.
// Outputs of this class include:
// Pre-processed file (with pre processor directives expanded, comments and unnecessary tabs removed)
// Symbol table
// Vector of Tokens, for the Processor

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
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

typedef std::pair<std::string, int> Token;

// create_tagged_token analisa uma string para ver se é um token reconhecido ou não.
// Retorna true se o token for válido (não-vazio), false se for inválido.
// Salva o token criado na variável tok
bool create_tagged_token(std::string str, Token& tok){

    // COPY A, B - Put all arguments in the same token
    if(str == ""){
        return false;
    } else if(str.back() == ','){
        str.pop_back();
        tok = std::make_pair(Utils::to_upper(str), -1);
    }

    std::string test = Utils::to_upper(str);
    if(test == "EQU"){
        tok = std::make_pair(test, EQU);
    } else if(test == "IF"){
        tok = std::make_pair(test, IF);
    } else if(is_opcode(test)){
        tok = std::make_pair(test, OP);
    } else if(test == "SECTION") {
        tok = std::make_pair(test, SEC);
    } else if(test == "SPACE" || test == "CONST") {
        tok = std::make_pair(test, DIR);
    } else {
        tok = std::make_pair(test, -1);
    }

    return true;
}

// lex_analyser analisa a string de um token LABEL
// Para verificar se está de acordo com as regras.
// Labels: /[\w_][\w\d_]{0,49}/
int lex_analyser(std::string token){
    // Erros lexicos
    if(isdigit(token[0])) return 1;
    if((int) token.size() > 50) return 1;

    for(char ch : token){
        if((!isalnum(ch)) && (!(ch == '_'))){
            return 1;
        }
    }
    // Palavra reservada
    std::vector<std::string> ops = {
        "ADD", "SUB", "MULT", "DIV", "JMP", "JMPN", "JMPP",
        "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT",
        "STOP", "EQU", "IF", "SPACE", "CONST", "END"
    };
    for(std::string str : ops){
        if(token == str) return 2;
    }
    return 0;
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
    std::vector<Error> __errs;          // Vector of Errors
    std::unordered_map<std::string, Symbol> __symbol_table;  // Symbol table
    std::unordered_map<std::string, Instruction> __instructions; // All intructions

    // EQU values
    std::map<std::string, int> __equs;
    void _expand_equs(std::vector<Token> curr_tokens, int& curr_line);

    // Expand IFs
    void _expand_ifs(std::vector<Token> curr_tokens, int& curr_line);

    // Sections
    bool __section_data;
    bool __section_text;
    int _identify_section(std::vector<Token> tokens, int& curr_line, int i, int curr_section);

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
    std::unordered_map<std::string, Symbol> get_symbol_table(){return __symbol_table;}
    bool get_done(){return __done;}
    // Run
    std::vector<Token> run();
    // Generate output
    std::string generate_output();
    // Internal use (public for testing purposes)
    std::vector<Token> _filter_line(std::string&line, int lst_line = 0);    
};

// Pre_processor sem arquivo de referência.
// Nada é inicializado como válido.
Pre_processor::Pre_processor(){
    __buffer = std::vector<std::string>();
    __input_name = "";
    __output_name = "";
    __done = false;
    __section_data = false;
    __section_text = false;
    __errs = std::vector<Error>();
}

// Pre_processor com arquivo de referência.
// Arquivo de referência já é aberto.
Pre_processor::Pre_processor(std::string input_name){
    __buffer = std::vector<std::string>();
    __output_name = input_name.substr(0, input_name.size() - 4) + ".pre";
    __done = false;
    __section_data = false;
    __section_text = false;
    __errs = std::vector<Error>();
    __symbol_table = std::unordered_map<std::string, Symbol>();
    __instructions = table_of_instructions();
    __input_name = input_name;
    // Verifica se extensão é .txt
    if(__input_name.substr(__input_name.size() - 4, 4) != ".txt" &&
       __input_name.substr(__input_name.size() - 4, 4) != ".asm"){
        throw "Arquivo " + __input_name + " não possui extensão correta (.txt ou .asm)";
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
// &A, &B MACRO     ;
std::vector<Token> Pre_processor::_filter_line(std::string &line, int lst_line){
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
            // Jump all spaces until end or next valid char
            while(i < (int)line.size() and (line[i] == ' ' or line[i] == '\t')) i++;
            if(i < (int)line.size() && line[i] == ':'){
                i--;
                continue;
            }
            // Check if curr token is any of the preprocessing directives
            // And this space is pointing to the end of it
            Token tok;
            bool curr_tok_valid = create_tagged_token(curr_token_str, tok);
            if(curr_tok_valid){
                next_tokens.push_back(tok);
                curr_token_str = "";
            }
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
            if(lex_analyser(curr_token_str) == 1){
                __errs.push_back(Error(LEX_ERR, lst_line + 1, "Label " + curr_token_str + " inválida", __input_name));
            } else if(lex_analyser(curr_token_str) == 2){
                __errs.push_back(Error(SEM_ERR, lst_line + 1, "Palavra reservada " + curr_token_str + " nao pode ser uma label.", __input_name));
            }
            next_tokens.push_back(std::make_pair(Utils::to_upper(curr_token_str), LABEL));
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

    next_tokens.push_back(std::make_pair("\n", 11));
    return next_tokens;
}

// run processes de original file.
// Saves processed lines in __buffer. Returns this value.
std::vector<Token> Pre_processor::run(){
    
    std::vector<std::string> processed_file;
    std::vector<Token> processed_tokens;
    std::string line;
    int curr_line = 0;
    int curr_section = -1;
    int curr_addrs = 0;
    std::vector<Token> tokens = std::vector<Token>();   // Empty token vector
    while(getline(__file_pointer,line)){
        bool has_equ = false;
        bool has_macro = false;
        bool has_if = false;
        bool has_label = false;
        int add_addrs = 0;  // Addres to add after processing line

        std::vector<Token> tokens_to_add = _filter_line(line, curr_line);
        curr_line++;
        // Verifica se precisa buscar outro linha do arquivo ou pode processar
        // Os tokens atuais.
        // Precisa buscar outra se: linha atual eh vazia
        // Ou se a linha eh apenas uma label.
        if(tokens_to_add.size() == 1) {
            continue;    // Empty line contains only ENDL token
        } else if(tokens_to_add.size() == 2 && tokens_to_add[0].second == LABEL){
            // Line with only a label.
            // Label should be on the line below for processing.
            tokens_to_add.pop_back();   // Remove <\n>
            tokens.insert(tokens.end(), tokens_to_add.begin(), tokens_to_add.end());
            continue;
        } else {
            // Line with anything not empty and not only label
            // So should be processed
            tokens.insert(tokens.end(), tokens_to_add.begin(), tokens_to_add.end());
        }

        for(int i = 0; i < (int) tokens.size(); i++){
            Token &pair = tokens[i];

            if(pair.second == EQU){
                // Marca que haverá processamento de EQU
                has_equ = true;
            } else if(pair.second == IF){
                // Marca que haverá processamento de IF
                has_if = true;
            } else if(pair.second == SEC) {
                // Esta linha identifica um nova seção.
                curr_section = _identify_section(tokens, curr_line, i, curr_section);
            } else if(pair.second == LABEL) {
                // Marca que possui uma label que talvez deva ir para a tabela de símbolos
                has_label = true;
                if(tokens[i+1].second == LABEL){
                    __errs.push_back(Error(SEM_ERR, curr_line, "Duas labels com o mesmo valor (na mesma linha)", __input_name));
                }
            } else if(pair.second == OP){
                // Aumenta o contador de endereços
                if(curr_section != SEC_TEXT){
                    __errs.push_back(Error(SEM_ERR, curr_line, "Operação " + pair.first + " nao pode estar fora da section text", __input_name));
                }
                add_addrs += 0;
            } else if(pair.second == DIR){
                // Aumenta o contador de endereços
                if(curr_section != SEC_DATA){
                    __errs.push_back(Error(SEM_ERR, curr_line, "Diretiva " + pair.first + " nao pode estar fora da section data", __input_name));
                }
                std::string err = "";
                int v = Utils::digit_value(tokens[i+1].first, err);
                if(Utils::to_upper(pair.first) == "CONST"){
                    add_addrs += 1;
                } else if(err == ""){
                    if(v <= 0){
                        v = 1;
                    }
                    add_addrs += v;
                } else if(err != "\n"){
                    __errs.push_back(Error(LEX_ERR, curr_line, err, __input_name));
                } else {
                    add_addrs += 1;
                }
            } else if(pair.second == -1){
                // Processa itens sem marcação.

                // Verifica se item pode estar escrito errado
                std::vector<std::string> splited_token = Utils::split(pair.first, ',');

                for(std::string tok : splited_token){
                    if(tok == "+" || has_macro || tok[0] == '&'){
                        // + nao esta escrito errado
                        // Se tiver macro os tokens relativos a parametros tem que
                        // ser vistos depois.
                        continue;
                    }
                    std::string err = "";
                    Utils::digit_value(tok, err);
                    if((err != "" && err != "\n") && lex_analyser(tok) == 1){
                        __errs.push_back(Error(LEX_ERR, curr_line, "Operando " + tok + " inválido", __input_name));
                    } else if((err != "" && err != "\n") && lex_analyser(tok) == 2){
                        __errs.push_back(Error(SEM_ERR, curr_line, "Palavra reservada " + tok + " nao pode ser operando.", __input_name));
                    }
                }
                // Prováveis LABELS em uso
                if(__equs.find(pair.first) != __equs.end()){
                    // Expand EQU
                    pair.first = std::to_string(__equs[pair.first]);
                } 
            }
        }
        if(has_if){
            // EXPAND IF - IF <value> <\n>
            // IF line not added to preprocessed file
            _expand_ifs(tokens, curr_line);
            tokens.clear();
            continue;
        } else if(has_equ){
            // EXPAND EQU - LABEL: EQU VALUE <\n>
            // EQU definition line not added to preprocessed file
            _expand_equs(tokens, curr_line);
            tokens.clear();
            continue;
        } else if(has_label){
            // Se tem uma label, mas nenhuma das diretivas de pre processador,
            // Entao esta label vai para a tabela de simbolos.
            // LABEL: <op ou dir>
            if(tokens[0].second != LABEL){
                __errs.push_back(Error(SYN_ERR, curr_line, "Label deve estar no inicio da linha", __input_name));
            }
            if(__symbol_table.find(tokens[0].first) != __symbol_table.end()){
                __errs.push_back(Error(SEM_ERR, curr_line, "Redefinição de label " + tokens[0].first, __input_name));
            }
            if(curr_section == -1){
                __errs.push_back(Error(SEM_ERR, curr_line, "Label " + tokens[0].first + " precisa estar dentro de alguma section", __input_name));
            }
            bool jumpable = false, constant = false, const_zero = false, vector = false;
            int vec_size = 0;
            if(curr_section == SEC_TEXT){
                // LABEL: <op>
                if(tokens[1].second == OP){
                    jumpable = true;
                } else {
                    __errs.push_back(Error(SEM_ERR, curr_line, "Label " + tokens[0].first + " não pode ser definida na section text", __input_name));
                }
            } else if(curr_section == SEC_DATA){
                // LABEL: <dir>
                if(tokens[1].second == DIR){
                    std::string err = "";
                    int s = Utils::digit_value(tokens[2].first, err);
                    if(Utils::to_upper(tokens[1].first) == "SPACE" && err == ""){
                        vector = true;
                        vec_size = s;
                    } else if(Utils::to_upper(tokens[1].first) == "CONST"){
                        constant = true;
                        if(tokens[2].first == "0"){
                            const_zero = true;
                        }
                    }
                } else{
                    __errs.push_back(Error(SEM_ERR, curr_line, "Label " + tokens[0].first + " não pode ser definida na section data", __input_name));
                }
            }
                    
            // Add label to table
            __symbol_table[tokens[0].first] = Symbol(
                curr_addrs, jumpable, constant, 
                const_zero, vector, vec_size);
        }


        // Transforma o vetor de tokens em string
        int s = tokens.size();
        std::string processed_line = tokens[0].first;
        if(tokens[0].second == LABEL){
            processed_line += ":";
        }
        for(int i = 1; i < s; i++){
            if(tokens[i].second == ENDL or tokens[i-1].second == ENDL){
                processed_line += tokens[i].first;
            } else if(tokens[i-1].first == "COPY"){
                processed_line += " " + tokens[i].first + ",";
            } else{
                processed_line += " " + tokens[i].first;
            }

            if(tokens[i].second == LABEL){
                processed_line += ":";
            }
        }
        processed_file.push_back(processed_line);
        processed_tokens.insert(processed_tokens.end(), tokens.begin(), tokens.end());
        tokens.clear();
        curr_addrs += add_addrs;
    }

    if(__section_text == false){
        // Section text missing
        __errs.push_back(Error(SEM_ERR, 0, "Section text faltando", __input_name));
    }
    __done = true;
    __buffer = processed_file;
    return processed_tokens;
}

// _identify_section identifica qual a seção que está sendo aberta.
// Faz o handling de erros também.
// Retorna qual a seção atual a partir desta linha
int Pre_processor::_identify_section(std::vector<Token> tokens, int& curr_line, int i, int curr_section){
    // sempre terá ao menos um token na frente, o ENDL
    if(tokens[i+1].second == ENDL){
        // SECTION sem nada na frente
        __errs.push_back(Error(SYN_ERR, curr_line, "Section indefinida",__input_name));
    } else if(Utils::to_upper(tokens[i+1].first) == "TEXT"){
        if(__section_text == true){
            // Section TEXT redefinition
            __errs.push_back(Error(SEM_ERR, curr_line, "Section text redefinida",__input_name));
        }
        __section_text = true;
        return SEC_TEXT;
    } else if(Utils::to_upper(tokens[i+1].first) == "DATA"){
        if(__section_data == true){
            // Section data redefinition
            __errs.push_back(Error(SEM_ERR, curr_line, "Section data redefinida",__input_name));
        }
        if(__section_text == false){
             // Section data has to come after section text
            __errs.push_back(Error(SEM_ERR, curr_line, "Section data deve vir depois da section text",__input_name));

        }
        __section_data = true;
        return SEC_DATA;
    } else {
        __errs.push_back(Error(SYN_ERR, curr_line, "Section " + tokens[i+1].first + " eh invalida",__input_name));   
    }
    return curr_section;
}

void Pre_processor::_expand_ifs(std::vector<Token> curr_tokens, int& curr_line){
    // IF ARG \n
    if( curr_tokens.size() == 3){
        if(curr_tokens[0].second == IF && curr_tokens[1].second == -1){
            if(Utils::is_digit(curr_tokens[1].first)){
                if(curr_tokens[1].first == "0"){
                    // Não inclui a próxima linha
                    std::string line;
                    while( getline(__file_pointer, line) ){
                        int line_num = curr_line;
                        std::vector<Token> tokens = _filter_line(line, line_num);
                        curr_line++;
                        if(tokens.size() != 1){
                            break;
                        }
                    }
                }
            } else {
            __errs.push_back(Error(SYN_ERR, curr_line, "Argumento do IF deve ser numerico.",__input_name));
            }
        } else {
            __errs.push_back(Error(SYN_ERR, curr_line, "IF mal formado. Sintaxe: IF ARG",__input_name));
        }
    } else {
        if(curr_tokens.size() > 3){
            __errs.push_back(Error(SYN_ERR, curr_line, "Muitos argumentos para diretiva IF",__input_name));
        } else {
            __errs.push_back(Error(SYN_ERR, curr_line, "Poucos argumentos para diretiva IF",__input_name));
        }
    }
}


void Pre_processor::_expand_equs(std::vector<Token> curr_tokens, int& curr_line){
    // LABEL: EQU ARG \n
    if(curr_tokens.size() == 4){
        if( curr_tokens[0].second == LABEL &&
            curr_tokens[1].second == EQU &&
            curr_tokens[2].second == -1){
            std::string err = "";
            int v = Utils::digit_value(curr_tokens[2].first, err);
            if(err == ""){
                __equs[curr_tokens[0].first] = v;
            } else {
                __errs.push_back(Error(SYN_ERR, curr_line, "Argumento EQU deve ser numerico",__input_name));
            }
            // EQU usada corretamente
        } else {
            __errs.push_back(Error(SYN_ERR, curr_line, "EQU mal formado. Sintaxe: LABEL: EQU ARG",__input_name));
        }
    } else {
        if(curr_tokens.size() > 4){
            __errs.push_back(Error(SYN_ERR, curr_line, "Muitos argumentos para diretiva EQU",__input_name));
        } else {
            __errs.push_back(Error(SYN_ERR, curr_line, "Poucos argumentos para diretiva EQU", __input_name));
        }
    }
}

#endif
