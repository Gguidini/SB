// Main do projeto.
// TODO: Atualizar estes cometários.
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
    
    printf("Executando analise do codigo....");
    // Tries to create the Pre_Processor
    Pre_processor pre_proc;
    try{
        pre_proc = Pre_processor(argv[1]);    // Creation may fail because of file
    } catch(std::string err) {
        printf(YELLOW "ABORTED\n" RESET);
        std::cout << "Aconteceu um erro fatal: " << err << std::endl;
        return 1;
    }
    // Runs pre_processor
    std::vector<Token> processed_file = pre_proc.run();
    std::vector<Error> pre_proc_errors = pre_proc.get_errors();
    // Verifies if it should generate output or not
    if(pre_proc_errors.size() == 0){
        printf(GREEN "OK\n" RESET);
        // generates output
        std::string pre_proc_output = pre_proc.generate_output();
        std::cout << "Arquivo pre processado gerado: " << pre_proc_output << std::endl;
    } else {
        printf(RED "FAILED\n" RESET);
        for(Error err : pre_proc_errors){
            std::cout << err << std::endl;
        }
        printf(YELLOW "Nenhum arquivo de saída foi gerado pelo pre-processador porque erros foram encontrados.\n" RESET);
    }
    // Collects info generated by pre_proc needed
    std::unordered_map<std::string, Symbol> symbol_table = pre_proc.get_symbol_table();
    std::string input_to_proc = pre_proc.get_output_name();
    // Created processor
    Processor proc(processed_file, symbol_table, input_to_proc);
    printf("Executando sintese do codigo....");
    proc.run();
    std::vector<Error> proc_errors = proc.get_errors();
    if(pre_proc_errors.size() != 0 && proc_errors.size() == 0){
        printf(GREEN "OK\n" RESET);
        printf(YELLOW "Nenhum arquivo de saida foi gerado porque a analise falhou\n");
    } else if(proc_errors.size() != 0){
        printf(RED "FAILED\n" RESET);
        printf(YELLOW "Nenhum arquivo de saida foi gerado pelo processador porque erros foram encontrados.\n" RESET);
    } else {
        printf(GREEN "OK\n" RESET);
        std::string proc_out = proc.generate_output();
        std::cout << "Arquivo objeto gerado: " << proc_out <<  std::endl;
    }

    if(pre_proc_errors.size() > 0 || proc_errors.size() > 0){
        std::cout << std::string('=', 50) << std::endl;

        printf("ERROS:\n");
        for(Error err : pre_proc_errors){
            std::cout << err << std::endl;
        }
        for(Error err : proc_errors){
            std::cout << err << std::endl;
        }
    }

}