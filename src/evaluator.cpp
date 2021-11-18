#include "evaluator.hpp"
#include "builtin.hpp"

extern std::map<std::string, Value*> env;
// TODO: add funcs as a value type
std::map<std::string, SubNode*> funcs;
std::map<std::string, Builtin*> builtins;

Value *evProgram(ProgramNode *program);
Value *evPrint(PrintNode *print);
Value *evBinaryOp(BinaryOpNode *binaryOp);
Value *evUnaryOp(UnaryOpNode *unaryOp);
Value *evVarAssign(VarAssignNode *varAssign);
Value *evIdentifier(IdentifierNode *identifier);
Value *evIf(IfNode *ifNode);
Value *evBlock(BlockNode *block);
Value *evWhile(WhileNode *whileNode);
Value *evFor(ForNode *forNode);
Value *evSub(SubNode *subNode);
Value *evCall(CallNode *callNode);
Value *evExprList(ExprListNode *listNode);
Value *evMap(MapNode *map);
Value *evIndex(IndexNode *idx);
Value *evIndexAssign(IndexAssignNode *idx);
Value *evBuiltin(BuiltInNode *b);

bool isError(Value *v) {
    if (v != NULL && v->type == VAL_ERROR) {
        return true;
    }

    return false;
}

void registerBuiltins() {
    builtins["random"] = new Random();
}

Value *ev(Node *root) {
    switch (root->type) {
        case NODE_PROGRAM:
            return evProgram(dynamic_cast<ProgramNode*>(root));
        case NODE_NUMBER:
            return (dynamic_cast<NumberNode*>(root))->value;
        case NODE_BOOLEAN:
            return (dynamic_cast<BooleanNode*>(root))->value;
        case NODE_STRING:
            return (dynamic_cast<StringNode*>(root))->value;
        case NODE_BINARY_OP:
            return evBinaryOp(dynamic_cast<BinaryOpNode*>(root));
        case NODE_UNARY_OP:
            return evUnaryOp(dynamic_cast<UnaryOpNode*>(root));
        case NODE_VAR_ASSIGN:
            return evVarAssign(dynamic_cast<VarAssignNode*>(root));
        case NODE_IDENTIFIER:
            return evIdentifier(dynamic_cast<IdentifierNode*>(root));
        case NODE_PRINT:
            return evPrint(dynamic_cast<PrintNode*>(root));
        case NODE_IF:
            return evIf(dynamic_cast<IfNode*>(root));
        case NODE_WHILE:
            return evWhile(dynamic_cast<WhileNode*>(root));
        case NODE_BLOCK:
            return evBlock(dynamic_cast<BlockNode*>(root));
        case NODE_FOR:
            return evFor(dynamic_cast<ForNode*>(root));
        case NODE_SUB:
            return evSub(dynamic_cast<SubNode*>(root));
        case NODE_CALL:
            return evCall(dynamic_cast<CallNode*>(root));
        case NODE_EXPR_LIST:
            return evExprList(dynamic_cast<ExprListNode*>(root));
        case NODE_MAP:
            return evMap(dynamic_cast<MapNode*>(root));
        case NODE_INDEX:
            return evIndex(dynamic_cast<IndexNode*>(root));
        case NODE_INDEX_ASSIGN:
            return evIndexAssign(dynamic_cast<IndexAssignNode*>(root));
        case NODE_BUILTIN:
            return evBuiltin(dynamic_cast<BuiltInNode*>(root));
        default:
            return new ErrorValue(root->lineNum, "Unrecognised node type!");
    }
    return NULL;
}

Value *evProgram(ProgramNode *program) {
    std::vector<Node*> *stmts = program->getStmts();
    for (int i = 0; i < stmts->size(); i++) {
        Node *node = (*stmts)[i];
        Value *curr = ev(node);
        if (isError(curr)) {
            return curr;
        }
    }
    return NULL;
}

Value *evPrint(PrintNode *print) {
    Value *val = ev(print->exp);
    if (isError(val)) {
        return val;
    }
    std::cout << val->stringify() << std::endl;
    return NULL;
}

