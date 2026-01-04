# Amadeus IDE & Programming Language

![Version](https://img.shields.io/badge/version-1.0.0-green)
![Status](https://img.shields.io/badge/status-stable-blue)
![Theme](https://img.shields.io/badge/theme-bioluminescent-121b17)

---

## Overview

Amadeus is a comprehensive development environment comprising a lightweight IDE and a strictly-typed programming language that compiles directly to MIPS64 assembly and machine code. The system is designed for developers seeking a minimalist, efficient toolchain with a focus on low-level programming and performance.

---

## Table of Contents

1. [Amadeus IDE](#amadeus-ide)
2. [The Amadeus Programming Language](#the-amadeus-programming-language)
3. [Installation & Setup](#installation--setup)
4. [Getting Started](#getting-started)

---

## Amadeus IDE

### Introduction

Amadeus IDE is a purpose-built integrated development environment featuring a bioluminescent color scheme optimized for extended coding sessions. The interface prioritizes functionality and visual comfort, offering essential development tools without unnecessary complexity.

### Design Philosophy

The IDE employs a carefully curated color palette designed to minimize eye strain during prolonged use:

- **Background**: Deep Organic Black (`#121b17`)
- **Primary Text**: Soft Mint Green (`#d0e8d8`)
- **Accent Elements**: Glowing Emerald (`#4db68d`)
- **Error Indicators**: Muted Red (`#e57373`)

### Core Features

#### Project Management
The integrated project explorer provides a hierarchical view of your workspace, enabling efficient file navigation, creation, and organization directly within the IDE.

#### Code Editor
The editor includes specialized support for Amadeus syntax with intelligent highlighting, multi-file tabbed editing, and adaptive line numbering that scales with file size.

#### View Customization
Adjust your editing environment with dynamic zoom controls (`Ctrl + Scroll` or `Ctrl +/-`) and optional word wrapping (`Alt + Z`) for handling lengthy code lines.

#### Integrated Build System
Execute your code with a single keystroke (`F5`). The IDE runs compilation in a separate thread to maintain UI responsiveness, with stdout and stderr displayed in the built-in console.

### Keyboard Shortcuts Reference

#### File Operations
- **New File**: `Ctrl + N`
- **Open File**: `Ctrl + O`
- **Open Folder**: `Ctrl + Shift + O`
- **Save File**: `Ctrl + S`
- **Close Tab**: `Ctrl + W`
- **Rename**: `F2`

#### Editor Controls
- **Toggle Word Wrap**: `Alt + Z`
- **Zoom In**: `Ctrl + +` or `Ctrl + Scroll Up`
- **Zoom Out**: `Ctrl + -` or `Ctrl + Scroll Down`

#### Execution
- **Compile & Run**: `F5`

---

## The Amadeus Programming Language

### Language Characteristics

Amadeus is a statically-typed, imperative programming language with C-like syntax that compiles directly to MIPS64 assembly (little-endian). The compiler implements a single-pass architecture, performing syntax analysis, semantic validation, and code generation in one unified process.

### Key Features

- **Target Architecture**: MIPS64 (Little Endian)
- **Type System**: Static typing with compile-time validation
- **Compilation Model**: Single-pass with atomic output generation
- **Memory Model**: Automatic stack and data segment allocation
- **Error Handling**: Atomic compilation—corrupted output is automatically discarded on compilation failure

---

## Language Specification

### Type System

Amadeus supports two primitive types:

| Type | Keyword | Size | Range | Description |
|------|---------|------|-------|-------------|
| Integer | `int` | 64-bit | −9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 | Signed integer type |
| Character | `char` | 8-bit | 0–255 | ASCII character storage |

### Variable Declaration

Variables must be declared before use. Identifiers must begin with a letter or underscore, followed by any combination of letters, digits, or underscores.

**Syntax Patterns**:

```c
int a;                    // Declaration without initialization
char b = 'A';             // Declaration with initialization
int x, y = 10, z;         // Multiple declarations (comma-separated)
```

### Expressions and Operators

The language supports standard arithmetic operations with proper precedence handling (PEMDAS/BODMAS):

| Operator | Description | Precedence |
|----------|-------------|------------|
| `()` | Grouping | Highest |
| `-` (unary) | Negation | High |
| `*`, `/` | Multiplication, Division | Medium |
| `+`, `-` | Addition, Subtraction | Low |

**Example**:
```c
int result = 10 + 5 * (20 - 4) / 2;  // Evaluates to: 10 + 5 * 16 / 2 = 50
```

### Increment and Decrement Operators

Both prefix and postfix forms are supported, mapping directly to MIPS `daddiu` instructions:

```c
value++;    // Post-increment: use current value, then add 1
++value;    // Pre-increment: add 1, then use new value
value--;    // Post-decrement: use current value, then subtract 1
--value;    // Pre-decrement: subtract 1, then use new value
```

### Output Operations

The `cdisplay()` function provides type-aware output:

**Syntax**:
```c
cdisplay(expression);
```

**Behavior**:
- **Integer values**: Display as decimal numbers
- **Character values**: Display as ASCII characters (when in valid range 0–127)

**Example**:
```c
int num = 100;
char letter = 100;

cdisplay(num);      // Output: 100
cdisplay(letter);   // Output: d
```

---

## Complete Example Program

```c
// Variable declarations with initialization
int count = 10;
int step = 2;
char status = 79;    // ASCII value for 'O'

// Arithmetic operations
count = count * step;
count++;

// Output operations
cdisplay(status);    // Displays: O
cdisplay(count);     // Displays: 21
```

---

## Installation & Setup

### System Requirements

- **Operating System**: Windows 10 or Windows 11
- **Runtime**: Python 3.7 or higher (for source execution)
- **Required Files**:
  - `lexer.py` (lexical analyzer module)
  - `a.exe` (Amadeus compiler executable)

### Installation Steps

1. Clone or download the Amadeus IDE repository
2. Verify that `lexer.py` and `a.exe` are present in the root directory
3. Launch the IDE:
   ```bash
   python main.py
   ```

### Directory Structure

```
amadeus-ide/
├── main.py           # IDE entry point
├── lexer.py          # Syntax highlighting and lexical analysis
├── a.exe             # Amadeus compiler
└── README.md         # This documentation
```

---

## Getting Started

1. Launch the IDE using `python main.py`
2. Create a new file (`Ctrl + N`) or open an existing project (`Ctrl + Shift + O`)
3. Write your Amadeus code using the syntax described above
4. Save your file with the `.amd` extension (recommended)
5. Press `F5` to compile and execute
6. View output in the integrated console panel

---

## Important Notes

### Current Limitations

Please be aware of the following language features that are currently unavailable:

**Comments**  
The Amadeus language does not yet support comment syntax. All code must be executable; documentation and notes cannot be embedded within source files at this time.

**String Literals**  
String data types and string literal support are currently under development. The language presently supports only integer and character types. String functionality will be available in a future release.

These features are planned for implementation in upcoming versions. We appreciate your patience as we continue to develop the Amadeus ecosystem.

---

## Additional Resources

For bug reports, feature requests, or contributions, please refer to the project repository or contact the development team.

---

**© 2024 Amadeus IDE Development Team** | *Built for developers who value simplicity and performance*
