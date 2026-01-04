CC = gcc
CFLAGS = -Wall -g -Wno-unused-function -Wno-unused-variable

# Check if WinFlexBison is available
ifeq ($(OS),Windows_NT)
    LEX = win_flex
    YACC = win_bison
else
    LEX = flex
    YACC = bison
endif

# Windows-specific settings
ifeq ($(OS),Windows_NT)
    TARGET = a.exe
    RM = del /Q
    RMDIR = rmdir /S /Q
    PATHSEP = \\
else
    TARGET = parser
    RM = rm -f
    RMDIR = rm -rf
    PATHSEP = /
endif

OBJS = lex.yy.o parser.tab.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

parser.tab.c parser.tab.h: parser.y
	$(YACC) -d parser.y

lex.yy.c: lexer.l parser.tab.h
	$(LEX) lexer.l

lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) -c parser.tab.c

clean:
ifeq ($(OS),Windows_NT)
	if exist a.exe $(RM) a.exe
	if exist lex.yy.o $(RM) lex.yy.o
	if exist parser.tab.o $(RM) parser.tab.o
	if exist parser.tab.c $(RM) parser.tab.c
	if exist parser.tab.h $(RM) parser.tab.h
	if exist lex.yy.c $(RM) lex.yy.c
	if exist output.txt $(RM) output.txt
else
	$(RM) $(TARGET) $(OBJS) parser.tab.c parser.tab.h lex.yy.c output.txt
endif

test: $(TARGET)
ifeq ($(OS),Windows_NT)
	a.exe test.txt
else
	./$(TARGET) test.txt
endif

.PHONY: all clean test