bool isEqual(Value *left, Value *right) {
    if (left->type != right->type) {
        return false;
    }

    switch (left->type) {
        case VAL_BOOL:
            return dynamic_cast<BoolValue*>(left)->boolean == dynamic_cast<BoolValue*>(right)->boolean;
        case VAL_STRING:
            return (strcmp(dynamic_cast<StringValue*>(left)->string, dynamic_cast<StringValue*>(right)->string)) == 0;
        case VAL_NUMBER:
            return dynamic_cast<NumberValue*>(left)->number == dynamic_cast<NumberValue*>(right)->number;
        default:
            // Unreachable
            break;
    }
    return false;
}

bool isTruthy(Value *v) {
    if (v->type == VAL_BOOL) {
        return dynamic_cast<BoolValue*>(v)->boolean;
    }
    // All other types truthy as we dont have a null
    return true;
}

Value *evStringBinaryOp(BinaryOpNode *binaryOp, Value *left, Value *right) {
    if (right->type != VAL_STRING) {
        return new ErrorValue(binaryOp->lineNum, "Expected string for right operand as left is string.");
    }
    StringValue *strLeft = dynamic_cast<StringValue*>(left);
    StringValue *strRight = dynamic_cast<StringValue*>(right);
    char *newStr = (char *)malloc(strlen(strLeft->string) + strlen(strRight->string) + 1);
    strcpy(newStr, strLeft->string);
    strcat(newStr, strRight->string);
    switch (binaryOp->op) {
        case '+':
            return new StringValue(newStr);
        case 'E': // ==
            return new BoolValue(isEqual(left, right));
        case 'A': // and
            return new BoolValue(isTruthy(left) && isTruthy(right));
        case 'O':
            return new BoolValue(isTruthy(left) || isTruthy(right));
        default:
            return new ErrorValue(binaryOp->lineNum, "Unsupported operator between strings!");
    }
}

Value *evNumberBinaryOp(BinaryOpNode *binaryOp, Value *left, Value *right) {
    if (right->type != VAL_NUMBER) {
        return new ErrorValue(binaryOp->lineNum, "Expected number for right operand as left is number.");
    }

    switch (binaryOp->op) {
        case '+':
            return new NumberValue(dynamic_cast<NumberValue*>(left)->number + dynamic_cast<NumberValue*>(right)->number);
        case '-':
            return new NumberValue(dynamic_cast<NumberValue*>(left)->number - dynamic_cast<NumberValue*>(right)->number);
        case '*':
            return new NumberValue(dynamic_cast<NumberValue*>(left)->number * dynamic_cast<NumberValue*>(right)->number);
        case '/':
            return new NumberValue(dynamic_cast<NumberValue*>(left)->number / dynamic_cast<NumberValue*>(right)->number);
        case '<':
            return new BoolValue(dynamic_cast<NumberValue*>(left)->number < dynamic_cast<NumberValue*>(right)->number);
        case '>':
            return new BoolValue(dynamic_cast<NumberValue*>(left)->number > dynamic_cast<NumberValue*>(right)->number);
        case 'L': // <=
            return new BoolValue(dynamic_cast<NumberValue*>(left)->number <= dynamic_cast<NumberValue*>(right)->number);
        case 'G': // >=
            return new BoolValue(dynamic_cast<NumberValue*>(left)->number >= dynamic_cast<NumberValue*>(right)->number);
        case 'E': // ==
            return new BoolValue(isEqual(left, right));
        case 'A': // and
            return new BoolValue(isTruthy(left) && isTruthy(right));
        case 'O': // or
            return new BoolValue(isTruthy(left) || isTruthy(right));
        default:
            return new ErrorValue(binaryOp->lineNum, "Unsupported operator between numbers!");
    }
}

