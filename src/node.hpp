#pragma once

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "value.hpp"

enum NodeType {
    NODE_PROGRAM,
    NODE_NUMBER,
    NODE_BOOLEAN,
    NODE_STRING,
    NODE_IDENTIFIER,
    NODE_PRINT,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_VAR_DECL,
    NODE_VAR_ASSIGN,
    NODE_BLOCK,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_SUB,
    NODE_CALL,
    NODE_EXPR_LIST,
    NODE_INDEX
};

class Node {
public:
    NodeType type;
    char *token;
    int lineNum;

    Node(NodeType type, const char *token, int lineNum) {
        this->type = type;
        this->token = (char *)malloc(strlen(token) + 1);
        strcpy(this->token, token);
        this->lineNum = lineNum;
    }

    virtual ~Node() {
        free(token);
    }

    virtual void f() {}
};

class ProgramNode : public Node {
public:
    ProgramNode(const char *token) : Node(NODE_PROGRAM, token, 0) {
    }

    void addNode(Node *node) { 
        stmts.push_back(node);
    }

    std::vector<Node*> *getStmts() { return &stmts; }

    virtual ~ProgramNode() {
        for (int i = 0; i < stmts.size(); i++) {
            Node *stmt = stmts[i];
            delete stmt;
        }
    }
private:
    std::vector<Node*> stmts;
};

class NumberNode : public Node {
public:
    NumberValue *value;

    NumberNode(double value, const char *token, int lineNum) : Node(NODE_NUMBER, token, lineNum) {
        this->value = new NumberValue(value);
    }

    virtual ~NumberNode() {
        delete value;
    }
};

class BooleanNode : public Node {
public:
    BoolValue *value;

    BooleanNode(bool value, const char *token, int lineNum) : Node(NODE_BOOLEAN, token, lineNum) {
        this->value = new BoolValue(value);
    }

    virtual ~BooleanNode() {
        delete value;
    }
};

class StringNode : public Node {
public:
    StringValue *value;

    StringNode(char *value, const char *token, int lineNum) : Node(NODE_STRING, token, lineNum) {
        this->value = new StringValue(value);
    }

    virtual ~StringNode() {
        delete value;
    }
};

class IdentifierNode : public Node {
public:
    char *ident;

    IdentifierNode(char *ident, const char *token, int lineNum) : Node(NODE_IDENTIFIER, token, lineNum) {
        this->ident = (char *)malloc(strlen(ident) + 1);
        strcpy(this->ident, ident);
    }

    virtual ~IdentifierNode() {
        free(ident);
    }
};

class PrintNode : public Node {
public:
    Node *exp;

    PrintNode(Node *exp, const char *token, int lineNum) : Node(NODE_PRINT, token, lineNum) {
        this->exp = exp;
    }

    virtual ~PrintNode() {
        delete exp;
    }
};

class BinaryOpNode : public Node {
public:
    char op;
    Node *left;
    Node *right;

    BinaryOpNode(Node *left, Node *right, char op, const char *token, int lineNum) : Node(NODE_BINARY_OP, token, lineNum) {
        this->op = op;
        this->left = left;
        this->right = right;
    }

    virtual ~BinaryOpNode() {
        delete left;
        delete right;
    }
};

class UnaryOpNode : public Node {
public:
    char op;
    Node *right;

    UnaryOpNode(Node *right, char op, const char *token, int lineNum) : Node(NODE_UNARY_OP, token, lineNum) {
        this->op = op;
        this->right = right;
    }

    virtual ~UnaryOpNode() {
        delete right;
    }
};

class VarDeclNode : public Node {
public:
    Node *ident;
    Node *value;

    VarDeclNode(Node *ident, Node *value, const char *token, int lineNum) : Node(NODE_VAR_DECL, token, lineNum) {
        this->ident = ident;
        this->value = value;
    }

    virtual ~VarDeclNode() {
        delete ident;
        delete value;
    }
};

class VarAssignNode : public Node {
public:
    Node *ident;
    Node *value;

    VarAssignNode(Node *ident, Node *value, const char *token, int lineNum) : Node(NODE_VAR_ASSIGN, token, lineNum) {
        this->ident = ident;
        this->value = value;
    }

    virtual ~VarAssignNode() {
        delete ident;
        delete value;
    }
};

class BlockNode : public Node {
public:
    BlockNode(const char *token, int lineNum) : Node(NODE_BLOCK, token, lineNum) {}

    void addNode(Node *node) { 
        stmts.push_back(node);
    }

    std::vector<Node*> *getStmts() { return &stmts; }

    virtual ~BlockNode() {
        for (int i = 0; i < stmts.size(); i++) {
            Node *stmt = stmts[i];
            delete stmt;
        }
    }

private:
    std::vector<Node*> stmts;
};

class IfNode : public Node {
public:
    Node *expr;
    Node *thenBranch;
    Node *elseBranch;

    IfNode(Node *expr, Node *thenBranch, Node *elseBranch, const char *token, int lineNum) : Node(NODE_IF, token, lineNum) {
        this->expr = expr;
        this->thenBranch = thenBranch;
        this->elseBranch = elseBranch;
    }

    virtual ~IfNode() {
        delete expr;
        delete thenBranch;
        delete elseBranch;
    }
};

class WhileNode : public Node {
public:
    Node *expr;
    Node *block;

    WhileNode(Node *expr, Node *block, const char *token, int lineNum) : Node(NODE_WHILE, token, lineNum) {
        this->expr = expr;
        this->block = block;
    }

    virtual ~WhileNode() {
        delete expr;
        delete block;
    }
};

class ForNode : public Node {
public:
    Node *ident;
    Node *value;
    Node *max;
    Node *step;
    Node *block;

    ForNode(Node *ident, Node *value, Node *max, Node *step, Node *block, const char *token, int lineNum) : Node(NODE_FOR, token, lineNum) {
        this->ident = ident;
        this->value = value;
        this->max = max;
        this->step = step;
        this->block = block;
    }

    virtual ~ForNode() {
        delete ident;
        delete value;
        delete max;
        delete step;
        delete block;
    }

};

class SubNode : public Node {
public:
    Node *ident;
    Node *block;

    SubNode(Node *ident, Node *block, const char *token, int lineNum) : Node(NODE_SUB, token, lineNum) {
        this->ident = ident;
        this->block = block;
    }

    virtual ~SubNode() {
        delete ident;
        delete block;
    }
};

class CallNode : public Node {
public:
    Node *ident;

    CallNode(Node *ident, const char *token, int lineNum) : Node(NODE_CALL, token, lineNum) {
        this->ident = ident;
    }

    virtual ~CallNode() {
        delete ident;
    }
};

class ExprListNode : public Node {
public:
    std::vector<Node*> exprs;

    ExprListNode(const char *token, int lineNum) : Node(NODE_EXPR_LIST, token, lineNum) {}

    void addNode(Node *expr) {
        exprs.push_back(expr);
    }

    virtual ~ExprListNode() {
        for (int i = 0; i < exprs.size(); i++) {
            Node *expr = exprs[i];
            delete expr;
        }
    }

};

class IndexNode : public Node {
public:
    Node *ident;
    Node *index;

    IndexNode(Node *ident, Node *index, const char *token, int lineNum) : Node(NODE_INDEX, token , lineNum) {
        this->ident = ident;
        this->index = index;
    }

    virtual ~IndexNode() {
        delete ident;
        delete index;
    }

};
