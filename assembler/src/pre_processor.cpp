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
    std::vector<std::string> run();
    // FIXME: Remove this implementation an code correct one outside class declaration
    std::string generate_output() {return "SB2019-1";}
    std::string _filter_line(std::string&line);    
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

std::string Pre_processor::_filter_line(std::string &line){
    std::string final_line = "";
    for(int i = 0; i < (int)line.size(); i++){
        // current char
        char c = line[i];
        // Erase comments
        if(c == ';'){
            break;
        }
        // Erase mutiples spaces and ending space
        else if(c == ' '){
            // Jump all spaces until end or next valid char
            while(i < (int)line.size() and line[i] == ' ')i++;
            // If has a valid char that is diff from comment, add space                
            if(i < (int)line.size()){   
                if(line[i] == ';') break;
                final_line += " ";
            }
            // Get current char diff from space
            i--;
        }
        // Add normal char
        else if( c >= '!' and c <= '~' ){
            final_line += c;
        }
    }
    return final_line;
}

std::vector<std::string> Pre_processor::run(){
    
    std::vector<std::string> processed_file;
    std::string line;

    while(getline(__file_pointer,line)){

        std::string processed_line = _filter_line(line);
        
        if(processed_line == "") continue;
        
        // TODO : EXPAND MACROS
        // TODO : SOLVE IFS
        // TODO : SOLVE EQU

        processed_file.push_back(processed_line);
    }
    __done = true;
    return processed_file;
}

#endif