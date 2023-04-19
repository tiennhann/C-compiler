

//  Subprograms which output MIPS code.
//
//   Shaun Cooper Spring 2015
/// Updated Spring 2017 to utilize the data segement portion of MIPS instead of advancing the Stack and Global pointers
/// to accomodate global variables.
//  Shaun Cooper December 2020 updated for DECAF format

//Nhan Le
//CS370
//emit.c



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "emit.h"
#include "symtable.h"


// The wordsize in bytes 
#define WSIZE     8
#define LOG_WSIZE 3

static int GLABEL=0;  // global label counter

char  * CURRENT_FUNC; //need to know Current Function for return on main

//creates one_up labels of the form _L%d
//  PRE:  none
//  POST:  new copy of strring of the form _L% and GLABEL is incremented
char * genlabel()
{  	char gen_label[100];
	sprintf(gen_label,"_L%d",GLABEL); 
  	GLABEL++; 
  	return(strdup(gen_label));
}

void EMIT ( ASTnode *p, FILE *fp) {
   	//prints the package 
   	fprintf(fp,"\n\n# Package   %s\n", p->S2->name);  
   	fprintf(fp,"\n");
   	fprintf(fp,"\t .section .rodata # start of the RODATA section, strings first\n");
   	fprintf(fp,"\n");
   	fprintf(fp,"percentD:  .string \"%%d\" # Always needed for print int\n");
   	fprintf(fp," # NEXT lines are the strings in the program\n");  

   	emit_strings(p,fp); 
   
   	fprintf(fp," # END of are the strings in the program\n\n");  
   	fprintf(fp,"\t.data # start of the DATA section for variables\n\n\n");
   	fprintf(fp,"\t.comm _SCANFHOLD,8,8    # MANDITORY space for SCANF reads\n\n");
   
   	emit_global(p,fp);
   	fprintf(fp,"# END  of the DATA section for variables\n");  
   
   	fprintf(fp,"\n");
   	fprintf(fp,"\n");

   	fprintf(fp,"\t.text    # start of CODE segement\n\n");
   
   
   	emitAST(p,fp);
	fprintf(fp,"\n");
    emit(fp,"",".ident  \"GCC: (SUSE Linux) 7.5.0\"", "");
    emit(fp, "",".section        .note.GNU-stack,\"\", @progbits","");
	fprintf(fp,"\n"); 
}//end EMIT

void emit_global(ASTnode *p, FILE *fp){
	//check to see if p is NULL or not
	if (p==NULL) return;

	if ((p->type == A_VARDEC) && (p->symbol->level == 0)){   
		// check to see if dec is intialized 
        if (p->S2 == NULL){
            fprintf(fp,"\t.comm %s, %d, %d  # define a global variable \n", p->name, p->symbol->mysize * WSIZE,p->symbol->mysize * WSIZE);
            fprintf(fp,"\t.align 8\n");  
        }
        //intiallize the variable
        else {
            fprintf(fp,"%s:   .long %d # define a global variable with intial value \n", p->name, p->S2->value);
        }
         
    }
    emit_global(p->S1,fp);
    emit_global(p->S2,fp);
    emit_global(p->next,fp);
}//end emit_global

/* Pre Pointer to AST TREE 
   Post outpout to FP that has Label strings 
 */
