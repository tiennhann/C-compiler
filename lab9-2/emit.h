//  
//
//   Garrett Turner 
/// lab 9 
/// 12 / 2 /2022 
// Takes decaf code and generates gas assembly. 


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef EMIT_H
#define EMIT_H

#include "ast.h"
#include "symtable.h"

FILE *fp;



void EMIT ( ASTnode *p, FILE *fp);
void Emit_Strings( ASTnode *p, FILE *fp);
void Emit_Globals(ASTnode *p, FILE *fp);
void Emit_AST (ASTnode *p, FILE *fp);
void Emit_method_dec(ASTnode *p, FILE *fp);
void emit (FILE *fp,char *label, char * action, char * comment); 
void Emit_Return (ASTnode *p, FILE *fp);
void Emit_Expr (ASTnode *p, FILE *fp);
void Emit_Method_Call(ASTnode *p, FILE *fp);
void Emit_Extern_Method_Call(ASTnode *p, FILE *fp);
void Emit_Assign (ASTnode *p, FILE *fp) ;
void Emit_Ident( ASTnode *p, FILE *fp);
void Emit_If (ASTnode *p, FILE *fp);
void Emit_While (ASTnode *p, FILE *fp);






#endif  // of EMIT.h