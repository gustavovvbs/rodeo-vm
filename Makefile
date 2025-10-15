CC = gcc
CFLAGS = -Wall -g
BISON = bison
FLEX = flex

TARGET = rodeo-parser
PARSER_SRC = parser.tab.c
LEXER_SRC = lex.yy.c
PARSER_HDR = parser.tab.h

all: $(TARGET)

$(TARGET): $(PARSER_SRC) $(LEXER_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(PARSER_SRC) $(LEXER_SRC)

$(PARSER_SRC) $(PARSER_HDR): parser.y
	$(BISON) -d parser.y

$(LEXER_SRC): lexer.l $(PARSER_HDR)
	$(FLEX) lexer.l

test: $(TARGET)
	./$(TARGET) test.rodeo

clean:
	rm -f $(TARGET) $(PARSER_SRC) $(LEXER_SRC) $(PARSER_HDR) *.o

.PHONY: all test clean

