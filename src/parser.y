%{
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "node.hpp"
int yylex();
int yyerror(char *s);
int lines = 1;
extern Node *root;
%}
%error-verbose
%union {
    Node *node;
    double number;
    int boolean;
    char *string;
}
%start program
%token NUMBER STRING TRUE FALSE
%token VAR IDENT PLUS MINUS
%token TIMES DIVIDE EQUALS OR AND
%token LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET
%token LEFT_BRACE RIGHT_BRACE COLON COMMA
%token END PRINT EQUALS_EQUALS LESS_THAN
%token GREATER_THAN LESS_THAN_EQUALS GREATER_THAN_EQUALS IF
%token ELSE THEN WHILE FOR 
%token LET TO STEP END_IF 
%token SUB END_WHILE END_FOR END_SUB
%token DO

%right EQUALS
%left PLUS MINUS
%left TIMES DIVIDE
%right U_MINUS
%nonassoc IF_UNMAT
%nonassoc ELSE

%type<node> program stmts stmt print_stmt assign_stmt
%type<node> expr term factor ident conditional_expr
%type<node> or_expr and_expr equality_expr relational_expr
%type<node> add_expr if_stmt block_stmt unmatched_if_stmt 
%type<node> matched_if_stmt while_stmt for_stmt sub_stmt
%type<node> call_stmt list expr_list expr_list_ext index
%type<node> map map_list map_list_ext index_assign_stmt
%type<node> builtin arg_list arg_list_ext expr_stmt
%type<number> NUMBER
%type<string> STRING
%type<string> IDENT
%type<boolean> TRUE FALSE
%%

program: stmts;

stmts: { $$ = new ProgramNode("PROG"); root = $$; }
    | stmts stmt { 
        if ($2 != NULL) {
            (dynamic_cast<ProgramNode*>($1))->addNode($2);
        }
    }
    ;

stmt: end { $$ = NULL; }
    | expr_stmt end { $$ = $1; }
    | print_stmt end { $$ = $1; }
    | assign_stmt end { $$ = $1; }
    | if_stmt end { $$ = $1; }
    | while_stmt end { $$ = $1; }
    | for_stmt end { $$ = $1; }
    | sub_stmt end { $$ = $1; }
    | call_stmt end { $$ = $1; }
    | index_assign_stmt end { $$ = $1; }
    ;

expr_stmt: expr { $$ = new ExprNode($1, "EXPR", lines); }
    ;

index_assign_stmt: ident LEFT_BRACKET expr RIGHT_BRACKET EQUALS expr { $$ = new IndexAssignNode($1, $3, $6, "INDEX_ASSIGN", lines); }
    ;

call_stmt: ident LEFT_PAREN RIGHT_PAREN { $$ = new CallNode($1, "CALL", lines); }
    ;

sub_stmt: SUB ident LEFT_PAREN RIGHT_PAREN end block_stmt END_SUB { $$ = new SubNode($2, $6, "SUB", lines); }
    ;

for_stmt: FOR LET ident EQUALS expr TO expr DO end block_stmt END_FOR { $$ = new ForNode($3, $5, $7, NULL, $10, "FOR", lines);}
    | FOR LET ident EQUALS expr TO expr STEP expr DO end block_stmt END_FOR { $$ = new ForNode($3, $5, $7, $9, $12, "FOR", lines); }
    ;

while_stmt: WHILE expr DO end block_stmt END_WHILE { $$ = new WhileNode($2, $5, "WHILE", lines); }
    ;

block_stmt: { $$ = new BlockNode("BLOCK", lines); }
    | block_stmt stmt { 
        if ($2 != NULL) {
            (dynamic_cast<BlockNode*>($1))->addNode($2);
        }
    }
    ;

if_stmt: unmatched_if_stmt { $$ = $1; }
    | matched_if_stmt { $$ = $1; }
    ;

unmatched_if_stmt: IF expr THEN end block_stmt END_IF %prec IF_UNMAT { $$ = new IfNode($2, $5, NULL, "IF", lines); }
    ;

matched_if_stmt: IF expr THEN end block_stmt ELSE end block_stmt END_IF { $$ = new IfNode($2, $5, $8, "IF", lines); }
    | IF expr THEN end block_stmt ELSE if_stmt { $$ = new IfNode($2, $5, $7, "IF", lines); }
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
    | index { $$ = $1; }
    | list { $$ = $1; }
    | map { $$ = $1; }
    | builtin { $$ = $1; }
    ;

builtin: ident LEFT_PAREN arg_list RIGHT_PAREN { $$ = new BuiltInNode($1, $3, "BUILTIN", lines); }
    ;

arg_list: { $$ = new ExprListNode("ARGS", lines); }
    | expr { $$ = new ExprListNode("ARGS", lines); (dynamic_cast<ExprListNode*>($$))->addNode($1); }
    | arg_list_ext COMMA expr { $$ = $1; (dynamic_cast<ExprListNode*>($$))->addNode($3); }
    ;

arg_list_ext: expr { $$ = new ExprListNode("ARGS", lines); (dynamic_cast<ExprListNode*>($$))->addNode($1); }
    | arg_list_ext COMMA expr         { $$ = $1; (dynamic_cast<ExprListNode*>($$))->addNode($3); }
    ;

index: ident LEFT_BRACKET expr RIGHT_BRACKET { $$ = new IndexNode($1, $3, "INDEX", lines); }
    ;

map: LEFT_BRACE map_list RIGHT_BRACE { $$ = $2; }
    ;

map_list: { $$ = new MapNode("MAP", lines); }
    | expr COLON expr { $$ = new MapNode("MAP", lines); (dynamic_cast<MapNode*>($$))->addNode($1, $3); }
    | map_list_ext COMMA expr COLON expr { $$ = $1; (dynamic_cast<MapNode*>($$))->addNode($3, $5); }
    ;

map_list_ext: expr COLON expr { $$ = new MapNode("MAP", lines); (dynamic_cast<MapNode*>($$))->addNode($1, $3); }
    | map_list_ext COMMA expr COLON expr { $$ = $1; (dynamic_cast<MapNode*>($$))->addNode($3, $5); }
    ;

list: LEFT_BRACKET expr_list RIGHT_BRACKET { $$ = $2; }
    ;

expr_list: { $$ = new ExprListNode("LIST", lines); }
    | expr { $$ = new ExprListNode("LIST", lines); (dynamic_cast<ExprListNode*>($$))->addNode($1); }
    | expr_list_ext COMMA expr { $$ = $1; (dynamic_cast<ExprListNode*>($$))->addNode($3); }
    ;

expr_list_ext: expr { $$ = new ExprListNode("LIST", lines); (dynamic_cast<ExprListNode*>($$))->addNode($1); }
    | expr_list_ext COMMA expr { $$ = $1; (dynamic_cast<ExprListNode*>($$))->addNode($3); }
    ;

end: END { lines++; }
    ;

%%

int yyerror(char *s) {
    fprintf(stderr, "%s at line %d\n", s, lines);
    return 0;
}