void emit_strings( ASTnode *p, FILE *fp)
{    // check if p is null 
   if (p == NULL)   return; 

   //generates a label if it is a string 
   if (p -> type == A_CONSTANT_STRING){
      p -> label = genlabel(); 
      fprintf(fp,"%s:\t.string\t%s\t# global string\n", p->label,p->name);
   }

   emit_strings(p->S1,fp);
   emit_strings(p->S2,fp);
   emit_strings(p->next,fp);
}
/*  EMIT the Abstract syntax tree */
//PRE: Pointer to AST
//POST:Generates all the GAS for everything in the tree,
//except global variables and strings  
void emitAST(ASTnode *p,FILE *fp){
	//check to see if p is NULL or not
   	if (p == NULL ) return;

		//do this to match the node for each case
    	switch (p->type) {
			case A_PROGRAM:
				emitAST(p->S1,fp);
				emitAST(p->S2,fp);
				emitAST(p->next,fp);
				break;

        	case A_PACKAGE :  
			 	emitAST(p->S1,fp); // externs
				emitAST(p->S2,fp); // PACKAGE_SUB
				emitAST(p->next,fp);
			   	break;
			
			case A_EXTERN:	break;
			case A_ExternType: break;
			case A_VARDEC: break;

			case A_METHODDEC:
				emit_method_dec(p,fp);
				emitAST(p->next,fp);
				break;
			
			case A_METHODID:
				emitAST(p->S1,fp);
				emitAST(p->next,fp);
				break;

			case A_METHODCALL:
				emit_method_call (p,fp);
				emitAST(p->next,fp);
				break;

			case A_BLOCK:
				emitAST(p->S1,fp);
				emitAST(p->S2,fp);
				emitAST(p->next,fp);
				break;

			case A_ASSIGN: 
				emit_assign(p,fp);
				emitAST(p->next,fp);
				break;

			case A_IFSTMT:
				emit_if(p,fp);
				break;
			
			case A_WHILESTMT:
				emit_while(p,fp);
				break;

			case A_RETURN: 
				emit_return(p,fp);
				emitAST(p->next, fp);
				break;

			default: printf("%d:  unknown type in EMITAST\n",p->type);
       	}// end switch

}//end EMITAST

//PRE: PTR to AST noded that points to Method Declarion and PTR open fp
//POST: GAS code for the method
void emit_method_dec(ASTnode *p,FILE *fp) {
	char s[100];
	int count = 8; 

	fprintf(fp,"\t.globl %s \n", p->name);
	fprintf(fp,"\t.type %s, @function \n", p->name);
	fprintf(fp, p->name, "", "Start of Function");
	fprintf(fp,"\n");
	fprintf(fp,".cfi_startproc #	STANDARD FUNCITON HEADER FOR GAS \n");
	fprintf(fp,"\tpushq	%rbp  #		STANDARD FUNCITON HEADER FOR GAS \n");
	fprintf(fp,"t.cfi_def_cfa_offset 16 #	STANDARD FUNCITON HEADER FOR GAS \n");
	fprintf(fp,"t.cfi_offset 6, -16 #	STANDARD FUNCITON HEADER FOR GAS \n");
	fprintf(fp,"tmovq	%rsp, %rbp  #	STANDARD FUNCITON HEADER FOR GAS \n");
	fprintf(fp,"t.cfi_def_cfa_register 6 #	STANDARD FUNCITON HEADER FOR GAS \n");

	//need to generate GAS code fo the body of the function
	fprintf(fp," \n");
	fprintf(fp," \n");
	//carve out our space on the stack
	sprintf(s, "subq $%d, %rsp", p->symbol->mysize * WSIZE);
	emit(fp,"",s,"Carve out Stack for method");
	emitAST(p->S1,fp); 

    //parmerter are held the value
    ASTnode *param = p->symbol->fparms;

	//getting all methodidentifers for registers 
    while((param != NULL) && (count <= 16)){ 
        sprintf(s,"mov %r%d, %d(%rsp)", count, param-> symbol->offset * WSIZE  ); // stores parmaters in the regsters 8 to 15 
        emit(fp,"",s,"copy actual to appropriate slot");
        param = param->next; 
        count++;    
    } //end while 

	//generate GAS code fir tge BODY of the function definition
    emitAST(p->S1,fp); 
    emitAST(p->S2,fp);
        
    //return 
    emit_return(NULL,fp);
        
    //end of function 
    emit(fp,"",".cfi_endproc","# STANDRD end function for GAS");  
    sprintf(s,".size	%s, .-%s",p->name,p->name);
    emit(fp,"",s,"STANDRD end function for GAS");
    fprintf(fp,"\n");
}//end of emit_method_dec

