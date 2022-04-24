#include "value.hpp"

/// Helper to check if a value is an error.
bool isError(Value *v) {
    if (v != NULL && v->type == VAL_ERROR) {
        return true;
    }

    return false;
}