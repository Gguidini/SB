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

// Processor class.
// Processes a stream of tokens, transforming them into IA-32 binary code
class Processor {
    // Private attributes
    // General information
    std::string __input_name;
    bool __done;
    
public:
    // Constructors 
    Processor();
    Processor(std::string input);
    // Process
    std::vector<std::pair < std::vector<char>, std::vector<char> > > run();

};


// Processor sem arquivo de referência.
// Nada é inicializado como válido.
Processor::Processor(){
    __done = false;
}

// Processor com arquivo de referência.
// Arquivo de referência já é aberto.
Processor::Processor(std::string input_name){
    __input_name = input_name;
    __done = false;
}

std::vector<std::pair < std::vector<char>, std::vector<char> > > Processor::Processor run(){
    
    fstream file(__input_name);
    std::vector<std::pair < std::vector<char>, std::vector<char> > > processed;

    if(!file){
        cout << "Invalid file name!" << endl;
        return processed;
    }

    // TODO : Finish this function

}



#endif