Value *evBinaryOp(BinaryOpNode *binaryOp) {
    Value *left = ev(binaryOp->left);
    Value *right = ev(binaryOp->right);

    if (isError(left)) {
        return left;
    }

    if (isError(right)) {
        return right;
    }

    if (left->type == VAL_STRING) {
        return evStringBinaryOp(binaryOp, left, right);
    } else if (left->type == VAL_NUMBER) {
        return evNumberBinaryOp(binaryOp, left, right);
    } else {
        switch (binaryOp->op) {
            case 'E': // ==
                return new BoolValue(isEqual(left, right));
            case 'A': // and
                return new BoolValue(isTruthy(left) && isTruthy(right));
            case 'O':
                return new BoolValue(isTruthy(left) || isTruthy(right));
            default:
                return new ErrorValue(binaryOp->lineNum, "Unrecognised binary operator!");
        }
    }
}

Value *evUnaryOp(UnaryOpNode *unaryOp) {
    Value *right = ev(unaryOp->right);
    if (right->type != VAL_NUMBER) {
        return new ErrorValue(unaryOp->lineNum, "Unary operators only support numbers!");
    }

    switch (unaryOp->op) {
        case '-':
            return new NumberValue(-(dynamic_cast<NumberValue*>(right)->number));
        default:
            return new ErrorValue(unaryOp->lineNum, "Unrecognised unary operator!");
    }

    return NULL;
}

Value *evVarAssign(VarAssignNode *varAssign) {
    std::string ident = dynamic_cast<IdentifierNode*>(varAssign->ident)->ident;
    Value *v = ev(varAssign->value);
    if (isError(v)) {
        return v;
    }
    env[ident] = v;
    
    return NULL;
}

Value *evIdentifier(IdentifierNode *identifier) {
    std::string ident = identifier->ident;
    if (env.find(ident) == env.end()) {
        return new ErrorValue(identifier->lineNum, "Unrecognised variable!");
    }
    Value *v = env[ident];
    return v;
}

Value *evIf(IfNode *ifNode) {
    Value *v = NULL;
    if (isTruthy(ev(ifNode->expr))) {
        v = ev(ifNode->thenBranch);
    } else {
        if (ifNode->elseBranch != NULL) {
            v = ev(ifNode->elseBranch);
        }
    }

    if (isError(v)) {
        return v;
    }

    return NULL;
}

Value *evBlock(BlockNode *block) {
    std::vector<Node*> *stmts = block->getStmts();
    for (int i = 0; i < stmts->size(); i++) {
        Node *stmt = (*stmts)[i];
        Value *v = ev(stmt);
        if (isError(v)) {
            return v;
        }
    }
    return NULL;
}

Value *evWhile(WhileNode *whileNode) {
    while(isTruthy(ev(whileNode->expr))) {
        Value *v = ev(whileNode->block);
        if (isError(v)) {
            return v;
        }
    }
    return NULL;
}

Value *evFor(ForNode *forNode) {
    IdentifierNode *identNode = dynamic_cast<IdentifierNode*>(forNode->ident);
    std::string ident = identNode->ident;
    Value *v = ev(forNode->value);
    if (v == NULL || v->type != VAL_NUMBER) {
        return new ErrorValue(forNode->lineNum, "For initialiser must be a number!");
    }
    env[ident] = v;
    Value *max = ev(forNode->max);
    if (max == NULL || max->type != VAL_NUMBER) {
        return new ErrorValue(forNode->lineNum, "For maximum must be a number!");
    }
    NumberValue *v2 = dynamic_cast<NumberValue*>(v);
    NumberValue *max2 = dynamic_cast<NumberValue*>(max);
    if (forNode->step != NULL) {
        Value *step = ev(forNode->step);
        if (step == NULL || step->type != VAL_NUMBER) {
            return new ErrorValue(forNode->lineNum, "For step must be a number!");
        }
        NumberValue *step2 = dynamic_cast<NumberValue*>(step);
        for (v2->number; v2->number < max2->number; v2->number = v2->number + step2->number) {
            Value *v = ev(forNode->block);
            if (isError(v)) {
                return v;
            }
        }
    } else {
        for (v2->number; v2->number < max2->number; v2->number++) {
            Value *v = ev(forNode->block);
            if (isError(v)) {
                return v;
            }
        }
    }

    return NULL;
}

