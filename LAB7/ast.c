/*   Abstract syntax tree code

		 This code is used to define an AST node, 
		routine for printing out the AST
		defining an enumerated type so we can figure out what we need to
		do with this.  The ENUM is basically going to be every non-terminal
		and terminal in our language.

		Shaun Cooper February 2020

        Nhan Le
        CS370
        Date:   October 23, 2022
        File: ast.c

*/

#include<stdio.h>
#include<malloc.h>
#include "ast.h" 

/* uses malloc to create an ASTnode and passes back the heap address of the newley created node */
ASTnode *ASTCreateNode(enum AST_Tree_Element_Type mytype)
{
		ASTnode *p;
		if (mydebug) 
			fprintf(stderr,"Creating AST Node \n");
		p=(ASTnode *)malloc(sizeof(ASTnode)); // get head data
		p->type=mytype; // set up the Element type
		p->S1=NULL;   //set default values
		p->S2=NULL;  //set default values
		p->value=0;
		return(p);
}

/*  Helper function to print tabbing */

//print as many space as the parameter given to the function
void PT(int howmany){
	for (int i = 0; i < howmany; i++){
		printf(" ");
	}
	 // MISSING
}//end PT

//add more A_Decaf_Types 
void AST_Print_Type( enum AST_Decaf_Types t)
{
	switch (t) {
		case  A_Decaf_INT : 
			printf(" INT ");
			break;
		case A_Decaf_BOOL:
			printf(" BOOL ");
			break;
		case A_Decaf_VOID:
			printf( " VOID ");
			break;
		case A_Decaf_STRING:
			printf(" STRING");
			break;
		case A_Decaf_CHAR:
			printf(" CHAR ");
			break;
		default:  fprintf(stderr,"Unknown AST DECAF TYPE !!!\n");
	} // of switch
}// of AST_Print_Type

