#pragma once

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h> 
#include <string.h> 
#include <stdarg.h> 

// --- 1. ENUMS ---
typedef enum {
    TOKEN_KEYWORD, TOKEN_IDENTIFIER, TOKEN_SYMBOL, TOKEN_NUMBER, TOKEN_UNKNOWN
} TokenType;

typedef enum {
    TYPE_INT, TYPE_CHAR, TYPE_UNKNOWN
} DataType;

typedef enum {
    AST_NUM, AST_VAR, AST_OP
} ASTNodeType;

// --- 2. STRUCTS ---
typedef struct TOKEN {
    char *value;
    TokenType type;
    int line;
    struct TOKEN *next;
} TOKEN;

typedef struct VARIABLE {
    char *name;
    DataType type;
    int value;
    struct VARIABLE *next;
} VARIABLE;

typedef struct ERROR {
    char *msg;
    int line;
    struct ERROR *next;
} ERROR;

typedef struct HISTORY {
    char *msg;
    struct HISTORY *next;
} HISTORY;

typedef struct MIPS_INSTR {
    char *code;
    struct MIPS_INSTR *next;
} MIPS_INSTR;

typedef struct ASTNode {
    ASTNodeType type;
    int int_val;
    char *var_name;
    char op;
    struct ASTNode *left;
    struct ASTNode *right;
    int line;
    int has_error;
} ASTNode;

// --- 3. GLOBALS ---
TOKEN *head = NULL;
TOKEN *tail = NULL;
int line_number = 1;
int token_count = 0;

TOKEN *parser_pos = NULL; 

VARIABLE *var_head = NULL;
VARIABLE *var_tail = NULL;

ERROR *err_head = NULL;
ERROR *err_tail = NULL;

HISTORY *hist_head = NULL;
HISTORY *hist_tail = NULL;

MIPS_INSTR *mips_head = NULL;
MIPS_INSTR *mips_tail = NULL;

int has_errors = 0;

// --- 4. BASIC HELPERS ---

void log_event(const char *format, ...) {
    HISTORY *new_node = (HISTORY *)malloc(sizeof(HISTORY));
    new_node->msg = strdup(format);
    new_node->next = NULL;
    if (hist_head == NULL) { hist_head = new_node; hist_tail = new_node; }
    else { hist_tail->next = new_node; hist_tail = new_node; }
}

void add_mips_instr(char *code) {
    MIPS_INSTR *new_node = (MIPS_INSTR *)malloc(sizeof(MIPS_INSTR));
    new_node->code = strdup(code);
    new_node->next = NULL;
    if (mips_head == NULL) { mips_head = new_node; mips_tail = new_node; }
    else { mips_tail->next = new_node; mips_tail = new_node; }
}

VARIABLE *find_variable(char *name) {
    VARIABLE *current = var_head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->next;
    }
    return NULL; 
}

VARIABLE *add_variable(char *name, DataType type) {
    VARIABLE *new_var = (VARIABLE *)malloc(sizeof(VARIABLE));
    new_var->name = strdup(name);
    new_var->type = type;
    new_var->value = 0; 
    new_var->next = NULL;
    if (var_head == NULL) { var_head = new_var; var_tail = new_var; }
    else { var_tail->next = new_var; var_tail = new_var; }
    return new_var;
}

void add_error(char *msg, int line) {
    ERROR *new_err = (ERROR *)malloc(sizeof(ERROR));
    new_err->msg = strdup(msg);
    new_err->line = line;
    new_err->next = NULL;
    if (err_head == NULL) { err_head = new_err; err_tail = new_err; }
    else { err_tail->next = new_err; err_tail = new_err; }
    has_errors = 1;
}

// --- VALIDATION FUNCTIONS ---
int is_valid_identifier(char *str) {
    if (str == NULL || str[0] == '\0') return 0;
    
    // Check if first character is digit
    if (isdigit(str[0])) return 0;
    
    // Check if first character is valid (letter or underscore)
    if (!isalpha(str[0]) && str[0] != '_') return 0;
    
    // Check rest of characters
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_') return 0;
    }
    
    return 1;
}

