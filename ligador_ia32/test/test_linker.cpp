#include <iostream>
#include <fstream>
#include <cassert>

#include "../src/linker.cpp"

void test_hello_world(){
    // Testing linker with hello world example
    std::vector<char> data = {
        '\x48', '\x65', '\x6C', '\x6C', '\x6F',   // msg: db   'Hello, World!', 10
        '\x2C', '\x20', '\x57', '\x6F', '\x72',
        '\x6C', '\x64', '\x21', '\x0A'
    };
    std::vector<char> text = { 
        '\xB8', '\x04', '\x00', '\x00', '\x00',   // mov eax, 4                   
        '\xBB', '\x01', '\x00', '\x00', '\x00',   // mov ebx, 1                   
        '\xB9', '\x00', '\xa0', '\x04', '\x08',   // mov ecx, msg                 
        '\xBA', '\x0E', '\x00', '\x00', '\x00',   // mov edx, 14                  
        '\xCD', '\x80',                           // int 0x80                     
        '\xB8', '\x01', '\x00', '\x00', '\x00',   // mov eax, 1      
        '\xBB', '\x00', '\x00', '\x00', '\x00',   // mov ebx, 0              
        '\xCD', '\x80'                            // int 0x80                     
    };
    std::pair<std::vector<char>, std::vector<char>> binary = std::make_pair(text, data);
    Linker linker(binary, std::string("hello_world"));
    bool ok = linker.run();
    assert(ok);
}

void test_bin(){
    // Testing linker with bin.s file
    // (Created with tradutor from bin.asm)
    std::vector<char> text;
    // Copia todo o código em binario
    int d = 0;

    std::string line;

    FILE* file;
    file = fopen("bin_code.txt", "r");
    char ch;
    while(fscanf(file, " %x", &ch) != EOF){
        text.push_back(ch);
    }
    fclose(file);

    printf("%d\n", d);
    printf("Finished reading text portion\n");
    std::vector<char> data = {
        '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00',
        '\x0d', '\x0a',
        '\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38', '\x39',
        '\x41', '\x42', '\x43', '\x44', '\x45', '\x46',
        '\x02', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00',
        '\x00', '\x00', '\x00', '\x00'
    };
    printf("Finished creating data portion\n");
    std::pair<std::vector<char>, std::vector<char>> binary = std::make_pair(text, data);
    Linker linker(binary, std::string("int_to_bin"));
    bool ok = linker.run();
    assert(ok);
}

int main() {
    test_hello_world();
    printf("Hello World created ok\n");
    test_bin();
    printf("Int_to_bin created ok\n");
}