Value *evSub(SubNode *subNode) {
    IdentifierNode *identNode = dynamic_cast<IdentifierNode*>(subNode->ident);
    std::string ident = identNode->ident;
    funcs[ident] = subNode;
    return NULL;
}

Value *evCall(CallNode *callNode) {
    IdentifierNode *identNode = dynamic_cast<IdentifierNode*>(callNode->ident);
    std::string ident = identNode->ident;
    SubNode *func = funcs[ident];
    return ev(func->block);
}

Value *evExprList(ExprListNode *listNode) {
    ListValue *v = new ListValue();
    for (int i = 0; i < listNode->exprs.size(); i++) {
        Node *expr = listNode->exprs[i];
        Value *eved = ev(expr);
        // TODO: check if error or null
        v->addValue(eved);
    }

    return v;
}

Value *evMap(MapNode *map) {
    MapValue *mapVal = new MapValue();
    std::map<Node*, Node*> m = map->exprs;
    for (auto it = m.begin(); it != m.end(); it++) {
        Node *key = it->first;
        Node *val = it->second;
        Value *k = ev(key); // TODO: check these
        Value *v = ev(val); // TODO: check these
        mapVal->addValue(k, v);
    }
    return mapVal;
}

Value *evIndex(IndexNode *idx) {
    // TODO: check if this is ident
    IdentifierNode *identNode = dynamic_cast<IdentifierNode*>(idx->ident);
    std::string ident = identNode->ident;
    Value *v = env[ident];
    if (v->type == VAL_LIST) {
        ListValue *v2 = dynamic_cast<ListValue*>(v);
        Value *i = ev(idx->index);
        // TODO:  only number for list
        NumberValue *i2 = dynamic_cast<NumberValue*>(i);
        return v2->values[int(i2->number)];
    } else if (v->type == VAL_MAP) {
        MapValue *v2 = dynamic_cast<MapValue*>(v);
        Value *i = ev(idx->index);
        return v2->getValue(i);
    } else {
        return new ErrorValue(idx->lineNum, "This identifier cannot be indexed!");
    }
}

Value *evIndexAssign(IndexAssignNode *idx) {
    // TODO: check if this is ident
    IdentifierNode *identNode = dynamic_cast<IdentifierNode*>(idx->ident);
    std::string ident = identNode->ident;
    Value *indexable = env[ident];
    if (indexable->type == VAL_LIST) {
        ListValue *v = dynamic_cast<ListValue*>(indexable);
        Value *i = ev(idx->index);
        // TODO: check if number
        NumberValue *i2 = dynamic_cast<NumberValue*>(i);
        Value *value = ev(idx->value);
        // TODO: check value type
        int finalIndex = i2->number;
        if (finalIndex >= v->values.size() || finalIndex < 0) {
            return new ErrorValue(idx->lineNum, "Cannot index outside bounds of list, use append instead!");
        }
        v->values[finalIndex] = value;
        return NULL;
    } else if (indexable->type == VAL_MAP) {
        MapValue *v = dynamic_cast<MapValue*>(indexable);
        Value *i = ev(idx->index);
        Value *value = ev(idx->value);
        // TODO: Check both types
        v->addValue(i, value);
        return NULL;
    } else {
        return new ErrorValue(idx->lineNum, "This identifier cannot be indexed!");
    }
}

Value *evBuiltin(BuiltInNode *b) {
    IdentifierNode *identNode = dynamic_cast<IdentifierNode*>(b->ident);
    ExprListNode *args = dynamic_cast<ExprListNode*>(b->args);
    std::vector<Value*> valueArgs;
    for (int i = 0; i < args->exprs.size(); i++) {
        Node *expr = args->exprs[i];
        Value *v = ev(expr);
        if (v == NULL) {
            return new ErrorValue(b->lineNum, "Cannot have a statement as an arguement!");
        }
        if (isError(v)) {
            return v;
        }
        valueArgs.push_back(v);
    }
    std::string ident = identNode->ident;
    Builtin *func = builtins[ident];
    return func->execute(&valueArgs);
}