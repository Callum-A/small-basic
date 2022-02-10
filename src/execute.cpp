#include "execute.hpp"

extern std::map<std::string, Value*> env;

void writeSymbolTable() {
    std::cout << "-- Symbol Table Start --" << std::endl;
    for (auto it = env.begin(); it != env.end(); it++) {
        std::string name = it->first;
        Value *v = it->second;
        std::cout << name << ": " << v->stringify() << std::endl;
    }
    std::cout << "-- Symbol Table End --" << std::endl;
}

void execute(ProgramNode *prog, bool debugMode, bool outputSymbolTable, std::vector<int> *breakpoints) {
    if (breakpoints->size() > 0) {
        std::vector<Node*> *stmts = prog->getStmts();
        for (int i = 0; i < stmts->size(); i++) {
            Node *stmt = (*stmts)[i];
            Value *v = ev(stmt);
            if (isError(v)) {
                std::cout << v->stringify() << std::endl;
                break;
            }
            if (outputSymbolTable) {
                writeSymbolTable();
            }
            if (std::find(breakpoints->begin(), breakpoints->end(), stmt->lineNum) != breakpoints->end()) {
                std::string input = "";
                while (input != "NEXT") {
                    std::getline(std::cin, input);
                }
            }
        }
        return;
    }
    if (debugMode) {
        std::vector<Node*> *stmts = prog->getStmts();
        for (int i = 0; i < stmts->size(); i++) {
            Node *stmt = (*stmts)[i];
            Value *v = ev(stmt);
            if (isError(v)) {
                std::cout << v->stringify() << std::endl;
                break;
            }
            if (outputSymbolTable) {
                writeSymbolTable();
            }
            std::string input = "";
            while (input != "NEXT") {
                std::getline(std::cin, input);
            }
        }
    } else {
        Value *v = ev(prog);
        if (isError(v)) {
            std::cout << v->stringify() << std::endl;
        }

        delete v;

        if (outputSymbolTable) {
            writeSymbolTable();
        }
    }    
}