// --- 5. TOKENIZER ---
TokenType classify_word(char *str) {
    if (strcmp(str, "int") == 0) return TOKEN_KEYWORD;
    if (strcmp(str, "char") == 0) return TOKEN_KEYWORD;
    return TOKEN_IDENTIFIER; 
}

int is_symbol(char c) {
    return (c == ',' || c == ';' || c == '=' || c == '+' || 
            c == '-' || c == '*' || c == '/' || c == '(' || c == ')');
}

void add_token(char *start, int length) {
    TOKEN *new_node = (TOKEN *)malloc(sizeof(TOKEN));
    new_node->value = (char *)malloc(length + 1);
    strncpy(new_node->value, start, length);
    new_node->value[length] = '\0';
    new_node->next = NULL;
    new_node->line = line_number; 

    if (is_symbol(new_node->value[0])) new_node->type = TOKEN_SYMBOL;
    else if (isdigit(new_node->value[0])) new_node->type = TOKEN_NUMBER;
    else new_node->type = classify_word(new_node->value);

    if (head == NULL) { head = new_node; tail = new_node; }
    else { tail->next = new_node; tail = new_node; }

    token_count++;
}

void tokenize_string(char *str) {
    int i = 0;
    line_number = 1; 
    while (str[i] != '\0') {
        if (isspace(str[i])) {
            if (str[i] == '\n') line_number++;
            i++; continue;
        }
        if (is_symbol(str[i])) { 
            add_token(&str[i], 1); i++; continue; 
        }
        if (isalnum(str[i]) || str[i] == '_') {
            int start = i;
            while (isalnum(str[i]) || str[i] == '_') { i++; }
            add_token(&str[start], i - start);
            continue;
        }
        i++; 
    }
}

// --- 6. AST PARSER ---
ASTNode* create_node(ASTNodeType type) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->line = (parser_pos != NULL) ? parser_pos->line : 0;
    node->has_error = 0;
    return node;
}

ASTNode* parse_expression();
ASTNode* parse_term();
ASTNode* parse_factor();

ASTNode* parse_factor() {
    if (parser_pos == NULL) return NULL;
    ASTNode *node = NULL;

    if (parser_pos->type == TOKEN_NUMBER) {
        node = create_node(AST_NUM);
        node->int_val = atoi(parser_pos->value);
        parser_pos = parser_pos->next;
    } 
    else if (parser_pos->type == TOKEN_IDENTIFIER) {
        node = create_node(AST_VAR);
        node->var_name = strdup(parser_pos->value);
        
        // Check if variable exists
        VARIABLE *var = find_variable(parser_pos->value);
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", parser_pos->value);
            add_error(buffer, parser_pos->line);
            node->has_error = 1;
        }
        
        parser_pos = parser_pos->next;
    } 
    else if (parser_pos->type == TOKEN_SYMBOL && strcmp(parser_pos->value, "(") == 0) {
        parser_pos = parser_pos->next; 
        node = parse_expression();
        if (parser_pos != NULL && strcmp(parser_pos->value, ")") == 0) {
            parser_pos = parser_pos->next; 
        }
    }
    else if (parser_pos->type == TOKEN_SYMBOL && 
             (strcmp(parser_pos->value, "-") == 0 || strcmp(parser_pos->value, "+") == 0)) {
        // Handle unary operators
        char op = parser_pos->value[0];
        int op_line = parser_pos->line;
        parser_pos = parser_pos->next;
        
        // Check if next token is also an operator (invalid sequence like --)
        if (parser_pos != NULL && parser_pos->type == TOKEN_SYMBOL &&
            (strcmp(parser_pos->value, "-") == 0 || strcmp(parser_pos->value, "+") == 0 ||
             strcmp(parser_pos->value, "*") == 0 || strcmp(parser_pos->value, "/") == 0)) {
            char buffer[200];
            sprintf(buffer, "Error: Invalid operator sequence '%c%s'. Use parentheses like %c(%s...) for valid arithmetic.", 
                    op, parser_pos->value, op, parser_pos->value);
            add_error(buffer, op_line);
            node = create_node(AST_NUM);
            node->int_val = 0;
            node->has_error = 1;
            return node;
        }
        
        node = parse_factor();
        if (node != NULL && op == '-') {
            // Create a negation node (0 - value)
            ASTNode *neg_node = create_node(AST_OP);
            neg_node->op = '-';
            neg_node->line = op_line;
            ASTNode *zero = create_node(AST_NUM);
            zero->int_val = 0;
            neg_node->left = zero;
            neg_node->right = node;
            node = neg_node;
        }
    }
    return node;
}

