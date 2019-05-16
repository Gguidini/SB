// This header file defines the INstruction class and 
// table_of_instructions() function.
//
//  Instruction class defines an instruction, with
//  OPCODE, NAME, OPERANDS, SIZE
//
//  table_of_instructions() returns an unordered_map
//  with all the instructions indexed by NAME.

#ifndef UTILS_H
#define UTILS_H

#include<string>
#include<vector>

namespace Utils {

std::vector<std::string> split(std::string &patt, char s){
    std::vector<std::string> ret;
    std::string current_string;
    for(auto c : patt){
        if(c == s){
            ret.push_back(current_string);
            current_string.clear();
        } else{
            current_string += c;
        }
    }
    ret.push_back(current_string);
    return ret;
}

std::string to_upper(std::string str){
    std::string out = "";
    for (char c: str){
        out += toupper(c); // curr_token_str UPPERCASE
    } 
    return out;
}

bool is_digit(std::string str){
    for(char ch : str){
        if(!isdigit(ch)){
            return false;
        }
    }
    return true;
}

}
#endif