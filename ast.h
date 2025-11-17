#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct ASTNode ASTNode;
typedef struct Expression Expression;

typedef enum {
    EXPR_NUMBER,
    EXPR_IDENTIFIER,
    EXPR_BINARY_OP
} ExprType;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
} BinaryOp;

typedef enum {
    REL_EQ,   // ==
    REL_NE,   // !=
    REL_GT,   // >
    REL_LT,   // <
    REL_GE,   // >=
    REL_LE    // <=
} RelOp;

typedef enum {
    PATTERN_CALM,
    PATTERN_SWIRL,
    PATTERN_AGGRESSIVE
} Pattern;

typedef enum {
    SENSOR_RIDER,
    SENSOR_TILT,
    SENSOR_RPM,
    SENSOR_EMERGENCY,
    SENSOR_TIME_MS
} SensorType;

struct Expression {
    ExprType type;
    union {
        int number;
        char *identifier;
        struct {
            BinaryOp op;
            struct Expression *left;
            struct Expression *right;
        } binary;
    } data;
};

typedef struct {
    RelOp op;
    Expression *left;
    Expression *right;
} Condition;

typedef enum {
    STMT_ASSIGNMENT,
    STMT_IF,
    STMT_WHILE,
    STMT_SPEED,
    STMT_TORQUE,
    STMT_YAW,
    STMT_BRAKE,
    STMT_WAIT,
    STMT_PATTERN,
    STMT_SENSOR_READ,
    STMT_BLOCK
} StmtType;

struct ASTNode {
    StmtType type;
    union {
        struct {
            char *var_name;
            Expression *expr;
        } assignment;
        
        struct {
            Condition *condition;
            ASTNode *then_block;
            ASTNode *else_block;
        } if_stmt;
        
        struct {
            Condition *condition;
            ASTNode *body;
        } while_stmt;
        
        struct {
            Expression *expr;
        } speed_cmd;
        
        struct {
            Expression *expr;
        } torque_cmd;
        
        struct {
            Expression *expr;
        } yaw_cmd;
        
        struct {
            Expression *expr;
        } brake_cmd;
        
        struct {
            Expression *expr;
        } wait_cmd;
        
        struct {
            Pattern pattern;
        } pattern_cmd;
        
        struct {
            SensorType sensor;
            char *var_name;
        } sensor_read;
        
        struct {
            ASTNode **statements;
            int count;
        } block;
    } data;
    
    ASTNode *next;
};

Expression *create_number_expr(int value);
Expression *create_identifier_expr(char *name);
Expression *create_binary_expr(BinaryOp op, Expression *left, Expression *right);
void free_expression(Expression *expr);

Condition *create_condition(RelOp op, Expression *left, Expression *right);
void free_condition(Condition *cond);

ASTNode *create_assignment(char *var_name, Expression *expr);
ASTNode *create_if_stmt(Condition *cond, ASTNode *then_block, ASTNode *else_block);
ASTNode *create_while_stmt(Condition *cond, ASTNode *body);
ASTNode *create_speed_cmd(Expression *expr);
ASTNode *create_torque_cmd(Expression *expr);
ASTNode *create_yaw_cmd(Expression *expr);
ASTNode *create_brake_cmd(Expression *expr);
ASTNode *create_wait_cmd(Expression *expr);
ASTNode *create_pattern_cmd(Pattern pattern);
ASTNode *create_sensor_read(SensorType sensor, char *var_name);
ASTNode *create_block(ASTNode **statements, int count);

void free_ast(ASTNode *node);
void append_statement(ASTNode **list, ASTNode *stmt);

#endif

