# Nhan Le
# August 22, 2022
# Lab1 -- simple lex mechanism

all: lab1		#create an exceutable
lab1: lab1.l	#contain all of the source files of lab1
	lex lab1.l	#lex compiler for lab1.1
	gcc lex.yy.c -o lab1	#compile

run: lab1		# run lab1
	./lab1 < /etc/passwd

clean: lab1		#clearn lab1
	rm -r lab1