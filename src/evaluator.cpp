#include "evaluator.hpp"

extern std::map<std::string, Value*> env;
// TODO: add funcs as a value type
std::map<std::string, SubNode*> funcs;

Value *evProgram(ProgramNode *program);
Value *evPrint(PrintNode *print);
Value *evBinaryOp(BinaryOpNode *binaryOp);
Value *evUnaryOp(UnaryOpNode *unaryOp);
Value *evVarDecl(VarDeclNode *varDecl);
Value *evVarAssign(VarAssignNode *varAssign);
Value *evIdentifier(IdentifierNode *identifier);
Value *evIf(IfNode *ifNode);
Value *evBlock(BlockNode *block);
Value *evWhile(WhileNode *whileNode);
Value *evFor(ForNode *forNode);
Value *evSub(SubNode *subNode);
Value *evCall(CallNode *callNode);

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
        case NODE_VAR_DECL:
            return evVarDecl(dynamic_cast<VarDeclNode*>(root));
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
        default:
            std::cout << "UNRECOGNISED NODE" << std::endl;
            break;
    }
    return NULL;
}

Value *evProgram(ProgramNode *program) {
    std::vector<Node*> *stmts = program->getStmts();
    for (int i = 0; i < stmts->size(); i++) {
        Node *node = (*stmts)[i];
        Value *curr = ev(node);
        if (curr != NULL && curr->type == VAL_ERROR) {
            return curr;
        }
    }
    return NULL;
}

Value *evPrint(PrintNode *print) {
    Value *val = ev(print->exp);
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
        case 'O':
            return new BoolValue(isTruthy(left) || isTruthy(right));
        default:
            return new ErrorValue(binaryOp->lineNum, "Unsupported operator between numbers!");
    }
}

Value *evBinaryOp(BinaryOpNode *binaryOp) {
    Value *left = ev(binaryOp->left);
    Value *right = ev(binaryOp->right);

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

Value *evVarDecl(VarDeclNode *varDecl) {
    std::string ident = dynamic_cast<IdentifierNode*>(varDecl->ident)->ident;
    Value *v = ev(varDecl->value);
    if (v != NULL && v->type == VAL_ERROR) {
        return v;
    }
    // TODO: check if ident already exists and error
    env[ident] = v;
    return NULL;
}

Value *evVarAssign(VarAssignNode *varAssign) {
    std::string ident = dynamic_cast<IdentifierNode*>(varAssign->ident)->ident;
    // TODO: check if var exists
    Value *v = ev(varAssign->value);
    if (v != NULL && v->type == VAL_ERROR) {
        return v;
    }
    env[ident] = v;
    
    return NULL;
}

Value *evIdentifier(IdentifierNode *identifier) {
    std::string ident = identifier->ident;
    // TODO: error if var does not exist
    Value *v = env[ident];
    return v;
}

Value *evIf(IfNode *ifNode) {
    if (isTruthy(ev(ifNode->expr))) {
        ev(ifNode->thenBranch);
    } else {
        if (ifNode->elseBranch != NULL) {
            ev(ifNode->elseBranch);
        }
    }
    return NULL;
}

Value *evBlock(BlockNode *block) {
    std::vector<Node*> *stmts = block->getStmts();
    for (int i = 0; i < stmts->size(); i++) {
        Node *stmt = (*stmts)[i];
        ev(stmt);
    }
    return NULL;
}

Value *evWhile(WhileNode *whileNode) {
    while(isTruthy(ev(whileNode->expr))) {
        ev(whileNode->block);
    }
    return NULL;
}

Value *evFor(ForNode *forNode) {
    IdentifierNode *identNode = dynamic_cast<IdentifierNode*>(forNode->ident);
    std::string ident = identNode->ident;
    // TODO: check types
    Value *v = ev(forNode->value);
    env[ident] = v;
    Value *max = ev(forNode->max);
    NumberValue *v2 = dynamic_cast<NumberValue*>(v);
    NumberValue *max2 = dynamic_cast<NumberValue*>(max);
    if (forNode->step != NULL) {
        Value *step = ev(forNode->step);
        NumberValue *step2 = dynamic_cast<NumberValue*>(step);
        for (v2->number; v2->number < max2->number; v2->number = v2->number + step2->number) {
            ev(forNode->block);
        }
    } else {
        for (v2->number; v2->number < max2->number; v2->number++) {
            ev(forNode->block);
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