ASTNode* parse_term() {
    ASTNode *left = parse_factor();
    while (parser_pos != NULL && (strcmp(parser_pos->value, "*") == 0 || strcmp(parser_pos->value, "/") == 0)) {
        ASTNode *node = create_node(AST_OP);
        node->op = parser_pos->value[0];
        parser_pos = parser_pos->next;
        node->left = left;
        node->right = parse_factor();
        
        // Propagate errors
        if ((left && left->has_error) || (node->right && node->right->has_error)) {
            node->has_error = 1;
        }
        
        left = node; 
    }
    return left;
}

ASTNode* parse_expression() {
    ASTNode *left = parse_term();
    while (parser_pos != NULL && (strcmp(parser_pos->value, "+") == 0 || strcmp(parser_pos->value, "-") == 0)) {
        ASTNode *node = create_node(AST_OP);
        node->op = parser_pos->value[0];
        parser_pos = parser_pos->next;
        node->left = left;
        node->right = parse_term();
        
        // Propagate errors
        if ((left && left->has_error) || (node->right && node->right->has_error)) {
            node->has_error = 1;
        }
        
        left = node; 
    }
    return left;
}

// --- 7. AST EVALUATOR ---
int eval_ast(ASTNode *node) {
    if (node == NULL) return 0;
    if (node->has_error) return 0;
    
    if (node->type == AST_NUM) return node->int_val;
    if (node->type == AST_VAR) {
        VARIABLE *v = find_variable(node->var_name);
        if (v) return v->value;
        return 0; 
    }
    if (node->type == AST_OP) {
        int left = eval_ast(node->left);
        int right = eval_ast(node->right);
        if (node->op == '+') return left + right;
        if (node->op == '-') return left - right;
        if (node->op == '*') return left * right;
        if (node->op == '/') return (right != 0) ? left / right : 0;
    }
    return 0;
}

// --- 8. MIPS GENERATOR (REGISTER BASED - OPTIMIZED) ---
void gen_mips_ast(ASTNode *node, int reg) {
    if (node == NULL || node->has_error) return;
    char buffer[100];

    if (node->type == AST_NUM) {
        sprintf(buffer, "    daddiu r%d, r0, %d", reg, node->int_val);
        add_mips_instr(buffer);
    } 
    else if (node->type == AST_VAR) {
        sprintf(buffer, "    ld r%d, %s(r0)", reg, node->var_name);
        add_mips_instr(buffer);
    } 
    else if (node->type == AST_OP) {
        gen_mips_ast(node->left, reg);  
        gen_mips_ast(node->right, reg + 1); 

        if (node->op == '+') {
            sprintf(buffer, "    daddu r%d, r%d, r%d", reg, reg, reg + 1);
        }
        else if (node->op == '-') {
            sprintf(buffer, "    dsubu r%d, r%d, r%d", reg, reg, reg + 1);
        }
        else if (node->op == '*') {
            sprintf(buffer, "    dmult r%d, r%d", reg, reg + 1);
            add_mips_instr(buffer);
            sprintf(buffer, "    mflo r%d", reg);
        }
        else if (node->op == '/') {
            sprintf(buffer, "    ddiv r%d, r%d", reg, reg + 1);
            add_mips_instr(buffer);
            sprintf(buffer, "    mflo r%d", reg);
        }
        add_mips_instr(buffer);
    }
}

