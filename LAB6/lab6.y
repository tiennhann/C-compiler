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
   File Name: lab6.y
   Date:   October 2, 2022
   YACC file to process the tokens recieved from Lex 
   Using the tokens given by Lex do simple mathematics for our caluclator 


*/

	/* begin specs */
#include <stdio.h>
#include <ctype.h>
#include "ast.h"

int yylex(); //fix the (error) implicit declaration of function yylex
extern int lineCount;   //import lineCount as static variable
struct ASTnodetype *PROGRAM;  //AST global variable

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
    struct ASTnodetype *astnode;
    enum AST_Decaf_Types asttype;
    enum AST_Operators operator;
}

/*Adding tokens that match with lex tokens */
%token <value> T_CHARCONSTANT
%token  T_DOT  T_FOR T_GT T_NULL T_LT
%token <string> T_STRINGCONSTANT
%token <value> T_INTCONSTANT
%token <string> T_ID 
%token T_AND T_ASSIGN T_BOOLTYPE T_BREAK T_CONTINUE 
%token T_ELSE T_EQ T_EXTERN T_FALSE T_FUNC T_GEQ 
%token T_IF T_INTTYPE T_LEFTSHIFT T_LEQ T_NEQ 
%token T_OR T_PACKAGE T_RETURN T_RIGHTSHIFT 
%token T_STRINGTYPE T_TRUE T_VAR T_VOID T_WHILE       
%token <string> T_STRING_LIT
%token <value> T_INT_LIT

/*Adding type that we use in this code*/
%type <astnode> Externs ExternDefn ExternParmList FullExternParmList 
%type <astnode> FieldDecls FieldDecl ArrayType Constant 
%type <astnode> MethodDecls MethodDecl Block 
%type <astnode> IdTypeList FullIdTypeList 
%type <astnode> VarDecls VarDecl 
%type <astnode> Statement Statements ReturnStmt BreakStmt IfStmt WhileStmt ContinueStmt
%type <astnode> Assign Lvalue MethodCall MethodArgs FullMethodArgs MethodArg
%type <astnode> Expr Simpleexpression AdditiveExpression 
%type <astnode> BoolConstant Term Factor  
%type <asttype> ExternType Type MethodType
%type <operator>  Relop Addop Multop


/*Adding all the rules from DECAF and also reformating them so they can work currently code*/
/*For a rule that has a list of token, created a recursive rule that allows to take one or more tokens*/
/*Replace '=' by T_ASSIGN*/
%%
Program  : Externs T_PACKAGE T_ID '{' FieldDecls MethodDecls '}'{
                PROGRAM = ASTCreateNode(A_PROGRAM);
                PROGRAM->S1 = $1;
                PROGRAM->S2 = ASTCreateNode(A_PACKAGE);
                PROGRAM->S2->name = $3;
                PROGRAM->S2->S1 = $5; 
                PROGRAM->S2->S2 = $6; 
         };

Externs  : /* empty */
            {$$ = NULL;}
         | ExternDefn Externs
            {$1->next = $2;}
         ;  

ExternDefn  : T_EXTERN T_FUNC T_ID '(' ExternParmList ')' MethodType ';'{
                $$ = ASTCreateNode(A_EXTERN);
                $$->name = $3;
                $$->A_Declared_Type = $7; 
                $$->S1 = $5;  
            };

ExternParmList :  /* empty */   {$$ = NULL;}
               | FullExternParmList {$$ = $1;}
               ; 

FullExternParmList:  ExternType {
                        $$ = ASTCreateNode(A_ExternType);
                        $$->A_Declared_Type = $1;
                    }

                  |  ExternType ',' ExternParmList {
                        $$ = ASTCreateNode(A_ExternType);
                        $$->A_Declared_Type = $1;
                        $$->next = $3;
                  };

FieldDecls  : /* empty */   {$$ = NULL;}
            |  FieldDecl   FieldDecls {
                $$ = $1;
                $$->next = $2;
            };
            
FieldDecl   : T_VAR T_ID Type ';' {
                $$ = ASTCreateNode(A_VARDEC);
                $$->name = $2;
                $$->A_Declared_Type = $3;
            };

            | T_VAR T_ID ArrayType ';' {
                $$ = ASTCreateNode(A_VARDEC);
                $$->name = $2;
                $$->S1 = $3;
                $$->A_Declared_Type = $3->A_Declared_Type;
            };

            | T_VAR T_ID Type T_ASSIGN Constant ';' {
                $$ = ASTCreateNode(A_VARDEC);
                $$->name = $2;
                $$->A_Declared_Type = $3;
                $$->S2 = $5;
            };

