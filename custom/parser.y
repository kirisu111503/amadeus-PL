%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "types.h"
#include "mips_bin.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int yylineno;

void yyerror(const char *s);

// Global variables
VARIABLE *var_head = NULL;
VARIABLE *var_tail = NULL;
ERROR *err_head = NULL;
ERROR *err_tail = NULL;
HISTORY *hist_head = NULL;
HISTORY *hist_tail = NULL;
MIPS_INSTR *mips_head = NULL;
MIPS_INSTR *mips_tail = NULL;
int has_errors = 0;
int line_number = 1;
DataType current_type = TYPE_UNKNOWN;

// Function declarations
void read_output_file(const char* filename);
void create_empty_file(const char* filename);
void log_event(const char *format, ...);
void add_error(char *msg, int line);
void show_errors();
void add_mips_instr(char *code);
VARIABLE *find_variable(char *name);
VARIABLE *add_variable(char *name, DataType type);
int is_valid_identifier(char *str);
ASTNode* create_node(ASTNodeType type, int line);
int eval_ast(ASTNode *node);
void gen_mips_ast(ASTNode *node, int reg);
void generate_mips_file(const char* filename);
void print_results();
void cleanup_all();

%}

%union {
    int num;
    char *str;
    struct ASTNode *node;
    DataType dtype;
}

%token <num> NUMBER
%token <num> CHAR_LITERAL
%token <str> IDENTIFIER
%token INT CHAR
%token ASSIGN SEMICOLON COMMA
%token PLUS MINUS MULTIPLY DIVIDE
%token LPAREN RPAREN
%token INCREMENT DECREMENT
%token CDISPLAY

%type <node> expression term factor
%type <dtype> type

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right UNARY

%%

program:
    /* empty */
    | program statement
    ;

statement:
    declaration SEMICOLON
    | assignment SEMICOLON
    | expression_statement SEMICOLON
    | increment_decrement SEMICOLON
    | cdisplay_statement SEMICOLON
    | error SEMICOLON { yyerrok; }
    ;

type:
    INT     { $$ = TYPE_INT; current_type = TYPE_INT; }
    | CHAR  { $$ = TYPE_CHAR; current_type = TYPE_CHAR; }
    ;

declaration:
    type var_list   { current_type = TYPE_UNKNOWN; }
    ;

var_list:
    var_decl
    | var_list COMMA var_decl
    ;

var_decl:
    IDENTIFIER {
        char log_buffer[256];
        if (!is_valid_identifier($1)) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' contains invalid characters!", $1);
            add_error(buffer, line_number);
            sprintf(log_buffer, "[Line %d] ERROR: Invalid variable name '%s'", line_number, $1);
            log_event(log_buffer);
        } else if (find_variable($1) != NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is already defined!", $1);
            add_error(buffer, line_number);
            sprintf(log_buffer, "[Line %d] ERROR: Failed to declare '%s' (Exists)", line_number, $1);
            log_event(log_buffer);
        } else {
            VARIABLE *var = add_variable($1, current_type);
            char *t = (current_type == TYPE_INT) ? "INT" : "CHAR";
            sprintf(log_buffer, "[Line %d] DECLARE: Created variable '%s' of type %s", line_number, $1, t);
            log_event(log_buffer);
        }
        free($1);
    }
    | IDENTIFIER ASSIGN expression {
        char log_buffer[256];
        char mips_buffer[256];
        
        if (!is_valid_identifier($1)) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' contains invalid characters!", $1);
            add_error(buffer, line_number);
        } else {
            VARIABLE *var = find_variable($1);
            if (var != NULL) {
                char buffer[200];
                sprintf(buffer, "Error: Variable '%s' is already defined!", $1);
                add_error(buffer, line_number);
            } else {
                var = add_variable($1, current_type);
                char *t = (current_type == TYPE_INT) ? "INT" : "CHAR";
                sprintf(log_buffer, "[Line %d] DECLARE: Created variable '%s' of type %s", line_number, $1, t);
                log_event(log_buffer);
                
                if ($3 != NULL && !$3->has_error) {
                    int old_val = var->value;
                    var->value = eval_ast($3);
                    
                    // Log with ASCII character if it's a char type and printable
                    if (current_type == TYPE_CHAR && var->value >= 32 && var->value <= 126) {
                        sprintf(log_buffer, "[Line %d] ASSIGN: Updated '%s' from %d to %d (ASCII: '%c')", 
                                line_number, var->name, old_val, var->value, (char)var->value);
                    } else {
                        sprintf(log_buffer, "[Line %d] ASSIGN: Updated '%s' from %d to %d", 
                                line_number, var->name, old_val, var->value);
                    }
                    log_event(log_buffer);
                    
                    gen_mips_ast($3, 2);
                    sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
                    add_mips_instr(mips_buffer);
                }
            }
        }
        free($1);
    }
    ;

