%{

/*
 *			**** CALC ****
 *
 * This routine will function like a desk calculator
 * There are 26 integer registers, named 'a' thru 'z'
 *
 */

/* This calculator depends on a LEX description which outputs either VARIABLE or INTEGER.
   The return type via yylval is integer 

   When we need to make yylval more complicated, we need to define a pointer type for yylval 
   and to instruct YACC to use a new type so that we can pass back better values
 
   The registers are based on 0, so we substract 'a' from each single letter we get.

   based on context, we have YACC do the correct memmory look up or the storage depending
   on position

   Nhan Le 
   September 16, 2022 

   problems  fix unary minus, fix parenthesis, add multiplication
   problems  make it so that verbose is on and off with an input argument instead of compiled in
*/
/*
   Nhan Le
   File Name: lab4.y (lab2docalc.y)
   Date: September 16, 2022 
   YACC file to process the tokens recieved from Lex 
   Using the tokens given by Lex do simple mathematics for our caluclator 


*/

	/* begin specs */
#include <stdio.h>
#include <ctype.h>
/*#include "lex.yy.c"	//remove #include "lexx.yy.c" like requirement */
#include "symtable.h"

int regs[4];
int base, debugsw;
int yylex(); //fix the (error) implicit declaration of function yylex
int regOffset = 0;
int max = 4;


void yyerror (s)  /* Called by yyparse on error */
     char *s;
{
  printf ("%s\n", s);
}


%}

%start P

%union{		/*add union type to YACC*/
	int value;
	char* string;
}
%type <value> expr
%token <value> INTEGER
%token <string> VARIABLE
%token T_INT

%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%' 
%left UMINUS
%left '(' ')'
%left '!'


%%	/* end specs, begin rules */

P		: DECLS list
		;

DECLS 	:   DECLS DECL
		|	/* EMPTY */
		;

DECL	: 	T_INT VARIABLE ';' '\n'
		{
			if (Search($2)){
				fprintf(stderr, "Variable already in the table\n");
			}
			else if(regOffset < max){
				Insert($2, regOffset);
				++regOffset;
			}
			else{
				fprintf(stderr, "Registers are full, reached maximum limit \n");
			}

		}
		;


list	:	/* empty */
		|	list stat '\n'
		|	list error '\n'
				{ yyerrok; }
		;

stat	:	expr
			//	{ fprintf(stderr, "the answer is %d\n", $1); }
		|	VARIABLE '=' expr { // make sure that $1 has been defined
				if (Search($1)) 
					regs[FetchAddr($1)] = $3; 
				else 
					printf("%s%s%s", "symbol ", $1, " has not been declared\n");
			}
	;

expr	: '(' expr ')'  
			{ $$ = $2; }
	|	expr '-' expr
			{ $$ = $1 - $3; }
	|	expr '+' expr
			{ $$ = $1 + $3; }
	|	expr '*' expr
			{ $$ = $1 * $3; }
	|	expr '/' expr
			{ $$ = $1 / $3; }
	|	expr '%' expr
			{ $$ = $1 % $3; }
	|	expr '&' expr
			{ $$ = $1 & $3; }
	|	expr '|' expr
			{ $$ = $1 | $3; }
	|	 '-' expr	%prec UMINUS
			{ $$ = -$2; }
	|	VARIABLE
			{ // Make SURE $1 HAS BEEN DEFINED 
				if(Search($1)){
					fprintf(stderr, "found variable: %s in Symbol Table, with address of %d, with a value %d \n", $1, FetchAddr($1), regs[FetchAddr($1)]);	
					$$ = regs[FetchAddr($1)];
				}
				// if it there then return the value
				else{
					printf("Variable >%s< not found\n", $1);
				}
			}

    |	INTEGER {$$ = $1; fprintf(stderr, "variable has been assigned\n\n");
}	
	;



%%	/* end of rules, start of program */

int main()
{ yyparse();
}
