# Amadeus IDE & Programming Language

![Version](https://img.shields.io/badge/version-1.0.0-green)
![Status](https://img.shields.io/badge/status-stable-blue)
![Theme](https://img.shields.io/badge/theme-bioluminescent-121b17)

<p align="center">
  <em>A lightweight, futuristic IDE with a strictly-typed language that compiles to MIPS64 assembly</em>
</p>

---

## 📋 Table of Contents

- [Overview](#overview)
- [Screenshots](#screenshots)
- [Amadeus IDE](#amadeus-ide)
  - [Design Philosophy](#design-philosophy)
  - [Core Features](#core-features)
  - [Keyboard Shortcuts](#keyboard-shortcuts)
- [Amadeus Programming Language](#amadeus-programming-language)
  - [Language Characteristics](#language-characteristics)
  - [Type System](#type-system)
  - [Syntax Guide](#syntax-guide)
- [Installation & Setup](#installation--setup)
- [Getting Started](#getting-started)
- [Current Limitations](#current-limitations)
- [License](#license)

---

## Overview

**Amadeus** is a comprehensive development environment designed for the `amadeus programming language`.

**Key Highlights:**
- 🎨 Bioluminescent color scheme optimized for extended coding sessions
- ⚡ Single-pass compiler with atomic output generation
- 🔧 Integrated build system with real-time console output
- 📁 Built-in project explorer and file management
- ⌨️ Comprehensive keyboard shortcuts for efficient workflow

---

## Screenshots

<img width="1919" alt="Amadeus IDE - Code Editor View" src="https://github.com/user-attachments/assets/bb508713-270a-41ad-aae6-223e8ee8bc30" />

<img width="1919" alt="Amadeus IDE - Console Output" src="https://github.com/user-attachments/assets/ce290ff2-a915-49ad-a490-c5526e73d7ed" />

---

## Amadeus IDE

### Design Philosophy

Amadeus IDE features a carefully curated **bioluminescent color palette** designed to minimize eye strain during prolonged coding sessions:

| Element | Color | Hex Code |
|---------|-------|----------|
| Background | Deep Organic Black | `#121b17` |
| Primary Text | Soft Mint Green | `#d0e8d8` |
| Accent Elements | Glowing Emerald | `#4db68d` |
| Error Indicators | Muted Red | `#e57373` |

### Core Features

#### 📁 Project Management
Integrated project explorer with hierarchical workspace view for efficient file navigation, creation, and organization.

#### ✏️ Smart Code Editor
- **Syntax Highlighting**: Custom lexer integration for Amadeus language
- **Multi-file Support**: Tabbed interface with drag-and-drop functionality
- **Adaptive Line Numbers**: Automatically scales with file size
- **Word Wrap**: Toggle long line wrapping with `Alt + Z`

#### 🎯 View Customization
- **Dynamic Zoom**: Scale text size using `Ctrl + Scroll` or `Ctrl +/-`
- **Flexible Layout**: Resizable panels and customizable workspace

#### ⚙️ Integrated Build System
- **One-Click Execution**: Compile and run with `F5`
- **Non-blocking Compilation**: Runs in separate thread to maintain UI responsiveness
- **Built-in Console**: View `stdout` and `stderr` in real-time

### Keyboard Shortcuts

#### File Operations
| Action | Shortcut |
|--------|----------|
| New File | `Ctrl + N` |
| Open File | `Ctrl + O` |
| Open Folder | `Ctrl + Shift + O` |
| Save File | `Ctrl + S` |
| Close Tab | `Ctrl + W` |
| Rename | `F2` |

#### Editor Controls
| Action | Shortcut |
|--------|----------|
| Toggle Word Wrap | `Alt + Z` |
| Zoom In | `Ctrl + +` or `Ctrl + Scroll Up` |
| Zoom Out | `Ctrl + -` or `Ctrl + Scroll Down` |

#### Execution
| Action | Shortcut |
|--------|----------|
| Compile & Run | `F5` |

---

## Amadeus Programming Language

### Language Characteristics

Amadeus is a **statically-typed, imperative programming language** with C-like syntax that compiles directly to MIPS64 assembly (little-endian).

**Compiler Architecture:**
- ✅ Single-pass compilation (syntax analysis + semantic validation + code generation)
- ✅ Compile-time type checking
- ✅ Automatic memory management (stack and data segment allocation)
- ✅ Atomic compilation (failed builds automatically discard corrupted output)

**Target Specifications:**
- **Architecture**: MIPS64 (Little Endian)
- **Output Formats**: MIPS64 Assembly (`.asm`) and Binary Machine Code

### Type System

Amadeus currently supports two primitive types:

| Type | Keyword | Size | Range | Description |
|------|---------|------|-------|-------------|
| **Integer** | `int` | 64-bit | −9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 | Signed integer type |
| **Character** | `char` | 8-bit | 0–255 | ASCII character storage |

### Syntax Guide

#### Variable Declaration

Variables must be declared before use. Identifiers must start with a letter or underscore, followed by alphanumeric characters or underscores.

```c
int a;                 
char b = 'A';            
int x, y = 10, z;      
```

#### Arithmetic Expressions

Standard operator precedence (PEMDAS/BODMAS) is enforced:

| Operator | Description | Precedence |
|----------|-------------|------------|
| `()` | Grouping | Highest |
| `-` (unary) | Negation | High |
| `*`, `/` | Multiplication, Division | Medium |
| `+`, `-` | Addition, Subtraction | Low |

**Example:**
```c
int result = 10 + 5 * (20 - 4) / 2;  
```

#### Increment & Decrement Operators

Pre- and post-increments and decrements:

```c
value++;   
++value;  
value--;  
--value;   
```

#### Output Operations

The `cdisplay()` function provides type-aware output:

```c
cdisplay(expression);
```

**Behavior:**
- **Integer**: Displays as decimal number
- **Character**: Displays as ASCII character (valid range: 0–127)

**Example:**
```c
int num = 100;
char letter = 100;

cdisplay(num);  
cdisplay(letter);   
```

#### Complete Example Program

```c
int count = 10;
int step = 2;
char status = 79;   

count = count * step;
count++;

cdisplay(status);  
cdisplay(count); 
```

---

## Installation & Setup

### System Requirements

- **Operating System**: Windows 10 or Windows 11
- **Runtime**: Python 3.7 or higher
- **Required Files**:
  - `amadeus.py` (IDE main application)
  - `lexer.py` (lexical analyzer module)
  - `custom_languageIDE.py` (custom language support)
  - `a.exe` (Amadeus compiler executable)
  - `ico.png` (application icon)
  - `close.png` (UI assets)

### Installation Steps

1. **Clone the repository**:
   ```bash
   git clone https://github.com/kirisu111503/amadeus-PL.git
   cd amadeus-PL
   ```

2. **Verify required files** are present in the root directory:
   - `amadeus.py`
   - `lexer.py`
   - `custom_languageIDE.py`
   - `ico.png`
   - `close.png`
   - `a.exe`

3. **Launch the IDE**:
   ```bash
   python amadeus.py
   ```

### Project Structure

```
amadeus-PL/
├── amadeus.py                  # IDE entry point
├── lexer.py                    # Syntax highlighting and lexical analysis
├── custom_languageIDE.py       # Custom language support module
├── a.exe                       # Amadeus compiler
├── ico.png                     # Application icon
├── close.png                   # UI assets
└── README.md                   # This documentation
```

---

## Getting Started

1. **Launch** the IDE: `python amadeus.py`
2. **Create** a new file: `Ctrl + N` or open an existing project: `Ctrl + Shift + O`
3. **Write** your Amadeus code following the syntax guide above
4. **Save** your file with the `.amd` extension (recommended)
5. **Compile & Run**: Press `F5`
6. **View output** in the integrated console panel

### Quick Example

Create a file named `hello.amd`:

```c
char h = 72;  // 'H'
char i = 105; // 'i'

cdisplay(h);
cdisplay(i);
```

Press `F5` to compile and run. The console will display: `Hi`

---

## Current Limitations

> **⚠️ Important**: Please be aware of the following features currently under development:

### 🚧 Comments
The Amadeus language does not yet support comment syntax. All code must be executable—documentation and notes cannot be embedded within source files at this time.

### 🚧 String Literals
String data types and string literal support are currently in development. The language presently supports only `int` and `char` types. String functionality will be available in a future release.

### 🚧 Output Functionality
The `cdisplay()` function currently has the following limitations:
- **Single Character Display**: Only one character can be displayed per `cdisplay()` call
- **Multiple Characters**: Support for displaying multiple characters in a single call is under development
- **Automatic Line Breaks**: Each `cdisplay()` statement automatically includes a newline character—manual line break control is not yet available

**Workaround**: To display multiple characters, use separate `cdisplay()` calls for each character.

---

**Report Issues**: Use the [GitHub issue tracker](https://github.com/kirisu111503/amadeus-PL/issues) to report bugs or request features.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Additional Resources

- **Repository**: [GitHub - kirisu111503/amadeus-PL](https://github.com/kirisu111503/amadeus-PL)
- **Bug Reports**: [GitHub Issues](https://github.com/kirisu111503/amadeus-PL/issues)
- **Discussions**: [GitHub Discussions](https://github.com/kirisu111503/amadeus-PL/discussions)

For questions, suggestions, or contributions, feel free to open an issue or start a discussion on GitHub.

<p align="center">
  <strong>© 2024 Amadeus IDE Development Team</strong><br>
  <em>Built for developers who value simplicity and performance</em>
</p>

<p align="center">
  Made with 💚
</p>

<p align="center">
  <em>El Psy Congroo!</em>
</p>
