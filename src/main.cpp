#include "execute.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <time.h>
#include <string.h>
#include <map>

extern int yyparse();
extern FILE *yyin;
char *inputFileName;
bool runDebug = false;
bool outputSymbolTable = false;
int breakpoint = -1;

Node *root;
std::map<std::string, Value*> env;

// Call interpeter in format ./sb input.sb --debug --sym
void parseArguments(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "ERROR: NO INPUT FILE PROVIDED" << std::endl;
        std::cout << "Usage: ./sb inputFile [--debug] [--sym] [bp]" << std::endl;
        std::cout << "    --debug                : Run program statement by statement" << std::endl;
        std::cout << "    --sym                  : Output symbol table after execution" << std::endl;
        inputFileName = NULL;
        return;
    }
    inputFileName = argv[1];
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            char *arg = argv[i];
            if (strcmp(arg, "--debug") == 0) {
                runDebug = true;
            } else if (strcmp(arg, "--sym") == 0) {
                outputSymbolTable = true;
            } else {
                int lineNum = (int) atoi(arg);
                breakpoint = lineNum;
            }
        }
    }
    
}

int main(int argc, char *argv[]) {
    parseArguments(argc, argv);
    if (inputFileName == NULL) {
        return 1;
    }

    yyin = fopen(inputFileName, "r");
    if (yyin == NULL) {
        std::cout << "ERROR: INPUT FILE COULD NOT BE FOUND!" << std::endl;
        return 1;
    }

    srand(time(NULL));
    registerBuiltins();
    int status = yyparse();
    if (status == 0) {
        // Successful parse
        ProgramNode *prog = dynamic_cast<ProgramNode*>(root);
        execute(prog, outputSymbolTable);
    }
    // Clean up the AST after we are done
    delete root;
    return 0;
}