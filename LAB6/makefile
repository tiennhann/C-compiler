# Name: Nhan Le
# Date: October 2, 2022
# File Name: makefile
# Description: this is makefile for lab6

all: lab6

lab6: lab6.l lab6.y ast.c ast.h
	lex lab6.l
	yacc -d lab6.y
	gcc y.tab.c lex.yy.c ast.c -o lab6

clean:
	rm  lab6 lex.yy.c y.tab.c y.tab.h
