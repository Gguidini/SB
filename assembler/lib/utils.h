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

#include <string>
#include <vector>
#include <algorithm>
#include <map>

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

// Decimais: 0, 1, 2, 0d10, 10d
// Hexadexial: 0x10, 10h

// ADD N + 2
// ADD N + 2d
// ADD N + d

// LABEL: SPACE d
int stox(std::string str, std::string &err){
   std::unordered_map<char,int> value;
   int base = 1;
   int sum = 0;
   for(int i = 0; i < 10; i++) value['0' + i] = i;
   for(int i = 10; i < 16; i++) value['A' + i - 10] = i;
   std::reverse(str.begin(), str.end());
   for(auto digit : str){
       if(! value.count(toupper(digit))){
           std::reverse(str.begin(), str.end());
           err = "Numero hexadecimal inválido (" + str + ")";
           return -1;
       }
      sum += (base * value[toupper(digit)]);
      base *= 16;
   }
   return sum;
}

int digit_value(std::string str, std::string & err){
    if(str == "\n"){
        err = "\n";
        return 0;
    }
    int sinal = 1;
    if(str[0] == '-'){
        sinal = -1;
        str = str.substr(1,str.size()-1);
    }
    if(is_digit(str)){
        return stoi(str);
    }
    if(str.substr(0, 2) == "0d"){
        if(str.size() == 2){
            err = "Número decimal inválido (" + str + ")";
            return -1;
        }
        return stoi(str.substr(2,str.size() - 2)) * sinal;
    }
    if(str[str.size()-1] == 'd'){
        if(str.size() == 1){
            err = "Número decimal inválido (" + str + ")";
            return -1;
        }
        str.pop_back();
        return stoi(str) * sinal;
    }
    if(str.substr(0, 2) == "0x"){
        if(str.size() == 2){
            err = "Número hexadecimal inválido (" + str + ")";
            return -1;
        }
        return stox(str.substr(2,str.size() - 2), err) * sinal;
    }
    if(str[str.size()-1] == 'h'){
        if(str.size() == 1){
            err = "Número hexadecimal inválido (" + str + ")";
            return -1;
        }
        str.pop_back();
        return stox(str, err) * sinal;
    }
    err = "Número inválido (" + str + ")";
    return -1;
}

}
#endif