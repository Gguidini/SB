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
    //TODO: Update class
    int __curr_section;
    int __curr_line;
    std::ifstream INPUT;
    std::string OUTPUT_NAME;
    std::ofstream OUTPUT;

public:
    Pre_processor(std::string input){
        __curr_line = 0;
        OUTPUT_NAME = input + ".pre";
        INPUT.open(input);
        OUTPUT.open(OUTPUT_NAME);
    }

    // Executes the pre processor.
    // Returns true if process completed ok, otherwise returns false.
    // Stores name of output file in out_name.
    // Any errors are added to error_buffer.
    // TODO: Make error_buffer a vector of errors, not strings.
    bool run(std::string& out_name, std::vector<std::string>& error_buffer);
};

bool Pre_processor::run(std::string& out_name, std::vector<std::string>& error_buffer) {
    printf("Running pre-processor!\n");
    return true;
}

#endif