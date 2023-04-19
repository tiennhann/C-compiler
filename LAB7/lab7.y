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
   File Name: lab7.y
   Date:   October 23, 2022
   YACC file to process the tokens recieved from Lex 
   Using the tokens given by Lex do simple mathematics for our caluclator 


*/

	/* begin specs */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "ast.h"
#include "symtable.h"

int level = 0; // the level of the scope we are in. 
int offset = 0; // Keeps track of fielddecl offset values
int goffset = 0; // keeps track of 
int maxoffset;


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
            {$1->next = $2;}    //externs to next of exerndfn
         ;  

ExternDefn  : T_EXTERN T_FUNC T_ID '(' ExternParmList ')' MethodType ';'{
                 //chekck to see if $3 has already been defined. if so, BARF
                if (Search($3, level, 0) != NULL) {
                        yyerror("Symbol already deafined");
                        yyerror($3);
                        exit(1);
                }
                $$ = ASTCreateNode(A_EXTERN);
                $$->name = $3;
                $$->A_Declared_Type = $7; 
                $$->size = 0; 
                $$->S1 = $5;  

                //insert symbol to symtable by the return value of Insert()
                $$->symbol = Insert($3,$7,ID_Sub_Type_Extern_Method, level, 0, 0, $5);
                
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
                 //check if the T_ID is used
                if (Search($2, level, 0) != NULL) {
                        yyerror("Variable already defined");
                        yyerror($2);
                        exit(1);
                }
                $$ = ASTCreateNode(A_VARDEC);
                $$->name = $2;
                $$->A_Declared_Type = $3;
                $$->size = 1; //scalars are size 1

                //insert the variable into the symtable
                $$->symbol = Insert($2,$3,ID_Sub_Type_Scalar, level, 1, offset, NULL);
                offset++;
            };

            | T_VAR T_ID ArrayType ';' {
                //check if the T_ID is used
                if (Search($2, level, 0) != NULL) {
                        yyerror("Variable already defined");
                        yyerror($2);
                        exit(1);
                }

                $$ = ASTCreateNode(A_VARDEC);
                $$->name = $2;
                $$->S1 = $3;    //set S1 to the ArrayType
                $$->A_Declared_Type = $3->A_Declared_Type;
                $$->size = $3->size; //arrays are based on the ArrayType value
                //insert the variable into the symtable
                $$->symbol = Insert($2,$3->A_Declared_Type,ID_Sub_Type_Array, level, $$->size, offset, NULL);
                offset += $3->size; //increment the offset by the size of the array.
            };

            | T_VAR T_ID Type T_ASSIGN Constant ';' {
                //Before we create the node let's check if the T_ID is used
                if (Search($2, level, 0) != NULL) {
                        yyerror("Variable already defined");
                        yyerror($2);
                        exit(1);
                }
                $$ = ASTCreateNode(A_VARDEC);
                $$->name = $2;
                $$->A_Declared_Type = $3; //set type to the Type output node 
                $$->size = 1; //this is still a scalar so we set size to 1. 
                $$->symbol = Insert($2,$3,ID_Sub_Type_Scalar, level,$$->size, offset, NULL);
                offset++; //increment offset by 1 because it's a scalar. 
                $$->S2 = $5;    //set S2 to the Constant node
            };

MethodDecls : /* empty */   {$$ = NULL;}
            | MethodDecl MethodDecls {
                $$ = $1;
                $$->next = $2;  //set the next to the right recursive value
            };
            

MethodDecl  :        
        //set global offset to offset 
        //resume global offsets and our offset are ready for next block
            T_FUNC T_ID { goffset = offset;  offset = 0; maxoffset = offset; } 
            '(' IdTypeList ')' MethodType { 
                //check if the T_ID is used
                if (Search($2, level, 0) != NULL) {
                        yyerror("Method name already defined");
                        yyerror($2);
                        exit(1);
                }

                Insert($2,$7,ID_Sub_Type_Method, level, 0, 0, $5); } Block {

                //increment anything greater than $2 by 1 for the new component
                $$ = ASTCreateNode(A_METHODDEC);
                $$->name = $2;
                $$->A_Declared_Type = $7;
                $$->S1 = $5; // $5;
                $$->S2 = $9; // $9;
                $$->symbol = Search($2, level, 0);
                $$->size = maxoffset;
                $$->symbol->mysize = maxoffset; //max size is need
                offset = goffset;
        };

IdTypeList  : /* empty */   {$$ = NULL;}
            | FullIdTypeList    {$$ = $1;}
            ;
         
