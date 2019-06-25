#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <iostream>

class Symbol {
    int __addrs;        // Actual value
    bool __jumpable;    // If symbol is in TEXT section
    bool __constant;    // If symbol is CONST
    bool __const_zero;  // If symbol is CONST 0 (cannot be arg for DIV)
    bool __vector;      // If symbol is SPACE + something
    int __vector_size;  // Size of SPACE + 

public:
    Symbol(){};
    Symbol(int addrs, bool can_jump, bool is_const, bool is_zero, bool is_vector, int vec_size){
        __addrs = addrs;
        __jumpable = can_jump;
        __constant = is_const;
        __const_zero = is_zero;
        __vector = is_vector;
        __vector_size = vec_size;
    }
    // Getters
    int get_value() {return __addrs;}
    bool can_jump() {return __jumpable;}
    bool is_const() {return __constant;}
    bool is_const_zero() {return __const_zero;}
    bool is_vector() {return __vector;}
    int get_vector_size() {return __vector_size;}
};

#endif