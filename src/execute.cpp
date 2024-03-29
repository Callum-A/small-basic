#include "execute.hpp"

extern std::map<std::string, Value*> env; // Variable map

/// Helper to write the symbol table.
void writeSymTable() {
    std::cout << "-- Symbol Table Start --" << std::endl;
    for (auto it = env.begin(); it != env.end(); it++) {
        std::string name = it->first;
        Value *v = it->second;
        std::cout << name << ": " << v->stringify() << std::endl;
    }
    std::cout << "-- Symbol Table End --" << std::endl;
}

// Execute a program.
void execute(ProgramNode *prog, bool outputSymbolTable) {
    Value *v = ev(prog);
    if (isError(v)) {
        std::cout << v->stringify() << std::endl;
    }

    delete v;

    if (outputSymbolTable) {
        writeSymTable();
    }
}