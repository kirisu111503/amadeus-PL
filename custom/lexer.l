%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "parser.tab.h"

extern int line_number;
%}

%option noyywrap
%option yylineno

%%

[ \t]+              { /* skip whitespace */ }
\n                  { line_number++; }

"int"               { return INT; }
"char"              { return CHAR; }
"cdisplay"          { return CDISPLAY; }

[a-zA-Z_][a-zA-Z0-9_]*  { 
                        yylval.str = strdup(yytext); 
                        return IDENTIFIER; 
                    }

[0-9]+              { 
                        yylval.num = atoi(yytext); 
                        return NUMBER; 
                    }


'([^'\\]|\\n|\\t|\\r|\\0|\\\\|\\')'  {
                        /* Character literal - handle escape sequences */
                        if (yytext[1] == '\\') {
                            /* Escape sequence */
                            switch (yytext[2]) {
                                case 'n':  yylval.num = '\n'; break;
                                case 't':  yylval.num = '\t'; break;
                                case 'r':  yylval.num = '\r'; break;
                                case '0':  yylval.num = '\0'; break;
                                case '\\': yylval.num = '\\'; break;
                                case '\'': yylval.num = '\''; break;
                                default:   yylval.num = yytext[2]; break;
                            }
                        } else {
                            /* Regular character */
                            yylval.num = yytext[1];
                        }
                        return CHAR_LITERAL;
                    }

'[^']*'             {
                        /* Multi-character literal - error */
                        fprintf(stderr, "Error on line %d: Multi-character literal '%s' is not supported\n", 
                                line_number, yytext);
                        yylval.num = 0;
                        return CHAR_LITERAL;
                    }

'                   {
                        /* Unclosed character literal */
                        fprintf(stderr, "Error on line %d: Unclosed character literal\n", line_number);
                    }

"++"                { return INCREMENT; }
"--"                { return DECREMENT; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MULTIPLY; }
"/"                 { return DIVIDE; }
"="                 { return ASSIGN; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }

.                   { 
                        fprintf(stderr, "Unknown character on line %d: %s\n", 
                                line_number, yytext); 
                    }

%%