// helper routine which outputs label, command,and comment lines
//PRE: FP and vaires strings to print, or NULL
//POST: output to FP with assembly formatting
void emit(FILE *fp, char *label, char *action, char *comment){
	if (strcmp(label, "") == 0) {
		if (strcmp(action, "") == 0){	//no label, no action
			if (strcmp(comment,"") == 0) {	//no action, no comment
				fprintf(fp,"\n");
			}
			else {	//no label, no action, yes comment
				fprintf(fp, "\t\t# %s\n", comment);
			}
		}//end if

		else {	//yes action, no label
			if (strcmp(comment,"") == 0){ 	//action yes, no label and comment
				fprintf(fp, "\t%s\t\n", action);
			}	
			else {	//yes command, yes comment
				fprintf(fp, "\t%s\t# %n", action, comment);
			}
        }
	}//end if

	//we have label		
	else {
		if (strcmp(action,"") == 0) {	//no action, yes label
			if(strcmp(comment,"") == 0) { 		//action no label yes comment no
				fprintf(fp,"%s:\n", label);
			}
			else {	//yes label yes action 
				fprintf(fp,"%s:\t%s\t# %s\n", label, action, comment);
			}//end else
		}//end if

	}//end else
}//end emit

//PRE: PTR to AST node that points to return or null 
//POST:	GAS code for the return, %rax is 0 for NULL and set by Emit_expr() for expression
void emit_return(ASTnode *p,FILE *fp) {
	char s[100];
	if(p == NULL || p->S1 == NULL)	//implict return
		emit(fp,"","mov $0, %rax", "Default Return Value");
	else
		emit_expr(p->S1,fp);
	
	//this is the GAS code for the return to leave the function
	emit(fp,"","leave","leave the function");
	emit(fp,"",".cfi_def_cfa 7,8","Standard end funciton for GAS");
	emit(fp,"","ret","");
	fprintf(fp,"\n\n");
}//end emit_return

