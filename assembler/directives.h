#ifndef DIRECTIVES_H
#define DIRECTIVES_H

// dependencies
#include <unordered_map>

// directives have a name, number of arguments, and if they are for pre-processing or not.
// Directives should implement they're exec function, that is,
// What the directive does. This must be overriden for each Directive.
class Directive {
    std::string NAME;
    int ARGS;
    bool PRE_PROCESS;

public:
    Directive(std::string name, int args, bool pre){
        NAME = name;
        ARGS = args;
        PRE_PROCESS = pre;
    }

    // This finction need to be implemented by EVERY DIRECTIVE
    virtual void exec(void* args, int argc);

    std::string name(){
        return NAME;
    }

    int num_args() {
        return ARGS;
    }

    bool is_preprocess(){
        return PRE_PROCESS;
    }
};

#endif