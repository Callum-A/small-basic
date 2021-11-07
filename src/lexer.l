%{
#include "node.hpp"
#include "y.tab.h"
#include <cstdlib>
#include <cstring>
int yyerror(char *s);
char *duplicateSegment(const char* token, int token_length);
%}
%%
[0-9]+ {
    yylval.number = atoi(yytext); 
    return NUMBER;
}
[\t ] ;
"+" return(PLUS);
"-" return(MINUS);
"*" return(TIMES);
"/" return(DIVIDE);
"(" return(LEFT_PAREN);
")" return(RIGHT_PAREN);
"\n" return(END);
"=" return EQUALS;
"==" return EQUALS_EQUALS;
"<" return LESS_THAN;
">" return GREATER_THAN;
"<=" return LESS_THAN_EQUALS;
">=" return GREATER_THAN_EQUALS;
"var" return VAR;
"print" return PRINT;
"true" {
    yylval.boolean = 1;
    return TRUE;
}
"false" {
    yylval.boolean = 0;
    return FALSE;
}
"or" return OR;
"and" return AND;
"if" return IF;
"else" return ELSE;
"then" return THEN;
"endif" return END_IF;
"while" return WHILE;
"for" return FOR;
"let" return LET;
"to" return TO;
"step" return STEP;
"sub" return SUB;
"endsub" return END_SUB;
"endwhile" return END_WHILE;
"endfor" return END_FOR;
\"(\\.|[^"\\])*\" {
    // trim ""
    yylval.string = duplicateSegment(yytext + 1, yyleng - 2);
    return STRING;
}
[a-z][a-zA-Z]* {
    char *copy;
    copy = (char *)malloc(sizeof(char) * (strlen(yytext) + 1));
    strcpy(copy, yytext);
    yylval.string = copy;
    return IDENT;
}
. { yyerror("unexpected character."); }
%%
int yywrap(void) {
    return 1;
}

char *duplicateSegment(const char* token, int token_length) {
    char* dup = (char *)malloc(token_length + 1);
    if (!dup) { /* handle memory allocation error */ }
    memcpy(dup, token, token_length);
    dup[token_length] = 0;
    return dup;
}