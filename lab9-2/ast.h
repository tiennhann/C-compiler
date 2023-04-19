/*   Abstract syntax tree code


 Header file   
 Shaun Cooper January 2020

      Nhan Le
      CS370
      File: ast.h 
*/
#include<stdio.h>
#include<malloc.h>
//#include "symtable.h" //added lab7 video 


#ifndef AST_H
#define AST_H
static int mydebug;

/* define the enumerated types for the AST.  THis is used to tell us what 
sort of production rule we came across */
// lab 7 update removed A_ELSE node unnessary node 
enum AST_Tree_Element_Type {
   A_PROGRAM, // added lab6 // 0   
   A_PACKAGE, // 1
   A_EXTERN, // 2
   A_EXTERN_TYPE, // 3
   A_ARRAYTYPE, // 4
   A_METHODDEC, // 5
   A_METHODIDENTIFER, //6
   A_METHODCALL, //7
   A_METHOD_ARG, // 8 added lab7
   A_NUMBER, // 9
   A_FORSTMT, // 10 
   A_WHILESTMT,// 11 
   A_IFSTMT,// 12
   A_IFBODY, // 13
   A_BLOCK, // 14 
   A_EXPR, // 15
   A_PARAM,// 16
   A_CONTSTANT_INT, // 17
   A_CONTSTANT_BOOL, // 18 
   A_CONTSTANT_STRING, // 19 
   A_BREAK, // 19
   A_CONTINUE, // 20
   A_RETURN, // 21 
   A_VAR_RVALUE, // 22 
   A_VAR_LVALUE, // 23
   A_ASSIGN, // 24
   A_VARDEC // 25 

	   
};


enum AST_Operators {
   
   A_PLUS, // lab6
   A_MINUS,
    A_UMINUS, // added lab6
   A_TIMES, // added lab6
   A_DIVIDE, // added lab6 
   A_AND, // added lab6 
   A_OR, // added lab6 
   A_LEFTSHIFT, // added lab6 
   A_RIGHTSHIFT,// added lab6               
   A_LEQ, //added lab6                                         
   A_LT,//added lab6  
   A_GT,   //added lab6   
   A_GEQ,  //added lab6    
   A_EQ,  //added lab6    
   A_NEQ,  //added lab6    
   A_NOT

	    
};

enum AST_Decaf_Types {
   A_Decaf_INT,
   A_Decaf_BOOL,
   A_Decaf_VOID,
   A_Decaf_STRING
	 
};

/* define a type AST node which will hold pointers to AST structs that will
   allow us to represent the parsed code 
*/

typedef struct ASTnodetype
{
     enum AST_Tree_Element_Type type;
     enum AST_Operators operator;
     char * name;
     int value;
     int size; // added lab7;
     struct SymbTab *symbol; //added lab7 video 
     enum AST_Decaf_Types A_Declared_Type; // added lab6 
     char *label; // added lab9
     struct ASTnodetype *S1,*S2, *next ; /* used for holding IF and WHILE components -- not very descriptive */
} ASTnode;

#include "symtable.h"// added lab7 video 


/* uses malloc to create an ASTnode and passes back the heap address of the newley created node */
ASTnode *ASTCreateNode(enum AST_Tree_Element_Type mytype);

void PT(int howmany); // prints howmany spaces


ASTnode *program;  // Global Pointer for connection between YACC and backend

/*  Print out the abstract syntax tree */
void ASTprint(int level,ASTnode *p); // prints tree with level horizontal spaceing

int check_parameters(ASTnode *formal, ASTnode *actual); // checks if formal and actual parameters match
#endif // of AST_H
