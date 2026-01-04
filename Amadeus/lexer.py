import re
import hashlib

class Token:
    def __init__(self, type_, value, line, column):
        self.type = type_
        self.value = value
        self.line = line
        self.column = column


class Lexer:
    """
    Simulates the 'existing lexer definitions'. 
    Maps raw text to specific token types.
    """
    def __init__(self, text):
        self.text = text
        self.pos = 0
        self.line = 1
        self.column = 0
        
        self.keywords = {
            'int', 'char', 'float', 'bool', 'string', 'void',
            'cdisplay', 'cscan',
            'if', 'else', 'while', 'for', 'return', 'break'
        }

    def tokenize(self):
        tokens = []
        token_spec = [
            ('STRING',   r'"[^"]*"'),           # Double-quoted strings "text"
            
            # --- NEW: Character Literals ---
            # Matches 'a', '\n', etc. 
            ('CHAR',     r"'[^']*'"),           
            
            ('COMMENT',  r'//.*'),              # Single line comments
            ('NUMBER',   r'\d+(\.\d*)?'),       # Numbers
            ('ID',       r'[A-Za-z_]\w*'),      # Identifiers
            ('OP',       r'[+\-*/=<>!&|]+'),    # Operators
            ('SYMBOLS',  r'[;:,(){}\[\]\.\']'), # Punctuation (Added ' just in case)
            ('NEWLINE',  r'\n'),                # Line tracking
            ('SKIP',     r'[ \t]+'),            # Skip spaces
            ('MISMATCH', r'.'),                 # Any other character
        ]
        tok_regex = '|'.join('(?P<%s>%s)' % pair for pair in token_spec)
        
        line_num = 1
        line_start = 0
        
        for mo in re.finditer(tok_regex, self.text):
            kind = mo.lastgroup
            value = mo.group()
            column = mo.start() - line_start
            
            if kind == 'NEWLINE':
                line_start = mo.end()
                line_num += 1
                continue
            elif kind == 'SKIP':
                continue
            elif kind == 'ID':
                if value in self.keywords:
                    kind = 'KEYWORD_' + value.upper() 
                else:
                    kind = 'IDENTIFIER'
            
            tokens.append(Token(kind, value, line_num, column))
            
        return tokens
    
def generate_color(seed_string):
    """
    Deterministically generates a distinct, readable color for a given string.
    """
    # 1. Hash the string to get a predictable integer
    hash_obj = hashlib.md5(seed_string.encode())
    hash_int = int(hash_obj.hexdigest(), 16)
    
    # 2. Use HSL logic for distinctness, but convert to RGB hex
    # We fix Saturation/Value to ensure readability against dark bg
    hue = (hash_int % 360) / 360.0
    saturation = 0.85 
    value = 0.90      
    
    # Simple HSL to RGB conversion
    import colorsys
    r, g, b = colorsys.hsv_to_rgb(hue, saturation, value)
    return '#{:02x}{:02x}{:02x}'.format(int(r*255), int(g*255), int(b*255))