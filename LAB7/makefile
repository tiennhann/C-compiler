# Name: Nhan Le
# Date:  Nhan Le
# Date:   October 23, 2022
# File Name: makefile
# Description: this is makefile for lab7

all: lab7

lab7: lab7.l lab7.y ast.c ast.h symtable.c symtable.h
	lex lab7.l
	yacc -d lab7.y
	gcc y.tab.c lex.yy.c ast.c symtable.c -o lab7

clean:
	rm  lab7 lex.yy.c y.tab.c y.tab.h

run:
	./lab7 < lab7_sub.decaf

