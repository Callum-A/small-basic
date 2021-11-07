#include "node.hpp"
#include "value.hpp"
#include "evaluator.hpp"
#include <iostream>
#include <map>

int yyparse();
Node *root;
std::map<std::string, Value*> env;

int main(void) {
    yyparse();
    ProgramNode *prog = dynamic_cast<ProgramNode*>(root);
    ev(prog);
    return 0;
}