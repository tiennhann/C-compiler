# Name: Nhan Le
# Date:  September 20, 2022
# File Name: makefile
# Description: this is makefile for lab5

all: lab5 

lab5: lab5.l lab5.y
	lex lab5.l
	yacc -d lab5.y
	gcc lex.yy.c y.tab.c -o lab5

clean:
	rm -f lab5