assignment:
    IDENTIFIER ASSIGN expression {
        char log_buffer[256];
        char mips_buffer[256];
        VARIABLE *var = find_variable($1);
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", $1);
            add_error(buffer, line_number);
            sprintf(log_buffer, "[Line %d] ERROR: Failed to use '%s' (Not Declared)", line_number, $1);
            log_event(log_buffer);
        } else if ($3 != NULL && !$3->has_error) {
            int old_val = var->value;
            var->value = eval_ast($3);
            
            // Log with ASCII character if it's a char type and printable
            if (var->type == TYPE_CHAR && var->value >= 32 && var->value <= 126) {
                sprintf(log_buffer, "[Line %d] ASSIGN: Updated '%s' from %d to %d (ASCII: '%c')", 
                        line_number, var->name, old_val, var->value, (char)var->value);
            } else {
                sprintf(log_buffer, "[Line %d] ASSIGN: Updated '%s' from %d to %d", 
                        line_number, var->name, old_val, var->value);
            }
            log_event(log_buffer);
            
            gen_mips_ast($3, 2);
            sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free($1);
    }
    ;

expression_statement:
    expression {
        if ($1 != NULL && !$1->has_error) {
            char log_buffer[256];
            int result = eval_ast($1);
            sprintf(log_buffer, "[Line %d] EXPRESSION: Evaluated standalone expression = %d", 
                    line_number, result);
            log_event(log_buffer);
            gen_mips_ast($1, 2);
        }
    }
    ;

