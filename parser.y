%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "vm.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int line_num;

void yyerror(const char *s);

ASTNode *root_program = NULL;
%}

%union {
    int number;
    char *string;
    Expression *expr;
    Condition *cond;
    ASTNode *stmt;
    ASTNode *stmt_list;
    BinaryOp binop;
    RelOp relop;
    Pattern pattern;
    SensorType sensor;
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

%type <expr> expression term
%type <cond> condition
%type <stmt> statement assignment if_stmt while_stmt command
%type <stmt> speed_cmd torque_cmd yaw_cmd brake_cmd wait_cmd pattern_cmd sensor_cmd
%type <stmt_list> statement_list program
%type <relop> relop
%type <pattern> mode
%type <sensor> sensor

%left PLUS MINUS
%left MULT DIV

%%

program:
    /* empty */ {
        root_program = NULL;
        $$ = NULL;
    }
    | statement_list {
        root_program = $1;
        $$ = $1;
    }
    ;

statement_list:
    statement {
        $$ = $1;
    }
    | statement_list statement {
        if ($1) {
            append_statement(&$1, $2);
            $$ = $1;
        } else {
            $$ = $2;
        }
    }
    ;

statement:
    assignment { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | command { $$ = $1; }
    ;

assignment:
    IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = create_assignment($1, $3);
        free($1);
    }
    ;

if_stmt:
    IF LPAREN condition RPAREN LBRACE statement_list RBRACE {
        $$ = create_if_stmt($3, $6, NULL);
    }
    | IF LPAREN condition RPAREN LBRACE statement_list RBRACE ELSE LBRACE statement_list RBRACE {
        $$ = create_if_stmt($3, $6, $10);
    }
    | IF LPAREN condition RPAREN LBRACE RBRACE {
        $$ = create_if_stmt($3, NULL, NULL);
    }
    | IF LPAREN condition RPAREN LBRACE RBRACE ELSE LBRACE statement_list RBRACE {
        $$ = create_if_stmt($3, NULL, $9);
    }
    ;

while_stmt:
    WHILE LPAREN condition RPAREN LBRACE statement_list RBRACE {
        $$ = create_while_stmt($3, $6);
    }
    | WHILE LPAREN condition RPAREN LBRACE RBRACE {
        $$ = create_while_stmt($3, NULL);
    }
    ;

command:
    speed_cmd SEMICOLON { $$ = $1; }
    | torque_cmd SEMICOLON { $$ = $1; }
    | yaw_cmd SEMICOLON { $$ = $1; }
    | brake_cmd SEMICOLON { $$ = $1; }
    | wait_cmd SEMICOLON { $$ = $1; }
    | pattern_cmd SEMICOLON { $$ = $1; }
    | sensor_cmd { $$ = $1; }
    ;

speed_cmd:
    SPEED LPAREN expression RPAREN {
        $$ = create_speed_cmd($3);
    }
    ;

torque_cmd:
    TORQUE LPAREN expression RPAREN {
        $$ = create_torque_cmd($3);
    }
    ;

yaw_cmd:
    YAW LPAREN expression RPAREN {
        $$ = create_yaw_cmd($3);
    }
    ;

brake_cmd:
    BRAKE LPAREN expression RPAREN {
        $$ = create_brake_cmd($3);
    }
    ;

wait_cmd:
    WAIT LPAREN expression RPAREN {
        $$ = create_wait_cmd($3);
    }
    ;

pattern_cmd:
    PATTERN LPAREN mode RPAREN {
        $$ = create_pattern_cmd($3);
    }
    ;

sensor_cmd:
    READ LPAREN sensor RPAREN ARROW IDENTIFIER SEMICOLON {
        $$ = create_sensor_read($3, $6);
        free($6);
    }
    ;

expression:
    term {
        $$ = $1;
    }
    | expression PLUS term {
        $$ = create_binary_expr(OP_ADD, $1, $3);
    }
    | expression MINUS term {
        $$ = create_binary_expr(OP_SUB, $1, $3);
    }
    | expression MULT term {
        $$ = create_binary_expr(OP_MUL, $1, $3);
    }
    | expression DIV term {
        $$ = create_binary_expr(OP_DIV, $1, $3);
    }
    ;

term:
    NUMBER {
        $$ = create_number_expr($1);
    }
    | IDENTIFIER {
        $$ = create_identifier_expr($1);
        free($1);
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;

condition:
    expression relop expression {
        $$ = create_condition($2, $1, $3);
    }
    ;

relop:
    EQ { $$ = REL_EQ; }
    | NE { $$ = REL_NE; }
    | GT { $$ = REL_GT; }
    | LT { $$ = REL_LT; }
    | GE { $$ = REL_GE; }
    | LE { $$ = REL_LE; }
    ;

mode:
    CALM { $$ = PATTERN_CALM; }
    | SWIRL { $$ = PATTERN_SWIRL; }
    | AGGRESSIVE { $$ = PATTERN_AGGRESSIVE; }
    ;

sensor:
    RIDER { $$ = SENSOR_RIDER; }
    | TILT { $$ = SENSOR_TILT; }
    | RPM { $$ = SENSOR_RPM; }
    | EMERGENCY { $$ = SENSOR_EMERGENCY; }
    | TIME_MS { $$ = SENSOR_TIME_MS; }
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

    printf("=== Rodeo VM - Lexical and Syntactic Analysis ===\n");
    
    if (yyparse() == 0) {
        printf("✓ Parsing completed successfully!\n");
        
        if (root_program) {
            // Initialize and run VM
            VMContext vm;
            vm_init(&vm);
            vm_execute(&vm, root_program);
            vm_print_state(&vm);
            
            // Cleanup
            vm_cleanup(&vm);
            free_ast(root_program);
        } else {
            printf("Warning: Empty program\n");
        }
    } else {
        printf("✗ Parsing failed with errors\n");
        return 1;
    }

    return 0;
}
