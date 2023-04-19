/*   Abstract syntax tree code


 Header file   
 Shaun Cooper January 2020

      Nhan Le
      CS370
      File: ast.h 
*/

#include<stdio.h>
#include<malloc.h>

#ifndef AST_H
#define AST_H
static int mydebug;

/* define the enumerated types for the AST.  THis is used to tell us what 
sort of production rule we came across */

//all element that we use in AST 
enum AST_Tree_Element_Type {
   A_PACKAGE,
   A_METHODDEC,
   A_NUMBER,
   A_WHILESTMT,
   A_BLOCK,
   A_EXPR,
   A_PARAM,
   A_VARDEC,
   A_PROGRAM,
   A_EXTERN,
   A_ExternType,
   A_ARRAYTYPE,
   A_METHODID,
   A_CONSTANT_INT,
   A_CONSTANT_BOOL,
   A_CONSTANT_STRING,
   A_BREAK,
   A_RIGHT_VALUE,
   A_LEFT_VALUE,
   A_RETURN,
   A_ASSIGN,
   A_IFSTMT,
   A_IFSTMTBODY,
   A_ELSESTMT,
   A_METHODCALL,
   A_METHODARG,
   A_METHODARGS,
   A_CONTINUE


	   //missing
};

//all operators that we use in AST
enum AST_Operators {
   A_PLUS,
   A_MINUS,
   A_TIMES,
   A_DIVIDE,
   A_MOD,
   A_AND,
   A_OR,
   A_LT,
   A_GT,
   A_EQ,
   A_LEQ,
   A_GEQ,
   A_NEQ,
   A_LEFTSHIFT,
   A_RIGHTSHIFT,
   A_NOT,
   A_UMINUS
	   //missing
};

// all type of decaf that we use in AST
enum AST_Decaf_Types {
   A_Decaf_INT,
   A_Decaf_BOOL,
   A_Decaf_VOID,
   A_Decaf_STRING,
   A_Decaf_CHAR
	   //missing
};

/* define a type AST node which will hold pointers to AST structs that will
  allow us to represent the parsed code 
*/
typedef struct ASTnodetype{
     enum AST_Tree_Element_Type type;
     enum AST_Operators operator;
     char * name;
     int value;
     struct SymbTab *symbol;
	  int size;
     char * label;
     enum AST_Decaf_Types A_Declared_Type;
     struct ASTnodetype *S1,*S2, *next ; /* used for holding IF and WHILE components -- not very descriptive */
} ASTnode;

#include "symtable.h"

/* uses malloc to create an ASTnode and passes back the heap address of the newley created node */
ASTnode *ASTCreateNode(enum AST_Tree_Element_Type mytype);

void PT(int howmany); // prints howmany spaces


ASTnode *program;  // Global Pointer for connection between YACC and backend

/*  Print out the abstract syntax tree */

void ASTprint(int level,ASTnode *p); // prints tree with level horizontal spaceing

//checks that two nodes match in type and length
int check_parameters(ASTnode *formal, ASTnode *actual);
#endif // of AST_H
