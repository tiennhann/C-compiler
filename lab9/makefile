# Name: Nhan Le
# Date:  Nhan Le
# Date:   October 23, 2022
# File Name: makefile
# Description: this is makefile for lab9

all: lab9

lab9: lab9.l lab9.y ast.c ast.h symtable.c symtable.h emit.c emit.h
	lex lab9.l
	yacc -d lab9.y
	gcc y.tab.c lex.yy.c ast.c symtable.c emit.c -o lab9

run: lab9
	./lab9

clean:
	rm -f lab9 lex.yy.c y.tab.c y.tab.h