// --- 9. MAIN PARSER ---
void parse_tokens() {
    parser_pos = head; 
    DataType current_type = TYPE_UNKNOWN; 
    char log_buffer[256]; 
    char mips_buffer[256];

    while (parser_pos != NULL) {
        TOKEN *current = parser_pos; 

        if (current->type == TOKEN_KEYWORD) {
            if (strcmp(current->value, "int") == 0) current_type = TYPE_INT;
            if (strcmp(current->value, "char") == 0) current_type = TYPE_CHAR;
            parser_pos = parser_pos->next;
            
            // Check if there's a token after the keyword
            if (parser_pos == NULL) {
                char buffer[200];
                sprintf(buffer, "Error: Expected variable name after '%s'", current->value);
                add_error(buffer, current->line);
                sprintf(log_buffer, "[Line %d] ERROR: Data type '%s' requires a variable name", 
                        current->line, current->value);
                log_event(log_buffer);
                current_type = TYPE_UNKNOWN;
                continue;
            }
            
            // Check if next token after keyword is a number
            if (parser_pos->type == TOKEN_NUMBER) {
                char buffer[200];
                sprintf(buffer, "Error: '%s' is not a valid variable name! Variable names cannot be pure numbers.", parser_pos->value);
                add_error(buffer, parser_pos->line);
                sprintf(log_buffer, "[Line %d] ERROR: Invalid variable name '%s' (cannot be a number)", parser_pos->line, parser_pos->value);
                log_event(log_buffer);
                // Skip this invalid declaration
                while (parser_pos != NULL && strcmp(parser_pos->value, ";") != 0) {
                    parser_pos = parser_pos->next;
                }
                if (parser_pos != NULL) parser_pos = parser_pos->next; // skip semicolon
                current_type = TYPE_UNKNOWN;
                continue;
            }
            
            // Check if next token is a comma or semicolon (invalid)
            if (parser_pos->type == TOKEN_SYMBOL && 
                (strcmp(parser_pos->value, ",") == 0 || strcmp(parser_pos->value, ";") == 0)) {
                char buffer[200];
                sprintf(buffer, "Error: Expected variable name after '%s', but found '%s'", 
                        current->value, parser_pos->value);
                add_error(buffer, parser_pos->line);
                sprintf(log_buffer, "[Line %d] ERROR: Data type '%s' requires a variable name", 
                        parser_pos->line, current->value);
                log_event(log_buffer);
                // Skip to next semicolon
                while (parser_pos != NULL && strcmp(parser_pos->value, ";") != 0) {
                    parser_pos = parser_pos->next;
                }
                if (parser_pos != NULL) parser_pos = parser_pos->next; // skip semicolon
                current_type = TYPE_UNKNOWN;
                continue;
            }
            
            // Check if next token is another invalid symbol
            if (parser_pos->type == TOKEN_SYMBOL && strcmp(parser_pos->value, "=") != 0) {
                char buffer[200];
                sprintf(buffer, "Error: Expected variable name after '%s', but found '%s'", 
                        current->value, parser_pos->value);
                add_error(buffer, parser_pos->line);
                sprintf(log_buffer, "[Line %d] ERROR: Invalid token '%s' after data type", 
                        parser_pos->line, parser_pos->value);
                log_event(log_buffer);
                // Skip to next semicolon
                while (parser_pos != NULL && strcmp(parser_pos->value, ";") != 0) {
                    parser_pos = parser_pos->next;
                }
                if (parser_pos != NULL) parser_pos = parser_pos->next; // skip semicolon
                current_type = TYPE_UNKNOWN;
                continue;
            }
        }
        else if (current->type == TOKEN_IDENTIFIER) {
            VARIABLE *target_var = NULL;
            char buffer[200];
            
            // Validate identifier name
            if (!is_valid_identifier(current->value)) {
                if (isdigit(current->value[0])) {
                    sprintf(buffer, "Error: Variable '%s' cannot start with a digit!", current->value);
                } else {
                    sprintf(buffer, "Error: Variable '%s' contains invalid characters! Only alphanumeric and underscore allowed.", current->value);
                }
                add_error(buffer, current->line);
                sprintf(log_buffer, "[Line %d] ERROR: Invalid variable name '%s'", current->line, current->value);
                log_event(log_buffer);
                parser_pos = parser_pos->next;
                continue;
            }
            
            parser_pos = parser_pos->next; 

            if (current_type != TYPE_UNKNOWN) { 
                // Declaration - check what follows
                if (parser_pos != NULL && 
                    parser_pos->type == TOKEN_SYMBOL && 
                    strcmp(parser_pos->value, ",") != 0 && 
                    strcmp(parser_pos->value, "=") != 0 && 
                    strcmp(parser_pos->value, ";") != 0) {
                    // Invalid token after variable declaration
                    sprintf(buffer, "Error: Expected '=', ',' or ';' after variable '%s', but found '%s'", 
                            current->value, parser_pos->value);
                    add_error(buffer, current->line);
                    sprintf(log_buffer, "[Line %d] ERROR: Invalid syntax after variable '%s'", 
                            current->line, current->value);
                    log_event(log_buffer);
                    
                    // Don't create the variable since declaration is invalid
                    target_var = NULL;
                    
                    // Skip to next semicolon to recover
                    while (parser_pos != NULL && strcmp(parser_pos->value, ";") != 0) {
                        parser_pos = parser_pos->next;
                    }
                    if (parser_pos != NULL) parser_pos = parser_pos->next; // skip semicolon
                    current_type = TYPE_UNKNOWN;
                    continue;
                }
                
                if (find_variable(current->value) != NULL) {
                    sprintf(buffer, "Error: Variable '%s' is already defined!", current->value);
                    add_error(buffer, current->line); 
                    sprintf(log_buffer, "[Line %d] ERROR: Failed to declare '%s' (Exists)", current->line, current->value);
                    log_event(log_buffer);
                } else {
                    target_var = add_variable(current->value, current_type);
                    char *t = (current_type == TYPE_INT) ? "INT" : "CHAR";
                    sprintf(log_buffer, "[Line %d] DECLARE: Created variable '%s' of type %s", current->line, current->value, t);
                    log_event(log_buffer);
                }
                
                // After declaring a variable, check what comes next
                // Valid: '=', ',', ';'
                // Invalid: another identifier without comma/semicolon
                if (parser_pos != NULL && parser_pos->type == TOKEN_IDENTIFIER) {
                    sprintf(buffer, "Error: Expected '=', ',' or ';' after variable '%s', but found identifier '%s'", 
                            current->value, parser_pos->value);
                    add_error(buffer, current->line);
                    sprintf(log_buffer, "[Line %d] ERROR: Missing separator between variables '%s' and '%s'", 
                            current->line, current->value, parser_pos->value);
                    log_event(log_buffer);
                    // Skip to next semicolon
                    while (parser_pos != NULL && strcmp(parser_pos->value, ";") != 0) {
                        parser_pos = parser_pos->next;
                    }
                    if (parser_pos != NULL) parser_pos = parser_pos->next;
                    current_type = TYPE_UNKNOWN;
                    continue;
                }
            } 
            else { 
                // Assignment without declaration or increment/decrement
                target_var = find_variable(current->value);
                if (target_var == NULL) {
                    sprintf(buffer, "Error: Variable '%s' is not declared!", current->value);
                    add_error(buffer, current->line);
                    sprintf(log_buffer, "[Line %d] ERROR: Failed to use '%s' (Not Declared)", current->line, current->value);
                    log_event(log_buffer);
                }
                
                // Check for post-increment (++) or post-decrement (--)
                if (parser_pos != NULL && parser_pos->type == TOKEN_SYMBOL && 
                    strcmp(parser_pos->value, "+") == 0) {
                    TOKEN *next = parser_pos->next;
                    if (next != NULL && strcmp(next->value, "+") == 0) {
                        // Post-increment: a++
                        parser_pos = next->next; // Skip both + tokens
                        
                        if (target_var != NULL) {
                            int old_val = target_var->value;
                            target_var->value++;
                            sprintf(log_buffer, "[Line %d] INCREMENT: Updated '%s' from %d to %d", 
                                    current->line, target_var->name, old_val, target_var->value);
                            log_event(log_buffer);
                            
                            // Generate MIPS: load, increment, store
                            sprintf(mips_buffer, "    ld r2, %s(r0)", target_var->name);
                            add_mips_instr(mips_buffer);
                            sprintf(mips_buffer, "    daddiu r2, r2, 1");
                            add_mips_instr(mips_buffer);
                            sprintf(mips_buffer, "    sd r2, %s(r0)", target_var->name);
                            add_mips_instr(mips_buffer);
                        }
                        continue;
                    }
                }
                else if (parser_pos != NULL && parser_pos->type == TOKEN_SYMBOL && 
                         strcmp(parser_pos->value, "-") == 0) {
                    TOKEN *next = parser_pos->next;
                    if (next != NULL && strcmp(next->value, "-") == 0) {
                        // Post-decrement: a--
                        parser_pos = next->next; // Skip both - tokens
                        
                        if (target_var != NULL) {
                            int old_val = target_var->value;
                            target_var->value--;
                            sprintf(log_buffer, "[Line %d] DECREMENT: Updated '%s' from %d to %d", 
                                    current->line, target_var->name, old_val, target_var->value);
                            log_event(log_buffer);
                            
                            // Generate MIPS: load, decrement, store
                            sprintf(mips_buffer, "    ld r2, %s(r0)", target_var->name);
                            add_mips_instr(mips_buffer);
                            sprintf(mips_buffer, "    daddiu r2, r2, -1");
                            add_mips_instr(mips_buffer);
                            sprintf(mips_buffer, "    sd r2, %s(r0)", target_var->name);
                            add_mips_instr(mips_buffer);
                        }
                        continue;
                    }
                }
            }

            // Handle assignment
            if (parser_pos != NULL && strcmp(parser_pos->value, "=") == 0) {
                parser_pos = parser_pos->next; 
                ASTNode *expr_tree = parse_expression();
                
                if (expr_tree != NULL) {
                    // Only proceed if no errors in expression and target variable exists
                    if (!expr_tree->has_error && target_var != NULL) {
                        int old_val = target_var->value;
                        target_var->value = eval_ast(expr_tree);
                        sprintf(log_buffer, "[Line %d] ASSIGN: Updated '%s' from %d to %d", 
                                current->line, target_var->name, old_val, target_var->value);
                        log_event(log_buffer);

                        gen_mips_ast(expr_tree, 2); 
                        sprintf(mips_buffer, "    sd r2, %s(r0)", target_var->name);
                        add_mips_instr(mips_buffer);
                    } else if (expr_tree->has_error) {
                        sprintf(log_buffer, "[Line %d] ERROR: Cannot assign to '%s' due to errors in expression", 
                                current->line, current->value);
                        log_event(log_buffer);
                    }
                }
            }
            // Handle comma (multiple declarations)
            else if (parser_pos != NULL && strcmp(parser_pos->value, ",") == 0) {
                // Check if we're in a declaration context
                if (current_type == TYPE_UNKNOWN) {
                    char buffer[200];
                    sprintf(buffer, "Error: Unexpected ',' - not in a declaration context");
                    add_error(buffer, parser_pos->line);
                    sprintf(log_buffer, "[Line %d] ERROR: Unexpected comma", parser_pos->line);
                    log_event(log_buffer);
                    parser_pos = parser_pos->next;
                } else {
                    parser_pos = parser_pos->next;
                    
                    // Check for consecutive commas or comma followed by invalid token
                    if (parser_pos == NULL) {
                        char buffer[200];
                        sprintf(buffer, "Error: Expected variable name after ','");
                        add_error(buffer, current->line);
                        sprintf(log_buffer, "[Line %d] ERROR: Missing variable name after comma", current->line);
                        log_event(log_buffer);
                        break;
                    }
                    
                    if (parser_pos->type == TOKEN_SYMBOL && strcmp(parser_pos->value, ",") == 0) {
                        char buffer[200];
                        sprintf(buffer, "Error: Multiple consecutive commas are not allowed");
                        add_error(buffer, parser_pos->line);
                        sprintf(log_buffer, "[Line %d] ERROR: Multiple consecutive commas detected", parser_pos->line);
                        log_event(log_buffer);
                        // Skip all consecutive commas
                        while (parser_pos != NULL && strcmp(parser_pos->value, ",") == 0) {
                            parser_pos = parser_pos->next;
                        }
                    } else if (parser_pos->type == TOKEN_SYMBOL && strcmp(parser_pos->value, ";") == 0) {
                        char buffer[200];
                        sprintf(buffer, "Error: Expected variable name after ',' but found ';'");
                        add_error(buffer, parser_pos->line);
                        sprintf(log_buffer, "[Line %d] ERROR: Comma followed by semicolon", parser_pos->line);
                        log_event(log_buffer);
                    } else if (parser_pos->type != TOKEN_IDENTIFIER) {
                        char buffer[200];
                        sprintf(buffer, "Error: Expected variable name after ',' but found '%s'", parser_pos->value);
                        add_error(buffer, parser_pos->line);
                        sprintf(log_buffer, "[Line %d] ERROR: Invalid token after comma", parser_pos->line);
                        log_event(log_buffer);
                        // Skip to semicolon
                        while (parser_pos != NULL && strcmp(parser_pos->value, ";") != 0) {
                            parser_pos = parser_pos->next;
                        }
                        current_type = TYPE_UNKNOWN;
                    }
                }
            }
        }
        else if (current->type == TOKEN_NUMBER || 
                (current->type == TOKEN_SYMBOL && strcmp(current->value, "(") == 0)) {
            
            ASTNode *expr_tree = parse_expression();
            
            if (expr_tree != NULL && !expr_tree->has_error) {
                int result = eval_ast(expr_tree);
                sprintf(log_buffer, "[Line %d] EXPRESSION: Evaluated standalone expression = %d", 
                        current->line, result);
                log_event(log_buffer);

                gen_mips_ast(expr_tree, 2);
            }
        }
        else if (current->type == TOKEN_SYMBOL && strcmp(current->value, "+") == 0) {
            // Check for pre-increment (++a)
            TOKEN *next = parser_pos;
            if (next != NULL && strcmp(next->value, "+") == 0) {
                parser_pos = next->next; // Skip second +
                
                if (parser_pos != NULL && parser_pos->type == TOKEN_IDENTIFIER) {
                    char *var_name = parser_pos->value;
                    VARIABLE *target_var = find_variable(var_name);
                    
                    if (target_var == NULL) {
                        char buffer[200];
                        sprintf(buffer, "Error: Variable '%s' is not declared!", var_name);
                        add_error(buffer, parser_pos->line);
                        sprintf(log_buffer, "[Line %d] ERROR: Failed to use '%s' (Not Declared)", 
                                parser_pos->line, var_name);
                        log_event(log_buffer);
                    } else {
                        int old_val = target_var->value;
                        target_var->value++;
                        sprintf(log_buffer, "[Line %d] PRE-INCREMENT: Updated '%s' from %d to %d", 
                                parser_pos->line, target_var->name, old_val, target_var->value);
                        log_event(log_buffer);
                        
                        // Generate MIPS: load, increment, store
                        sprintf(mips_buffer, "    ld r2, %s(r0)", target_var->name);
                        add_mips_instr(mips_buffer);
                        sprintf(mips_buffer, "    daddiu r2, r2, 1");
                        add_mips_instr(mips_buffer);
                        sprintf(mips_buffer, "    sd r2, %s(r0)", target_var->name);
                        add_mips_instr(mips_buffer);
                    }
                    
                    parser_pos = parser_pos->next; // Move past variable name
                } else {
                    // Not a pre-increment, just skip the token
                    parser_pos = parser_pos->next;
                }
            } else {
                parser_pos = parser_pos->next;
            }
        }
        else if (current->type == TOKEN_SYMBOL && strcmp(current->value, "-") == 0) {
            // Check for pre-decrement (--a)
            TOKEN *next = parser_pos;
            if (next != NULL && next->type == TOKEN_SYMBOL && strcmp(next->value, "-") == 0) {
                parser_pos = next->next; // Skip second -
                
                if (parser_pos != NULL && parser_pos->type == TOKEN_IDENTIFIER) {
                    char *var_name = parser_pos->value;
                    VARIABLE *target_var = find_variable(var_name);
                    
                    if (target_var == NULL) {
                        char buffer[200];
                        sprintf(buffer, "Error: Variable '%s' is not declared!", var_name);
                        add_error(buffer, parser_pos->line);
                        sprintf(log_buffer, "[Line %d] ERROR: Failed to use '%s' (Not Declared)", 
                                parser_pos->line, var_name);
                        log_event(log_buffer);
                    } else {
                        int old_val = target_var->value;
                        target_var->value--;
                        sprintf(log_buffer, "[Line %d] PRE-DECREMENT: Updated '%s' from %d to %d", 
                                parser_pos->line, target_var->name, old_val, target_var->value);
                        log_event(log_buffer);
                        
                        // Generate MIPS: load, decrement, store
                        sprintf(mips_buffer, "    ld r2, %s(r0)", target_var->name);
                        add_mips_instr(mips_buffer);
                        sprintf(mips_buffer, "    daddiu r2, r2, -1");
                        add_mips_instr(mips_buffer);
                        sprintf(mips_buffer, "    sd r2, %s(r0)", target_var->name);
                        add_mips_instr(mips_buffer);
                    }
                    
                    parser_pos = parser_pos->next; // Move past variable name
                } else {
                    // Not a valid pre-decrement, just skip the token
                    parser_pos = parser_pos->next;
                }
            } else {
                parser_pos = parser_pos->next;
            }
        }
        else if (current->type == TOKEN_SYMBOL && strcmp(current->value, ";") == 0) {
            current_type = TYPE_UNKNOWN;
            parser_pos = parser_pos->next;
        }
        else if (current->type == TOKEN_SYMBOL && strcmp(current->value, ",") == 0) {
            parser_pos = parser_pos->next;
        }
        else {
            parser_pos = parser_pos->next; 
        }
    }
}