//PRE: Pointer to ASTnode that is in the class of Expression
//including constants and method call
//POST: GAS code that sets %rax to the evaluation of the expression
void emit_expr(ASTnode *p, FILE *fp){
   	char s[100];
   	
	//check to see if p is NULL or not
   	if (p == NULL) {
		printf("Illegal use of emit_expr NULL input");
		exit(1);
	}

   	//base cases
   	switch (p->type) {  // missing
		case A_CONSTANT_INT:		//get the value to the memory
			sprintf(s,"mov $%d, %rax", p->value);
			emit(fp,"",s,"Expression a Number");
			return;
			break;

	   	case A_CONSTANT_BOOL:
		   sprintf(s,"mov $%d, %rax", p->value);
		   emit(fp,"", s, "Expression a Bool");
		   return;
		   break;

	   	case A_CONSTANT_STRING:
		   sprintf(s,"mov $%d, %rax", p->value);
		   emit(fp,"", s, "expr constant is a string");
		   return;
			break;

	   	case A_METHODCALL:
		   emit_method_call(p,fp); 	//go to method call func
		   return;		
		   break;

		case A_BLOCK : 
			emitAST(p,fp); 			//go to AST to evalute the body 
            break;    

		case A_LEFT_VALUE:
			emit_ident(p,fp); 
		   	return;
        	break;

	   	case A_RIGHT_VALUE:
	   		emit_ident(p,fp);
			emit(fp,"","mov (%rax), %rax","Expression is identifier");
            return;
			break;

		//these are the base cases for all operator 
		//as the operator, each will get the Right value or Left value 
		case A_EXPR:
			switch(p->operator) {
			case A_PLUS : 	//plus case
				emit_expr(p->S1,fp);
             	sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
                emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                emit_expr(p->S2,fp);
                emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset *WSIZE );
               	emit(fp, "", s, "fetch LHS of expression from memory" );
                emit(fp,"", "add %rbx, %rax" , "EXPR ADD");
                return;
                break;

                case A_MINUS:	//minus case 
					emit_expr(p->S1,fp);  
                    sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
                    emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                    emit_expr(p->S2,fp);
                    emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                    sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset *WSIZE);
                    emit(fp, "", s, "fetch LHS of expression from memory");
                    emit(fp,"", "sub %rbx, %rax" , "EXPR SUB");
                    return; 
                	break;

                case A_DIVIDE: 	//divide case
					emit_expr(p->S1,fp);  
                	sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
                    emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                    emit_expr(p->S2,fp);
                    emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                    sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset *WSIZE);
                    emit(fp, "", s, "fetch LHS of expression from memory");
                    emit(fp,"","mov $0,%rdx ","EXPR upper word 0 DIV");
                    emit(fp,"", "idiv %rbx" , "EXPR DIV");
                	return;
                    break;

				case A_TIMES: 	//times case
					emit_expr(p->S1,fp);  
                	sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
                    emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                    emit_expr(p->S2,fp);
                    emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                    sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset *WSIZE );
                    emit(fp, "", s, "fetch LHS of expression from memory" );
                    emit(fp,"", "imul %rbx, %rax" , "EXPR MULT RAX has lower word");
                    return;
                    break; 
              
                case A_AND:  	//AND case
					emit_expr(p->S1,fp);
					sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
					emit(fp, "",s, "Stores the  LHS of expression to memory"); 
					emit_expr(p->S2,fp);
					emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
					sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset *WSIZE);
           			emit(fp, "", s, "fetch LHS of expression from memory");
                    emit(fp,"", "and %rbx, %rax", "EXPR AND"); 
                	return;
        			break;
                        
                case A_OR: 		//OR case
                    emit_expr(p->S1,fp);
                    sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
                    emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                    emit_expr(p->S2,fp);
                    emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                    sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset *WSIZE);
                    emit(fp, "", s, "fetch LHS of expression from memory");
                    emit(fp,"", "or %rbx, %rax", "EXPR OR" ); 
                	return;
                    break;
                                               
                case A_LEQ:		//LEQ case 
                	emit_expr(p->S1,fp);  
                	sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
                	emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                	emit_expr(p->S2,fp);
                	emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                    sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset *WSIZE);
                    emit(fp, "", s, "fetch LHS of expression from memory" );
                    emit(fp, "", "cmp %rbx, %rax","EXPR LESSthan Equal");  
                    emit(fp,"", "setle %al","EXPR Less than");   
                    emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax"); 
	                emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                    return;
                    break;
                         
                case A_GEQ:		//GEQ case 
                    emit_expr(p -> S1,fp );  
                    sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
                    emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                    emit_expr(p -> S2,fp);
                    emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                    sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset *WSIZE );
                    emit(fp, "", s, "fetch LHS of expression from memory" );
                    emit(fp, "", "cmp %rbx, %rax","EXPR GREATERthan Equal");  
                    emit(fp,"", "setge %al","EXPR Lessthan");   
                    emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax"); 
	                emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                    return;
                    break;
                                
                case A_LT: 		//LT case
                    emit_expr(p->S1,fp);  
                    sprintf(s,"mov %rax , %d(%rsp)", p->symbol-> offset * WSIZE); 
                    emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                	emit_expr(p -> S2,fp);
                    emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                    sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset * WSIZE);
                    emit(fp, "", s, "fetch LHS of expression from memory");
                    emit(fp, "", "cmp %rbx, %rax","EXPR LESSthan");  
                    emit(fp,"", "setl %al","EXPR Lessthan");   
                    emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax"); 
	                emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                    return;
                    break;
                  
            	case A_GT: 		//GT case
					emit_expr(p->S1,fp);  
                    sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
                    emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                    emit_expr(p->S2,fp);
                    emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                    sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset * WSIZE);
                    emit(fp, "", s, "fetch LHS of expression from memory" );
                    emit(fp, "", "cmp %rbx, %rax","EXPR Greaterthan");  
                    emit(fp,"", "setg %al","EXPR Lessthan");   
                    emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax"); 
	                emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                    return;
	                break; 
                          
                case A_EQ: 		//EQ case
                    emit_expr(p-> S1,fp);
                	sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
                    emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                    emit_expr(p->S2,fp);
                    emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                    sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset * WSIZE ); 
                    emit(fp, "", s, "fetch LHS of expression from memory" );
                    emit(fp, "", "cmp %rbx, %rax","EXPR NOT EQUAL");  
                    emit(fp,"", "sete %al","EXPR Lessthan");   
                    emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax");
                    emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                    return;
                    break;

                case A_NEQ:   	//NEQ case
					emit_expr(p-> S1,fp);
                    sprintf(s,"mov %rax , %d(%rsp)", p->symbol->offset * WSIZE); 
                    emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                    emit_expr(p -> S2,fp );
                    emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                    sprintf(s, "mov %d(%rsp), %rax", p->symbol->offset * WSIZE); 
                    emit(fp, "", s, "fetch LHS of expression from memory" );
                    emit(fp, "", "cmp %rbx, %rax","EXPR NOT EQUAL");  
                    emit(fp,"", "setne %al","EXPR Lessthan");   
                    emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax");
                    emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                    return;
                	break;
                    
                case A_NOT :  
                    emit(fp,"","not %rax","EXPR NOT");
                    return;
                    break;   

                default : printf("unknow expr operator !!! %d", p -> operator);
                    break;
                     
                }// end of EXPR 
                break;

		   default:
			   printf("Exist EXPR operator unknown %d\n", p->type);	//fall go to recursive
		}//end switch
}//end emit_expr