MethodDecls : /* empty */   {$$ = NULL;}
            | MethodDecl MethodDecls {
                $$ = $1;
                $$->next = $2;
            };
            

MethodDecl  : T_FUNC T_ID '(' IdTypeList ')' MethodType Block {
                $$ = ASTCreateNode(A_METHODDEC);
                $$->name = $2;
                $$->A_Declared_Type = $6;
                $$->S1 = $4; 
                $$->S2 = $7; 
            };

IdTypeList  : /* empty */   {$$ = NULL;}
            | FullIdTypeList    {$$ = $1;}
            ;
         
FullIdTypeList : T_ID Type {
                    $$ = ASTCreateNode(A_METHODID);
                    $$->name = $1;
                    $$->A_Declared_Type = $2;
                }
               | T_ID Type ',' IdTypeList {
                    $$ = ASTCreateNode(A_METHODID);
                    $$->name = $1;
                    $$->A_Declared_Type = $2;
                    $$->next = $4;
               };

Block : '{' VarDecls Statements '}' {
            $$ = ASTCreateNode(A_BLOCK);
            $$->S1 = $2;
            $$->S2 = $3;
        };

VarDecls : /* empty */  {$$ = NULL;}
         | VarDecl VarDecls {
            $$ = $1;
            $$->next = $2; 
         };

VarDecl : T_VAR T_ID Type ';' {
            $$ = ASTCreateNode(A_VARDEC);
            $$->name = $2;
            $$->A_Declared_Type = $3;
        };

        | T_VAR T_ID ArrayType ';'{
            $$ = ASTCreateNode(A_VARDEC);
            $$->name = $2;
            $$->S1 = $3;
            $$->A_Declared_Type = $$->S1->A_Declared_Type;
        };

Statements  : /* empty */   {$$ = NULL;}
            | Statement Statements {
                $$ = $1;
                $$->next = $2;
            };

Statement   : Block {$$ = $1;}
            | Assign ';' {$$ = $1;}
            | MethodCall ';' {$$ = $1;}
            | IfStmt  {$$ = $1;}
            | WhileStmt {$$ = $1;}
	        | BreakStmt {$$ = $1;}
            | ReturnStmt {$$ = $1;}
            | ContinueStmt {$$ = $1;}
            ;

IfStmt  : T_IF '(' Expr ')' Block {
                $$ = ASTCreateNode(A_IFSTMT);
                $$->S1 = $3;
                $$->S2 = ASTCreateNode(A_IFSTMTBODY);
                $$->S2->S1 = $5;
                $$->S2->S2 = NULL;
        }

        | T_IF '(' Expr ')' Block T_ELSE Block {
            $$ = ASTCreateNode(A_IFSTMT);
            $$->S1 = $3;
            $$->S2 = ASTCreateNode(A_IFSTMTBODY);
            $$->S2->S1 = $5;
            $$->S2->S2 = ASTCreateNode(A_ELSESTMT);
            $$->S2->S2->S1 = $7;
        };
        
WhileStmt: T_WHILE '(' Expr ')' Block {
                $$ = ASTCreateNode(A_WHILESTMT);
                $$->S1 = $3;
                $$->S2 = $5;
            };

BreakStmt: T_BREAK ';' {$$ = ASTCreateNode(A_BREAK);};

ReturnStmt  : T_RETURN ';' { $$ = ASTCreateNode(A_RETURN);}
            | T_RETURN '(' ')' ';'  { $$ = ASTCreateNode(A_RETURN);}
            | T_RETURN '(' Expr ')' ';' {
                $$ = ASTCreateNode(A_RETURN);
                $$->S1 = $3; 
            };

ContinueStmt: T_CONTINUE ';' {$$ = ASTCreateNode(A_CONTINUE);};

Assign  : Lvalue T_ASSIGN Expr  {
                $$ = ASTCreateNode(A_ASSIGN);
                $$->S1 = $1;
                $$->S2 = $3;
        };
         
Lvalue  : T_ID {
            $$ = ASTCreateNode(A_RIGHT_VALUE);
            $$->name = $1;
        }
         | T_ID '[' Expr ']' {
            $$ = ASTCreateNode(A_RIGHT_VALUE);
            $$->name = $1;
            $$->S1 = $3;
         ;}


