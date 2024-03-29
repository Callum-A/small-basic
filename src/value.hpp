#pragma once

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

/// Enum containing all the different value types
/// so that value types can be identified prior
/// to casting.
enum ValueType {
    VAL_NUMBER,
    VAL_BOOL,
    VAL_STRING,
    VAL_LIST,
    VAL_MAP,
    VAL_ERROR
};

/// Helper hash function to hash values to an integer,
/// (used for maps).
static unsigned int fnv(const char *str) {
    const size_t length = strlen(str) + 1;
    unsigned int hash = 2166136261u;
    for (size_t i = 0; i < length; ++i) {
        hash ^= *str++;
        hash *= 16777619u;
    }
    return hash;
}

/// Abstract value class to be inheritted from.
/// Represents a value in Small Basic.
class Value {
public:
    ValueType type;

    Value(ValueType type) {
        this->type = type;
    }

    virtual void f() {}

    virtual const char *stringify() const { return ""; }

    virtual unsigned int hashKey() const {
        return 0;
    }

    virtual Value *copy() const {
        return NULL;
    }

    inline bool operator< (const Value& rhs) const {
        return this->hashKey() < rhs.hashKey();
    }

};

/// Class representing a number value in Small Basic.
/// Returns the raw double number.
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

    virtual Value *copy() const {
        return new NumberValue(this->number);
    }

    unsigned int hashKey() const override {
        return fnv(this->stringify());
    }
};

/// Class representing a boolean value in Small Basic.
/// Stores the raw bool value.
class BoolValue : public Value {
public:
    bool boolean;

    BoolValue(bool boolean) : Value(VAL_BOOL) {
        this->boolean = boolean;
    }

    virtual Value *copy() const {
        return new BoolValue(this->boolean);
    }

    const char *stringify() const override {
        if (boolean) {
            return "True";
        } else {
            return "False";
        }
    }

    unsigned int hashKey() const override {
        if (boolean) {
            return 1;
        } else {
            return 0;
        }
    }
};

/// Class representing a string value in Small Basic.
/// Stores the raw char* value.
class StringValue : public Value {
public:
    char *string;

    StringValue(const char *string) : Value(VAL_STRING) {
        this->string = (char *)malloc(strlen(string) + 1);
        strcpy(this->string, string);
    }

    virtual Value *copy() const {
        return new StringValue(this->string);
    }

    const char *stringify() const override {
        // May have to strcpy
        return string;
    }

    virtual ~StringValue() {
        free(string);
    }

    unsigned int hashKey() const override {
        return fnv(string);
    }
};

/// Class representing a list in Small Basic.
/// Stores a vector of values.
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

    unsigned int hashKey() const override {
        return fnv(this->stringify());
    }
};

/// Helper struct used to define how to determine keys are
/// equal for Value*
struct ValueMap {
    bool operator()(Value *lhs, Value *rhs) const {
        return lhs->hashKey() < rhs->hashKey();
    }
};

/// Class representing a map in SmallBasic
/// Contains a C++ map of values.
class MapValue : public Value {
public:
    std::map<Value*, Value*, ValueMap> map;

    MapValue() : Value(VAL_MAP) {}

    void addValue(Value *key, Value *val) {
        map[key->copy()] = val->copy();
    }

    void removeValue(Value *key) {
        auto it = map.find(key);
        map.erase(it);
    }

    Value *getValue(Value *key) {
        return map[key];
    }

    const char *stringify() const override {
        if (map.size() == 0) {
            return "{}";
        }
        std::string str = "{";
        auto endIt = map.end();
        endIt--; // second last ele
        
        for (auto it = map.begin(); it != map.end(); it++) {
            Value *key = it->first;
            Value *val = it->second;
            str = str + key->stringify() + ": " + val->stringify();
            if (it != endIt) {
                str += ", ";
            }
        }
        str += "}";
        char *tmp = (char *)malloc(str.size() + 1);
        strcpy(tmp, str.c_str());
        return tmp;
    }

    virtual ~MapValue() {
        for (auto it = map.begin(); it != map.end(); it++) {
            Value *key = it->first;
            Value *val = it->second;
            delete key;
            delete val;
        }
    }

    unsigned int hashKey() const override {
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

    unsigned int hashKey() const override {
        return fnv(this->stringify());
    }
};

bool isError(Value *v);