increment_decrement:
    IDENTIFIER INCREMENT {
        char log_buffer[256];
        char mips_buffer[256];
        VARIABLE *var = find_variable($1);
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", $1);
            add_error(buffer, line_number);
        } else {
            int old_val = var->value;
            var->value++;
            sprintf(log_buffer, "[Line %d] INCREMENT: Updated '%s' from %d to %d", 
                    line_number, var->name, old_val, var->value);
            log_event(log_buffer);
            
            sprintf(mips_buffer, "    ld r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    daddiu r2, r2, 1");
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free($1);
    }
    | IDENTIFIER DECREMENT {
        char log_buffer[256];
        char mips_buffer[256];
        VARIABLE *var = find_variable($1);
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", $1);
            add_error(buffer, line_number);
        } else {
            int old_val = var->value;
            var->value--;
            sprintf(log_buffer, "[Line %d] DECREMENT: Updated '%s' from %d to %d", 
                    line_number, var->name, old_val, var->value);
            log_event(log_buffer);
            
            sprintf(mips_buffer, "    ld r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    daddiu r2, r2, -1");
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free($1);
    }
    | INCREMENT IDENTIFIER {
        char log_buffer[256];
        char mips_buffer[256];
        VARIABLE *var = find_variable($2);
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", $2);
            add_error(buffer, line_number);
        } else {
            int old_val = var->value;
            var->value++;
            sprintf(log_buffer, "[Line %d] PRE-INCREMENT: Updated '%s' from %d to %d", 
                    line_number, var->name, old_val, var->value);
            log_event(log_buffer);
            
            sprintf(mips_buffer, "    ld r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    daddiu r2, r2, 1");
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free($2);
    }
    | DECREMENT IDENTIFIER {
        char log_buffer[256];
        char mips_buffer[256];
        VARIABLE *var = find_variable($2);
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", $2);
            add_error(buffer, line_number);
        } else {
            int old_val = var->value;
            var->value--;
            sprintf(log_buffer, "[Line %d] PRE-DECREMENT: Updated '%s' from %d to %d", 
                    line_number, var->name, old_val, var->value);
            log_event(log_buffer);
            
            sprintf(mips_buffer, "    ld r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    daddiu r2, r2, -1");
            add_mips_instr(mips_buffer);
            sprintf(mips_buffer, "    sd r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free($2);
    }
    ;

expression:
    term
    | expression PLUS term {
        $$ = create_node(AST_OP, line_number);
        $$->op = '+';
        $$->left = $1;
        $$->right = $3;
        if (($1 && $1->has_error) || ($3 && $3->has_error)) {
            $$->has_error = 1;
        }
    }
    | expression MINUS term {
        $$ = create_node(AST_OP, line_number);
        $$->op = '-';
        $$->left = $1;
        $$->right = $3;
        if (($1 && $1->has_error) || ($3 && $3->has_error)) {
            $$->has_error = 1;
        }
    }
    ;

term:
    factor
    | term MULTIPLY factor {
        $$ = create_node(AST_OP, line_number);
        $$->op = '*';
        $$->left = $1;
        $$->right = $3;
        if (($1 && $1->has_error) || ($3 && $3->has_error)) {
            $$->has_error = 1;
        }
    }
    | term DIVIDE factor {
        $$ = create_node(AST_OP, line_number);
        $$->op = '/';
        $$->left = $1;
        $$->right = $3;
        if (($1 && $1->has_error) || ($3 && $3->has_error)) {
            $$->has_error = 1;
        }
    }
    ;

factor:
    NUMBER {
        $$ = create_node(AST_NUM, line_number);
        $$->int_val = $1;
    }
    | CHAR_LITERAL {
        $$ = create_node(AST_NUM, line_number);
        $$->int_val = $1;
    }
    | IDENTIFIER {
        $$ = create_node(AST_VAR, line_number);
        $$->var_name = strdup($1);
        
        VARIABLE *var = find_variable($1);
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", $1);
            add_error(buffer, line_number);
            $$->has_error = 1;
        }
        free($1);
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    | MINUS factor %prec UNARY {
        $$ = create_node(AST_OP, line_number);
        $$->op = '-';
        ASTNode *zero = create_node(AST_NUM, line_number);
        zero->int_val = 0;
        $$->left = zero;
        $$->right = $2;
        if ($2 && $2->has_error) {
            $$->has_error = 1;
        }
    }
    | PLUS factor %prec UNARY {
        $$ = $2;
    }
    ;



cdisplay_statement:
    CDISPLAY LPAREN expression RPAREN {
        if ($3 != NULL && !$3->has_error) {
            char log_buffer[256];
            int result = eval_ast($3);
            
            // Print to console
            printf("%d\n", result);
            
            // Log the event
            sprintf(log_buffer, "[Line %d] CDISPLAY: Printed value = %d", 
                    line_number, result);
            log_event(log_buffer);
            
            // Generate MIPS code to evaluate expression
            gen_mips_ast($3, 2);
        }
    }
    | CDISPLAY LPAREN IDENTIFIER RPAREN {
        char log_buffer[256];
        VARIABLE *var = find_variable($3);
        
        if (var == NULL) {
            char buffer[200];
            sprintf(buffer, "Error: Variable '%s' is not declared!", $3);
            add_error(buffer, line_number);
        } else {
            // Print based on type
            if (var->type == TYPE_CHAR && var->value >= 32 && var->value <= 126) {
                printf("%c\n", (char)var->value);
                sprintf(log_buffer, "[Line %d] CDISPLAY: Printed '%s' = %d (ASCII: '%c')", 
                        line_number, var->name, var->value, (char)var->value);
            } else {
                printf("%d\n", var->value);
                sprintf(log_buffer, "[Line %d] CDISPLAY: Printed '%s' = %d", 
                        line_number, var->name, var->value);
            }
            log_event(log_buffer);
            
            // Generate MIPS code to load variable
            char mips_buffer[256];
            sprintf(mips_buffer, "    ld r2, %s(r0)", var->name);
            add_mips_instr(mips_buffer);
        }
        free($3);
    }
    ;

%%



void yyerror(const char *s) {
    char buffer[256];
    sprintf(buffer, "Syntax error: %s", s);
    add_error(buffer, line_number);
}

// Helper function implementations
void log_event(const char *format, ...) {
    HISTORY *new_node = (HISTORY *)malloc(sizeof(HISTORY));
    new_node->msg = strdup(format);
    new_node->next = NULL;
    if (hist_head == NULL) { 
        hist_head = new_node; 
        hist_tail = new_node; 
    } else { 
        hist_tail->next = new_node; 
        hist_tail = new_node; 
    }
}

void add_mips_instr(char *code) {
    MIPS_INSTR *new_node = (MIPS_INSTR *)malloc(sizeof(MIPS_INSTR));
    new_node->code = strdup(code);
    new_node->next = NULL;
    if (mips_head == NULL) { 
        mips_head = new_node; 
        mips_tail = new_node; 
    } else { 
        mips_tail->next = new_node; 
        mips_tail = new_node; 
    }
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
    if (var_head == NULL) { 
        var_head = new_var; 
        var_tail = new_var; 
    } else { 
        var_tail->next = new_var; 
        var_tail = new_var; 
    }
    return new_var;
}

void add_error(char *msg, int line) {
    ERROR *new_err = (ERROR *)malloc(sizeof(ERROR));
    new_err->msg = strdup(msg);
    new_err->line = line;
    new_err->next = NULL;
    if (err_head == NULL) { 
        err_head = new_err; 
        err_tail = new_err; 
    } else { 
        err_tail->next = new_err; 
        err_tail = new_err; 
    }
    has_errors = 1;
}

int is_valid_identifier(char *str) {
    if (str == NULL || str[0] == '\0') return 0;
    if (!isalpha(str[0]) && str[0] != '_') return 0;
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_') return 0;
    }
    return 1;
}