/*fix methoc call like the instructions, add FullMethodArgs*/
MethodCall  : T_ID '(' MethodArgs ')' {
                $$ = ASTCreateNode(A_METHODCALL);
                $$->name = $1;
                $$->S1 = $3;
            };

MethodArg   : Expr  {$$ = $1;}
            | T_STRINGCONSTANT {
                $$ = ASTCreateNode(A_CONSTANT_STRING);
                $$->name = $1;
            };

MethodArgs  : /* empty */ {$$ = NULL;}
            | FullMethodArgs {$$ = $1;}
            ;

FullMethodArgs : MethodArg {$$ = $1;}
               | MethodArg ',' MethodArgs {
                    $$ = $1;
                    $$->next = $3;
               };

Expr  : Simpleexpression {$$ = $1;};

Simpleexpression : AdditiveExpression {$$ = $1;}
                  | Simpleexpression Relop AdditiveExpression {
                        $$ = ASTCreateNode(A_EXPR);
                        $$->S1 = $1;
                        $$->operator = $2;
                        $$->S2 = $3;
                  }
                  ;

Relop   : T_GT  {$$ = A_GT;}
        | T_LT  {$$ = A_LT;}
        | T_LEQ {$$ = A_LEQ;}
        | T_GEQ {$$ = A_GEQ;}
        | T_EQ  {$$ = A_EQ;}
        | T_NEQ {$$ = A_NEQ;}
        ;

AdditiveExpression   : Term {$$ = $1;}
                     | AdditiveExpression Addop Term {
                        $$ = ASTCreateNode(A_EXPR);
                        $$->S1 = $1;
                        $$->operator = $2;
                        $$->S2 = $3;
                     };

Addop   : '+'   {$$ = A_PLUS;} 
        | '-'   {$$ = A_MINUS;} 
        ;

Term    : Factor  {$$ = $1;}
        | Term Multop Factor {
            $$ = ASTCreateNode(A_EXPR);
            $$->S1 = $1;
            $$->operator = $2;
            $$->S2 = $3;
        };

Multop  : '*'   {$$ = A_TIMES;}
        | '/'   {$$ = A_DIVIDE;}
        | '%'   {$$ = A_MOD;}
        | T_AND {$$ = A_AND;}
        | T_OR  {$$ = A_OR;}
        | T_LEFTSHIFT   {$$ = A_LEFTSHIFT;}
        | T_RIGHTSHIFT  {$$ = A_RIGHTSHIFT;}
        ;

Factor  : T_ID {
            $$ = ASTCreateNode(A_RIGHT_VALUE);
            $$->name = $1;
        }
        | MethodCall {$$ = $1;}
        | T_ID '[' Expr ']' {
            $$ = ASTCreateNode(A_RIGHT_VALUE);
            $$->name = $1;
            $$->S1 = $3;
        }
        | Constant  {$$ = $1;}
        | '(' Expr ')'  {$$ = $2;}
        | '!' Factor {
            $$ = ASTCreateNode(A_EXPR);
            $$->operator = A_NOT;
            $$->S1 = $2;
        }
        | '-' Factor {
            $$ = ASTCreateNode(A_EXPR);
            $$->operator = A_UMINUS;
            $$->S1 = $2;
        }
         ;

ExternType  : T_STRINGTYPE {$$ = A_Decaf_STRING;}
            | Type  
            ;

Type  : T_INTTYPE   {$$ = A_Decaf_INT;}
      | T_BOOLTYPE  {$$ = A_Decaf_BOOL;}
      ;

MethodType  : T_VOID {$$ = A_Decaf_VOID;}
            | Type   {$$ = $1;}
            ;

BoolConstant   : T_TRUE {   
                    $$ = ASTCreateNode(A_CONSTANT_BOOL);
                    $$->value = 1;
                }
               | T_FALSE    {
                    $$ = ASTCreateNode(A_CONSTANT_BOOL);
                    $$->value = 0;
               }
               ;

ArrayType   : '[' T_INTCONSTANT ']' Type {
                $$ = ASTCreateNode(A_ARRAYTYPE);
                $$->value = $2;
                $$->A_Declared_Type = $4;
            };

/*add string adn char as constant*/
Constant    : T_INTCONSTANT {
                $$ = ASTCreateNode(A_CONSTANT_INT);
                $$->value = $1;
            }
            | BoolConstant  {$$ = $1;}
            ;

%%	/* end of rules, start of program */

int main()
{ yyparse();
   printf("Parsing complted\n");
   //ast global PROGRAM point to our AST
   ASTprint(0,PROGRAM);
   printf("Finished printing AST\n");
}
