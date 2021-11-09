%{
#include "node.hpp"
int yylex();
int yyerror(char *s);
int lines = 1;
#include <cstdlib>
#include <cstdio>
#include <cstring>

extern Node *root;
%}
%union {
    Node *node;
    double number;
    int boolean;
    char *string;
}
%start program
%token NUMBER STRING TRUE FALSE
%token VAR IDENT
%token PLUS MINUS TIMES DIVIDE EQUALS OR AND
%token LEFT_PAREN RIGHT_PAREN
%token END
%token PRINT
%token EQUALS_EQUALS
%token LESS_THAN GREATER_THAN LESS_THAN_EQUALS GREATER_THAN_EQUALS
%token IF ELSE THEN WHILE FOR LET TO STEP
%token END_IF END_WHILE END_FOR END_SUB
%token SUB
%right EQUALS
%left PLUS MINUS
%left TIMES DIVIDE
%right U_MINUS
%type<node> program stmts stmt print_stmt assign_stmt expr term factor ident conditional_expr or_expr and_expr equality_expr relational_expr add_expr if_stmt block_stmt unmatched_if_stmt matched_if_stmt while_stmt for_stmt sub_stmt call_stmt
%type<number> NUMBER
%type<string> STRING
%type<string> IDENT
%type<boolean> TRUE FALSE
%nonassoc IF_UNMAT
%nonassoc ELSE
%%

program: stmts;

stmts: { $$ = new ProgramNode("PROG"); root = $$; }
    | stmts stmt { (dynamic_cast<ProgramNode*>($1))->addNode($2); }
    ;

stmt: print_stmt end { $$ = $1; }
    | assign_stmt end { $$ = $1; }
    | if_stmt end { $$ = $1; }
    | while_stmt end { $$ = $1; }
    | for_stmt end { $$ = $1; }
    | sub_stmt end { $$ = $1; }
    | call_stmt end { $$ = $1; }
    ;

call_stmt: ident LEFT_PAREN RIGHT_PAREN { $$ = new CallNode($1, "CALL", lines); }
    ;

sub_stmt: SUB ident LEFT_PAREN RIGHT_PAREN end block_stmt END_SUB { $$ = new SubNode($2, $6, "SUB", lines); }
    ;

for_stmt: FOR LET ident EQUALS expr TO expr THEN end block_stmt END_FOR { $$ = new ForNode($3, $5, $7, NULL, $10, "FOR", lines);}
    | FOR LET ident EQUALS expr TO expr STEP expr THEN end block_stmt END_FOR { $$ = new ForNode($3, $5, $7, $9, $12, "FOR", lines); }
    ;

while_stmt: WHILE LEFT_PAREN expr RIGHT_PAREN THEN end block_stmt END_WHILE { $$ = new WhileNode($3, $7, "WHILE", lines); }
    ;

block_stmt: { $$ = new BlockNode("BLOCK", lines); }
    | block_stmt stmt { (dynamic_cast<BlockNode*>($1))->addNode($2); }
    ;

if_stmt: unmatched_if_stmt { $$ = $1; }
    | matched_if_stmt { $$ = $1; }
    ;

unmatched_if_stmt: IF LEFT_PAREN expr RIGHT_PAREN THEN end block_stmt END_IF %prec IF_UNMAT { $$ = new IfNode($3, $7, NULL, "IF", lines); }
    ;

matched_if_stmt: IF LEFT_PAREN expr RIGHT_PAREN THEN end block_stmt ELSE end block_stmt END_IF { $$ = new IfNode($3, $7, $10, "IF", lines); printf("making if"); }
    | IF LEFT_PAREN expr RIGHT_PAREN THEN end block_stmt ELSE if_stmt { $$ = new IfNode($3, $7, $9, "IF", lines); }
    ;

assign_stmt: ident EQUALS expr { $$ = new VarAssignNode($1, $3, "ASSIGN", lines); };

print_stmt: PRINT LEFT_PAREN expr RIGHT_PAREN { $$ = new PrintNode($3, "PRINT", lines); }
    ;

ident: IDENT { $$ = new IdentifierNode(yylval.string, "IDENT", lines); }
    ;

expr: conditional_expr { $$ = $1; };

conditional_expr: or_expr { $$ = $1 };

or_expr: and_expr { $$ = $1; }
    | or_expr OR and_expr { $$ = new BinaryOpNode($1, $3, 'O', "or", lines); }
    ;

and_expr: equality_expr { $$ = $1; }
    | and_expr AND equality_expr { $$ = new BinaryOpNode($1, $3, 'A', "and", lines); }
    ;

equality_expr: relational_expr { $$ = $1; }
    | equality_expr EQUALS_EQUALS relational_expr { $$ = new BinaryOpNode($1, $3, 'E', "==", lines); }
    ;

relational_expr: add_expr { $$ = $1; }
    | relational_expr LESS_THAN add_expr { $$ = new BinaryOpNode($1, $3, '<', "<", lines); }
    | relational_expr GREATER_THAN add_expr { $$ = new BinaryOpNode($1, $3, '>', ">", lines); }
    | relational_expr LESS_THAN_EQUALS add_expr { $$ = new BinaryOpNode($1, $3, 'L', "<=", lines); }
    | relational_expr GREATER_THAN_EQUALS add_expr { $$ = new BinaryOpNode($1, $3, 'G', "<", lines); }
    ;

add_expr: term { $$ = $1; }
    | expr PLUS term { $$ = new BinaryOpNode($1, $3, '+', "+", lines); }
    | expr MINUS term { $$ = new BinaryOpNode($1, $3, '-', "-", lines); }
    ;

term: factor { $$ = $1; }
    | term TIMES factor { $$ = new BinaryOpNode($1, $3, '*', "*", lines); }
    | term DIVIDE factor { $$ = new BinaryOpNode($1, $3, '/', "/", lines); }
    ;

factor: NUMBER { $$ = new NumberNode(yylval.number, "NUM", lines); }
    | ident { $$ = $1; }
    | STRING { $$ = new StringNode(yylval.string, "STRING", lines); }
    | TRUE { $$ = new BooleanNode(true, "true", lines); }
    | FALSE { $$ = new BooleanNode(false, "false", lines); }
    | LEFT_PAREN expr RIGHT_PAREN { $$ = $2; }
    | MINUS expr %prec U_MINUS { $$ = new UnaryOpNode($2, '-', "UNARY", lines); }
    | ident LEFT_PAREN RIGHT_PAREN { $$ = new CallNode($1, "CALL", lines); }
    ;

end: END { lines++; }
    ;

%%

int yyerror(char *s) {
    //fprintf(stderr, "%s\n", s);
    return 0;
}