ASTNode* create_node(ASTNodeType type, int line) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->line = line;
    node->has_error = 0;
    node->int_val = 0;
    node->var_name = NULL;
    node->op = 0;
    return node;
}

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

void generate_mips_file(const char* filename) {
    if (has_errors) {
        printf(">> MIPS64 code generation skipped due to errors.\n");
        return;
    }
    
    FILE *f = fopen(filename, "w");
    if (f == NULL) { 
        printf("Error writing MIPS file.\n"); 
        return; 
    }

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
    
    fclose(f);
    printf(">> MIPS64 code generated in '%s'\n", filename);
}

void print_results() {
    printf("\n=== CHRONOLOGICAL HISTORY ===\n");
    HISTORY *h = hist_head;
    while (h != NULL) { 
        printf("  %s\n", h->msg); 
        h = h->next; 
    }

    printf("\n=== SYMBOL TABLE ===\n");
    VARIABLE *v = var_head;
    if (v == NULL) printf("  (No variables declared)\n");
    while (v != NULL) {
        char *type_str = (v->type == TYPE_INT) ? "INT" : "CHAR";
        if (v->type == TYPE_CHAR && v->value >= 32 && v->value <= 126) {
            printf("  VAR: %-10s | TYPE: %-4s | VAL: %-3d (ASCII: '%c')\n", 
                   v->name, type_str, v->value, (char)v->value);
        } else {
            printf("  VAR: %-10s | TYPE: %-4s | VAL: %d\n", 
                   v->name, type_str, v->value);
        }
        v = v->next;
    }

    printf("\n=== ERROR LOG ===\n");
    ERROR *e = err_head;
    if (e == NULL) printf("  (No errors found)\n");
    while (e != NULL) { 
        printf("  Line %d: %s\n", e->line, e->msg); 
        e = e->next; 
    }
    
    generate_mips_file("output.txt");
}

void cleanup_all() {
    VARIABLE *v = var_head;
    while (v != NULL) {
        VARIABLE *next = v->next;
        free(v->name);
        free(v);
        v = next;
    }
    var_head = NULL;
    var_tail = NULL;
    
    ERROR *e = err_head;
    while (e != NULL) {
        ERROR *next = e->next;
        free(e->msg);
        free(e);
        e = next;
    }
    err_head = NULL;
    err_tail = NULL;
    
    HISTORY *h = hist_head;
    while (h != NULL) {
        HISTORY *next = h->next;
        free(h->msg);
        free(h);
        h = next;
    }
    hist_head = NULL;
    hist_tail = NULL;
    
    MIPS_INSTR *m = mips_head;
    while (m != NULL) {
        MIPS_INSTR *next = m->next;
        free(m->code);
        free(m);
        m = next;
    }
    mips_head = NULL;
    mips_tail = NULL;
    
    line_number = 1;
    has_errors = 0;
}

void read_output_file(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        return;
    }
    char line[256];
    printf("\n\n=== CONTENTS OF '%s' ===\n", filename);
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
    printf("=== END OF FILE ===\n\n");
}

// --- FUNCTION TO CREATE EMPTY FILE ---
void create_empty_file(const char* filename) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fclose(file);
    }
}

void show_errors(){
    if(err_head == NULL){
        return;
    }

    printf("\n=== ERROR LOG ===\n"); // Added header for readability
    ERROR *current_err = err_head;
    
    // BUG FIX: Changed from (!current_err) to (current_err != NULL)
    while(current_err != NULL){ 
        printf("[LINE %d]: %s\n", current_err->line, current_err->msg);
        current_err = current_err->next;
    }
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

    // 1. Parse the file
    yyparse();

    // 2. Check for errors immediately after parsing
    if(has_errors || err_head != NULL) {
        // Show the errors found during parsing/lexing
        show_errors();

        printf("\n>> Errors detected. Creating empty output files...\n");
        
        // Clear files because build failed
        create_empty_file("output.txt");
        create_empty_file("program.txt");
        
        printf(">> All output files cleared due to errors.\n");
        printf(">> Please fix the errors above and try again.\n");
    } 
    else {
        // 3. No errors: Proceed with Code Generation
        
        // Generate Assembly Text
        generate_mips_file("output.txt");
        
        // Generate Binary (assuming this function is defined in mips_bin.h)
        // Ensure this function exists in your included headers
        generate_binary_file("output.txt", "program.txt"); 
        
        printf(">> Program finished successfully.\n");
    }

    // 4. Cleanup memory
    cleanup_all();
    
    return 0;
}