
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

int GLABEL = 0; // number for labels 


char  * CURRENT_FUNC; //need to know Current Function for return on main

/* genlabel
   Precondition : No Parameters 
   Postcondition : returns a label symbol. 
*/
char *genlabel ()
{
  char label_id [100]; 
  sprintf(label_id,"_L%d",GLABEL); 
  GLABEL++; 
  return(strdup(label_id));

}

void EMIT ( ASTnode *p, FILE *fp) 
{
   // prints out the package 
   fprintf(fp,"\n\n# Package   %s\n", p -> S2 -> name );
   fprintf(fp,"\n");
   fprintf(fp,"\t .section .rodata # start of the RODATA section, strings first\n");
   fprintf(fp,"\n");
   fprintf(fp,"percentD:  .string \"%%d\" # Always needed for print int\n");
   fprintf(fp," # NEXT lines are the strings in the program\n");
   Emit_Strings(p,fp);  
   fprintf(fp," # END of are the strings in the program\n\n");
   fprintf(fp,"\t.data # start of the DATA section for variables\n\n\n");
   fprintf(fp,"\t.comm _SCANFHOLD,8,8    # MANDITORY space for SCANF reads\n\n");
   Emit_Globals(p,fp);
   fprintf(fp,"# END  of the DATA section for variables\n");   
   fprintf(fp,"\n");
   fprintf(fp,"\n");
   fprintf(fp,"\t.text    # start of CODE segement\n\n");
   Emit_AST(p,fp);
    fprintf(fp,"\n");
    emit(fp,"",".ident  \"GCC: (SUSE Linux) 7.5.0\"", "");
    emit(fp, "",".section        .note.GNU-stack,\"\", @progbits","");
	  fprintf(fp,"\n"); 
}

/* Pre Pointer to AST TREE 
   Post outpout to FP that has Label strings 
 */
void Emit_Strings( ASTnode *p, FILE *fp)
{    // check if p is null 
   if (p == NULL)
   {
      return; 
   }
   // Generates a label if it is a string 
   if (p -> type == A_CONTSTANT_STRING)
   {
      p -> label = genlabel(); 
      fprintf(fp,"%s:\t.string\t%s\t# global string\n", p-> label, p -> name);
   }

   Emit_Strings(p -> S1, fp);
   Emit_Strings(p -> S2, fp);
   Emit_Strings(p -> next, fp);
}
/*PRE Pointer to AST Tree and opens file *
  POST GAS code for GLOBAL variable to be placed in .data section*/ 
void Emit_Globals(ASTnode *p, FILE *fp)
{
   // check if p is null 
   if (p == NULL)
      {
        return; 
      }

    if ((p -> type == A_VARDEC) && (p -> symbol -> level == 0))
    
    {   // check if field dec is intialized 
        if (p -> S2 == NULL)
        {
            fprintf(fp,"\t.comm %s, %d, %d  # define a global variable \n", p -> name, p-> symbol -> mysize * WSIZE,  p-> symbol -> mysize * WSIZE);
            fprintf(fp,"\t.align 8\n");  
        }
        // variable is intialize 
        else 
        {
            fprintf(fp,"%s:   .long %d # define a global variable with intial value \n", p -> name, p -> S2 -> value);
        }
         
    }
    Emit_Globals(p -> S1,fp);
    Emit_Globals(p -> S2,fp);
    Emit_Globals(p -> next,fp);
}

