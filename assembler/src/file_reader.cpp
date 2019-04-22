// This class reads a file, removes empty lines, and
// stores the line count referring to the original file.
// TODO: Update file description

#ifndef FILE_READER_H
#define FILE_READER_H
// Requirements
#include <vector>
#include <iostream>
#include <fstream>

class File_reader {
    // Internal buffer to store (line, line_number) pairs.
    std::vector<std::pair<std::string, int>> __buffer;
    std::string FILE_REF;
    int __current_line;
    int __size;
    bool __is_loaded;

public:
    File_reader() {
        __current_line = 0;
        __size = 0;
        __buffer.clear();
        __is_loaded = false;
    }

    File_reader(std::string file_name){
        File_reader();
        FILE_REF = file_name;
    }

    // Methods
    bool load_file();
    bool load_file(std::string file_path);
    std::string next_line();
    int current_line_number();
    // Getter methods
    int size() {return __size;}

    bool is_loaded() {return __is_loaded;}

    // Returns if all lines have been read.
    // That happens if __current_line = __size.
    bool done() {return __size == __current_line;}
};

// Reads file using the FILE_REF name.
// The file is stored in the __buffer.
// Lines that are only comments, or empty, are not stored.
bool File_reader::load_file() {
    std::ifstream fd (FILE_REF);
    if(!fd.is_open()){
        // FIXME: Maybe return an error, or throw an exception?
        return false;
    }
    // FIXME: Initially assuming that file is small enough to be held in memory.
    // Later, we should implement ways to load only parts of the file, if it's too big.
    int line_idx = 1;
    std::string line;
    while(getline(fd, line)){
        // Removes empty lines
        if(line == ""){
            line_idx++;
            continue;
        }
        int start = 0, end = line.size() - 1;
        // find first non-space index
        while(line[start] == ' ' && start < end) start++;
        // find last non-space index
        while(line[end] == ' ' && end > start) end--;
        // If there's nothing left, the line was empty
        if(start == end){
            line_idx++;
            continue;
        }
        // Removes comments.
        // A comment starts with a ';' and goes till the end of the line.
        size_t comment_start = line.find(";", start);
        if(comment_start != std::string::npos){
            end = comment_start;
        }

        // Whatever is left, is relevant for the program.
        __buffer.push_back(std::make_pair(line, line_idx));
        line_idx++;
    }

    fd.close();
    __size = line_idx;
    __is_loaded = true;
    return true;
}

// Saves file ref internally before executing load_file
bool File_reader::load_file(std::string file_path){
    FILE_REF = file_path;
    return load_file();
}

// Returns the reference of the current line in the original file.
int File_reader::current_line_number(){
    return __buffer[__current_line].second;
}

// Returns the next line,
// And moves the line counter.
std::string File_reader::next_line(){
    __current_line++;
    return __buffer[__current_line].first;
}

#endif