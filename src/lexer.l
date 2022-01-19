%{
#include "node.hpp"
#include "y.tab.h"
#include "buffer.hpp"
#include <cstdlib>
#include <cstring>
int yyerror(char *s);
char *duplicateSegment(const char* token, int token_length);
%}
%x str
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
"Do"            return DO;
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

\" { BEGIN str; clearBuffer(); }
<str>[^\\"\n]*   { appendBufferStr(yytext); }
<str>\\n         { appendBuffer('\n'); }
<str>\\t         { appendBuffer('\t'); }
<str>\\[0-7]*    { appendBuffer(strtol(yytext+1, 0, 8)); }
<str>\\[\\"]     { appendBuffer(yytext[1]); }
<str>\"          { yylval.string = copyBuffer(); BEGIN 0; return STRING; }
<str>\\.         { yyerror("bogus escape '%s' in string\n"); }
<str>\n          { yyerror("newline in string\n"); }

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