//Pre: Pointer to AST Node that points to extern Method 
//Post: GAS code for the Method, %rax is set by the method 
//There is special code for external methods exists with error if we dont know if it exist
//at first, I tried to combine the emit_extern_method_call with the emit_method_call,
//but it does not work, so I decide to split them like the video
void emit_extern_method_call(ASTnode *p, FILE *fp){
    char s[100];        
    //this is GAS code to print_string 
    if (strcmp("print_string", p->name) == 0 ) {
        emit(fp, "", "", "Print String");
        sprintf(s,"mov $%s , %rdi", p->S1->S1->label);
               
        emit(fp, "", s, "RDI is the label address");
	    emit(fp,"","mov $0, %rax", "RAX need to be zero for printf");
	    emit(fp,"","call printf", "print a string");
        fprintf(fp,"\n");   
        return; 
    //this is GAS code to print_int 
    if (strcmp("print_int", p->name) == 0 ) {
        emit_expr(p->S1->S1,fp);
        emit(fp, "", "", "Print Int ");
               	
	    emit(fp,"","mov %rax, %rsi ", "RSI needs the value");
	    emit(fp, "","mov $percentD, %rdi" ,	"RDI needs to be the int format");
	    emit(fp, "","mov $0, %rax","RAX needs to be 0");
	    emit(fp, "","call printf" ,	"print a number from expression");

        fprintf(fp,"\n");
        return; 
	//this is GAS code to read_int 
    if (strcmp ("read_int", p->name) == 0 ) {
        emit(fp, "", "", "Read Int");
        emit( fp,"","mov $_SCANFHOLD, %rsi" , "read in a number" );
	    emit(fp,"", "mov $percentD , %rdi","rdi has integer format for scanf");
	    emit(fp,"","mov $0 , %rax"	, "No other parameters for scanf" );
	    emit(fp, "", "call  __isoc99_scanf", "call read ");
	    emit(fp, "" , "mov _SCANFHOLD, %rax", "bring value on STACK into RAX register for default value" );
        fprintf(fp,"\n");
        return;
    }//end if

    
    } //end if 

	
    } //end if 

    fprintf(stderr,"Extern is method %s is unkown \n", p->name); 

} // end of Extern_Emit_Method_Call

