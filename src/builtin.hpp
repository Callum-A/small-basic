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
        return new StringValue("PLACEHOLDER");
    }
};

class Random : public Builtin {
public:
    Random() {}
    Value *execute(std::vector<Value*> *args) {
        if (args->size() != 2) {
            return new ErrorValue(0, "Expected 2 arguments when calling random!");
        }
        Value *mi = (*args)[0];
        Value *ma = (*args)[1];
        if (mi->type != VAL_NUMBER || ma->type != VAL_NUMBER) {
            return new ErrorValue(0, "Expected 2 number values for min and max!");
        }
        NumberValue *min = dynamic_cast<NumberValue*>(mi);
        NumberValue *max = dynamic_cast<NumberValue*>(ma);
        double f = sqrt((double)rand() / RAND_MAX);
        return new NumberValue(min->number + f * (max->number - min->number));
    }
};
