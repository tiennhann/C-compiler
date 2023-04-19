# Name: Nhan Le
# Date:  Nhan Le
# File Name: makefile
# Description: this is makefile for lab9
all:	lab9

lab9: lab9.y lab9.l ast.c ast.h symtable.c symtable.h emit.c emit.h
	  yacc -d lab9.y
	  lex lab9.l
	  gcc -o lab9 y.tab.c lex.yy.c ast.c symtable.c emit.c -lm
	

run: lab9
	./lab9

clean: 
	  rm -f lab9	
	
