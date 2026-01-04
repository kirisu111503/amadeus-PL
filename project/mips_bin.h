#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

// --- MIPS64 OPCODES & FUNCTS ---
#define OP_DADDIU 0x19
#define OP_LD     0x37
#define OP_SD     0x3F
#define OP_SPECIAL 0x00
#define FUNCT_DADDU 0x2D
#define FUNCT_DSUBU 0x2F
#define FUNCT_DMULT 0x1C  // DMULT function code
#define FUNCT_DDIV  0x1E  // DDIV function code
#define FUNCT_MFLO  0x12  // MFLO function code (move from LO register)
#define FUNCT_MFHI  0x10  // MFHI function code (move from HI register)

// --- SYMBOL TABLE ---
typedef struct {
    char name[32];
    int offset;
} Symbol;

Symbol symbols[100];
int symbol_count = 0;

void add_symbol(char *name, int offset) {
    strcpy(symbols[symbol_count].name, name);
    symbols[symbol_count].offset = offset;
    symbol_count++;
}

int get_symbol_offset(char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i].name, name) == 0) return symbols[i].offset;
    }
    return 0; // Default to 0 if not found
}

// Helper: Extract Register Number "r2" -> 2
int get_reg(char *str) {
    while (*str && !isdigit(*str)) str++;
    return atoi(str);
}

// Helper: Extract variable name "x(r0)" -> returns "x", sets base_reg
void parse_mem_operand(char *str, char *var_out, int *base_reg) {
    char *paren = strchr(str, '(');
    if (paren) {
        *paren = '\0'; // Split string at (
        strcpy(var_out, str);
        *base_reg = get_reg(paren + 1);
    }
}

