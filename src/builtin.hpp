#pragma once
#include "value.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <random>
#include <fstream>

class Builtin {
public:
    Builtin() {}
    virtual Value *execute(int lineNum, std::vector<Value*> *args) { return NULL; };
};

class ReadLine : public Builtin {
public:
    ReadLine() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 0) {
            return new ErrorValue(lineNum, "Expected 0 arguments when calling input!");
        }
        std::string line;
        std::getline(std::cin, line);
        char *tmp = (char *)malloc(line.size() + 1);
        strcpy(tmp, line.c_str());
        return new StringValue(tmp);
    }
};

class Random : public Builtin {
public:
    Random() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 2) {
            return new ErrorValue(lineNum, "Expected 2 arguments when calling random!");
        }
        Value *mi = (*args)[0];
        Value *ma = (*args)[1];
        if (mi->type != VAL_NUMBER || ma->type != VAL_NUMBER) {
            return new ErrorValue(lineNum, "Expected 2 number values for min and max!");
        }
        NumberValue *min = dynamic_cast<NumberValue*>(mi);
        NumberValue *max = dynamic_cast<NumberValue*>(ma);
        double f = sqrt((double)rand() / RAND_MAX);
        return new NumberValue(min->number + f * (max->number - min->number));
    }
};

class Floor : public Builtin {
public:
    Floor() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 1) {
            return new ErrorValue(lineNum, "Expected 1 arguments when calling floor!");
        }
        Value *num = (*args)[0];
        if (num->type != VAL_NUMBER) {
            return new ErrorValue(lineNum, "Expected 1 number value!");
        }
        NumberValue *number = dynamic_cast<NumberValue*>(num);
        double floored = std::floor(number->number);
        return new NumberValue(floored);
    }
};

class Ceil : public Builtin {
public:
    Ceil() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 1) {
            return new ErrorValue(lineNum, "Expected 1 arguments when calling ceil!");
        }
        Value *num = (*args)[0];
        if (num->type != VAL_NUMBER) {
            return new ErrorValue(lineNum, "Expected 1 number value!");
        }
        NumberValue *number = dynamic_cast<NumberValue*>(num);
        double ceiled = std::ceil(number->number);
        return new NumberValue(ceiled);
    }
};

class Pi : public Builtin {
public:
    Pi() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 0) {
            return new ErrorValue(lineNum, "Expected 0 arguments when calling pi!");
        }
        return new NumberValue(3.14159);
    }
};

class Sqrt : public Builtin {
public:
    Sqrt() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 1) {
            return new ErrorValue(lineNum, "Expected 1 arguments when calling sqrt!");
        }
        Value *num = (*args)[0];
        if (num->type != VAL_NUMBER) {
            return new ErrorValue(lineNum, "Expected 1 number value!");
        }
        NumberValue *number = dynamic_cast<NumberValue*>(num);
        double rooted = std::sqrt(number->number);
        return new NumberValue(rooted);
    }
};

class Cos : public Builtin {
public:
    Cos() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 1) {
            return new ErrorValue(lineNum, "Expected 1 arguments when calling cos!");
        }
        Value *num = (*args)[0];
        if (num->type != VAL_NUMBER) {
            return new ErrorValue(lineNum, "Expected 1 number value!");
        }
        NumberValue *number = dynamic_cast<NumberValue*>(num);
        double cossed = std::cos(number->number);
        return new NumberValue(cossed);
    }
};

class Sin : public Builtin {
public:
    Sin() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 1) {
            return new ErrorValue(lineNum, "Expected 1 arguments when calling sin!");
        }
        Value *num = (*args)[0];
        if (num->type != VAL_NUMBER) {
            return new ErrorValue(lineNum, "Expected 1 number value!");
        }
        NumberValue *number = dynamic_cast<NumberValue*>(num);
        double sinned = std::sin(number->number);
        return new NumberValue(sinned);
    }
};

class Tan : public Builtin {
public:
    Tan() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 1) {
            return new ErrorValue(lineNum, "Expected 1 arguments when calling tan!");
        }
        Value *num = (*args)[0];
        if (num->type != VAL_NUMBER) {
            return new ErrorValue(lineNum, "Expected 1 number value!");
        }
        NumberValue *number = dynamic_cast<NumberValue*>(num);
        double tanned = std::tan(number->number);
        return new NumberValue(tanned);
    }
};

class ReadFile : public Builtin {
public:
    ReadFile() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 1) {
            return new ErrorValue(lineNum, "Expected 1 argument when calling read file!");
        }
        Value *path = (*args)[0];
        if (path->type != VAL_STRING) {
            return new ErrorValue(lineNum, "Expect file path to be a string!");
        }
        StringValue *filePath = dynamic_cast<StringValue*>(path);

        std::ifstream file(filePath->string);
        ListValue *lines = new ListValue();
        if (file.is_open()) {
            std::string line;
                while (std::getline(file, line)) {
                    // Read each line into a string value and add to list
                    lines->addValue(new StringValue(line.c_str()));
                }
            file.close();
        } else {
            return new ErrorValue(lineNum, "Could not find file with specified path!");
        }

        return lines;
    }
};

class Len : public Builtin {
public:
    Len() {}
    Value *execute(int lineNum, std::vector<Value*> *args) {
        if (args->size() != 1) {
            return new ErrorValue(lineNum, "Expected 1 argument when calling read file!");
        }
        Value *structure = (*args)[0];
        if (structure->type == VAL_LIST) {
            ListValue *list = dynamic_cast<ListValue*>(structure);
            return new NumberValue(list->values.size());
        } else if (structure->type == VAL_MAP) {
            MapValue *map = dynamic_cast<MapValue*>(structure);
            return new NumberValue(map->map.size());
        } else if (structure->type == VAL_STRING) {
            StringValue *string = dynamic_cast<StringValue*>(structure);
            return new NumberValue(strlen(string->string));
        } else {
            return new ErrorValue(lineNum, "Cannot compute len of given type!");
        }
    }
};
