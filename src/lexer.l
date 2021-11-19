%{
#include "node.hpp"
#include "y.tab.h"
#include <cstdlib>
#include <cstring>
int yyerror(char *s);
char *duplicateSegment(const char* token, int token_length);
%}
%%
[\t ]           ;
"+"             return PLUS;
"-"             return MINUS;
"*"             return TIMES;
"/"             return DIVIDE;
"("             return LEFT_PAREN;
")"             return RIGHT_PAREN;
"["             return LEFT_BRACKET;
"]"             return RIGHT_BRACKET;
"{"             return LEFT_BRACE;
"}"             return RIGHT_BRACE;
":"             return COLON;
","             return COMMA;
"\n"            return END;
"="             return EQUALS;
"=="            return EQUALS_EQUALS;
"<"             return LESS_THAN;
">"             return GREATER_THAN;
"<="            return LESS_THAN_EQUALS;
">="            return GREATER_THAN_EQUALS;
"Var"           return VAR;
"Print"         return PRINT;
"Or"            return OR;
"And"           return AND;
"If"            return IF;
"Else"          return ELSE;
"Then"          return THEN;
"EndIf"         return END_IF;
"While"         return WHILE;
"For"           return FOR;
"Let"           return LET;
"To"            return TO;
"Step"          return STEP;
"Sub"           return SUB;
"EndSub"        return END_SUB;
"EndWhile"      return END_WHILE;
"EndFor"        return END_FOR;

[0-9]+ {
    yylval.number = atoi(yytext); 
    return NUMBER;
}

[0-9]*(\.)[0-9]+ {
    yylval.number = atof(yytext);
    return NUMBER;
}

\"(\\.|[^"\\])*\" {
    // trim ""
    yylval.string = duplicateSegment(yytext + 1, yyleng - 2);
    return STRING;
}

[a-z][a-zA-Z0-9]* {
    char *copy;
    copy = (char *)malloc(sizeof(char) * (strlen(yytext) + 1));
    strcpy(copy, yytext);
    yylval.string = copy;
    return IDENT;
}

"True" {
    yylval.boolean = 1;
    return TRUE;
}

"False" {
    yylval.boolean = 0;
    return FALSE;
}

. { yyerror("unexpected character."); }
%%
int yywrap(void) {
    return 1;
}

char *duplicateSegment(const char* token, int tokenLength) {
    char* dup = (char *)malloc(tokenLength + 1);
    memcpy(dup, token, tokenLength);
    dup[tokenLength] = 0;
    return dup;
}