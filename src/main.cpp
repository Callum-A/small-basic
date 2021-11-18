#include "node.hpp"
#include "value.hpp"
#include "evaluator.hpp"
#include <iostream>
#include <random>
#include <time.h>
#include <string.h>
#include <map>

extern int yyparse();
extern FILE* yyin;
char *inputFileName;

Node *root;
std::map<std::string, Value*> env;

void parseArguments(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "ERROR: NO INPUT FILE PROVIDED" << std::endl;
        inputFileName = NULL;
    }
    inputFileName = argv[1];
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
        Value *v = ev(prog);
        if (v != NULL && v->type == VAL_ERROR) {
            std::cout << v->stringify() << std::endl;
        }
        delete v;
    }
    // Clean up the AST after we are done
    delete root;
    return 0;
}