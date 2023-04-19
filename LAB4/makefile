# Name: Nhan Le
# September 16, 2022 
# File Name: makefile
# Description: this is makefile for lab4

all: lab4	#create executable

lab4: lab4.l lab4.y symtable.c symtable.h
	lex lab4.l		#scan target
	yacc -d lab4.y	#scan target
	gcc -o lab4 lex.yy.c y.tab.c symtable.c	#compile

clean:
	rm -f lab4	#remove lab4

run:	lab4
	./lab4		#run lab4	
