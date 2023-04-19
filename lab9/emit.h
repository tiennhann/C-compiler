

//  Subprograms which output NASM code.
//
//   Shaun Cooper Spring 2017
/// Updated Spring 2017 to utilize the data segement portion of NASM instead of advancing the Stack and Global pointers
/// to accomodate global variables.

// NASM is called on linux as
//  gcc foo.s 

//Nhan Le
//CS370

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef EMIT_H
#define EMIT_H

#include "ast.h"
#include "symtable.h"

FILE *fp;

void EMIT ( ASTnode *p, FILE *fp);
void emit (FILE *fp,char *label, char * action, char * comment); 
void emitAST(ASTnode *p, FILE *fp);
void emit_global(ASTnode *p, FILE *fp);
void emit_assign (ASTnode *p, FILE *fp) ;
void emit_ident( ASTnode *p, FILE *fp);
void emit_strings( ASTnode *p, FILE *fp);
void emit_return (ASTnode *p, FILE *fp);
void emit_expr (ASTnode *p, FILE *fp);
void emit_method_dec(ASTnode *p, FILE *fp);
void emit_method_call(ASTnode *p, FILE *fp);
void emit_extern_method_call(ASTnode *p, FILE *fp);
void emit_if (ASTnode *p, FILE *fp);
void emit_while (ASTnode *p, FILE *fp);



#endif  //end emit.h
