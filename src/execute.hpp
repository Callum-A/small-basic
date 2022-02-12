#pragma once
#include "node.hpp"
#include "value.hpp"
#include "evaluator.hpp"
#include <map>
#include <iostream>
#include <algorithm>
#include <vector>

void execute(ProgramNode *prog, bool outputSymbolTable);