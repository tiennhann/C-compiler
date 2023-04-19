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
   September 20, 2022

   problems  fix unary minus, fix parenthesis, add multiplication
   problems  make it so that verbose is on and off with an input argument instead of compiled in
*/
/*
   Nhan Le
   File Name: lab5.y
   Date:  September 20, 2022
   YACC file to process the tokens recieved from Lex 
   Using the tokens given by Lex do simple mathematics for our caluclator 


*/

	/* begin specs */
#include <stdio.h>
#include <ctype.h>

int yylex(); //fix the (error) implicit declaration of function yylex
extern int lineCount;   //import lineCount variable

void yyerror (s)  /* Called by yyparse on error */
     char *s;
{
   // changed these to output to stderr
   fprintf(stderr, "%s \n", s);
   fprintf(stderr, "Error on line: %d\n", lineCount);
}

%}
/*Define start symbol*/
%start Program

/*Define type*/
%union{		/*add union type to YACC*/
	int value;
	char* string;
}

/*Adding tokens that match with lex tokens */
%token <value> T_CHARCONSTANT
%token <value> T_STRINGCONSTANT
%token <value> T_INTCONSTANT
%token T_AND T_ASSIGN T_BOOLTYPE T_BREAK T_CONTINUE T_DOT T_ELSE T_EQ          
%token T_EXTERN T_FALSE T_FOR T_FUNC T_GEQ T_ID T_GT T_IF T_INTTYPE   
%token T_LEFTSHIFT T_LEQ T_NEQ T_NULL T_OR T_PACKAGE T_RETURN         
%token T_RIGHTSHIFT T_STRINGTYPE T_TRUE T_VAR T_VOID T_WHILE          


/*Adding all the rules from DECAF and also reformating them so they can work currently code*/
/*For a rule that has a list of token, created a recursive rule that allows to take one or more tokens*/
/*Replace '=' by T_ASSIGN*/
%%
Program  : Externs T_PACKAGE T_ID '{' FieldDecls MethodDecls '}'
         ;

Externs  : /* empty */
         | ExternDefn Externs
         ;  

ExternDefn  : T_EXTERN T_FUNC T_ID '(' ExternParmList ')' MethodType ';'
            ;

ExternParmList :  /* empty */
               | FullExternParmList
               ; 

FullExternParmList:  ExternType
                  |  ExternType ',' ExternParmList
                  ;

FieldDecls  : /* empty */
            |  FieldDecl   FieldDecls 
            ;
            
FieldDecl   : T_VAR T_ID Type ';' 
            ;

FieldDecl   : T_VAR T_ID ArrayType ';' 
            ;

FieldDecl   : T_VAR T_ID Type T_ASSIGN Constant ';' 
            ;

MethodDecls : /* empty */
            | MethodDecl MethodDecls 
            ;

MethodDecl  : T_FUNC T_ID '(' IdTypeList ')' MethodType Block
            ;

IdTypeList  : /* empty */
            | FullIdTypeList
            ;
         
FullIdTypeList : T_ID Type
               | T_ID Type ',' IdTypeList
               ;

Block : '{' VarDecls Statements '}'
      ;

VarDecls : /* empty */
         | VarDecl VarDecls  
         ;

VarDecl  : T_VAR T_ID Type ';' 
         ;

VarDecl  : T_VAR T_ID ArrayType ';'
         ;

Statements  : /* empty */ 
            | Statement Statements 
            ;

Statement   : Block 
            ;

Statement   : Assign ';' 
            ;

Assign   : Lvalue T_ASSIGN Expr 
         ;

Lvalue   : T_ID 
         | T_ID '[' Expr ']' 
         ;

Statement   : MethodCall ';' 
            ;

/*fix methoc call like the instructions, add FullMethodArgs*/
MethodCall  : T_ID '(' MethodArgs ')'
            ;

MethodArg   : Expr
            | T_STRINGCONSTANT
            ;

MethodArgs  : /* empty */
            | FullMethodArgs
            ;

FullMethodArgs : MethodArg
               | MethodArg ',' MethodArgs
               ;

Statement   : T_IF '(' Expr ')' Block

Statement   : T_IF '(' Expr ')' Block  T_ELSE  Block 
            ;

Statement   : T_WHILE '(' Expr ')' Block 
            ;

/*add more case for return statment)*/
Statement   : T_RETURN ';'
            | T_RETURN '(' ')' ';'
            | T_RETURN '(' Expr ')' ';'
            ;

Statement   : T_BREAK ';'
            ;

Statement   : T_CONTINUE ';'
            ;

Expr  : Simpleexpression

Simpleexpression : Additiveexpression
                  | Simpleexpression Relop Additiveexpression
                  ;

/*fix '>' to T_GT*/ 
Relop : T_LEQ | '<' | T_GT | T_GEQ | T_EQ |  T_NEQ
      ;

Additiveexpression   : Term
                     | Additiveexpression Addop Term
                     ;

Addop : '+' | '-'
      ;

Term  : Factor
      | Term Multop Factor
      ;

Multop   : '*' | '/' | '%' | T_AND | T_OR | T_LEFTSHIFT | T_RIGHTSHIFT
         ;

Factor   : T_ID
         | MethodCall
         | T_ID '[' Expr ']'
         | Constant
         | '(' Expr ')'
         | '!' Factor
         | '-' Factor
         ;

ExternType  : T_STRINGTYPE
            | Type  
            ;

Type  : T_INTTYPE 
      | T_BOOLTYPE
      ;

MethodType  : T_VOID 
            | Type
            ;

BoolConstant   : T_TRUE   
               | T_FALSE
               ;

ArrayType   : '[' T_INTCONSTANT ']' Type 
            ;

/*add string adn char as constant*/
Constant    : T_INTCONSTANT 
            | BoolConstant
            ;

%%	/* end of rules, start of program */

int main()
{ yyparse();
   printf("Done!\n");
}