FullIdTypeList : T_ID Type {
                // Before we create the node let's check if the T_ID is used
                    if(Search ($1, level + 1, 0)!= NULL) {
                        yyerror("Duplicate method arg name");
                        yyerror($1);
                        exit(1);
                    }

                    $$ = ASTCreateNode(A_METHODID);
                    $$->name = $1;
                    $$->A_Declared_Type = $2;
                    $$->size = 1;

                    $$->symbol=Insert($1,$2,ID_Sub_Type_Scalar, level+1, 1, offset, NULL);
                    offset++; //increment offset
                }
               | T_ID Type ',' IdTypeList {
                // check if there are duplicate args
                    if(Search ($1, level + 1, 0)!= NULL) {
                        yyerror("Duplicate method arg name");
                        yyerror($1);
                        exit(1);
                    }

                    $$ = ASTCreateNode(A_METHODID);
                    $$->name = $1;
                    $$->A_Declared_Type = $2;
                    $$->size = 1;
                    $$->next = $4;
                    //level + 1 because the args are part of the block, which is created by the method. 
                    $$->symbol=Insert($1,$2,ID_Sub_Type_Scalar, level+1, 1, offset, NULL);
                    offset++; //increment offset  
               };

Block : '{' { level++; } VarDecls Statements '}' {      //increment the level when we open a block
            $$ = ASTCreateNode(A_BLOCK);
            //changed because added in-rule action
            $$->S1 = $3;    //set S1 to the VarDecls
            $$->S2 = $4;    //set S2 to the Statements
            Display(); // Display here so we can see the symtab at each level
            //if the offset is greater than the maxoffset of the method
            //then the maxoffset = the offset
            if (offset > maxoffset) {
                maxoffset = offset;
            }

            //delete all the symbols at the current level before leaving. 
            //so no conflict with levels lower. 
            offset -= Delete(level); 
            
            //decrement level because the block is finished
            level--;
        };

VarDecls : /* empty */  {$$ = NULL;}
         | VarDecl VarDecls {
            $$ = $1;
            $$->next = $2; 
         };

VarDecl : T_VAR T_ID Type ';' {
            //Before we create the node let's check if the T_ID is used
            if (Search($2, level, 0) != NULL) {
                yyerror("Variable already defined");
                yyerror($2);
                exit(1);
            }    
            $$ = ASTCreateNode(A_VARDEC);
            $$->name = $2;  //set it's name to T_ID
            $$->A_Declared_Type = $3;   //set it's type to outpit of Type

            //insert the variable into the symtable
            $$->symbol = Insert($2,$3,ID_Sub_Type_Scalar, level, 1, offset, NULL);
            offset++;
        };

        | T_VAR T_ID ArrayType ';'{
            //check if the T_ID is used
            if (Search($2, level, 0) != NULL) {
                yyerror("Variable already defined");
                yyerror($2);
                exit(1);
            }   
            $$ = ASTCreateNode(A_VARDEC);
            $$->name = $2;
            $$->S1 = $3;        //set S1 to the ArrayType 
            $$->A_Declared_Type = $$->S1->A_Declared_Type;
            $$->size = $3->size; //set the size to size of ArrayType node

            //insert the variable into the symtable
            $$->symbol = Insert($2,$3->A_Declared_Type, ID_Sub_Type_Array, level, $$->size, offset, NULL);
            offset += $3->size; //increment offset by the size of the array.
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
            $$->S1 = $3;        //set S1 to the Expression node
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
            //make sure the left and right hand types are matching
            if ( $1->A_Declared_Type != $3->A_Declared_Type) {
                yyerror("Type mismatch on assignment");
                exit(1);
            }
            $$ = ASTCreateNode(A_ASSIGN);
            $$->S1 = $1;
            $$->S2 = $3;
        };
         
Lvalue  : T_ID {
            //the symbol from the table match with the T_ID
            struct SymbTab *p; 
            p = Search($1, level, 1); //search for the T_ID

            //check symbol on LVALUE 
            if(p == NULL) {
                yyerror("Symbol on LVALUE not defined");
                 yyerror($1);
                exit(1);
            }
            //chek to see if the type is a scalar and not an array/method
            if (p->SubType != ID_Sub_Type_Scalar){
                    yyerror($1);
                    yyerror("needs to be a scalar, wrong subtype");
                    exit(1);
            }

                $$ = ASTCreateNode(A_LEFT_VALUE);
                $$->name = $1;
                $$->symbol = p;
                $$->A_Declared_Type = p->Type;
        }
         | T_ID '[' Expr ']' {
            struct SymbTab *p;
            p = Search($1, level, 1);

            //check symbol on LVALUE
            if(p == NULL) {
                 yyerror("Symbol on LVALUE not defined");
                yyerror($1);
                exit(1);
            }
                
            //check to if it's an array and not a Scalar/Method
            if (p->SubType != ID_Sub_Type_Array){
                yyerror($1);
                yyerror("needs to be an array, wrong subtype");
                exit(1);
            }
            $$ = ASTCreateNode(A_RIGHT_VALUE);
            $$->name = $1;
            $$->S1 = $3;
            $$->symbol = p;
            $$->A_Declared_Type = p->Type; // the type is the same as the symbol we got it from
         ;}


