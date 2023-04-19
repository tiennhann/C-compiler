
%{
   /*
   

  Garrett Turner 
     lab 9 
   12 / 2 /2022 
   Takes decaf code and generates gas assembly. 
     

   */
#include "ast.h"
#include "y.tab.h"
#include <math.h> // added inorder to use pow function in ahextoint


static int mydebug = 0; // set debug to zero to turn the debug off
int linecount = 1; // a counter variable for newline 

/* ahextoint method
 Pre condition must take a hexdecminal char *
 the contains the 0x followed by digits 0 -9 
 or letter A-Fa-f
 The method takes a hexdecimal char * 
  and returns a the decimical equilvent 
  as a integer
  Post condition return int (decmical value equilvent hexdecimal )
*/
int ahextoint (char *hex)
{
  int value = 0; // store gets hexvalue from the characters 
  int result = 0;// store the final hexvalue 
  int power = strlen(hex) - 1; // this is the exponent value we intialize the length of hex - 1 to compute the right most character in hex
      
    // This will loop through the entire hex character sequence
    for (int i = 0; i < strlen(hex);  i ++)
    {
        // if character is a number 
        
        if (hex[i] >= '0' && hex[i] <= '9')
        {
             // ggets the decimal value for each character 
             value = hex[i] - '0' ;
             
        } // end if 
        // if character is a lowercase letter 
        
        if (hex [i] >= 'a' && hex [i] <= 'f')
          {
              // gets the decimal value for each character 
              value = hex[i] - 'a' + 10;
              
          } // end if 
          // if character is a upper case later 
        
        if (hex [i] >= 'A' && hex [i] <= 'F' )
         { 
            // gets the decimal value for each character 
            value  = hex [i] - 'A' + 10; 
            
         } // end if 
         // store the converted total decminal value for all characters in hex
         result += value * pow(16, power);
         
         // decrement the exponenment in order to move to righ character in the hex. 
         power --; 
       
          
    } // end for 
    return result; 
} // end of ahextoint

%}

 // Set of Tokens for decaf
%%
"&&"             {return(T_AND);}
"="              {return(T_ASSIGN);}
"bool"           {return(T_BOOLTYPE);}
"break"          {return(T_BREAK);}
"continue"       {return(T_CONTINUE);}
"."              {return(T_DOT);}
"else"           {return(T_ELSE);}
"=="             {return(T_EQ);}
"extern"         {return(T_EXTERN);}
"false"          {return(T_FALSE);}
"for"            {return(T_FOR);}
"func"           {return(T_FUNC);}
">="             {return(T_GEQ);}
">"              {return(T_GT);}
"if"             {return(T_IF);}
"int"            {return(T_INTTYPE);}
"<<"             {return(T_LEFTSHIFT);}
"<="             {return(T_LEQ);}
"!="             {return(T_NEQ);}
"null"           {return(T_NULL);}
"||"             {return(T_OR);}
"package"        {return(T_PACKAGE);}
"return"         {return(T_RETURN);}
">>"             {return(T_RIGHTSHIFT);}
"string"         {return(T_STRINGTYPE);}
"true"           {return(T_TRUE);}
"var"            {return(T_VAR);}
"void"          {return(T_VOID);}
"while"         {return(T_WHILE);}

\".*\"            {yylval.string = strdup (yytext); return (T_STRINGCONSTANT);}
                        
[a-zA-Z][a-zA-Z0-9_]*		{if (mydebug) fprintf(stderr,"Letter found\n"); 
                       yylval.string=strdup(yytext); return(T_ID);}
                          
0x[0-9A-Fa-f][0-9A-Fa-f]* {if (mydebug) fprintf(stderr,"Digit found\n"); 
                       yylval.value=ahextoint(yytext); return(T_INTCONSTANT);} 
                                                                 
[0-9][0-9]*	{if (mydebug) fprintf(stderr,"Digit found\n");
                       yylval.value=atoi((const char *)yytext); return(T_INTCONSTANT);}
                                            
[ \t]		{if (mydebug) fprintf(stderr,"Whitespace found\n");}
                       
[\[\]{}=\-+*/%&|();,!< ] 	{ if (mydebug) fprintf(stderr,"return a token %c\n",*yytext); /*Added () so the lex can return the ()  */ 
                       return (*yytext);}
                       
\/\/.*     {/* This is for double lined comments // */}                       
                   
\n 		{ if (mydebug) fprintf(stderr,"cariage return %c\n",*yytext); 
                       linecount++; }

%%

int yywrap(void)
{ return 1;}


