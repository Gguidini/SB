#ifndef ERRORS_H
#define ERRORS_H

// Defines
#define LEX_ERR 401 // Lexical error
#define SYN_ERR 402 // Syntatic error
#define SEM_ERR 403 // Semantic error

#include <string>
#include <iostream>

#include "../lib/colors.h"

class Error {
    int __err_line;
    int __err_code;
    std::string __err_msg;
    std::string __err_file;

public:
    // Constructors
    Error();
    Error(int code, int line, std::string msg, std::string file);
    // Getters
    int get_line() {return __err_line;}
    int get_code() {return __err_code;}
    std::string get_msg() {return __err_msg;}
    std::string get_file(){return __err_file;}
};

Error::Error(){
    __err_code = -1;
    __err_line = -1;
    __err_msg = "";
}

Error::Error(int code, int line, std::string msg, std::string file){
    __err_code = code;
    __err_line = line;
    __err_msg = msg;
    __err_file = file;
}

std::ostream& operator <<(std::ostream& oss, Error& err){
    oss << RED;
    switch(err.get_code()){
        case LEX_ERR:
            oss << "[Erro Lexico]";
            break;
        case SYN_ERR:
            oss << "[Erro Sintatico]";
            break;
        case SEM_ERR:
            oss << "[Erro Semantico]";
    }
    oss << RESET << " Arquivo: " << err.get_file() << ":" << err.get_line();
    oss << RED << " error: " << RESET << err.get_msg();
    return oss;
}

#endif