// Main do projeto.
// TODO: Atualizar estes cometários.
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "../lib/colors.h"

#include "pre_processor.cpp"

void showHelp(int args){
    printf("./tradutor <assembly file>\n");
    if(args == 1)
        printf(RED "Looks like you forgot the input file!\n\n" RESET);
    else
        printf(RED "You entered too many files!\n\n" RESET);

    printf("tradutor was created by Giovanni M Guidini and Thiago Veras Machado.\n");
    printf("This program will assemble files written in the imaginary assembly language.\n");
}

int main(int argc, char **argv){
    // Verify that number of arguments is correct.
    if(argc !=  2){
        // If no input files were given, or too many files.
        showHelp(argc);
        return 0;
    } 
    // Try to open input file. If file not found, abort.
    std::ifstream fd (argv[1]);
    if(!fd.is_open()){
        printf(RED "[Fatal Error]" RESET " Input file not found.\n");
        return 0;
    }
    // Runs pre_processor
}