void generate_binary_file(const char* mips_filename, const char* bin_filename) {
    FILE *in = fopen(mips_filename, "r");
    FILE *out = fopen(bin_filename, "w");  // Changed to "w" for text mode
    
    if (!in || !out) { 
        printf("Error opening files.\n"); 
        if (in) fclose(in);
        if (out) fclose(out);
        return; 
    }

    char line[256];
    int data_offset = 0;
    int is_text_section = 0;
    int instruction_count = 0;
    
    // Store machine codes for hex output later
    uint32_t machine_codes[1000];
    char instruction_lines[1000][256];

    // --- PASS 1: MAP SYMBOLS (.data) ---
    printf(">> Pass 1: Mapping Symbols...\n");
    while (fgets(line, sizeof(line), in)) {
        if (strstr(line, ".text")) break; // Stop at text section
        if (strstr(line, ": .word")) {
            char var_name[32];
            sscanf(line, "%[^:]", var_name); // Read until ':'
            
            // Trim whitespace from variable name
            char *start = var_name;
            while (*start && isspace(*start)) start++;
            char *end = start + strlen(start) - 1;
            while (end > start && isspace(*end)) end--;
            *(end + 1) = '\0';
            
            add_symbol(start, data_offset);
            printf("   Mapped '%s' to address Offset %d\n", start, data_offset);
            data_offset += 16; // Increment by 16 bytes (.word 0, 0 = two 64-bit words)
        }
    }

    // --- PASS 2: ENCODE INSTRUCTIONS (.text) ---
    rewind(in); // Start from top
    printf(">> Pass 2: Encoding Instructions...\n");
    fprintf(out, "MIPS64 Machine Code (Binary Format)\n");
    fprintf(out, "=========================================\n\n");
    
    while (fgets(line, sizeof(line), in)) {
        // Skip whitespace/comments
        if (line[0] == '#' || line[0] == '\n') continue;
        
        // Detect Section
        if (strstr(line, ".text")) { is_text_section = 1; continue; }
        if (strstr(line, ".data")) { is_text_section = 0; continue; }
        if (!is_text_section) continue; 
        if (strchr(line, ':')) continue; // Skip labels like "main:"

        uint32_t machine_code = 0;
        char op[32], arg1[32], arg2[32], arg3[32];
        
        // Clean up newlines and save original line
        line[strcspn(line, "\n")] = 0;
        char original_line[256];
        strcpy(original_line, line);
        
        // Skip empty lines
        char *trimmed = line;
        while (*trimmed && isspace(*trimmed)) trimmed++;
        if (*trimmed == '\0') continue;

        // Parse: "opcode arg1, arg2, arg3"
        memset(arg1, 0, sizeof(arg1));
        memset(arg2, 0, sizeof(arg2));
        memset(arg3, 0, sizeof(arg3));
        
        int count = sscanf(trimmed, "%s %[^,], %[^,], %s", op, arg1, arg2, arg3);
        
        // Handle 2-argument instructions (might not have third arg)
        if (count < 4) {
            sscanf(trimmed, "%s %[^,], %s", op, arg1, arg2);
        }
        
        // 
        // R-Type: Op(6) rs(5) rt(5) rd(5) shamt(5) funct(6)
        // I-Type: Op(6) rs(5) rt(5) imm(16)
        
        // 1. DADDIU rt, rs, imm (I-Type)
        if (strcmp(op, "daddiu") == 0) {
            int rt = get_reg(arg1);
            int rs = get_reg(arg2);
            int imm = atoi(arg3);
            machine_code = (OP_DADDIU << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF);
        }
        // 2. DADDU rd, rs, rt (R-Type)
        else if (strcmp(op, "daddu") == 0) {
            int rd = get_reg(arg1);
            int rs = get_reg(arg2);
            int rt = get_reg(arg3);
            machine_code = (OP_SPECIAL << 26) | (rs << 21) | (rt << 16) | (rd << 11) | FUNCT_DADDU;
        }
        // 3. DSUBU rd, rs, rt (R-Type)
        else if (strcmp(op, "dsubu") == 0) {
            int rd = get_reg(arg1);
            int rs = get_reg(arg2);
            int rt = get_reg(arg3);
            machine_code = (OP_SPECIAL << 26) | (rs << 21) | (rt << 16) | (rd << 11) | FUNCT_DSUBU;
        }
        // 4. DMULT rs, rt (R-Type - multiply and store in HI/LO)
        // Format: dmult rs, rt
        // Encoding: SPECIAL (0x00) | rs << 21 | rt << 16 | FUNCT_DMULT (0x1C)
        else if (strcmp(op, "dmult") == 0) {
            int rs = get_reg(arg1);
            int rt = get_reg(arg2);
            machine_code = (OP_SPECIAL << 26) | (rs << 21) | (rt << 16) | FUNCT_DMULT;
        }
        // 5. DDIV rs, rt (R-Type - divide and store quotient in LO, remainder in HI)
        // Format: ddiv rs, rt
        // Encoding: SPECIAL (0x00) | rs << 21 | rt << 16 | FUNCT_DDIV (0x1E)
        else if (strcmp(op, "ddiv") == 0) {
            int rs = get_reg(arg1);
            int rt = get_reg(arg2);
            machine_code = (OP_SPECIAL << 26) | (rs << 21) | (rt << 16) | FUNCT_DDIV;
        }
        // 6. MFLO rd (R-Type - move from LO register)
        // Format: mflo rd
        // Encoding: SPECIAL (0x00) | rd << 11 | FUNCT_MFLO (0x12)
        else if (strcmp(op, "mflo") == 0) {
            int rd = get_reg(arg1);
            machine_code = (OP_SPECIAL << 26) | (rd << 11) | FUNCT_MFLO;
        }
        // 7. MFHI rd (R-Type - move from HI register)
        // Format: mfhi rd
        // Encoding: SPECIAL (0x00) | rd << 11 | FUNCT_MFHI (0x10)
        else if (strcmp(op, "mfhi") == 0) {
            int rd = get_reg(arg1);
            machine_code = (OP_SPECIAL << 26) | (rd << 11) | FUNCT_MFHI;
        }
        // 8. SD rt, offset(base) (I-Type)
        else if (strcmp(op, "sd") == 0) {
            int rt = get_reg(arg1);     // Data to store
            char var_name[32];
            int base = 0;
            char temp_arg2[64];
            strcpy(temp_arg2, arg2);
            parse_mem_operand(temp_arg2, var_name, &base); // Parse "x(r0)"
            int offset = get_symbol_offset(var_name); // Look up "x" -> 8
            
            machine_code = (OP_SD << 26) | (base << 21) | (rt << 16) | (offset & 0xFFFF);
        }
        // 9. LD rt, offset(base) (I-Type)
        else if (strcmp(op, "ld") == 0) {
            int rt = get_reg(arg1);     // Destination
            char var_name[32];
            int base = 0;
            char temp_arg2[64];
            strcpy(temp_arg2, arg2);
            parse_mem_operand(temp_arg2, var_name, &base); 
            int offset = get_symbol_offset(var_name);
            
            machine_code = (OP_LD << 26) | (base << 21) | (rt << 16) | (offset & 0xFFFF);
        }

        // Write if we generated code
        if (machine_code != 0) {
            // Store for later hex output
            machine_codes[instruction_count] = machine_code;
            strcpy(instruction_lines[instruction_count], original_line);
            instruction_count++;
            
            // Convert to binary string
            char binary[33];
            binary[32] = '\0';
            for (int i = 31; i >= 0; i--) {
                binary[31 - i] = (machine_code & (1U << i)) ? '1' : '0';
            }
            
            // Write with 6-5-5-16 bit grouping
            fprintf(out, "[%02d] %c%c%c%c%c%c %c%c%c%c%c %c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    # %s\n", 
                    instruction_count,
                    binary[0], binary[1], binary[2], binary[3], binary[4], binary[5],       // opcode (6 bits)
                    binary[6], binary[7], binary[8], binary[9], binary[10],                  // rs (5 bits)
                    binary[11], binary[12], binary[13], binary[14], binary[15],              // rt (5 bits)
                    binary[16], binary[17], binary[18], binary[19], binary[20],              // remaining 16 bits
                    binary[21], binary[22], binary[23], binary[24], binary[25],
                    binary[26], binary[27], binary[28], binary[29], binary[30], binary[31],
                    original_line);
            
            printf("   [%02d] %s -> 0x%08X -> %s\n", instruction_count, op, machine_code, binary);
        }
    }

    fprintf(out, "\n\n# Hex below\n\n");
    
    // Write hex output
    for (int i = 0; i < instruction_count; i++) {
        fprintf(out, "[%02d] 0x%08X    # %s\n", i + 1, machine_codes[i], instruction_lines[i]);
    }

    fprintf(out, "=========================================\n");
    fprintf(out, "Total Instructions: %d\n", instruction_count);

    fclose(in);
    fclose(out);
    printf(">> Binary generation complete: %s\n", bin_filename);
    printf(">> Total instructions encoded: %d\n", instruction_count);
}