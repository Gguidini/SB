// The pre processor classes.
// TODO: Update file description

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
// Dependencies
#include <iostream>
#include <fstream>
#include <vector>

// defines for Sections
#define TEXT 1
#define DATA 0

// The pre processor class.
// Receives the original file to process,
// Outputs a new file, with expanded macros.
class Pre_processor {
    
    std::vector<std::string> __buffer;  // Saves lines after processing
    std::string __input_name;      // Name of input file
    std::string __output_name;           // Name of output file
    std::ifstream __file_pointer;             // Input file pointer
    bool __done;                        // If processed the entire file

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
    std::vector<std::string> run(){
        // FIXME: Remove this implementation an code correct one outside class declaration
        std::vector<std::string> aux;
        aux.push_back("Thiago");
        aux.push_back("Veras");
        aux.push_back("Machado");
        return aux;
    };
    // FIXME: Remove this implementation an code correct one outside class declaration
    std::string generate_output() {return "SB2019-1";}
};

Pre_processor::Pre_processor(){
    __buffer = std::vector<std::string>();
    __input_name = "";
    __output_name = "";
    __done = false;
}

Pre_processor::Pre_processor(std::string input_name){
    __buffer = std::vector<std::string>();
    __input_name = input_name;
    __output_name = input_name.substr(0, input_name.size() - 4) + ".pre";
    __done = false;
    __file_pointer.open(input_name);
}

#endif