/*fix methoc call like the instructions, add FullMethodArgs*/
MethodCall  : T_ID '(' MethodArgs ')' {
                struct SymbTab *p;
                p = Search($1, level, 1);
                
                //check symbol on LVALUE
                if(p == NULL) {
                    yyerror("Symbol on LVALUE not defined");
                    yyerror($1);
                    exit(1);
                }

                //check to see if it's a Method or Extern Method and not an Array/Scalar
                if ((p->SubType != ID_Sub_Type_Method) && (p->SubType != ID_Sub_Type_Extern_Method)){
                    yyerror($1);
                    yyerror("needs to be a method, wrong subtype");
                    exit(1);
                }

                //check that the formal params match the params of actual param
                if (check_parameters(p->fparms, $3) == 0) {
                    yyerror("Formal and actual params do not match");
                    yyerror($1);
                    exit(1);
                }
                $$ = ASTCreateNode(A_METHODCALL);
                $$->name = $1;
                $$->S1 = $3;
                $$->symbol = p;
                $$->A_Declared_Type = p->Type; 
            };

MethodArg   : Expr  {
                $$ = ASTCreateNode(A_METHODARG);
                $$->S1=$1;
                $$->A_Declared_Type = $1->A_Declared_Type;// assign type
                $$->name = TEMP_CREATE();//create temp for storage
                $$->symbol =Insert($$->name,$$->A_Declared_Type,ID_Sub_Type_Scalar,level,1,offset,NULL);// insert to symtable
                offset++;//increase offset                      //  calls Expr
                }

	        | T_STRINGCONSTANT
     		{ $$ = ASTCreateNode(A_CONSTANT_STRING);// create node
			  $$->A_Declared_Type = A_Decaf_STRING;
			  $$ ->name = $1;// assign name
        	}
	  ;

MethodArgs  : /* empty */ {$$ = NULL;}
            | FullMethodArgs {$$ = $1;}
            ;

FullMethodArgs : MethodArg {
                    $$ = ASTCreateNode(A_METHODARGS);
                    $$->A_Declared_Type = $1->A_Declared_Type;
                    $$ = $1;}
               | MethodArg ',' MethodArgs {
                $$ = ASTCreateNode(A_METHODARGS);
                    $$ = $1;
                    $$->A_Declared_Type = $1 -> A_Declared_Type;
                    $$->next = $3;
               };

Expr  : Simpleexpression {$$ = $1;};

Simpleexpression : AdditiveExpression {$$ = $1;}
                  | Simpleexpression Relop AdditiveExpression {
                        // make type of left is type of right 
                        if (($1->A_Declared_Type != $3->A_Declared_Type)) {
                            yyerror("both sides need to be the same type");
                            exit(1);
                        }
                        $$ = ASTCreateNode(A_EXPR);
                        $$->S1 = $1;    // set S1 to left hand side 
                        $$->operator = $2;  // set the operator to return of Relop
                        $$->A_Declared_Type=A_Decaf_BOOL;
                        $$->S2 = $3;        // set S2 to the right hand side
                        $$->name = TEMP_CREATE(); // create a temporary symbol to hold value
                        $$->symbol = Insert($$->name,$$->A_Declared_Type, ID_Sub_Type_Scalar, level, 1, offset, NULL);

                        // increment offset by 1 because of the temporary value being stored
                        offset++;
                  };

Relop   : T_GT  {$$ = A_GT;}
        | T_LT  {$$ = A_LT;}
        | T_LEQ {$$ = A_LEQ;}
        | T_GEQ {$$ = A_GEQ;}
        | T_EQ  {$$ = A_EQ;}
        | T_NEQ {$$ = A_NEQ;}
        ;

AdditiveExpression   : Term {$$ = $1;}
                     | AdditiveExpression Addop Term {
                        if (($1->A_Declared_Type != $3->A_Declared_Type) ||
	                        ($1->A_Declared_Type != A_Decaf_INT)) {
		                        yyerror("addition and subtraction need INTs only");
		                        exit(1);
	                    }
                        $$ = ASTCreateNode(A_EXPR);
                        $$->S1 = $1;
                        $$->operator = $2;
                        $$->S2 = $3;
                        $$->A_Declared_Type = A_Decaf_INT;
                        $$->name = TEMP_CREATE();
                        $$->symbol = Insert($$->name,$$->A_Declared_Type, ID_Sub_Type_Scalar, level, 1, offset, NULL);
                        offset++;
                     };

