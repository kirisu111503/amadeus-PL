#ifndef TYPES_H
#define TYPES_H

// Data type enumeration
typedef enum { 
    TYPE_INT, 
    TYPE_CHAR, 
    TYPE_UNKNOWN 
} DataType;

// AST node type enumeration
typedef enum { 
    AST_NUM, 
    AST_VAR, 
    AST_OP 
} ASTNodeType;

// AST Node structure
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

// Variable structure
typedef struct VARIABLE {
    char *name;
    DataType type;
    int value;
    struct VARIABLE *next;
} VARIABLE;

// Error structure
typedef struct ERROR {
    char *msg;
    int line;
    struct ERROR *next;
} ERROR;

// History structure
typedef struct HISTORY {
    char *msg;
    struct HISTORY *next;
} HISTORY;

// MIPS instruction structure
typedef struct MIPS_INSTR {
    char *code;
    struct MIPS_INSTR *next;
} MIPS_INSTR;




#endif /* TYPES_H */