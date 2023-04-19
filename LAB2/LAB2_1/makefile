#Nhan Le
#September 1,2022
#makefile for lab2.1

all: lab2.1			#create executable

lab2.1: lab2remove.l			#write all the file you need to compile
		lex lab2remove.l		#scan your target
		gcc -o lab2.1 lex.yy.c	#compile target

clean:	
		rm -f lab2.1	#remove lab2.1

run:
		./lab2.1 < comment.tst	#run 2.1