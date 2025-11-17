#include "vm.h"
#include <unistd.h>
#include <sys/time.h>

long get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void vm_init(VMContext *ctx) {
    ctx->var_count = 0;
    
    ctx->rodeo.speed = 0;
    ctx->rodeo.torque = 0;
    ctx->rodeo.yaw = 0;
    ctx->rodeo.brake = 1; 
    ctx->rodeo.pattern = PATTERN_CALM;
    
    ctx->rodeo.rider_present = 0;
    ctx->rodeo.tilt_angle = 0;
    ctx->rodeo.rpm = 0;
    ctx->rodeo.emergency = 0;
    ctx->rodeo.start_time_ms = get_time_ms();
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║    RODEO VM - Mechanical Bull Simulator   ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
}

int vm_get_variable(VMContext *ctx, const char *name) {
    for (int i = 0; i < ctx->var_count; i++) {
        if (strcmp(ctx->variables[i].name, name) == 0) {
            return ctx->variables[i].value;
        }
    }
    return 0; 
}

void vm_set_variable(VMContext *ctx, const char *name, int value) {
    for (int i = 0; i < ctx->var_count; i++) {
        if (strcmp(ctx->variables[i].name, name) == 0) {
            ctx->variables[i].value = value;
            return;
        }
    }
    
    if (ctx->var_count < MAX_VARIABLES) {
        ctx->variables[ctx->var_count].name = strdup(name);
        ctx->variables[ctx->var_count].value = value;
        ctx->var_count++;
    } else {
        fprintf(stderr, "Error: Maximum number of variables exceeded\n");
    }
}

int vm_eval_expression(VMContext *ctx, Expression *expr) {
    if (!expr) return 0;
    
    switch (expr->type) {
        case EXPR_NUMBER:
            return expr->data.number;
            
        case EXPR_IDENTIFIER:
            return vm_get_variable(ctx, expr->data.identifier);
            
        case EXPR_BINARY_OP:
            {
                int left = vm_eval_expression(ctx, expr->data.binary.left);
                int right = vm_eval_expression(ctx, expr->data.binary.right);
                
                switch (expr->data.binary.op) {
                    case OP_ADD: return left + right;
                    case OP_SUB: return left - right;
                    case OP_MUL: return left * right;
                    case OP_DIV: 
                        if (right == 0) {
                            fprintf(stderr, "Error: Division by zero\n");
                            return 0;
                        }
                        return left / right;
                    default: return 0;
                }
            }
    }
    return 0;
}

int vm_eval_condition(VMContext *ctx, Condition *cond) {
    if (!cond) return 0;
    
    int left = vm_eval_expression(ctx, cond->left);
    int right = vm_eval_expression(ctx, cond->right);
    
    switch (cond->op) {
        case REL_EQ: return left == right;
        case REL_NE: return left != right;
        case REL_GT: return left > right;
        case REL_LT: return left < right;
        case REL_GE: return left >= right;
        case REL_LE: return left <= right;
        default: return 0;
    }
}

void vm_simulate_sensors(VMContext *ctx) {
    ctx->rodeo.rider_present = (ctx->rodeo.speed > 0) ? 1 : 1; 
    
    ctx->rodeo.tilt_angle = (ctx->rodeo.speed * ctx->rodeo.yaw) / 10;
    if (ctx->rodeo.tilt_angle > 45) ctx->rodeo.tilt_angle = 45;
    
    ctx->rodeo.rpm = ctx->rodeo.speed * 10;
    
}

int vm_read_sensor(VMContext *ctx, SensorType sensor) {
    vm_simulate_sensors(ctx);
    
    switch (sensor) {
        case SENSOR_RIDER:
            return ctx->rodeo.rider_present;
            
        case SENSOR_TILT:
            return ctx->rodeo.tilt_angle;
            
        case SENSOR_RPM:
            return ctx->rodeo.rpm;
            
        case SENSOR_EMERGENCY:
            return ctx->rodeo.emergency;
            
        case SENSOR_TIME_MS:
            return (int)(get_time_ms() - ctx->rodeo.start_time_ms);
            
        default:
            return 0;
    }
}

