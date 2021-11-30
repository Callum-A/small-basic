#include "value.hpp"

bool isError(Value *v) {
    if (v != NULL && v->type == VAL_ERROR) {
        return true;
    }

    return false;
}