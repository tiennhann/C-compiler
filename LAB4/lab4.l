/*                 Small LEX routine which returns two formal tokens (INTEGER and VARIABLE)
                   along with single string elements like '+'.
 
                    This LEX definition is the companion to the docalc.y YACC routine which
                   is a simple calculator

                    Shaun Cooper
                    January 2015

    
                  Name: Nhan Le
                  File Name: lab4.y (lab2docalc.y)
                  September 16, 2022  
                  Description: Take the token and identify the variables when reading from stderr
                  
                  Changes made: We added * to the set rules to multipcation symbol, so YACC can find the math to do it 
                  Lastly, added a data type to main which is integer since we are dealing with integer variables 
*/ 

%{

int mydebug = 0;
#include "y.tab.h"
%}


%%

"int" 		{return(T_INT);} //added for int tokens

[a-zA-Z][a-zA-Z0-9]*		{if (mydebug) fprintf(stderr,"Letter found\n"); 
                yylval.string=strdup(yytext); return(VARIABLE);}

[0-9][0-9]*	{if (mydebug) fprintf(stderr,"Digit found\n"); 
               yylval.value=atoi((const char *)yytext); return(INTEGER);}

[ \t]		{if (mydebug) fprintf(stderr,"Whitespace found\n");} //recognizes a whitespace

[=\-+*/%&|();] 	{ if (mydebug) fprintf(stderr,"return a token %c\n",*yytext); 
                   return (*yytext);}

\n 		{ if (mydebug) fprintf(stderr,"carriage return %c\n",*yytext); 
            return (*yytext);}

%%

int yywrap(void)
{ return 1;}


