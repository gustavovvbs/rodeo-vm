#include "ast.h"

Expression *create_number_expr(int value) {
    Expression *expr = (Expression *)malloc(sizeof(Expression));
    expr->type = EXPR_NUMBER;
    expr->data.number = value;
    return expr;
}

Expression *create_identifier_expr(char *name) {
    Expression *expr = (Expression *)malloc(sizeof(Expression));
    expr->type = EXPR_IDENTIFIER;
    expr->data.identifier = strdup(name);
    return expr;
}

Expression *create_binary_expr(BinaryOp op, Expression *left, Expression *right) {
    Expression *expr = (Expression *)malloc(sizeof(Expression));
    expr->type = EXPR_BINARY_OP;
    expr->data.binary.op = op;
    expr->data.binary.left = left;
    expr->data.binary.right = right;
    return expr;
}

void free_expression(Expression *expr) {
    if (!expr) return;
    
    switch (expr->type) {
        case EXPR_IDENTIFIER:
            free(expr->data.identifier);
            break;
        case EXPR_BINARY_OP:
            free_expression(expr->data.binary.left);
            free_expression(expr->data.binary.right);
            break;
        case EXPR_NUMBER:
            break;
    }
    free(expr);
}

Condition *create_condition(RelOp op, Expression *left, Expression *right) {
    Condition *cond = (Condition *)malloc(sizeof(Condition));
    cond->op = op;
    cond->left = left;
    cond->right = right;
    return cond;
}

void free_condition(Condition *cond) {
    if (!cond) return;
    free_expression(cond->left);
    free_expression(cond->right);
    free(cond);
}

ASTNode *create_assignment(char *var_name, Expression *expr) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_ASSIGNMENT;
    node->data.assignment.var_name = strdup(var_name);
    node->data.assignment.expr = expr;
    node->next = NULL;
    return node;
}

ASTNode *create_if_stmt(Condition *cond, ASTNode *then_block, ASTNode *else_block) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_IF;
    node->data.if_stmt.condition = cond;
    node->data.if_stmt.then_block = then_block;
    node->data.if_stmt.else_block = else_block;
    node->next = NULL;
    return node;
}

ASTNode *create_while_stmt(Condition *cond, ASTNode *body) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_WHILE;
    node->data.while_stmt.condition = cond;
    node->data.while_stmt.body = body;
    node->next = NULL;
    return node;
}

ASTNode *create_speed_cmd(Expression *expr) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_SPEED;
    node->data.speed_cmd.expr = expr;
    node->next = NULL;
    return node;
}

ASTNode *create_torque_cmd(Expression *expr) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_TORQUE;
    node->data.torque_cmd.expr = expr;
    node->next = NULL;
    return node;
}

ASTNode *create_yaw_cmd(Expression *expr) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_YAW;
    node->data.yaw_cmd.expr = expr;
    node->next = NULL;
    return node;
}

ASTNode *create_brake_cmd(Expression *expr) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_BRAKE;
    node->data.brake_cmd.expr = expr;
    node->next = NULL;
    return node;
}

ASTNode *create_wait_cmd(Expression *expr) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_WAIT;
    node->data.wait_cmd.expr = expr;
    node->next = NULL;
    return node;
}

ASTNode *create_pattern_cmd(Pattern pattern) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_PATTERN;
    node->data.pattern_cmd.pattern = pattern;
    node->next = NULL;
    return node;
}

ASTNode *create_sensor_read(SensorType sensor, char *var_name) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_SENSOR_READ;
    node->data.sensor_read.sensor = sensor;
    node->data.sensor_read.var_name = strdup(var_name);
    node->next = NULL;
    return node;
}

ASTNode *create_block(ASTNode **statements, int count) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = STMT_BLOCK;
    node->data.block.statements = statements;
    node->data.block.count = count;
    node->next = NULL;
    return node;
}

void free_ast(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case STMT_ASSIGNMENT:
            free(node->data.assignment.var_name);
            free_expression(node->data.assignment.expr);
            break;
            
        case STMT_IF:
            free_condition(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.then_block);
            free_ast(node->data.if_stmt.else_block);
            break;
            
        case STMT_WHILE:
            free_condition(node->data.while_stmt.condition);
            free_ast(node->data.while_stmt.body);
            break;
            
        case STMT_SPEED:
            free_expression(node->data.speed_cmd.expr);
            break;
            
        case STMT_TORQUE:
            free_expression(node->data.torque_cmd.expr);
            break;
            
        case STMT_YAW:
            free_expression(node->data.yaw_cmd.expr);
            break;
            
        case STMT_BRAKE:
            free_expression(node->data.brake_cmd.expr);
            break;
            
        case STMT_WAIT:
            free_expression(node->data.wait_cmd.expr);
            break;
            
        case STMT_PATTERN:  
            break;
            
        case STMT_SENSOR_READ:
            free(node->data.sensor_read.var_name);
            break;
            
        case STMT_BLOCK:
            for (int i = 0; i < node->data.block.count; i++) {
                free_ast(node->data.block.statements[i]);
            }
            free(node->data.block.statements);
            break;
    }
    
    free_ast(node->next);
    free(node);
}

void append_statement(ASTNode **list, ASTNode *stmt) {
    if (!stmt) return;
    
    if (!*list) {
        *list = stmt;
    } else {
        ASTNode *current = *list;
        while (current->next) {
            current = current->next;
        }
        current->next = stmt;
    }
}

