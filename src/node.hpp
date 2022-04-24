#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "value.hpp"

/// Enum containing every distinct node type
/// contained in the abstract node class
/// to safely identify node types prior
/// to casting
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
    NODE_MAP,
    NODE_INDEX_ASSIGN,
    NODE_INDEX,
    NODE_BUILTIN,
    NODE_EXPR
};

/// Abstract node class containg information needed for all nodes.
class Node {
public:
    NodeType type; // The type of node.
    char *token;   // Debug string helper to help identify nodes when printing.
    int lineNum;   // What line this node is on.

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

/// Node representing the whole program. Simply a list of
/// statements in order.
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

/// Node representing a number value,
/// simply contains the corresponding number value.
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

/// Node representing a boolean value,
/// simply contains the corresponding bool value.
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

/// Node representing a string value,
/// simply contains the corresponding string value.
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

/// Node representing an identifier,
/// contains the raw char* ident.
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

/// Node representing a print call.
/// Contains the expression to be printed.
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

/// Node for a binary operation.
/// Contains the operator, left and right.
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

/// Node for a unary operation.
/// Contains the operator and the right side.
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

/// Node for a var declaration, contains the
/// identifier node and the value node.
/// ident = value
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

/// Node for a var assignment contains the
/// identifier node and valye node.
/// ident = value
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

/// Node for a block of statements.
/// Contains a list of other nodes within the block.
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

/// Node for an if statement.
/// Contains the expression, then then branch
/// (executed when true) and the optional else branch
/// (executed when false but can be NULL)
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

/// Node for a while statement.
/// Contains the expression and the 
/// block node to be executed when it is true.
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

/// Node for a for statement.
/// Contains the identifier used,
/// the start value, the max value (stop condition),
/// the step (increment) and the block to be executed.
/// For Let ident = value To max Step step Do block
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

/// Node for subroutine definition.
/// Contains the subroutine's ident
/// and the block to be executed when called.
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

/// Node for a subroutine call
/// Stores the ident of the sub called.
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

/// Node representing a list of expressions.
/// Contains a list of other nodes.
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

/// Node representing a map.
/// Contains node key value pairs.
class MapNode : public Node {
public:
    std::map<Node*, Node*> exprs;
    MapNode(const char *token, int lineNum) : Node(NODE_MAP, token, lineNum) {}

    void addNode(Node *key, Node *val) {
        exprs[key] = val;
    }

    virtual ~MapNode() {
        std::map<Node*, Node*>::iterator it;
        for (it = exprs.begin(); it != exprs.end(); it++) {
            Node *key = it->first;
            Node *value = it->second;
            delete key;
            delete value;
        }
    }

};

/// Node for index assignment.
/// Contains the ident of the variable,
/// the index value and the value it is
/// being set to.
/// ident[index] = value
class IndexAssignNode : public Node {
public:
    Node *ident;
    Node *index;
    Node *value;

    IndexAssignNode(Node *ident, Node *index, Node *value, const char *token, int lineNum) : Node(NODE_INDEX_ASSIGN, token, lineNum) {
        this->ident = ident;
        this->index = index;
        this->value = value;
    }

    virtual ~IndexAssignNode() {
        delete ident;
        delete index;
        delete value;
    }

};

/// Node for an index retrieval.
/// ident[index]
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

/// Node representing a builtin standard library
/// function call. Contains the identifier for it
/// and the arguements it is called with.
class BuiltInNode : public Node {
public:
    Node *ident;
    Node *args;

    BuiltInNode(Node *ident, Node *args, const char *token, int lineNum) : Node(NODE_BUILTIN, token, lineNum) {
        this->ident = ident;
        this->args = args;
    }

    virtual ~BuiltInNode() {
        delete ident;
        delete args;
    }
};

/// Node representing an expression.
/// Contains the expression.
class ExprNode : public Node {
public:
    Node *expr;

    ExprNode(Node *expr, const char *token, int lineNum) : Node(NODE_EXPR, token, lineNum) {
        this->expr = expr;
    }

    virtual ~ExprNode() {
        delete expr;
    }
};
