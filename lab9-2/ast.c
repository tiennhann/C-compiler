/*   Abstract syntax tree code

		 This code is used to define an AST node, 
		routine for printing out the AST
		defining an enumerated type so we can figure out what we need to
		do with this.  The ENUM is basically going to be every non-terminal
		and terminal in our language.

		Shaun Cooper February 2020

        Nhan Le
        CS370
        File: ast.c

*/

#include<stdio.h>
#include<malloc.h>
#include "ast.h" 
#include "emit.h"


/* uses malloc to create an ASTnode and passes back the heap address of the newley created node */
ASTnode *ASTCreateNode(enum AST_Tree_Element_Type mytype)
{
    ASTnode *p;
    if (mydebug) fprintf(stderr,"Creating AST Node \n");
    p=(ASTnode *)malloc(sizeof(ASTnode)); // get head data
    p->type=mytype; // set up the Element type
    p->S1=NULL;
    p->S2=NULL;  // det default values
    p->value=0;
    return(p);
}

/*  Helper function to print spaces */

void PT(int howmany)
{
	 for (int i = 1; i <= howmany; i++)
   {
     printf(" ");
   
   }
}
/*
   Precondition must be a decaf type INT BOOL or STRING 
   This method prints out the decaf type which are VOID INT BOOL or STRING
   If the switch statement doesnt have the defac type it will print out message 
   saying unknown decaf type 
   Postcondition will print out the decaf type 
*/
void AST_Print_Type( enum AST_Decaf_Types t)
{
	switch (t) {

    case A_Decaf_VOID : printf("VOID");
                        break;     
		case  A_Decaf_INT : printf(" INT ");
	                              break;
    case  A_Decaf_BOOL : printf(" BOOL ");  //added lab 6
	                              break; 
    case  A_Decaf_STRING : printf(" STRING ");  // added lab 6
	                              break;                                                      

          default:  fprintf(stderr,"Unknown AST DECAF TYPE !!!\n");
	} // of switch


}// of AST_Print_Type
/*check_parameters 
 Method makes sure the length and type of parameters are equilvent
 Precondition: ASTnode *actual  AST node *formal
 Postcondtion: returns 1 if the paramters match returns 0 if they dont.
*/
 
