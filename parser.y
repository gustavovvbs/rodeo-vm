%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int line_num;

void yyerror(const char *s);
%}

%union {
    int number;
    char *string;
}

%token IF ELSE WHILE
%token SPEED TORQUE YAW BRAKE WAIT PATTERN READ
%token CALM SWIRL AGGRESSIVE
%token RIDER TILT RPM EMERGENCY TIME_MS
%token EQ NE GE LE GT LT
%token ASSIGN PLUS MINUS MULT DIV
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON ARROW
%token <string> IDENTIFIER
%token <number> NUMBER

%left PLUS MINUS
%left MULT DIV

%%

program:
    /* empty */
    | statement_list
    ;

statement_list:
    statement
    | statement_list statement
    ;

statement:
    assignment
    | if_stmt
    | while_stmt
    | command
    ;

assignment:
    IDENTIFIER ASSIGN expression SEMICOLON {
        printf("Assignment: %s = <expression>\n", $1);
        free($1);
    }
    ;

if_stmt:
    IF LPAREN condition RPAREN LBRACE statement_list RBRACE {
        printf("If statement\n");
    }
    | IF LPAREN condition RPAREN LBRACE statement_list RBRACE ELSE LBRACE statement_list RBRACE {
        printf("If-Else statement\n");
    }
    | IF LPAREN condition RPAREN LBRACE RBRACE {
        printf("If statement (empty body)\n");
    }
    | IF LPAREN condition RPAREN LBRACE RBRACE ELSE LBRACE statement_list RBRACE {
        printf("If-Else statement (empty if body)\n");
    }
    ;

while_stmt:
    WHILE LPAREN condition RPAREN LBRACE statement_list RBRACE {
        printf("While statement\n");
    }
    | WHILE LPAREN condition RPAREN LBRACE RBRACE {
        printf("While statement (empty body)\n");
    }
    ;

command:
    speed_cmd SEMICOLON
    | torque_cmd SEMICOLON
    | yaw_cmd SEMICOLON
    | brake_cmd SEMICOLON
    | wait_cmd SEMICOLON
    | pattern_cmd SEMICOLON
    | sensor_cmd
    ;

speed_cmd:
    SPEED LPAREN expression RPAREN {
        printf("Speed command\n");
    }
    ;

torque_cmd:
    TORQUE LPAREN expression RPAREN {
        printf("Torque command\n");
    }
    ;

yaw_cmd:
    YAW LPAREN expression RPAREN {
        printf("Yaw command\n");
    }
    ;

brake_cmd:
    BRAKE LPAREN expression RPAREN {
        printf("Brake command\n");
    }
    ;

wait_cmd:
    WAIT LPAREN expression RPAREN {
        printf("Wait command\n");
    }
    ;

pattern_cmd:
    PATTERN LPAREN mode RPAREN {
        printf("Pattern command\n");
    }
    ;

sensor_cmd:
    READ LPAREN sensor RPAREN ARROW IDENTIFIER SEMICOLON {
        printf("Sensor read: -> %s\n", $6);
        free($6);
    }
    ;

expression:
    term
    | expression PLUS term {
        printf("Expression: + operation\n");
    }
    | expression MINUS term {
        printf("Expression: - operation\n");
    }
    | expression MULT term {
        printf("Expression: * operation\n");
    }
    | expression DIV term {
        printf("Expression: / operation\n");
    }
    ;

term:
    NUMBER {
        printf("Term: number %d\n", $1);
    }
    | IDENTIFIER {
        printf("Term: identifier %s\n", $1);
        free($1);
    }
    | LPAREN expression RPAREN {
        printf("Term: parenthesized expression\n");
    }
    ;

condition:
    expression relop expression {
        printf("Condition\n");
    }
    ;

relop:
    EQ { printf("RelOp: ==\n"); }
    | NE { printf("RelOp: !=\n"); }
    | GT { printf("RelOp: >\n"); }
    | LT { printf("RelOp: <\n"); }
    | GE { printf("RelOp: >=\n"); }
    | LE { printf("RelOp: <=\n"); }
    ;

mode:
    CALM { printf("Mode: CALM\n"); }
    | SWIRL { printf("Mode: SWIRL\n"); }
    | AGGRESSIVE { printf("Mode: AGGRESSIVE\n"); }
    ;

sensor:
    RIDER { printf("Sensor: rider\n"); }
    | TILT { printf("Sensor: tilt\n"); }
    | RPM { printf("Sensor: rpm\n"); }
    | EMERGENCY { printf("Sensor: emergency\n"); }
    | TIME_MS { printf("Sensor: time_ms\n"); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", line_num, s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror(argv[1]);
            return 1;
        }
        yyin = file;
    }

    printf("=== Rodeo VM - Lexical and Syntactic Analysis ===\n\n");
    
    if (yyparse() == 0) {
        printf("\n=== Parsing completed successfully! ===\n");
    } else {
        printf("\n=== Parsing failed with errors ===\n");
        return 1;
    }

    return 0;
}

