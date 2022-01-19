CC = g++
CFLAGS = -Wall
LDFLAGS = 
SRCFILES = ./src/*.cpp ./src/*.c
TARGET = ./build/sb
LEXOUT = ./src/lex.yy.c
YACCOUT = ./src/y.tab.c ./src/y.tab.h

all: yacc lex main

test:
	python3 src/test/main.py

lex:
	flex -o src/lex.yy.c src/lexer.l

yacc:
	bison -b ./src/y -d src/parser.y

main:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCFILES)

clean:
	rm -f $(TARGET) $(LEXOUT) $(YACCOUT)