int check_parameters(ASTnode *actual, ASTnode *formal)
{
   
   if ((formal == NULL )&& (actual == NULL))
   {
      return 1; 
   }

   if ((formal == NULL) || (actual == NULL))
    {    
      
      return 0; 
    }

    if (formal ->  A_Declared_Type != actual ->  A_Declared_Type )
    {
      return 0;
    }

    return check_parameters( actual -> next, formal -> next);


}
/*  Precondition The tree is not null 
    Print out the abstract syntax tree
    Postconditon it print out the current abstract syntax tree. 
*/
void ASTprint(int level,ASTnode *p)
{
   int i;
   if (p == NULL ) return;
   else
     { 
       PT(level); /*indent */
       switch (p->type)  
       {
        
            case A_PROGRAM : printf ("PROGRAM \n"); // 0 
                             ASTprint(level + 1, p -> S1 ); 
                             ASTprint(level + 1, p -> S2 ); 
                             printf("\n");
                             break;
           
           case A_PACKAGE : printf ("PACKAGE %s \n ", p -> name); // 1 
                            ASTprint(level + 1, p -> S1 ); 
                            ASTprint(level + 1, p -> S2 );
                            printf("\n");
                            break;
           
           case A_EXTERN :  printf("EXTERN FUNC %s ", p-> name); // 2 
                             printf("(");
                            ASTprint(level, p -> S1);
                            printf(")");
                           AST_Print_Type(p -> A_Declared_Type);
                           printf("\n");
                           break;
           
           case A_EXTERN_TYPE : printf("EXTERNTYPE "); //3 
                                AST_Print_Type(p -> A_Declared_Type);
                                break;          
           case A_VARDEC :  printf(" VARIBLE"); // 4 
                            printf(" %s",p->name);
                             if (p -> S1 != NULL) // then it is an array
                             {
                                
                                printf(" [%d", p-> S1-> value);
                                printf(" ]");
                             }
                             
                           AST_Print_Type(p -> A_Declared_Type);
                           // this is a intiatl value set for A_Decaf_types except BOOL
                           if(p -> S2 != NULL && p -> A_Declared_Type != A_Decaf_BOOL)
                           {
                                printf("= %d", p->S2 -> value);
                           }
                          // this is the intial value set for A_Decaf_BOOL
                          if (p -> A_Declared_Type == A_Decaf_BOOL) 
                           {
                               if (p -> value == 1)
                                   printf("true");
                                else 
                                   printf ("false");    
                           } 
                           printf(" Offest = %d", p->symbol -> offset) ;
                           printf("\n");
                           break;      
        case A_VAR_RVALUE :  printf("RVALUE"); // 5 
                             printf(" %s",p->name); 
                             AST_Print_Type(p -> A_Declared_Type);
                             printf("\n");
                             break;

        case A_VAR_LVALUE :  printf("LVALUE"); // 6 
                             printf(" %s",p->name); 
                             AST_Print_Type(p -> A_Declared_Type);
                             printf("\n");
                             break;                                      
                               
        case A_METHODDEC :  // 7
                     
                    printf(" Method  Dec");
                    printf(" %s with type ",p->name);
                    printf(" Offest = %d", p->symbol -> offset) ;
                     AST_Print_Type(p -> A_Declared_Type);
                     printf("\n");
                     /* print out the parameter list */
                     if (p->S1 == NULL ) 
		                    { PT(level+2); 
		                     printf (" (None) "); 
                         }
                     else
                       { 
                         PT(level+2);
                         printf( "( \n");
                          ASTprint(level+2, p->S1);
                         PT(level+2);
                         printf( ")");
                       }
                     printf("\n");
                     ASTprint(level+2, p->S2); // print out the block
                     break;
        case A_METHODIDENTIFER : printf("Method Variable "); // 8
                                printf(" %s",p->name);
                                AST_Print_Type(p -> A_Declared_Type);
                                printf("\n" );
                                break;
                                         
        case A_PARAM :  printf("PARAMETER "); //9 
                        if (p->operator == A_Decaf_INT) // INT 
                             printf (" INT ");
                        if (p->operator == A_Decaf_BOOL) // BOOL 
                             printf (" BOOL ");
                        if (p->operator == A_Decaf_VOID) // VOID 
                             printf ("VOID");
                        printf (" %s",p->name);
                        if (p->value == -1 ) 
                             printf("[]");
                      printf("\n");                  
                     break;
        case A_EXPR :  printf("EXPR "); // 10 
                     switch(p->operator) 
                     {
   			               case A_PLUS : printf(" + ");
                                      break;
   			               case A_MINUS: printf(" - ");
                                     break;
                       case A_DIVIDE: printf(" / ");
                                     break;
                       case A_TIMES: printf(" * ");
                                    break; 
                       case A_AND: printf(" && "); 
                                  break;  
                       case A_OR: printf (" || ");
                                  break; 
                       case A_LEFTSHIFT : printf(" << ");
                                         break;
                       case A_RIGHTSHIFT : printf(" >>  ");
                                        break;                           
                       case A_LEQ: printf(" <= ");
                                   break;
                       case A_GEQ: printf(" >= ");
                                   break;
                       case A_LT: printf(" < ");
                                  break;
                       case A_GT: printf(" > ");  
                                  break; 
                       case A_EQ: printf(" == ");
                                  break;
                       case A_NEQ: printf(" != ");
                                   break;
                       case A_NOT : printf("!");
                                    break;                    
                     default : printf("unknow expr operator !!!");

                  }
                  printf("\n");
                  ASTprint(level + 1, p->S1);
		            if (p->operator != A_NOT) 
                        ASTprint(level + 1, p->S2);
                  break;
        case A_BREAK : printf("BREAK STATEMENT \n"); // 11   
                       break;
        case A_CONTINUE : printf("CONTINUE STATEMENT \n ");    // 12        
                          break;   
         case A_RETURN : printf("RETURN STATEMENT \n"); // 13
                          ASTprint(level + 1, p -> S1); // if there is a  return expr  
                          break; 
         case A_ASSIGN : printf("ASSIGN STATEMENT \n"); // 14 
                        ASTprint(level + 1, p -> S1); 
                        ASTprint(level + 1, p -> S2); 
                        break;                                    
        case A_BLOCK : printf("BLOCK STATEMENT \n"); // 15 
                       ASTprint(level, p->S1);
                       ASTprint(level, p->S2);
                      break;
        case A_WHILESTMT :  printf("WHILE STATEMENT \n"); // 16 
                            ASTprint(level+1, p->S1);
                            ASTprint(level+1, p->S2);
                            break;
        case A_FORSTMT : printf("FOR STATEMENT \n"); // 17 
                         ASTprint(level+1, p->S1);
                         ASTprint(level+1, p->S2);
                         break;             
                     
       case A_IFSTMT : printf("IF STATEMENT   \n"); //18
                       ASTprint(level+1, p->S1);
                       ASTprint(level+3, p->S2->S1);
                       // check for an else 
                       if (p -> S2 -> S2 != NULL){
                           PT(level + 2);
                           printf(" Else \n ");
                           ASTprint(level + 3, p -> S2->S2);
                       }
                       break; 

      /*case A_IFBODY :  printf("\n");
                      ASTprint(level+1, p->S1);
                      ASTprint(level+1, p->S2);
                    break;            
      //case A_ELSE :  printf("ELSE \n");
                     //ASTprint(level + 1, p->S1);
                    // break;
                    */ 
      case A_METHODCALL: printf("MethodCall %s \n", p -> name);
                         PT(level);
                         printf("( \n");
                         //PT(level);
                         //printf("Method Arg \n");
                         ASTprint(level + 1 , p -> S1);
                         PT(level);
                         printf(") \n");
                        break; 
       case A_METHOD_ARG : printf("Method Arg \n");  
                           ASTprint(level + 1 , p -> S1);
                           break;               
        case A_ARRAYTYPE : printf("ARRAYTYPE \n");
                           ASTprint(level + 1, p -> S1);
                           ASTprint(level + 1, p -> S2);
                           break;                                                 
        case A_CONTSTANT_INT:  printf("CONSTANT INT with value %d\n",p->value);
                               ASTprint(level + 1, p->S1);
                               break;
         case A_CONTSTANT_BOOL:  printf("Boolean with value ");
                                 if (p -> value == 1) // if value one mean it is true 
                                    {
                                      printf("true \n");
                                    }
                                 else  // false 
                                    {
                                       printf("false \n");
                                    }
                     
                                break;  
         case A_CONTSTANT_STRING : printf("A_CONSTANT_STRING  %s \n", p -> name);
                                   break;                    
        default: printf("unknown type in ASTprint\n");


       }
       ASTprint(level, p->next);
     }

}



/* dummy main program so I can compile for syntax error independently   
 
void main()
{
}
/* */
