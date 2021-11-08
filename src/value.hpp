#pragma once

#include <cstring>
#include <cstdlib>
#include <iostream>

enum ValueType {
    VAL_NUMBER,
    VAL_BOOL,
    VAL_STRING,
    VAL_ERROR
};

class Value {
public:
    ValueType type;

    Value(ValueType type) {
        this->type = type;
    }

    virtual void f() {}

    virtual const char *stringify() { return ""; }
};

class NumberValue : public Value {
public:
    double number;

    NumberValue(double number) : Value(VAL_NUMBER) {
        this->number = number;
    }

    const char *stringify() override {
        std::string t = std::to_string(number);
        char *str = (char *)malloc(t.size() + 1);
        strcpy(str, t.c_str());
        return str;
    }
};

class BoolValue : public Value {
public:
    bool boolean;

    BoolValue(bool boolean) : Value(VAL_BOOL) {
        this->boolean = boolean;
    }

    const char *stringify() override {
        if (boolean) {
            return "true";
        } else {
            return "false";
        }
    }
};

class StringValue : public Value {
public:
    char *string;

    StringValue(char *string) : Value(VAL_STRING) {
        this->string = (char *)malloc(strlen(string) + 1);
        strcpy(this->string, string);
    }

    const char *stringify() override {
        // May have to strcpy
        return string;
    }
};

class ErrorValue : public Value {
public:
    char *error;
    int lineNum;

    ErrorValue(int lineNum, char *error) : Value(VAL_ERROR) {
        this->lineNum = lineNum;
        this->error = (char *)malloc(strlen(error) + 1);
        strcpy(this->error, error);
    }

    const char *stringify() override {
        std::string errPrelude = "ERROR AT LINE " + std::to_string(this->lineNum);
        std::string err = errPrelude + ": " + this->error;
        const char *str = err.c_str();
        return str;
    }
};