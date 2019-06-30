#include <stdio.h>
#include <iostream>
#include <fstream>

#include "../lib/colors.h"

#include "pre_processor.cpp"
#include "processor.cpp"

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
    // Tries to create the Pre_Processor
    Pre_processor* pre_proc;
    try{
        pre_proc = new Pre_processor(argv[1]);    // Creation may fail because of file
    } catch(std::string err) {
        printf(YELLOW "ABORTED\n" RESET);
        std::cout << "Aconteceu um erro fatal: " << err << std::endl;
        return 1;
    }
    // Runs pre_processor
    std::vector<Token> processed_file = pre_proc->run();
    // Creates processor
    Processor proc(argv[1], processed_file);
    // Runs processor
    proc.run();
    // Generates output
    try {
        std::string out = proc.generate_output();
        printf(GREEN "Arquivo de saída gerado com sucesso.\n" RESET);
        printf("Saída salva no arquivo %s\n", out.c_str());
    } catch(std::string err) {
        printf(YELLOW "ABORTED\n" RESET);
        std::cout << "Aconteceu um erro fatal: " << err << std::endl;
        return 1;
    }

    return 0;
}