/* Pre Pointer to AST Tree and open FILE 
   Post st GAS code that generates all the GAS for the rest of the tree beside global variables and strings  */
   void Emit_AST (ASTnode *p, FILE *fp)
   {
      // check if p is null if null there is no tree so return 
       if ( p == NULL)
       {
          return; 
       }     
          //  matches the node to the approite case and does the apporiate action 
          switch (p -> type)
          {         
              case A_PROGRAM : Emit_AST (p-> S1,fp);
                               Emit_AST (p-> S2,fp);
                               Emit_AST (p-> next,fp);
                               break;
                case A_PACKAGE: Emit_AST(p -> S2, fp );
                                Emit_AST(p -> S1, fp );
                                Emit_AST(p -> next, fp );
                                break; 
               
               case A_VARDEC : break;
               case A_EXTERN: break; 
                case A_EXTERN_TYPE: break; 
                case A_METHODDEC  : Emit_method_dec(p,fp);  
                                   Emit_AST(p -> next,fp); 
                                    break; 
                  
                case A_METHODIDENTIFER : Emit_AST(p -> S1, fp);
                                         Emit_AST(p -> next, fp);      
                                          break;
                
                case A_METHODCALL : Emit_Method_Call(p,fp);
                                    Emit_AST(p -> next,fp);                                  
                                     break;                    
                
                case A_RETURN : Emit_Return(p,fp);
                                Emit_AST(p -> next, fp);
                                break;     
                
                case A_BLOCK : Emit_AST(p -> S1, fp);
                               Emit_AST(p -> S2, fp);
                               Emit_AST(p -> next, fp);                               
                               break;
                
                case A_ASSIGN : Emit_Assign(p,fp);
                                Emit_AST(p -> next,fp); 
                                break;  
               
                case A_IFSTMT:Emit_If (p,fp);
                              break;  

                case A_WHILESTMT : Emit_While (p,fp);
                                  break;  

                default: printf("BROKEN EMIT AST %d  \n" , p -> type );                
          }//end switch                           
            
   } // end Emit_AST 

   /*
     PRE  Pointer to AST Node that points to Method Declaration and pointer to fp 
     POST GAS code for the method 
     */
     void Emit_method_dec(ASTnode *p, FILE *fp)
     {
       char s[100];
       int counter = 8; 
       ASTnode *parms = p-> symbol -> fparms;

        fprintf(fp,"\t.globl %s \n",p -> name);
        fprintf(fp,"\t.type %s, @ function \n", p-> name);
        emit(fp,p->name,"","Start of Function \n");
        fprintf(fp,"\n");
        fprintf(fp,"\t.cfi_startproc #   STANDARD FUNCTION HEADER FOR GAS \n");
        fprintf(fp,"\tpushq  %rbp    #   STANDARD FUNCTION HEADER FOR GAS \n");
        fprintf(fp,"\t.cfi_def_cfa_offset 16    #   STANDARD FUNCTION HEADER FOR GAS \n");
        fprintf(fp,"\t.cfi_offset 6, -16   #   STANDARD FUNCTION HEADER FOR GAS \n");
        fprintf(fp,"\tmovq   %rsp, %rbp    #   STANDARD FUNCTION HEADER FOR GAS \n");
        fprintf(fp,"\t.cfi_def_cfa_register 6 #   STANDARD FUNCTION HEADER FOR GAS \n");
        fprintf(fp,"\n\n");

       // Carve out our space on the stack
        sprintf(s,"subq $%d, %rsp",p -> symbol-> mysize * WSIZE);
        emit(fp,"", s,"Carve out activation record for function ");
        Emit_AST(p -> S1, fp); 
         
        // Placing methodidentifers into registers 
         while (parms != NULL && counter <= 16)
          { 
            sprintf(s,"mov %r%d, %d(%rsp)", counter, parms -> symbol -> offset  * WSIZE  ); // stores parmaters in the regsters 8 to 15 
            emit(fp,"",s,"copy actual to appropriate slot");
            parms = parms -> next; 
            counter++;
          
           } // end while 
        // generate GAS code fir tge BODY of the function definition
        Emit_AST(p -> S1, fp); 
        Emit_AST(p -> S2, fp);
        
        // implicit return 
        Emit_Return(NULL,fp);
        
        // end of function 
        emit(fp,"",".cfi_endproc","# STANDRD end function for GAS");  
        sprintf(s,".size	%s, .-%s",p->name,p->name);
        emit(fp,"",s,"STANDRD end function for GAS");
        fprintf(fp,"\n");
       
     }//end of Emit_methoddec
     
     /* helper function 
       Pre  fp, label, action, comment
       Post Generates  Presentable Gase Code in file 
     */
     void emit (FILE *fp,char *label, char * action, char * comment) 
     {
            if (strcmp(label,"") == 0) // no label
             { 
               if(strcmp(action,"") == 0)// no action no label 
               {
                  if (strcmp(comment,"") == 0) // no action no label no comment
                  {
                     fprintf(fp,"\n");
                  }// end if 

                  else // not action no label yes comment 
                  {
                      fprintf(fp,"\t#%s\n",comment);

                  } // end else 

               } // end if 
            else // no label yes action 
               {  if (strcmp(comment,"") == 0 )
                   { // action yes comment no 
                   fprintf(fp,"\t%s\n", action);
                   }// end if 
                  
                  else 
                 {
                    fprintf(fp,"\t%s\t#%s\n", action,comment);
                 } //end else 
              
               } // end else 
             }// end if 
            
             // there is a label 
             else 
             {
               if (strcmp(action,"") == 0) // no action 
                 {
                     if (strcmp(comment,"") == 0) // action no label yes comment no
                       {
                      fprintf(fp,"%s:\n",label);
                    
                      }// end if 
                  else // yes action yes label 
                     {
                       fprintf(fp,"%s:\t%s\t# %s\n",label, action,comment);
                     }// end else 

                  }   //end if 
                
               
             } // end else 

             
      }// end emit
    
    /* PRE PTR to ASTNode that points to Return or NULL Declcaration 
       POST GAS code for the Return, %rax is 0 for NULL and set by Emit_Expr()
       for expression.
    */
     void Emit_Return (ASTnode *p, FILE *fp)
     
     {
       char s [100];
      if (p == NULL || p -> S1 == NULL) // implicit return
      {
         emit(fp,"","mov $0, %rax", "Default Return value");
         
      }
      else 
       {
         Emit_Expr(p->S1,fp); // explicit return 
        
       
       }
       // gas code  to leave function 
      emit(fp,"","leave", "leave the function");
      emit(fp,"",".cfi_def_cfa 7, 8	","STANDRD end function for GAS");   
	    emit(fp,"","ret","");
      fprintf(fp,"\n");

	 
	
     } //end of return 

     /*
      Pre Pointer to ASTNode that is in the class or expression 
      Post GAS code that sets %rax to the evaluation of the expression.
     */
    
     void Emit_Expr (ASTnode *p, FILE *fp)
     {
      // SHould not get null because of the precondition 

      char s [100]; 
       
        // Base cases 
           switch(p -> type)
           {
              case A_CONTSTANT_INT : // places the value in memory 
                                     sprintf(s,"mov $%d , %rax", p -> value); 
                                     emit(fp, "",s, "Expresion a number"); 
                                     return; 
                                     break; 
             
              case A_CONTSTANT_BOOL : // places the value in memory 
                                     sprintf(s,"mov $%d , %rax", p -> value); 
                                     emit(fp, "",s, "Expresion a bool"); 
                                     return; 
                                     break;                   
                case A_METHODCALL : Emit_Method_Call(p,fp); // goes to method function 
                                     return; 
                                     break; 
                                          
               case A_VAR_RVALUE : // gets the RValue out of memory 
                                   Emit_Ident(p,fp);
                                   emit(fp,"","mov (%rax), %rax","Expression is identifier");
                                   return;
                                   break;
             
               case A_VAR_LVALUE: // gets the LValue out of memory 
                                  Emit_Ident(p,fp);
                                  return;
                                  break;
                  case A_BLOCK : Emit_AST(p, fp); // Goes to AST to evalute the body 
                                  break;                                                          
              
               // base cases for operators 
               // The operator will get  the RValue or LValue out of memory and calcutate their value 
              case A_EXPR :  switch(p->operator) 
                      
                     {  
   			            case A_PLUS : Emit_Expr(p -> S1,fp );
                                     sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                     emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                     Emit_Expr(p -> S2,fp);
                                     emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                     sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE );
                                     emit(fp, "", s, "fetch LHS of expression from memory" );
                                     emit(fp,"", "add %rbx, %rax" , "EXPR ADD");
                                     return;
                                     break;
   			               
                       case A_MINUS: Emit_Expr(p -> S1,fp );  
                                    sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                     emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                     Emit_Expr(p -> S2,fp);
                                     emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                     sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE );
                                     emit(fp, "", s, "fetch LHS of expression from memory" );
                                     emit(fp,"", "sub %rbx, %rax" , "EXPR SUB");
                                     return; 
                                     break;
                       
                       case A_DIVIDE: Emit_Expr(p -> S1,fp );  
                                     sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                     emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                     Emit_Expr(p -> S2,fp);
                                     emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                     sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE );
                                     emit(fp, "", s, "fetch LHS of expression from memory" );
                                     emit(fp,"","mov $0,%rdx ","EXPR upper word 0 DIV");
                                     emit(fp,"", "idiv %rbx" , "EXPR DIV");
                                     return;
                                     break;
                        
                       case A_TIMES: Emit_Expr(p -> S1,fp );  
                                     sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                     emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                     Emit_Expr(p -> S2,fp);
                                     emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                     sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE );
                                     emit(fp, "", s, "fetch LHS of expression from memory" );
                                     emit(fp,"", "imul %rbx, %rax" , "EXPR MULT RAX has lower word");
                                     return;
                                     break; 
                       
                       case A_AND:  Emit_Expr(p -> S1,fp );
                                     sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                     emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                     Emit_Expr(p -> S2,fp );
                                     emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                     sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE );
                                     emit(fp, "", s, "fetch LHS of expression from memory" );
                                     emit(fp,"", "and %rbx, %rax", "EXPR AND" ); 
                                     return;
                                     break;
                                
                       case A_OR: 
                                      Emit_Expr(p -> S1,fp );
                                     sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                     emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                     Emit_Expr(p -> S2,fp );
                                     emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                     sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE );
                                     emit(fp, "", s, "fetch LHS of expression from memory" );
                                     emit(fp,"", "or %rbx, %rax", "EXPR OR" ); 
                                     return;
                                     break;
                                                
                       case A_LEQ: 
                                  Emit_Expr(p -> S1,fp );  
                                  sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                  emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                  Emit_Expr(p -> S2,fp);
                                  emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                  sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE );
                                  emit(fp, "", s, "fetch LHS of expression from memory" );
                                  emit(fp, "", "cmp %rbx, %rax","EXPR LESSthan Equal");  
                                  emit(fp,"", "setle %al","EXPR Lessthan");   
                                  emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax"); 
	                               emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                                  return;
                                  break;
                                  
                       case A_GEQ: 
                                  Emit_Expr(p -> S1,fp );  
                                  sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                  emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                  Emit_Expr(p -> S2,fp);
                                  emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                  sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE );
                                  emit(fp, "", s, "fetch LHS of expression from memory" );
                                  emit(fp, "", "cmp %rbx, %rax","EXPR GREATERthan Equal");  
                                  emit(fp,"", "setge %al","EXPR Lessthan");   
                                  emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax"); 
	                               emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                                  return;
                                  break;
                                
                       case A_LT: 
                                  Emit_Expr(p -> S1,fp );  
                                  sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                  emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                  Emit_Expr(p -> S2,fp);
                                  emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                  sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE );
                                  emit(fp, "", s, "fetch LHS of expression from memory" );
                                  emit(fp, "", "cmp %rbx, %rax","EXPR LESSthan");  
                                  emit(fp,"", "setl %al","EXPR Lessthan");   
                                  emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax"); 
	                               emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                                  return;
                                  break;
                       
                       case A_GT: Emit_Expr(p -> S1,fp );  
                                  sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                  emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                  Emit_Expr(p -> S2,fp);
                                  emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                  sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE );
                                  emit(fp, "", s, "fetch LHS of expression from memory" );
                                  emit(fp, "", "cmp %rbx, %rax","EXPR Greaterthan");  
                                  emit(fp,"", "setg %al","EXPR Lessthan");   
                                  emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax"); 
	                               emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                                 return;
	                               break; 
                                 
                         case A_EQ: 
                                    Emit_Expr(p -> S1,fp );
                                     sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                     emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                     Emit_Expr(p -> S2,fp );
                                     emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                     sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE ); 
                                     emit(fp, "", s, "fetch LHS of expression from memory" );
                                     emit(fp, "", "cmp %rbx, %rax","EXPR NOT EQUAL");  
                                     emit(fp,"", "sete %al","EXPR Lessthan");   
                                     emit(fp, "", "mov $1, %rbx",  "set rbx to one to filter rax");
                                     emit(fp, "", "and %rbx, %rax", "filter RAX"); 
                                     return;
                                     break;
                       
                       case A_NEQ:   Emit_Expr(p -> S1,fp );
                                     sprintf(s,"mov %rax , %d(%rsp)", p -> symbol-> offset * WSIZE); 
                                     emit(fp, "",s, "Stores the  LHS of expression to memory"); 
                                     Emit_Expr(p -> S2,fp );
                                     emit(fp,"","mov %rax, %rbx" , "right hand side needs to be set");
                                     sprintf(s, "mov %d(%rsp), %rax", p -> symbol-> offset *WSIZE ); 
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

               default: printf(" fall through to the base case  %d\n", p -> type );// fall through to the recursive case 
                        break; 
                                           
           }// end of switch    
     } //end emit_express
         
          /* Pre Pointer to AST Node that points to Method 
            Post GAS code for the Method, %rax is set by the method 
            There is special code for external methods 
          */
      
      void Emit_Method_Call(ASTnode *p, FILE *fp)
        {
          char s [100]; 
         
          ASTnode *parms = p-> S1;
          int counter = 8;
            // check to see if it is an extern 
             if( p -> symbol -> SubType == ID_Sub_Type_Extern)
              {
                 
                  Emit_Extern_Method_Call(p,fp);
                  return; 
               }// end if 
                // we know it is internal method method call
            
                 // evaluate all the method args 
                 while (parms != NULL && counter <= 16 ) 
                 {
                   Emit_Expr(parms -> S1, fp);
                   sprintf(s," mov %rax, %d(%rsp)",  parms -> symbol -> offset * WSIZE );
                   emit(fp,"",s,"stor arg value in our runtime stack");
                   parms = parms -> next;

                   counter++;
                  

                 }//end while 
                 
                 // assign values to intial values iterate through the whilee 
                 counter = 8;
                 parms = p -> S1;
                 // loads calculated args and stores the parameters into approiate registers 
                 while (parms != NULL && counter <= 16 ) 
                 {
                 
                  sprintf(s," mov %d(%rsp), %rax", parms -> symbol -> offset  * WSIZE );
                  emit(fp,"",s,"load calc argument into RAX");
                  sprintf(s,"mov %rax,  %r%d",counter); 
                  emit(fp,"",s,"store parameter value into appropriate registers ");
                  parms = parms -> next;
                  counter++;
                  
                 }// end while
                  
                  // Call the function 
                  emit(fp," "," ","about to call a function, set up each parameter in the new activation record");
                  sprintf(s,"CALL %s",p -> symbol -> name);
                  emit(fp, "", s, "CALL to function" );
                  Emit_Ident(p,fp);
                  
       
              
         } // end of Emit_Method_Call
          
           /* Pre Pointer to AST Node that points to extern Method 
            Post GAS code for the Method, %rax is set by the method 
            There is special code for external methods 
            exists with error if we dont know if it exist
          */
       
      void Emit_Extern_Method_Call(ASTnode *p, FILE *fp)
         {
             char s[100];
              // Gas Code Extern method print_string 
             if (strcmp("print_string", p-> name) == 0 ) 
             {
                emit(fp, "", "", "Print String");
               sprintf(s,"mov $%s , %rdi", p -> S1-> S1 -> label);
               
               emit(fp, "", s, "RDI is the label address");
	            emit(fp,"","mov $0, %rax", "RAX need to be zero for printf");
	            emit(fp,"","call printf", "print a string");
               fprintf(fp,"\n");
               
               return; 
             } //end if 
               
              // Gas Extern method print_int 
              if (strcmp("print_int", p-> name) == 0 ) 
             {
                 Emit_Expr(p -> S1 -> S1, fp);
                 emit(fp, "", "", "Print Int ");
               	
	               emit(fp,"","mov %rax, %rsi ", "RSI needs the value");
	               emit(fp, "","mov $percentD, %rdi" ,	"RDI needs to be the int format");
	               emit(fp, "","mov $0, %rax","RAX needs to be 0");
	               emit(fp, "","call printf" ,	"print a number from expression");

                  fprintf(fp,"\n");
               return; 
             } //end if 
             
             // Gas Extern method read_int 
             if (strcmp ("read_int", p -> name) == 0 ) 
             {
               
                    
                    emit(fp, "", "", "Read Int");
                    emit( fp,"","mov $_SCANFHOLD, %rsi" , "read in a number" );
	                  emit(fp,"", "mov $percentD , %rdi","rdi has integer format for scanf");
	                  emit(fp,"","mov $0 , %rax"	, "No other parameters for scanf" );
	                  emit(fp, "", "call  __isoc99_scanf", "call read ");
	                  emit(fp, "" , "mov _SCANFHOLD, %rax", "bring value on STACK into RAX register for default value" );
                     fprintf(fp,"\n");
                     return;
             }

             fprintf(stderr,"Extern is method %s is unkown \n", p -> name); 

         } // end of Extern_Emit_Method_Call
       
        /*Pre Pointer to an LValue or RValue 
          Post Gas code to make %rax exact address of the Identifier 
         --upstream caller may need to extract the value from 
         -- memory 
        */
         void Emit_Ident( ASTnode *p, FILE *fp)
         {
            char s [100];
            

            // need to check if it is global or local 
            if (p -> symbol-> level == 0)  // global scalars 
            {
               // check the sub type if it is a scalar or array
               // global scalar 
               if (p -> symbol ->  SubType == ID_Sub_Type_Scalar  ){
                 sprintf(s, "mov $%s, %rax", p -> symbol -> name);
                 emit(fp,"", s,"Load gobal variable from data segment");
               } //end if 
                
                // Global Arrays 
                else if (p -> symbol ->  SubType == ID_Sub_Type_Array  )
                {
                   
                  // evaluate the expression in the brackets
                  Emit_Expr(p -> S1,fp); 
                 
                  emit(fp,"", "mov %rax, %rbx","Array Componment");
                  sprintf(s,"shl $%d, %rbx", LOG_WSIZE);    // We need to shift Left of the LOGSIZE of the so that
                  emit(fp, "",s,"multiply wordsize for array reference  "); // multiply by WSIZE 
                  sprintf(s, "mov $%s, %rax", p -> symbol -> name);
                  emit(fp,"", s,"Load gobal variable from data segment");
                  emit(fp,"", "add %rbx, %rax","move add on %rbx as this is an array reference");
              
                
                  
                } //end else if 
            }
             else // local We need to load RAX with stack pointer plus offset
            {
                  // local scalars 
                  if (p -> symbol ->  SubType == ID_Sub_Type_Scalar  )
                  {
                       sprintf(s, "mov $%d, %rax", p -> symbol-> offset *WSIZE);

                        emit(fp,"",s,"Get Identifier offset"); 
                         emit(fp,"", "add %rsp, %rax", "we have direct reference to memory "); 
                  } // end if 
                  // local arrays 
                 else if (p -> symbol ->  SubType == ID_Sub_Type_Array  ) 
                 {
                  // evaluate the expression in the brackets
                  Emit_Expr(p -> S1,fp); 
                  // calculate and generate the gas code 
                  emit(fp,"", "mov %rax, %rbx","Array Componment");
                  sprintf(s,"shl $%d, %rbx", LOG_WSIZE);
                  emit(fp, "",s,"multiply wordsize for array reference  ");
                  sprintf(s, "mov $%d, %rax", p -> symbol-> offset *WSIZE);
                  emit(fp,"",s,"Get Identifier offset"); 
                  emit(fp,"", "add %rsp, %rax", "we have direct reference to memory ");
                  emit(fp,"", "add %rbx, %rax","move add on %rbx as this is an array reference");
                 
                
                } // end else if 


            }// end else 
                
            

          } // end of Emit_Ident 
         

      /*
        Pre Pointer to an Assign 
        Post Gas code that implements the following algorithm 
            Emit the expression of S2 
            Stores that value in the temporary 
            Emits the Address of the LVALUE 
            extracts bacm from memory the stored value 
            Stores the extracted value into proper memory address     
      */
     void Emit_Assign (ASTnode *p, FILE *fp) 
      {
            char s [100];
             
            //  Emit the expression of S2 
            Emit_Expr(p ->S2, fp); 
                 
            // Stores that value in the temporary 
            sprintf(s, "mov %rax, %d(%rsp)",  p -> symbol -> offset * WSIZE); 
            emit(fp, "", s, "store RHS value in memory");
               
            // Emits the Address of the LVALUE      
            Emit_Ident(p-> S1, fp);
           
            sprintf(s,"mov %d(%rsp), %rbx", p -> symbol -> offset * WSIZE);
            emit(fp,"",s, "Get RHS stored value"); 
            emit(fp,"", "mov %rbx, (%rax)" , "ASSIGN final store " );
            fprintf(fp,"\n");
          
        }// end of Emit_Assign

       /*PRE:  PTR to an IFSTMT
         POST:  GAS code to implement if 
       */
         void Emit_If (ASTnode *p, FILE *fp) 


        {
           char s [100];

             // generate two labels 
             char *label_one = genlabel();
             char *label_two = genlabel(); 

             Emit_Expr( p -> S1, fp); // Expression in the if statment 
             emit(fp, "", "cmp $0, %rax",	"IF compare");
             sprintf(s,"JE  %s",label_one); 
             emit(fp, "", s, "IF branch to else part"); // else
             Emit_AST( p -> S2 -> S1, fp); 
             sprintf (s, "JMP %s", label_two);
             emit(fp, "",s, "IF S1 end" );
             
             // Else Label 
             sprintf(s,"%s",label_one); 
             emit(fp,s,"","Else Target "); 
             // it is an else if p -> S2 ->S2 is not null 
             if (p -> S2 -> S2 != NULL) 
             {
              // traverse the else block 
               Emit_AST ( p -> S2 -> S2, fp); 
             }
             sprintf(s,"%s",label_two); 
             emit(fp,s,"","End of If "); 
                  
        }// end Emit_If 
        
         /*PRE:  PTR to an WHILESTMT
         POST:  GAS code to implement While
          */
        void Emit_While (ASTnode *p, FILE *fp)
        {
          char s [100];

             // generate two labels 
             char *label_one = genlabel();
             char *label_two = genlabel(); 
             // Execute the block int the while loop 
             sprintf(s, "%s",label_one); 
             emit(fp,s,"", "WHILE TOP target");
             Emit_Expr( p -> S1, fp);
             emit(fp, "", "cmp $0, %rax",	"WHILE compare");
             sprintf(s,"JE  %s",label_two); 
             emit(fp, "", s, "While  branch out"); 
             Emit_AST ( p -> S2 , fp); 
             sprintf (s, "JMP %s", label_one);
             emit(fp, "",s, "WHILE Jump back" );
             // out of while loop 
             sprintf(s,"%s",label_two); 
             emit(fp,s,"","End of While Loop");
              


        }// end Emit_While 
      
      /*PRE:  PTR to an FORSTMT
         POST:  GAS code to implement For
          */
    

