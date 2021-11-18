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
char *symbolTableFileName;

Node *root;
std::map<std::string, Value*> env;

void parseArguments(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "ERROR: NO INPUT FILE PROVIDED" << std::endl;
        inputFileName = NULL;
    }
    inputFileName = argv[1];

    if (argc >= 3) {
        symbolTableFileName = argv[2];
    } else {
        symbolTableFileName = NULL;
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
        Value *v = ev(prog);
        if (v != NULL && v->type == VAL_ERROR) {
            std::cout << v->stringify() << std::endl;
        }
        delete v;
    }
    // Clean up the AST after we are done
    if (symbolTableFileName != NULL) {
        for (auto it = env.begin(); it != env.end(); it++) {
            std::string name = it->first;
            Value *v = it->second;
            // TODO: write to file
            std::cout << name << ": " << v->stringify() << std::endl;
        }
    }
    delete root;
    return 0;
}