//PRE: Pointer to AST node that points to Method
//POST: GAS code for the method, %rax is set by the method
//we can run a special code for external method
void emit_method_call (ASTnode *p,FILE *fp) {
	char s [100]; 
    //param hold the value of s1
    ASTnode *param = p->S1;
    int count = 8;
    //check to see if there is extern exists
    if( p->symbol-> SubType == ID_Sub_Type_Extern_Method){
        emit_extern_method_call(p,fp);
        return; 
    }// end if 

    //evaluate all the method args 
    while (param != NULL && count <= 16 ) {
        emit_expr(param->S1,fp);
        sprintf(s," mov %rax, %d(%rsp)",param->symbol->offset * WSIZE );
        emit(fp,"",s,"stor arg value in our runtime stack");
        param = param->next;
		count++;
    }//end while 
                 
    //get values to intial values through the while loop
    //and param hold the value of S1
    count = 8;
    param = p->S1;
    //load all args and stores the all parameters into registers 
    while (param != NULL && count <= 16 ) {
        sprintf(s," mov %d(%rsp), %rax", param->symbol->offset * WSIZE );
        emit(fp,"",s,"load calc argument into RAX");
        sprintf(s,"mov %rax,  %r%d",count); 
        emit(fp,"",s,"store parameter value into appropriate registers ");
        param=param-> next;
        count++;
	}//end while
                  
    // Call the function 
    emit(fp," "," ","about to call a function, set up each parameter in the new activation record");
    sprintf(s,"CALL %s",p->symbol->name);
    emit(fp, "", s, "CALL to function" );
    emit_ident(p,fp);

}//end emit_method_call

//Pre: Pointer to an Left value or Right value 
//Post: GAS code to make %rax exact address of the Identifier 
void emit_ident( ASTnode *p, FILE *fp){
    char s [100];
	
	//check if it is global or local 
    if (p->symbol->level == 0) {	//global scalars
        //check to see if it is a scalar or array global scalar 
        if (p->symbol->SubType == ID_Sub_Type_Scalar){
            sprintf(s, "mov $%s, %rax", p->symbol->name);
            emit(fp,"", s,"Load gobal variable from data segment");
    	}//end if 
                
        //Global arrays part
        else if (p->symbol->SubType == ID_Sub_Type_Array){
            //the expression in the brackets
            emit_expr(p->S1,fp); 
                 
            emit(fp,"", "mov %rax, %rbx","Array Componment");
            sprintf(s,"shl $%d, %rbx", LOG_WSIZE);    //shift Left of the LOGSIZE of the so that
            emit(fp, "",s,"multiply wordsize for array reference  ");
            sprintf(s, "mov $%s, %rax", p->symbol ->name);
            emit(fp,"", s,"Load gobal variable from data segment");
            emit(fp,"", "add %rbx, %rax","move add on %rbx as this is an array reference");
        }//end else if 
    }
    else  {		// load RAX with stack pointer plus offset if it's local
        //local scalars 
        if (p->symbol->SubType == ID_Sub_Type_Scalar){
            sprintf(s, "mov $%d, %rax", p->symbol->offset * WSIZE);
			emit(fp,"",s,"Get Identifier offset"); 
            emit(fp,"", "add %rsp, %rax", "we have direct reference to memory"); 
        }// end if 

        //local array
        else if (p->symbol->SubType == ID_Sub_Type_Array) {
        	//the expression in the brackets
            emit_expr(p -> S1,fp); 

            //generate the gas code 
            emit(fp,"", "mov %rax, %rbx","Array Componment");
            sprintf(s,"shl $%d, %rbx", LOG_WSIZE);
            emit(fp, "",s,"multiply wordsize for array reference  ");
            sprintf(s, "mov $%d, %rax", p->symbol->offset *WSIZE);
            emit(fp,"",s,"Get Identifier offset"); 
            emit(fp,"", "add %rsp, %rax", "we have direct reference to memory ");
        	emit(fp,"", "add %rbx, %rax","move add on %rbx as this is an array reference");
		}// end else if 
    }//end else 

} //end of emit_ident

