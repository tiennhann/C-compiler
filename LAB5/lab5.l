/*                 Small LEX routine which returns two formal tokens (INTEGER and VARIABLE)
                   along with single string elements like '+'.
 
                    This LEX definition is the companion to the docalc.y YACC routine which
                   is a simple calculator

                    Shaun Cooper
                    January 2015

    
                  Name: Nhan Le
                  File Name: lab5.l
                  Date:  September 20, 2022
                  Description: Take the token and identify the variables when reading from stderr
                  
                  Changes made: We added * to the set rules to multipcation symbol, so YACC can find the math to do it 
                  Lastly, added a data type to main which is integer since we are dealing with integer variables 
*/

%{

#include "y.tab.h"
#include <string.h>
#include <math.h>
int yylex(); //protttyep for yylex() get rid of warning
int mydebug = 0;
int lineCount = 1;

int hexToInt(char *hex){
    //declare all variable need to convert hex to decimal
    int length = strlen(hex)-1;
    long long decimal = 0, base = 1;
    int i = 0, value;
    
    //for loop to check it's 0-9 
    for(i = length--; i >= 0; i--) {
        if(hex[i] >= '0' && hex[i] <= '9'){
            decimal += (hex[i] - 48) * base;
            base *= 16;
        }
        else if(hex[i] >= 'A' && hex[i] <= 'F'){
            decimal += (hex[i] - 55) * base;
            base *= 16;
        }
        else if(hex[i] >= 'a' && hex[i] <= 'f'){
            decimal += (hex[i] - 87) * base;
            base *= 16;
        }
    }
    return decimal;
}
%}
/*Changes that I made:  
    -add all tokens base on DECAF rules
    -add '%' '<' and square brackets tokens to directive
    -add directive for Hex number and function that converst hex to int value
    -disable newline return token
*/

%%
"&&"          {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_AND);}           
"="           {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_ASSIGN);}         
"bool"        {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_BOOLTYPE);}      
"break"       {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_BREAK);}     
"continue"    {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_CONTINUE);}   
\.            {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_DOT);}           
"else"        {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_ELSE);}           
"=="          {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_EQ);}             
"extern"      {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_EXTERN);}        
"false"       {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_FALSE);}         
"for"         {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_FOR);}            
"func"        {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_FUNC);}           
">="          {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_GEQ);}            
">"           {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_GT);}             
"if"          {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_IF);}    
int_lit       {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_INTCONSTANT);}         
"int"         {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_INTTYPE);}        
"<<"          {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_LEFTSHIFT);}      
"<="          {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_LEQ);}            
"!="          {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_NEQ);}            
"null"        {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_NULL);}           
"||"          {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_OR);}             
"package"     {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_PACKAGE);}        
"return"      {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_RETURN);}         
">>"          {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_RIGHTSHIFT);}     
"string"      {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_STRINGTYPE);}     
"true"        {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_TRUE);}           
"var"         {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_VAR);}            
"void"        {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_VOID);}           
"while"       {if(mydebug)  fprintf(stderr, "%s found\n",yytext); return(T_WHILE);}          

[a-zA-Z_][a-zA-Z0-9_]*  {if(mydebug)  fprintf(stderr, "Identifier found\n");
                            yylval.string=strdup(yytext); return(T_ID);}

[0-9][0-9]*	             {if(mydebug) fprintf(stderr, "Interger found\n"); 
                            yylval.value=atoi((const char *)yytext); return(T_INTCONSTANT);}

0x[0-9A-Za-f][0-9A-Za-f]* {if (mydebug) fprintf(stderr,"Hex found %s\n", yytext); 
                            yylval.value=hexToInt((char *)yytext); return(T_INTCONSTANT);} 

\'.\'   {yylval.value=yytext[1]; return(T_CHARCONSTANT);}

\".*\"  {yylval.string=strdup(yytext);  return(T_STRINGCONSTANT);}

[ \t]		{if(mydebug) fprintf(stderr, "Whitespace found\n");} //recognizes a whitespace

[\-+*/%(){},;\[\]!<] 	{if(mydebug) fprintf(stderr,"Return a token %c\n", *yytext);  return (*yytext);}

\n 		{if (mydebug) fprintf(stderr,"carriage return %c\n",*yytext);
          lineCount++;
      }

\/\/.*        {/*do nothing comment */}
%%

int yywrap(void)
{ return 1;}


