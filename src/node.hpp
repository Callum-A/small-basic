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
    NODE_CALL
};

class Node {
public:
    NodeType type;
    char *token;

    Node(NodeType type, const char *token) {
        this->type = type;
        this->token = (char *)malloc(strlen(token) + 1);
        strcpy(this->token, token);
    }

    virtual void f() {}
};

class ProgramNode : public Node {
public:
    ProgramNode(const char *token) : Node(NODE_PROGRAM, token) {
    }

    void addNode(Node *node) { 
        stmts.push_back(node);
    }

    std::vector<Node*> *getStmts() { return &stmts; }
private:
    std::vector<Node*> stmts;
};

class NumberNode : public Node {
public:
    NumberValue *value;

    NumberNode(double value, const char *token) : Node(NODE_NUMBER, token) {
        this->value = new NumberValue(value);
    }
};

class BooleanNode : public Node {
public:
    BoolValue *value;

    BooleanNode(bool value, const char *token) : Node(NODE_BOOLEAN, token) {
        this->value = new BoolValue(value);
    }
};

class StringNode : public Node {
public:
    StringValue *value;

    StringNode(char *value, const char *token) : Node(NODE_STRING, token) {
        this->value = new StringValue(value);
    }
};

class IdentifierNode : public Node {
public:
    char *ident;

    IdentifierNode(char *ident, const char *token) : Node(NODE_IDENTIFIER, token) {
        this->ident = (char *)malloc(strlen(ident) + 1);
        strcpy(this->ident, ident);
    }
};

class PrintNode : public Node {
public:
    Node *exp;

    PrintNode(Node *exp, const char *token) : Node(NODE_PRINT, token) {
        this->exp = exp;
    }
};

class BinaryOpNode : public Node {
public:
    char op;
    Node *left;
    Node *right;

    BinaryOpNode(Node *left, Node *right, char op, const char *token) : Node(NODE_BINARY_OP, token) {
        this->op = op;
        this->left = left;
        this->right = right;
    }
};

class UnaryOpNode : public Node {
public:
    char op;
    Node *right;

    UnaryOpNode(Node *right, char op, const char *token) : Node(NODE_UNARY_OP, token) {
        this->op = op;
        this->right = right;
    }
};

class VarDeclNode : public Node {
public:
    Node *ident;
    Node *value;

    VarDeclNode(Node *ident, Node *value, const char *token) : Node(NODE_VAR_DECL, token) {
        this->ident = ident;
        this->value = value;
    }
};

class VarAssignNode : public Node {
public:
    Node *ident;
    Node *value;

    VarAssignNode(Node *ident, Node *value, const char *token) : Node(NODE_VAR_ASSIGN, token) {
        this->ident = ident;
        this->value = value;
    }
};

class BlockNode : public Node {
public:
    BlockNode(const char *token) : Node(NODE_BLOCK, token) {}

    void addNode(Node *node) { 
        stmts.push_back(node);
    }

    std::vector<Node*> *getStmts() { return &stmts; }
private:
    std::vector<Node*> stmts;
};

class IfNode : public Node {
public:
    Node *expr;
    Node *thenBranch;
    Node *elseBranch;

    IfNode(Node *expr, Node *thenBranch, Node *elseBranch, const char *token) : Node(NODE_IF, token) {
        this->expr = expr;
        this->thenBranch = thenBranch;
        this->elseBranch = elseBranch;
    }
};

class WhileNode : public Node {
public:
    Node *expr;
    Node *block;

    WhileNode(Node *expr, Node *block, const char *token) : Node(NODE_WHILE, token) {
        this->expr = expr;
        this->block = block;
    }
};

class ForNode : public Node {
public:
    Node *ident;
    Node *value;
    Node *max;
    Node *step;
    Node *block;

    ForNode(Node *ident, Node *value, Node *max, Node *step, Node *block, const char *token) : Node(NODE_FOR, token) {
        this->ident = ident;
        this->value = value;
        this->max = max;
        this->step = step;
        this->block = block;
    }
};

class SubNode : public Node {
public:
    Node *ident;
    Node *block;

    SubNode(Node *ident, Node *block, const char *token) : Node(NODE_SUB, token) {
        this->ident = ident;
        this->block = block;
    }
};

class CallNode : public Node {
public:
    Node *ident;

    CallNode(Node *ident, const char *token) : Node(NODE_CALL, token) {
        this->ident = ident;
    }
};
