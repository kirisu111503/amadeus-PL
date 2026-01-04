#include <stdio.h>
#include <stdlib.h> // Required for malloc, free
#include "lex_par.h" // This must contain the updated code with MIPS generator
#include "mips_bin.h"

// --- FILE READER FUNCTION ---
char* read_file(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*)malloc(length + 1);
    if (buffer == NULL) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0'; // IMPORTANT: Add the null terminator!

    fclose(file);
    return buffer;
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

// --- FUNCTION TO DELETE FILE ---
void delete_file(const char* filename) {
    remove(filename);
}


// --- MAIN ---
int main() {
    // 0. Cleanup any previous data (important for multiple runs)
    cleanup_all();
    
    // 1. Read the text file
    // Ensure you have a file named 'code.txt' in the same folder!
    char *filename = "code.txt";
    char *file_content = read_file(filename);

    if (file_content == NULL) {
        return 1; // Exit if file error
    }

    // 2. Tokenize (This fills the TOKEN list)
    tokenize_string(file_content); 

    // 3. Parse (This fills the SYMBOL TABLE, ERROR LOG, HISTORY, and MIPS LIST)
    // The parser now automatically generates MIPS instructions in the background
    // whenever it encounters an assignment (e.g., x = 10).
    parse_tokens();        

    // 4. Print results & Export MIPS
    // The updated header's print_results() function will:
    //    a. Print the History Log
    //    b. Print the Symbol Table
    //    c. Print Errors
    //    d. Call generate_mips_file("output.asm") automatically
    print_results();       
    
    // 5. Check for errors and handle file generation accordingly
    printf("TOKENS: %d\n", token_count);
    
    if(err_head == NULL) {
        // No errors found - proceed with normal operations
        printf("\n>> No errors found. Proceeding with binary generation...\n");
        read_output_file("output.txt");
        generate_binary_file("output.txt", "program.txt");
        printf(">> Program finished successfully.\n");
    } else {
        // Errors found - create empty files
        printf("\n>> Errors detected. Creating empty output files...\n");
        
        // Create empty output.txt
        create_empty_file("output.txt");
        
        // Create empty program.txt
        create_empty_file("program.txt");
        
        printf(">> All output files cleared due to errors.\n");
        printf(">> Please fix the errors above and try again.\n");
    }
    
    // 6. Cleanup
    free(file_content); 
    cleanup_all();

    return 0;
}