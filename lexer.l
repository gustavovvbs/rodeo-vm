%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"

int line_num = 1;
%}

%option noyywrap

%%

[ \t]+              { /* ignore whitespace */ }
\n                  { line_num++; }
"//".*              { /* ignore single-line comments */ }

"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"speed"             { return SPEED; }
"torque"            { return TORQUE; }
"yaw"               { return YAW; }
"brake"             { return BRAKE; }
"wait"              { return WAIT; }
"pattern"           { return PATTERN; }
"read"              { return READ; }

"CALM"              { return CALM; }
"SWIRL"             { return SWIRL; }
"AGGRESSIVE"        { return AGGRESSIVE; }

"rider"             { return RIDER; }
"tilt"              { return TILT; }
"rpm"               { return RPM; }
"emergency"         { return EMERGENCY; }
"time_ms"           { return TIME_MS; }

"=="                { return EQ; }
"!="                { return NE; }
">="                { return GE; }
"<="                { return LE; }
">"                 { return GT; }
"<"                 { return LT; }

"="                 { return ASSIGN; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MULT; }
"/"                 { return DIV; }

"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
";"                 { return SEMICOLON; }
"->"                { return ARROW; }

[a-zA-Z][a-zA-Z0-9_]*  { 
                        yylval.string = strdup(yytext); 
                        return IDENTIFIER; 
                    }

[0-9]+              { 
                        yylval.number = atoi(yytext); 
                        return NUMBER; 
                    }

.                   { 
                        fprintf(stderr, "Lexical error at line %d: unexpected character '%s'\n", 
                                line_num, yytext); 
                    }

%%