void vm_execute_statement(VMContext *ctx, ASTNode *stmt) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case STMT_ASSIGNMENT:
            {
                int value = vm_eval_expression(ctx, stmt->data.assignment.expr);
                vm_set_variable(ctx, stmt->data.assignment.var_name, value);
                printf("  [VAR] %s = %d\n", stmt->data.assignment.var_name, value);
            }
            break;
            
        case STMT_IF:
            {
                int cond_result = vm_eval_condition(ctx, stmt->data.if_stmt.condition);
                printf("  [IF] condition = %s\n", cond_result ? "TRUE" : "FALSE");
                
                if (cond_result) {
                    ASTNode *current = stmt->data.if_stmt.then_block;
                    while (current) {
                        vm_execute_statement(ctx, current);
                        current = current->next;
                    }
                } else if (stmt->data.if_stmt.else_block) {
                    ASTNode *current = stmt->data.if_stmt.else_block;
                    while (current) {
                        vm_execute_statement(ctx, current);
                        current = current->next;
                    }
                }
            }
            break;
            
        case STMT_WHILE:
            {
                printf("  [WHILE] entering loop\n");
                int iterations = 0;
                while (vm_eval_condition(ctx, stmt->data.while_stmt.condition)) {
                    ASTNode *current = stmt->data.while_stmt.body;
                    while (current) {
                        vm_execute_statement(ctx, current);
                        current = current->next;
                    }
                    iterations++;
                    if (iterations > 10000) {
                        fprintf(stderr, "  [WARNING] Loop exceeded 10000 iterations, breaking\n");
                        break;
                    }
                }
                printf("  [WHILE] exited after %d iterations\n", iterations);
            }
            break;
            
        case STMT_SPEED:
            {
                int speed = vm_eval_expression(ctx, stmt->data.speed_cmd.expr);
                if (speed < 0) speed = 0;
                if (speed > 100) speed = 100;
                ctx->rodeo.speed = speed;
                printf("  [SPEED] set to %d%%\n", speed);
            }
            break;
            
        case STMT_TORQUE:
            {
                int torque = vm_eval_expression(ctx, stmt->data.torque_cmd.expr);
                if (torque < 0) torque = 0;
                if (torque > 100) torque = 100;
                ctx->rodeo.torque = torque;
                printf("  [TORQUE] set to %d%%\n", torque);
            }
            break;
            
        case STMT_YAW:
            {
                int yaw = vm_eval_expression(ctx, stmt->data.yaw_cmd.expr);
                ctx->rodeo.yaw = yaw;
                printf("  [YAW] set to %d degrees/step\n", yaw);
            }
            break;
            
        case STMT_BRAKE:
            {
                int brake = vm_eval_expression(ctx, stmt->data.brake_cmd.expr);
                ctx->rodeo.brake = brake ? 1 : 0;
                printf("  [BRAKE] %s\n", ctx->rodeo.brake ? "ON" : "OFF");
            }
            break;
            
        case STMT_WAIT:
            {
                int wait_ms = vm_eval_expression(ctx, stmt->data.wait_cmd.expr);
                printf("  [WAIT] %d ms\n", wait_ms);
            }
            break;
            
        case STMT_PATTERN:
            {
                ctx->rodeo.pattern = stmt->data.pattern_cmd.pattern;
                const char *pattern_name[] = {"CALM", "SWIRL", "AGGRESSIVE"};
                printf("  [PATTERN] set to %s\n", pattern_name[ctx->rodeo.pattern]);
            }
            break;
            
        case STMT_SENSOR_READ:
            {
                int value = vm_read_sensor(ctx, stmt->data.sensor_read.sensor);
                vm_set_variable(ctx, stmt->data.sensor_read.var_name, value);
                const char *sensor_name[] = {"rider", "tilt", "rpm", "emergency", "time_ms"};
                printf("  [SENSOR] %s -> %s = %d\n", 
                       sensor_name[stmt->data.sensor_read.sensor],
                       stmt->data.sensor_read.var_name,
                       value);
            }
            break;
            
        case STMT_BLOCK:
            for (int i = 0; i < stmt->data.block.count; i++) {
                vm_execute_statement(ctx, stmt->data.block.statements[i]);
            }
            break;
    }
}

void vm_execute(VMContext *ctx, ASTNode *program) {
    printf("▶ Starting program execution...\n\n");
    
    ASTNode *current = program;
    while (current) {
        vm_execute_statement(ctx, current);
        current = current->next;
    }
    
    printf("\n✓ Program execution completed.\n");
}

void vm_print_state(VMContext *ctx) {
    printf("\n┌────────────────────────────────────────────┐\n");
    printf("│          FINAL RODEO STATE                 │\n");
    printf("├────────────────────────────────────────────┤\n");
    printf("│ Speed:   %3d%%  %-28s│\n", ctx->rodeo.speed, 
           ctx->rodeo.speed > 0 ? "⚡ ACTIVE" : "💤 STOPPED");
    printf("│ Torque:  %3d%%                             │\n", ctx->rodeo.torque);
    printf("│ Yaw:     %3d° per step                     │\n", ctx->rodeo.yaw);
    printf("│ Brake:   %-34s│\n", ctx->rodeo.brake ? "🔴 ON" : "🟢 OFF");
    
    const char *pattern_name[] = {"🌊 CALM", "🌀 SWIRL", "⚡ AGGRESSIVE"};
    printf("│ Pattern: %-34s│\n", pattern_name[ctx->rodeo.pattern]);
    printf("├────────────────────────────────────────────┤\n");
    printf("│          SENSOR READINGS                   │\n");
    printf("├────────────────────────────────────────────┤\n");
    printf("│ Rider:     %-32s│\n", ctx->rodeo.rider_present ? "👤 PRESENT" : "❌ ABSENT");
    printf("│ Tilt:      %3d°                            │\n", ctx->rodeo.tilt_angle);
    printf("│ RPM:       %4d                             │\n", ctx->rodeo.rpm);
    printf("│ Emergency: %-32s│\n", ctx->rodeo.emergency ? "🚨 ACTIVE" : "✓ OK");
    printf("└────────────────────────────────────────────┘\n");
    
    if (ctx->var_count > 0) {
        printf("\n┌────────────────────────────────────────────┐\n");
        printf("│          VARIABLES                         │\n");
        printf("├────────────────────────────────────────────┤\n");
        for (int i = 0; i < ctx->var_count; i++) {
            printf("│ %-20s = %-18d│\n", 
                   ctx->variables[i].name, 
                   ctx->variables[i].value);
        }
        printf("└────────────────────────────────────────────┘\n");
    }
}

void vm_cleanup(VMContext *ctx) {
    for (int i = 0; i < ctx->var_count; i++) {
        free(ctx->variables[i].name);
    }
    ctx->var_count = 0;
}