// --- 10. FILE GENERATOR ---
void generate_mips_file(const char* filename) {
    if (has_errors) {
        printf(">> MIPS64 code generation skipped due to errors.\n");
        return;
    }
    
    FILE *f = fopen(filename, "w");
    if (f == NULL) { printf("Error writing MIPS file.\n"); return; }

    fprintf(f, "    .data\n");
    VARIABLE *v = var_head;
    while (v != NULL) {
        fprintf(f, "%s: .word 0, 0\n", v->name);
        v = v->next;
    }

    fprintf(f, "\n    .text\n");
    fprintf(f, "main:\n");

    MIPS_INSTR *m = mips_head;
    while (m != NULL) {
        fprintf(f, "%s\n", m->code);
        m = m->next;
    }
    
    fprintf(f, "    daddiu r2, r0, 0\n"); 
    
    fclose(f);
    printf(">> MIPS64 code generated in '%s'\n", filename);
}

// --- 11. CLEANUP FUNCTIONS ---
void cleanup_all() {
    // Free tokens
    TOKEN *t = head;
    while (t != NULL) {
        TOKEN *next = t->next;
        free(t->value);
        free(t);
        t = next;
    }
    head = NULL;
    tail = NULL;
    token_count = 0;
    
    // Free variables
    VARIABLE *v = var_head;
    while (v != NULL) {
        VARIABLE *next = v->next;
        free(v->name);
        free(v);
        v = next;
    }
    var_head = NULL;
    var_tail = NULL;
    
    // Free errors
    ERROR *e = err_head;
    while (e != NULL) {
        ERROR *next = e->next;
        free(e->msg);
        free(e);
        e = next;
    }
    err_head = NULL;
    err_tail = NULL;
    
    // Free history
    HISTORY *h = hist_head;
    while (h != NULL) {
        HISTORY *next = h->next;
        free(h->msg);
        free(h);
        h = next;
    }
    hist_head = NULL;
    hist_tail = NULL;
    
    // Free MIPS instructions
    MIPS_INSTR *m = mips_head;
    while (m != NULL) {
        MIPS_INSTR *next = m->next;
        free(m->code);
        free(m);
        m = next;
    }
    mips_head = NULL;
    mips_tail = NULL;
    
    // Reset state
    parser_pos = NULL;
    line_number = 1;
    has_errors = 0;
}

void print_results() {
    printf("\n=== CHRONOLOGICAL HISTORY ===\n");
    HISTORY *h = hist_head;
    while (h != NULL) { printf("  %s\n", h->msg); h = h->next; }

    printf("\n=== SYMBOL TABLE ===\n");
    VARIABLE *v = var_head;
    if (v == NULL) printf("  (No variables declared)\n");
    while (v != NULL) {
        printf("  VAR: %-10s | VAL: %d\n", v->name, v->value);
        v = v->next;
    }

    printf("\n=== ERROR LOG ===\n");
    ERROR *e = err_head;
    if (e == NULL) printf("  (No errors found)\n");
    while (e != NULL) { printf("  Line %d: %s\n", e->line, e->msg); e = e->next; }
    
    generate_mips_file("output.txt");
}