Addop   : '+'   {$$ = A_PLUS;} 
        | '-'   {$$ = A_MINUS;} 
        ;

Term    : Factor  {$$ = $1;}
        | Term Multop Factor {
            //check that left is equal to right
            if ($1->A_Declared_Type != $3->A_Declared_Type) {
                yyerror("type mismatch");
                exit(1);
            }

            if( $1->A_Declared_Type ==A_Decaf_BOOL && (($2==A_TIMES)||($2==A_DIVIDE)||($2==A_MOD) )){
                yyerror("cannot use booleans in arithmatic operations");
            exit(1);
            }
          
            if( $1->A_Declared_Type ==A_Decaf_INT && (($2==A_AND)||($2==A_OR)||($2==A_LEFTSHIFT)||
          ($2==A_RIGHTSHIFT)))
          {yyerror("cannot use integers in boolean operations");
           exit(1);
          }
            $$ = ASTCreateNode(A_EXPR);
            $$->S1 = $1;
            $$->operator = $2;
            $$->S2 = $3;
            $$->name = TEMP_CREATE();
            $$->symbol = Insert($$->name,$$->A_Declared_Type, ID_Sub_Type_Scalar, level, 1, offset, NULL);
            offset++;
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
            struct SymbTab *p;
            p = Search($1, level, 1);
            if(p == NULL) {
                yyerror("Symbol on LVALUE not defined");
                yyerror($1);
                exit(1);
            }

            if (p->SubType != ID_Sub_Type_Scalar){
                yyerror($1);
                yyerror("needs to be an scalar, wrong subtype");
                exit(1);
            }
            $$ = ASTCreateNode(A_RIGHT_VALUE);
            $$->name = $1;
            $$->symbol = p;
            $$->A_Declared_Type = p->Type;
        }

        | MethodCall {$$ = $1;}
        | T_ID '[' Expr ']' {
            struct SymbTab *p;
            p = Search($1, level, 1);
            if(p == NULL) {
                yyerror("Symbol on RVALUE not defined");
                yyerror($1);
                exit(1);
            }

            if (p->SubType != ID_Sub_Type_Array){
                yyerror($1);
                yyerror("needs to be an array, wrong subtype");
                exit(1);
            }

            $$ = ASTCreateNode(A_RIGHT_VALUE);
            $$->name = $1;
            $$->S1 = $3;
            $$->symbol = p;
            $$->A_Declared_Type = p->Type;
        }
        | Constant  {$$ = $1;}
        | '(' Expr ')'  {$$ = $2;}
        | '!' Factor {
            //check if our right is a bool
            if ($2->A_Declared_Type != A_Decaf_BOOL) {
                yyerror("Type mismatch, expected boolean");
                exit(1);
            }
            $$ = ASTCreateNode(A_EXPR);
            $$->operator = A_NOT;
            $$->S1 = $2;

            //set type to boolean because !bool is a bool 
            $$->A_Declared_Type = A_Decaf_BOOL;

            $$->name = TEMP_CREATE();
            $$->symbol = Insert($$->name,A_Decaf_BOOL, ID_Sub_Type_Scalar, level, 1, offset, NULL);
            offset++;
        }

        | '-' Factor {
            //check if our right is an int
            if ($2->A_Declared_Type != A_Decaf_INT) {
                yyerror("Type mismatch, expected integer");
                exit(1);
            }
            $$ = ASTCreateNode(A_EXPR);
            $$->operator = A_UMINUS;
            $$->S1 = $2;
            //set type to boolean because UMINUS is for numbers 
            $$->A_Declared_Type = A_Decaf_INT;
            $$->name = TEMP_CREATE();
            $$->symbol = Insert($$->name,A_Decaf_BOOL, ID_Sub_Type_Scalar, level, 1, offset, NULL);
            offset++;
        };

        /* moved this around a lot until I got it working in here 
        | T_STRINGCONSTANT {
                $$ = ASTCreateNode(A_CONSTANT_STRING);
                $$->name = $1;
                $$->A_Declared_Type = A_Decaf_STRING; 
        };*/

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
                    $$->A_Declared_Type = A_Decaf_BOOL;
                }
               | T_FALSE    {
                    $$ = ASTCreateNode(A_CONSTANT_BOOL);
                    $$->value = 0;
                    $$->A_Declared_Type = A_Decaf_BOOL;
               };

ArrayType   : '[' T_INTCONSTANT ']' Type {
                $$ = ASTCreateNode(A_ARRAYTYPE);
                $$->value = $2;
                $$->A_Declared_Type = $4;
                $$->size = $2;
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
    Display();
   printf("Parsing complted\n");
   //ast global PROGRAM point to our AST
   ASTprint(0,PROGRAM);
   printf("Finished printing AST\n");
}
