#Nhan Le
#September 1, 2022
#Lab 2_2
#
#Answer the question:
#
# "a=3+5" is the var = expr. 
#In the lex, get the value Variable(a)
#	var is var(0), because we have yylval=*yytext-'a'. 
#	expr turn in to e + E, then e+E turns into Integer(3) + Integer(5).
# 	We have $1 + $3, which means 3+5, so our expr is expr(8).
#	as the result, var(0) = expr(8).
#In yacc, we have VARIABLE '=' expr { regs[$1] = $3; }.
#	So, regs[0] = 8
#
#"a" is a stat turns into expr and expr turns into var(0)
#In the lex, get the value Variable(a)
#In the Yacc, we have VARIABLE { $$ = regs[$1]}, so $$ = regs[0]and regs[0] is 8
#So the print statement is "the answer is 8".
#The print statement is "Found a variable value" because yacc treats a
#	variable value as expression

all: lab2_2 #create executable

lab2_2:	lab2docalc.l lab2docalc.y 
	lex lab2docalc.l	#scan your target
	yacc -d lab2docalc.y #scan your target
	gcc y.tab.c -o lab2_2 #compiles target

clean: 
	rm -f lab2_2	#remove lab2_2

run:	lab2_2
	./lab2_2	#run lab2_2