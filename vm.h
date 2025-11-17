#ifndef VM_H
#define VM_H

#include "ast.h"
#include <time.h>

#define MAX_VARIABLES 100

typedef struct {
    char *name;
    int value;
} Variable;

typedef struct {
    int speed;          
    int torque;         
    int yaw;            
    int brake;          
    Pattern pattern;    
    
    int rider_present;  
    int tilt_angle;     
    int rpm;            
    int emergency;      
    long start_time_ms; 
} RodeoState;

typedef struct {
    Variable variables[MAX_VARIABLES];
    int var_count;
    RodeoState rodeo;
} VMContext;

void vm_init(VMContext *ctx);
void vm_execute(VMContext *ctx, ASTNode *program);
void vm_print_state(VMContext *ctx);
void vm_cleanup(VMContext *ctx);

int vm_get_variable(VMContext *ctx, const char *name);
void vm_set_variable(VMContext *ctx, const char *name, int value);

int vm_eval_expression(VMContext *ctx, Expression *expr);
int vm_eval_condition(VMContext *ctx, Condition *cond);

void vm_execute_statement(VMContext *ctx, ASTNode *stmt);

int vm_read_sensor(VMContext *ctx, SensorType sensor);
void vm_simulate_sensors(VMContext *ctx);

#endif

