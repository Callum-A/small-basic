#include "node.hpp"
#include "value.hpp"
#include "evaluator.hpp"
#include <iostream>
#include <map>

int yyparse();
Node *root;
std::map<std::string, Value*> env;

int main(void) {
    int status = yyparse();
    if (status == 0) {
        ProgramNode *prog = dynamic_cast<ProgramNode*>(root);
        Value *v = ev(prog);
        if (v != NULL && v->type == VAL_ERROR) {
            std::cout << v->stringify() << std::endl;
        }
    }
    return 0;
}