//routine to do an assignment statement
//PRE:  Pointer to assignment
//POST: GAS code for assignment, emit the expression of S2,
//emit the address of Left value, store the value into the memory address.
void emit_assign(ASTnode *p, FILE *fp) {
   	char s[100];

   	emit_expr(p->S2,fp); // expression of the assign to S2 
	// make sure node is type A_ASSIGN
	if (p->type != A_ASSIGN) {
		printf("Illegal use of emit_assign");
		exit(1);
	}

	//get value for Right value
	//Store it temporarily so that left hand side 
	//get the address of LHS
	//retrieve value from memory
	// store ther value at the address of LHS

	//value from temp memory
    sprintf(s, "mov %rax, %d(%rsp)",  p -> symbol -> offset * WSIZE); 
    emit(fp, "", s, "store RHS value in memory");
               
    //emits address of the Left value      
    emit_ident(p->S1,fp);
           
    sprintf(s,"mov %d(%rsp), %rbx", p ->symbol-> offset * WSIZE);
    emit(fp,"",s, "Get RHS stored value"); 
    emit(fp,"", "mov %rbx, (%rax)" , "ASSIGN final store " );
    fprintf(fp,"\n");

}//end emit_assign

// PRE: Pointer to if statement
// POST: emit the code needed for an if/else statement
void emit_if(ASTnode *p, FILE *fp){  
   	char s[100];  

	//generate the two labels
	char *label_one = genlabel();
	char *label_two = genlabel();

	//expression in the if 
	emit_expr(p->S1,fp);

	//if label
	//branch when the expression is equal to 0
	//to the first label in order to skip over the if block
	//and execute the else 
	emit(fp, "", "cmp $0, %rax",	"IF compare");
    sprintf(s,"JE  %s",label_one); 
    emit(fp, "", s, "IF branch to else part"); //else part
    emitAST(p->S2->S1, fp); 
    sprintf (s, "JMP %s", label_two);
    emit(fp, "",s, "IF S1 end" );
             
    //else  label
    sprintf(s,"%s",label_one); 
    emit(fp,s,"","Else Target "); 
    //it will be an else, if p->S2->S2 is not null 
    if (p -> S2 -> S2 != NULL)  
        emitAST (p->S2->S2, fp); 

    sprintf(s,"%s",label_two); 
    emit(fp,s,"","End of If ");                   
}// end Emit_If 

//handle the WHILE statement
//PRE: Pointer to a while statment
//POST: emit the GAS assembly needed to do a while loop to fp
void emit_while(ASTnode *p, FILE *fp){    
   	char s[100];
	
	//generate the two labels
	char *label_one = genlabel();
	char *label_two = genlabel();

	emit(fp,label_one, "", "First label to restart while loop's block");

	// now the expression is evaluated in $a0
	emit_expr(p->S1,fp);
    //execute the block int the while loop 
    sprintf(s,"%s",label_one); 
    emit(fp,s,"", "WHILE TOP target");
    emit_expr(p->S1, fp);
    emit(fp, "", "cmp $0, %rax",	"WHILE compare");
    sprintf(s,"JE  %s",label_two); 
    emit(fp, "", s, "While  branch out"); 
    emitAST(p->S2, fp); 
    sprintf (s, "JMP %s", label_one);
    emit(fp, "",s, "WHILE Jump back" );
    //out of while loop 
    sprintf(s,"%s",label_two); 
    emit(fp,s,"","End of While Loop");

}//end emit_while

