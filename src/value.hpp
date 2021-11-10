#pragma once

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

enum ValueType {
    VAL_NUMBER,
    VAL_BOOL,
    VAL_STRING,
    VAL_LIST,
    VAL_MAP,
    VAL_ERROR
};

template<class T> struct ptr_less {
    bool operator()(T* lhs, T* rhs) {
        return *lhs < *rhs;
    }
};

static unsigned int fnv(const char *str) {
    const size_t length = strlen(str) + 1;
    unsigned int hash = 2166136261u;
    for (size_t i = 0; i < length; ++i) {
        hash ^= *str++;
        hash *= 16777619u;
    }
    return hash;
}

class Value {
public:
    ValueType type;

    Value(ValueType type) {
        this->type = type;
    }

    virtual void f() {}

    virtual const char *stringify() const { return ""; }

    virtual const unsigned int hashKey() const {
        return 0;
    }

    inline bool operator< (const Value& rhs) const {
        return this->hashKey() < rhs.hashKey();
    }

};

class NumberValue : public Value {
public:
    double number;

    NumberValue(double number) : Value(VAL_NUMBER) {
        this->number = number;
    }

    const char *stringify() const override {
        std::string t = std::to_string(number);
        char *str = (char *)malloc(t.size() + 1);
        strcpy(str, t.c_str());
        return str;
    }

    const unsigned int hashKey() const override {
        return fnv(this->stringify());
    }
};

class BoolValue : public Value {
public:
    bool boolean;

    BoolValue(bool boolean) : Value(VAL_BOOL) {
        this->boolean = boolean;
    }

    const char *stringify() const override {
        if (boolean) {
            return "true";
        } else {
            return "false";
        }
    }

    const unsigned int hashKey() const override {
        if (boolean) {
            return 1;
        } else {
            return 0;
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

    const char *stringify() const override {
        // May have to strcpy
        return string;
    }

    virtual ~StringValue() {
        free(string);
    }

    const unsigned int hashKey() const override {
        return fnv(string);
    }
};

class ListValue : public Value {
public:
    std::vector<Value*> values;
    ListValue() : Value(VAL_LIST) {

    }

    void addValue(Value *v) {
        values.push_back(v);
    }

    const char *stringify() const override {
        std::string str = "[";
        for (int i = 0; i < values.size(); i++) {
            Value *v = values[i];
            str = str + v->stringify();
            if (i < values.size() - 1) {
                str = str + ", ";
            }
        }
        str += "]";
        char *tmp = (char *)malloc(str.size() + 1);
        strcpy(tmp, str.c_str());
        return tmp;
    }

    virtual ~ListValue() {
        for (int i = 0; i < values.size(); i++) {
            Value *v = values[i];
            delete v;
        }
    }

    const unsigned int hashKey() const override {
        return fnv(this->stringify());
    }
};

class MapValue : public Value {
public:
    std::vector<Value*> keys;
    std::map<unsigned int, Value*> map;

    MapValue() : Value(VAL_MAP) {}

    void addValue(Value *key, Value *val) {
        keys.push_back(key);
        map[key->hashKey()] = val;
    }

    void removeValue(Value *key) {
        keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
        auto it = map.find(key->hashKey());
        map.erase(it);
    }

    Value *getValue(Value *key) {
        return map[key->hashKey()];
    }

    Value *getKeyByHash(unsigned int hashKey) const {
        for (int i = 0; i < keys.size(); i++) {
            Value *v = keys[i];
            if (v->hashKey() == hashKey) {
                return v;
            }
        }
        return NULL;
    }

    const char *stringify() const override {
        std::string str = "{";
        auto endIt = map.end();
        endIt--; // second last ele
        for (auto it = map.begin(); it != map.end(); it++) {
            unsigned int key = it->first;
            Value *val = it->second;
            str = str + getKeyByHash(key)->stringify() + ": " + val->stringify();
            if (it != endIt) {
                str = str + ", ";
            }
        }
        str += "}";
        char *tmp = (char *)malloc(str.size() + 1);
        strcpy(tmp, str.c_str());
        return tmp;
    }

    virtual ~MapValue() {
        std::map<unsigned int, Value*>::iterator it;
        for (it = map.begin(); it != map.end(); it++) {
            Value *val = it->second;
            delete val;
        }
    }

    const unsigned int hashKey() const override {
        return fnv(this->stringify());
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

    const char *stringify() const override {
        std::string errPrelude = "ERROR AT LINE " + std::to_string(this->lineNum);
        std::string err = errPrelude + ": " + this->error;
        const char *str = err.c_str();
        return str;
    }

    virtual ~ErrorValue() {
        free(error);
    }

    const unsigned int hashKey() const override {
        return fnv(this->stringify());
    }
};