/*  Print out the abstract syntax tree, based on the ouput file
from Dr.Cooper. 
//parameter expected ASTNode, and int level. 
//Print the abstract syntax tree with some level of indentation given in the parameter in the AST order.
//if the type of the node given is not defined, the function will print out an error message.*/
void ASTprint(int level,ASTnode *p){
	int i;
    //NULL we will not print anything because it is empty 
	if (p == NULL ) 
	    return;
	else { 
        //for each type of node, print content of that note
	    PT(level); /*indent */
	    switch (p->type) {
            case A_PROGRAM:
			    //printf("PROGRAM \n");
			    ASTprint(level + 1, p->S1);
			    ASTprint(level + 1, p->S2);
		    	break;

            //print package and its name, and some spacing(level)
            case A_PACKAGE:
			    printf("Package : %s\n", p->name);
                PT(level + 2);
			    ASTprint(level + 1, p->S1);
			    ASTprint(level + 1, p->S2);
                PT(level + 2);
			    printf("\n");
			    break;

            //print extern function's name
            case A_EXTERN:
			    printf("EXTERN FUNC %s\n", p->name);
			    //printf(" ( ");
			    ASTprint(level + 1, p->S1);
			    //printf(" ) ");
			
			    printf("END EXTERN with Type: ");
                AST_Print_Type(p->A_Declared_Type); 
                printf("\n");
                printf("\n");
			    break;

            //type of the extern function
            case A_ExternType:
			    printf("EXTERN Type ");
			    AST_Print_Type(p->A_Declared_Type);
                printf("\n");
                break;

            //Variable's name and type of that variable
			case A_VARDEC :  
                printf("Variable ");
				printf(" %s", p->name);

                if (p->S1 != NULL) {        //then is an array
				    printf("[");
				    printf("%d", p->S1->value);
				    printf("] ");
			    }
                printf(" with type ");
			    AST_Print_Type(p->A_Declared_Type);
			    printf(" ");
			    if (p->S2 != NULL) {
				    printf("= %d", p->S2->value);
			    }
			    printf("\n");
				break;

            //Method funtion and name
			case A_METHODDEC :  
                printf("METHOD FUNCTION  '%s' with type ", p->name);
                AST_Print_Type(p->A_Declared_Type);
				/* print out the parameter list */
				if (p->S1 == NULL ) { 
                    printf("\n");
                    PT(level+2); 
					printf (" (NONE) ");
                }
				else { 
                    printf("\n");
					PT(level+2);
					printf( "( \n");
					ASTprint(level+2, p->S1);
					PT(level+2);
					printf( ") ");
				}
			    printf("\n");
			    ASTprint(level+2, p->S2); // print out the block
			    break;
            
            //name of variable in method
            case A_METHODID:
			    printf("Method Variable");
			    printf(" %s", p->name);
			    AST_Print_Type(p->A_Declared_Type);
			    printf("\n");
			    break;
			
            //parameter 
            case A_PARAM :  
                printf("PARAMETER ");
				if (p->operator == A_Decaf_INT)
					printf (" INT ");
				if (p->operator == A_Decaf_BOOL)
					printf (" VOID ");
				if (p->operator == A_Decaf_VOID)
					printf (" BOOLEAN ");
				printf (" %s",p->name);
				if (p->value == -1 ) 
					printf("[]");
                ASTprint(level + 2, p->S1);
				printf("\n");                  
				break;
                
            //expression case
			case A_EXPR :  
                printf("EXPR ");
				switch(p->operator) {
	 			case A_PLUS : 
                    printf(" + ");
					break;
	 	        case A_MINUS: 
                    printf(" - ");
					break;
                case A_TIMES:
				    printf(" * ");
				    break;
                case A_DIVIDE:
				    printf(" / ");
				    break;
                case A_MOD:
				    printf(" %% ");
				    break;
			    case A_AND:
				    printf(" && ");
				    break;
			    case A_OR:
				    printf(" || ");
				    break;
			    case A_LEQ:
				    printf(" <= ");
				    break;
			    case A_GEQ:
				    printf(" >= ");
				    break;
                case A_EQ:
				    printf(" == ");
				    break;
			    case A_NEQ:
				    printf(" != ");
				    break;
			    case A_GT:
				    printf(" > ");
				    break;
			    case A_LT:
				    printf(" < ");
				    break;
			    case A_LEFTSHIFT:
				    printf(" << ");
				    break;
			    case A_RIGHTSHIFT:
				    printf(" >> ");
				    break;
			    case A_NOT:
				    printf(" !");
				    break;
			    case A_UMINUS:
                    // print words so we know it's minus
				    printf(" Unary Minus ");
				    break;
			    default:
				    printf("Unknown EXPR Operator");  
				}
				printf("\n");
				ASTprint(level+1, p->S1);
				if (p->operator != A_NOT && p->operator != A_UMINUS) 
					ASTprint(level+1, p->S2);
				break;

                //Block
				case A_BLOCK :  
                    printf("BLOCK STATEMENT  \n",p->name);
					ASTprint(level+1, p->S1);
					ASTprint(level+1, p->S2);
					break;
                
                //While
				case A_WHILESTMT :  
                    printf("WHILE STATEMENT \n");
					ASTprint(level+1, p->S1);
					ASTprint(level+2, p->S2);
					break;

                //Constant integer
		        case A_CONSTANT_INT:
			        printf("CONSTANT INTEGER  %d\n", p->value);
			        break;

                //Constant string
                case A_CONSTANT_STRING:
			        printf("CONSTANT STRING  %s\n", p->name);
			        break;
                
                //Constant boolean
		        case A_CONSTANT_BOOL:
			        printf("CONSTANT BOOLEAN  ");
			        if (p->value == 1) {
				        printf("true\n");
			        }
                    else {
				        printf("false\n");
			        }
			        break;

                //Break
		        case A_BREAK:
			        printf("BREAK STATEMENT   \n");
			        break;
                
                //Return 
		        case A_RETURN:
			        printf("RETURN STATEMENT   \n");
			        ASTprint(level + 1, p->S1);
			        break;

                //Assign
                case A_ASSIGN:
			        printf("ASSIGNMENT STATEMENT\n");
			        ASTprint(level + 1, p->S1);
                    ASTprint(level + 1, p->S2);
			        break;

                //If statement
                case A_IFSTMT:
                    printf("IF STATEMENT   \n");
			        ASTprint(level + 1, p->S1);
                    ASTprint(level + 1, p->S2);
			        break;

                //If statement body
                case A_IFSTMTBODY:
			        ASTprint(level + 1, p->S1);
                    ASTprint(level + 1, p->S2);
			        break;
                
                //Else statement
                case A_ELSESTMT:
                    printf("ELSE   \n");
			        ASTprint(level + 1, p->S1);
			        break;
                
                //Print method call and the name of it
                case A_METHODCALL:
                    printf(" METHOD CALL name: %s\n", p->name);
                    PT(level+2);
                    printf("(\n");
                    printf("METHOD ARG \n");
		            ASTprint(level + 2, p->S1);
                    PT(level+2);
                    printf(")\n");
			    break;

                //Right variable value
                case A_RIGHT_VALUE:
			        printf("Variable %s \n", p->name);
                        if (p->S1 != NULL) {
                            PT(level + 2);
                            printf("[\n");
                            ASTprint(level+2, p->S1);
                            PT(level + 2);
                            printf("]\n");
                        }
			    break;
				
                //number
                case A_NUMBER :  
                    printf("NUMBER with value %d\n",p->value);
					ASTprint(level+1, p->S1);
					break;
				default: printf("unknown type in ASTprint\n");

			 }
			 ASTprint(level, p->next);
		 }

}


// checks that two nodes match in type and length
int check_parameters(ASTnode *formal, ASTnode *actual)
{
	if (actual == NULL && formal == NULL) {
		return (1);
	}
	if (actual == NULL) {
		return (0);
	}
	if (formal == NULL) {
		return (0);
	}
	if (actual->A_Declared_Type != formal->A_Declared_Type) {
		return (0);
	}
	return check_parameters(formal->next, actual->next);
}



/* dummy main program so I can compile for syntax error independently   
 */
// void main()
// {
// }
/* */
