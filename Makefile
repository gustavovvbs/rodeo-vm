CC = gcc
CFLAGS = -Wall -g
BISON = bison
FLEX = flex

TARGET = rodeo-vm
PARSER_SRC = parser.tab.c
LEXER_SRC = lex.yy.c
PARSER_HDR = parser.tab.h
AST_SRC = ast.c
VM_SRC = vm.c
OBJS = parser.tab.o lex.yy.o ast.o vm.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

parser.tab.o: $(PARSER_SRC) $(PARSER_HDR)
	$(CC) $(CFLAGS) -c $(PARSER_SRC)

lex.yy.o: $(LEXER_SRC) $(PARSER_HDR)
	$(CC) $(CFLAGS) -c $(LEXER_SRC)

ast.o: $(AST_SRC) ast.h
	$(CC) $(CFLAGS) -c $(AST_SRC)

vm.o: $(VM_SRC) vm.h ast.h
	$(CC) $(CFLAGS) -c $(VM_SRC)

$(PARSER_SRC) $(PARSER_HDR): parser.y
	$(BISON) -d parser.y

$(LEXER_SRC): lexer.l $(PARSER_HDR)
	$(FLEX) lexer.l

test: $(TARGET)
	./$(TARGET) test.rodeo

clean:
	rm -f $(TARGET) $(PARSER_SRC) $(LEXER_SRC) $(PARSER_HDR) $(OBJS)
	rm -rf *.dSYM

.PHONY: all test clean
