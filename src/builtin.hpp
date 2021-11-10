#pragma once
#include "value.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <random>

class Builtin {
public:
    Builtin() {}
    virtual Value *execute(std::vector<Value*> *args) { return NULL; };
};

class ReadLine : public Builtin {
public:
    ReadLine() {}
    Value *execute(std::vector<Value*> *args) {
        std::string line;
        std::getline(std::cin, line);
        char *tmp = (char *)malloc(line.size() + 1);
        strcpy(tmp, line.c_str());
        return new StringValue("TWAT");
    }
};

class Random : public Builtin {
public:
    Random() {}
    Value *execute(std::vector<Value*> *args) {
        double f = sqrt((double)rand() / RAND_MAX);
        // TODO: replace with min max args
        return new NumberValue(0 + f * (1 - 0));
    }
};
