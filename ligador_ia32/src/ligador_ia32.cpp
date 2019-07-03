#include <iostream>
#include "linker.cpp"
#include "processor.cpp"
#include "../lib/colors.h"

void showHelp(int args){
    printf("./linker <assembly IA32 file>\n");
    if(args == 1)
        printf(RED "Looks like you forgot the input file!\n\n" RESET);
    else
        printf(RED "You entered too many files!\n\n" RESET);

    printf("tradutor was created by Giovanni M Guidini and Thiago Veras Machado.\n");
    printf("This program will assemble files written in the assembly I32 language.\n");
}

int main(int argc, char **argv){
    
    // Verify that number of arguments is correct.
    if(argc !=  2){
        // If no input files were given, or too many files.
        showHelp(argc);
        return 0;
    } 
    
    printf("Executando analise do codigo....");
    // Tries to create the Pre_Processor
    Processor* proc;
    Linker* link;
    
    proc = new Processor(argv[1]);    // Creation may fail because of file
    link = new Linker(proc->run(), argv[1]);
    if (link->run()){
        printf(GREEN "OK\n" RESET);
        std::cout << "Arquivo objeto gerado: " << link->get_file_name() <<  std::endl;
    } else{
        printf(RED "FAILED\n" RESET);
        printf(YELLOW "Nenhum arquivo de saida foi gerado pelo ligador porque erros foram encontrados.\n" RESET);
    }

}