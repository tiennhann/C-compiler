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
   File Name: lab9.y

   YACC file to process the tokens recieved from Lex 
   Using the tokens given by Lex do simple mathematics for our caluclator 


*/
/* begin specs */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h" // added lab7
#include "ast.h" // added lab6
#include "emit.h"

 
 int LEVEL = 0; // Add a new variable called level that allows me to keep track of scoping with the blocks  
 int offset= 0 ; // how much space we are using for the enviroment
 int goffset = 0; // global offset maintain space in method delecarations lab7 video
 int maxoffset;

int yylex(); // to get rid of warning of yylex()

extern int linecount;// This is a linecount counter variable from lex

struct ASTnodetype *Program; // AST global variable 

/* Gets called by yyparse when there is an error in the program.
   The program will use linecount variable from lex print out the line numbers
   where they error occured 
*/
void yyerror (s)  /* Called by yyparse on error */
     char *s;
{
  printf("Error on line %d ", linecount) ; 
  printf (" %s\n", s);
}


%}
/*  defines the start symbol, what values come back from LEX and 
   how the operators are associated . Start changed from P. Union 
   was left the same. Added tokens from lex  
   */

%start Program
%union 
 {
   
   int value; 
   char* string;
   struct ASTnodetype *astnode;// added lab6
   enum AST_Decaf_Types asttype; //added lab6
   enum AST_Operators operator;
 }
 // tokens from lex 
 // lab6 update gave some the token value types or string types 
%token <string> T_ID // added string type 
%token T_AND
%token T_ASSIGN
%token T_BOOLTYPE
%token T_BREAK
%token T_CONTINUE
%token T_DOT
%token T_ELSE
%token T_EQ
%token T_EXTERN
%token T_FALSE
%token T_FOR
%token T_FUNC
%token T_GEQ
%token T_GT
%token T_IF
%token <value> T_INTTYPE // added value type 
%token <value> T_INTCONSTANT // added value type
%token T_LEFTSHIFT
%token T_LEQ
%token T_NEQ
%token T_NULL
%token T_OR
%token T_PACKAGE
%token T_RETURN
%token T_RIGHTSHIFT
%token <string> T_STRINGTYPE // added string type 
%token <string> T_STRINGCONSTANT // added string type 
%token T_TRUE
%token T_VAR
%token T_VOID
%token T_WHILE

// gave the nonterminals there approiate type ther astnode types asttypes and operator types in order to work with ast 
%type <astnode> Externs ExternDefn ExternTypeList ExternTypeList1 FieldDecl FieldDecls // added lab6 
%type <astnode> ArrayType MethodDecl MethodDecls IdenList IdenList1 Block VarDecl VarDecls MethodArg MethodArgs MethodArgsList MethodCall// added lab6
%type <asttype> MethodType Type ExternType // added lab6
%type <astnode>  Constant BoolConstant ForStatement IfStatement BreakStatement ReturnStatement Statement Statements  WhileStatement// added lab6
%type <astnode>  Factor Expr Term Additiveexpression Simpleexpression ContinueStatement Assign Lvalue   // added lab6
%type <operator> Multop Addop Relop

%%	/* end specs, begin rules of decaf. All the rules from Decaf
      were added to work with YACC . Rules with list of the tokens have 
      recursive rules so they can have one or more tokens. 
      lab6 added sematanic action in order to work AST.
     */
// The syntanx directed semantic action for Program and Package
Program : Externs T_PACKAGE T_ID '{' FieldDecls MethodDecls '}'
         { 
           
           Program = ASTCreateNode(A_PROGRAM); // Creates A_PROGRAM Node
           Program -> S1 = $1; // Externs is S1 connencted 
           if (Search($3, LEVEL,0) != NULL)
                {
                  yyerror("Package has been defined already ");
                  yyerror($3);
                  exit(1);
                }

          
           
           Program -> S2 = ASTCreateNode(A_PACKAGE); // S2 creates A_PACKAGE.
          Program -> S2 -> name = $3; // T_ID will be store in name of S2 .
          Program -> S2 -> S1 = $5; // S1 will connect FieldDecls in from S2.
          Program -> S2 -> S2 = $6; // S2 will connect MethodDecls in from S2. 
          Program -> S2 -> symbol = Insert ($3,A_Decaf_INT ,ID_Sub_Type_Package,LEVEL,0,0,NULL); // Inserts into the symbol table   

          } // end of semantic action 
        ; // end of Program
 
 //The syntanx directed semantic action for Externs       
Externs  : /* empty  */ 
         { 
          $$ = NULL; // set to null if empty
         }  // end of semantic action 
         | ExternDefn Externs
          {
            $1 -> next = $2; // Since there can be more than more we use a list and there for it can be next connencted.
          } // end of semantic action  
         ; // end of Externs. 

     
ExternDefn : T_EXTERN T_FUNC T_ID  '(' ExternTypeList ')' MethodType ';' 
            {
              // check to  make sure extern has been defined 
               if (Search($3, LEVEL,0) != NULL)
                {
                  yyerror("Extern has been defined already ");
                  yyerror($3);
                  exit(1);
                }

               $$ = ASTCreateNode(A_EXTERN); // Creates A_EXTERN Node
               $$ -> name = $3; // T_ID will be store in name of A_EXTERN .
               $$ -> S1 = $5; // ExternTypeList will be S1 connected 
               $$ -> A_Declared_Type = $7; // The declared type which are The Decaf types INT BOOL VOID and STRING . 
               $$ -> symbol = Insert ($3,$7,ID_Sub_Type_Extern,LEVEL,0,0,$5); // Inserts into the symbol table
               
                
                
              
             } // end of semantic action 
           ;// end of ExternDefn
ExternTypeList : /* Empty */ 
                {
                $$ = NULL; // if the list is empty
                }
               | ExternTypeList1 {$$ = $1;} 
               ; // end of ExternTypeList
ExternTypeList1 : ExternType
                  {
                  $$ = ASTCreateNode(A_EXTERN_TYPE) ; // Creates A_EXTERN_ TYPE Node
                  $$ -> A_Declared_Type = $1; // The Decaf Type in the Parameters in the Extern Def
                  } // end of semantic action 
                 |  ExternType ','  ExternTypeList1
                   {
                    $$ = ASTCreateNode(A_EXTERN_TYPE ); // Creates A_EXTERN_ TYPE Node
                    $$ -> A_Declared_Type = $1; // // The Decaf Type in the Parameters in the Extern Def Decaf types are  INT BOOL VOID and STRING . 
                    $$ -> next = $3; // Since there can be more than more we use a list and there for it can be next connencted.
                    } // end of semantic action
							  ;	// end of ExternTypeList1	 

// The syntanx directed semantic action for  FieldDecls 
FieldDecls : /* empty */
            {
             $$ = NULL; 
             }// end of empty 
            
            |  FieldDecl FieldDecls 
            { // if one or more field delecarations 
              $$ = $1; // first field decl
              $$ -> next = $2; // next connect if more than one 
            
            } //end of semantic action
            ; // end of FieldDecls
 
           
FieldDecl  : T_VAR T_ID  Type ';' 
             { 
              // check if the dec is the symtable
               if (Search($2,LEVEL,0) != NULL )
                  {yyerror("Variable has been defined");
                  yyerror($2);
                  exit(1);
                  }
               $$ = ASTCreateNode(A_VARDEC); // creates a node for A_VARDEC  
               $$ -> name = $2; // name gets $2 
               $$ -> A_Declared_Type = $3; // declared type INT STRING BOOL or VOID will be $3  
               $$ -> symbol = Insert($2,$3, ID_Sub_Type_Scalar, LEVEL, 1,offset,NULL); // Inserts into the symbol table
               offset += 1; // increment offset by 1 
               
             } //end of semantic action 
             |T_VAR T_ID Type T_ASSIGN  Constant ';'
             {
              // check to make it has been declared  
              if (Search ($2,LEVEL, 0) != NULL)
              {
                  yyerror ("Variable has been defined");
                  yyerror ($2);
                  exit (1);
              }
             $$ = ASTCreateNode(A_VARDEC);// creates a node for A_VARDEC  
             $$ -> name = $2; // name gets $ 2 
             $$ -> A_Declared_Type = $3; // declared type INT STRING BOOL or VOID will be $3
             $$ -> S2 = $5; // intial value that is being assigned  
             $$ -> size = 1; // since it is a scalar assign size to 1.
             $$ -> symbol = Insert($2,$3, ID_Sub_Type_Scalar, LEVEL, 1,offset,NULL); // Inserts into the symbol table
             offset += 1; // increment offset by 1 
             } //end of semantic action
           
           |T_VAR T_ID ArrayType ';' 
            {
              // checks if array is in the SymbTab
              if (Search($2,LEVEL,0) != NULL)
              {
                    yyerror("Variable for array has been defined ");
                    yyerror($2);
                    exit(1);
               }
              $$ = ASTCreateNode(A_VARDEC); // creates a node for A_VARDEC 
              $$ -> name = $2; // name gets $2 
             $$ -> S1 = $3; // ArrayType gets connected to S1.
             $$ -> A_Declared_Type = $3 -> A_Declared_Type; // The declared type is whatever the ArrayType will be 
             $$ -> size =  $3 -> size; // added lab7set the size what  of T_INTCONSTANT in ArrayType is 
             $$ -> symbol = Insert($2,$3 -> A_Declared_Type, ID_Sub_Type_Array, LEVEL,$$ -> size,offset,NULL); // Inserts into the symbol table
             offset += $3 -> size;
            
             } // end of semantic
          
           ; // end of FieldDecl
 
 // The syntanx directed semantic action for MethodDecls
MethodDecls :/* Empty */ {$$ = NULL;} // If there is zero MethodDecls
            
            // one or more MethodDecls
            | MethodDecl MethodDecls
              {$$ = $1; // first methoddecl
              $$-> next = $2; // if more than one next connect them 
              }  
            ; // end of MethodDecls

          /* set goffset to offset and assign offset to 2 for the next block */ 
MethodDecl  : T_FUNC T_ID { goffset = offset; offset = 0; maxoffset=offset;} '(' IdenList ')' MethodType 
             {
              // check if methoddecl is in the symtable
               if (Search($2, LEVEL,0) != NULL)
               {
                  yyerror ("Method name has been used");
                  yyerror($2);
                  exit(1);
               } 
                
                // insert into the symtable
                Insert($2,$7, ID_Sub_Type_Method, LEVEL, 0,0, $5);
               
                 maxoffset = offset;
            
                
               } 
               
              Block 
              
             { 
               
              $$ = ASTCreateNode (A_METHODDEC); // Creates a A_METHODDEC node 
              $$ -> name = $2;// $2 becomes the name 
              $$ -> A_Declared_Type = $7; // MethodType  will be its declared type a MethodType inclueds INT BOOL and VOID
              $$ -> S1 = $5; // IndentList will be S1 conneceted 
              $$ -> S2 = $9; // Block will be S2 connected 
              $$ -> symbol = Search($2, LEVEL,0); // insterts a symbol 
              $$ -> size = maxoffset; // set the size to max offset 
              
              
              //maxoffset = offset-1;
              $$ -> symbol -> mysize = maxoffset; // max size needed 
                
                offset = goffset;
             }// end of semantic action
            ;
// The syntanx directed semantic action for IndentList            
IdenList : /* Empty   */ {$$ = NULL;} 
         | IdenList1 {$$ = $1;} // if one or more IndentList
         ; // end of IdenList
IdenList1 : T_ID Type 
          {
            // Checks if method is in the SymbTab
           if(Search($1,LEVEL + 1, 0) != NULL)
            {
                yyerror("duplicate method arg name"); 
                yyerror($1);
                exit(1);
            }
           $$ = ASTCreateNode (A_METHODIDENTIFER  ); // Creates A_METHODIDENTIFER node 
            $$ -> name = $1; // The name will be $1.  
            $$ -> A_Declared_Type = $2; // $2 becomes the declared type The Type will be INT or BOOL
            $$ -> symbol = Insert($1,$2, ID_Sub_Type_Scalar, LEVEL + 1, 1,offset,NULL) ; // Inserts into the symbol table
              offset += 1; // increment offset
            }// end of semantic action 
          | T_ID Type ',' IdenList1 
             { 
              // check if method arg is in the symtable
              if(Search($1,LEVEL + 1, 0) != NULL)
               {
                  yyerror("duplicate method arg name"); 
                  yyerror($1);
                  exit(1);
                } //end if Search
              
              $$ = ASTCreateNode (A_METHODIDENTIFER); // Creates A_METHODIDENTIFER node 
               $$ -> name = $1;// The name will be $1 
               $$ -> A_Declared_Type = $2; // $2 becomes the declared type. The Type will be INT or BOOL
               $$ -> size = 1; // set scalar to size 1 
               $$ -> next = $4; // Since this a list we next connect any other possible IdenList
               $$ -> symbol = Insert ($1,$2, ID_Sub_Type_Scalar, LEVEL + 1, 1,offset,NULL) ; // insterts a symbol 
               offset += 1; // increment offset 
            }  // end of semantic action                  
          ; // end of IdenList1
  
  // The syntanx directed semantic action for  Block    
  // Level must be increment at the beginning of each new block 
Block    : '{' {LEVEL++;}VarDecls Statements '}'
           {
            $$ = ASTCreateNode(A_BLOCK); // Creates a A_BLOCK node 
            $$ -> S1 = $3 ;    /* VarDecls are S1 connected since we added level++  $$ -> S1 = $3 instead of $2 */
            $$ -> S2 = $4; // Statements are S2 connected since we added level++  $$ -> S1 = $4 instead of $3
            if(mydebug )Display(); // display shows up when in debugging mode 
            // get the maxium size for maxoffset 
            if (offset > maxoffset)
            {
              maxoffset = offset;
            }
           offset -= Delete(LEVEL); // delete before exiting the block 
            LEVEL --; // decrement level when block is finished 
           } // end of semantic action 
         ; // end of Block 
// The syntanx directed semantic action for Var Decls
VarDecls : /* Empty */ {$$ = NULL;}  // zero VarDecls
         // if one or more 
         | VarDecl VarDecls 
         {$$ = $1; // once var decl
         $$ -> next = $2; // next connected if there are one more VarDecls
         }// end of sematanic action
         ; // end of VarDecls
  // The syntanx directed semantic action for VarDecl       
VarDecl  :  
          
           T_VAR T_ID Type';' 
            {
              // check if the VarDecl is the symtable video is at 0 had orignially at one 
                if (Search($2,LEVEL,0) != NULL){
                    yyerror("Variable is already defined ");
                    yyerror($2);
                    exit(1);
                   
             }
                 
            $$ = ASTCreateNode(A_VARDEC); // creates a A_VARDEC node
            $$ -> name = $2; // The T_ID is stored in name 
            $$ -> A_Declared_Type = $3;// Gets the type which can be INT or BOOL 
            // inserts the VarDecl in the symtable 
            $$ -> symbol = Insert($2,$3, ID_Sub_Type_Scalar, LEVEL, 1,offset,NULL); // insterts a symbol 
                 
                offset ++; // increment offset by 1
                
             
          } // end of semantic action
         
          | T_VAR T_ID ArrayType ';'
           {
           if (Search($2,LEVEL,0) != NULL)
            {
                    yyerror("Variable for array has been defined ");
                    yyerror($2);
                    exit(1);
              }      
           $$ = ASTCreateNode(A_VARDEC); // creates a A_VARDEC node
           $$ -> name = $2; // The T_ID is stored in name 
           $$ -> S1 = $3; // The ArrayType is S1 connected 
           $$ -> A_Declared_Type = $3 -> A_Declared_Type;  // Gets the type which can be INT or BOOL
           $$ -> size = $3 -> size; // size of the array 

           $$ -> symbol = Insert($2,$3 ->A_Declared_Type, ID_Sub_Type_Array, LEVEL, $$->size ,offset,NULL); // insterts a symbol 
         
           offset += $3 -> size; // increment offset by array size
          } // end of semantic action 
        
         ; // end of VarDecl
        

// The syntanx directed semantic action for Statements 
Statements :/* Empty  */ {$$ = NULL;} // if zero statements 
            // if one or more statements 
            |  Statement Statements 
              {$$ =$1; 
               $$ -> next = $2;} // The statements will next connect if more statements are in the program
           ;
// Moved all the statements in to one statement
// Did change the names of the statement to fit which 
// statement it was performing.
// The syntanx directed semantic action for Statement
Statement : Block {$$ = $1;}
          | Assign';'{$$ = $1;}
          | MethodCall';' {$$ = $1;}
          | IfStatement {$$ = $1;}
          | WhileStatement{$$ = $1;}
          | ForStatement {$$ = $1 ;}
          | ReturnStatement {$$ = $1;}
          | BreakStatement {$$ = $1;}
          | ContinueStatement{$$ = $1;}


          ; // end of Statement 

// The syntanx directed semantic action for Assign
Assign    : Lvalue T_ASSIGN Expr  
           { 
            // make sure lvalue and expression have the same type 
            if ($1 -> A_Declared_Type != $3 -> A_Declared_Type)
              {
                yyerror("type mismatch on assignment"); 
                exit(1);
              }
              $$ = ASTCreateNode (A_ASSIGN); // Creates a A_ASSIGN  node
              $$ -> S1 = $1;// Lvalue is S1 connected 
              $$ -> S2 = $3; // Expr is S2 connected 
              $$ -> name = Create_Temp(); // assigns name with Create_Temp
              
              $$ -> symbol = Insert ($$ -> name, $$ -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // insert symbol
              offset++; // increments offset 
              }
// The syntanx directed semantic action for Lvalue          
Lvalue    : T_ID 
           { 
             struct SymbTab *p ;
             p = Search($1,LEVEL,1); // looking to see if the variable has been declared 

             // Barfs if not in the symtable 
             if (p == NULL)
             {
              yyerror("Lvalue hasnt been defined ");
              yyerror ($1);
              exit(1);
             }
             // barfs if it is not a scalar 
             if (p -> SubType != ID_Sub_Type_Scalar)
             {
              yyerror($1);
              yyerror("Wrong SubType , needs to be a Scalar");
              exit(1);
             }
            
            $$ = ASTCreateNode(A_VAR_LVALUE); // Creates a A_VAR_LVALUE
            $$ -> name = $1; // T_ID is stored in  name 
            $$ -> symbol = p; // assigns symbol from p 
            $$ -> A_Declared_Type = p -> Type; // assigns declared p type 
           }// end ofsemantic action
           
           | T_ID '['Expr ']' 
           { struct SymbTab *p;
             p = Search($1,LEVEL,1); // looking to see if the variable has been declared 
             
             // barfs if not in the SymbTab
             if (p == NULL)
             {
              yyerror("Lvalue array hasnt been defined ");
              yyerror ($1);
              exit(1);
             }
             // barfs if not array type 
             if (p -> SubType != ID_Sub_Type_Array)
             {
              yyerror($1);
              yyerror("Wrong SubType , needs to be am array");
              exit(1);
             }
            $$ = ASTCreateNode(A_VAR_LVALUE); // Creates A_VAR_LVALUE node
            $$ -> name = $1; // T_ID is store in name 
            $$ -> S1 = $3; // Expr is S1 connected
            $$ -> symbol = p;// assigns symbol from p 

            $$ -> A_Declared_Type = p -> Type; // assigns declared p type 
            } //  end of semantic aciton
            
          ;// end Lvalue
 // The syntanx directed semantic action for MethodCall          
MethodCall : T_ID '('  MethodArgs ')' 
             {
               struct SymbTab *p; 
             p = Search($1,LEVEL,1); // looking to see if the method has been declared 
             // barfs if method has not been declared 
             if (p == NULL)
             {
              yyerror("Method hasnt been defined ");
              yyerror ($1);
              exit(1);
             }
             // barfs if the subType is not a method or extern method  
             if ((p -> SubType != ID_Sub_Type_Method) && (p -> SubType != ID_Sub_Type_Extern))
             
             {
              yyerror($1);
              yyerror("Wrong SubType, needs to be a Method");
              exit(1);
             }
              $$ = ASTCreateNode(A_METHODCALL); // Creates a A_METHODCALL node 
              $$ -> name = $1; // T_ID is store in name 
              $$ ->symbol = p; // set the symbol 
              $$ -> S1 = $3;
              $$ -> A_Declared_Type = p -> Type;
            // we need to check the formal parameters' length and type is the same as the actual parameters
             if (check_parameters ( p->fparms , $3) == 0 )
             {
              yyerror ("Formal  and actual paramters  do not match");
              yyerror ($3);
              printf("p -> fparms = %d $3 = %d \n", p -> fparms ->A_Declared_Type, $3 -> A_Declared_Type);
              exit(1);
             }// end check_parameters
              } // MethodArgsList is connected. 
              
             
           ;// end of MethodCall
// The syntanx directed semantic action for MethArg           
MethodArg  : Expr {$$ = ASTCreateNode(A_METHOD_ARG);
              $$ -> S1 = $1; // Creates a Method Arg node and connects S1 Expr 
              $$ ->  A_Declared_Type = $1 -> A_Declared_Type;
              $$ -> name = Create_Temp(); // assigns name with Create_Temp
              $$ -> symbol = Insert ($$ -> name, $$ -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // inserts symbol in symtab
               offset ++; // increment offset 
              
             } 
            | T_STRINGCONSTANT

             {
              $$ = ASTCreateNode (A_METHOD_ARG );
              $$ -> symbol = Insert (Create_Temp(), A_Decaf_STRING,ID_Sub_Type_String ,LEVEL,1,offset++,NULL); // inserts symbol in symtab
              $$ ->  A_Declared_Type = A_Decaf_STRING;
              $$ -> S1 = ASTCreateNode(A_CONTSTANT_STRING); //Creates A_CONTSTANT_STRING node 
              $$ -> S1 ->  A_Declared_Type = A_Decaf_STRING; // Assign to String 
               $$ -> S1 -> name = $1;
             
              
              
             } // end of semantic action 
            
             ;
/*Added Method Args and MethodArgsList inorder for method call to have zero or more arguments */
// The syntanx directed semantic action for MethArgs              

  MethodArgs : /* Empty */  {$$ = NULL;} // if there no arguments 
             | MethodArgsList {$$ = $1;} // if there are one or more arguments
              ; // end of MethArgs
             
// The syntanx directed semantic action for MethodArgsList              
MethodArgsList : MethodArg {$$ = $1;} // one argument in method call gets $1  
                 | MethodArg ',' MethodArgs { // if more than one  argument
                  $$ = $1; // first argument wil get $1 
                  $$ -> next = $3; // since this is a list it will be next connected for two or more arguments.
                  //$$ -> name = Create_Temp(); // assigns name with Create_Temp
                  //$$ -> symbol = Insert ($$ -> name, $$ -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // inserts symbol in symtab
                  //offset++;
                  
                 }                                   
// The syntanx directed semantic action for IfStatement
IfStatement : T_IF '(' Expr ')' Block T_ELSE Block 
             {
              $$ = ASTCreateNode(A_IFSTMT);// Creates a A_IFSTMT node 
              $$ -> S1 = $3; //  Expr is S1 connected 
              $$ -> S2 = ASTCreateNode(A_IFBODY); // Creates A_IFBODY node that will be S2 connected 
              $$ -> S2 -> S1 = $5; // Since A_IFBODY is S2 connected IFBODY S1 will be Block 
              $$ -> S2 -> S2 = $7; // removed A_ELSE node unnessary node T_ELSE is S2 connected to IFBody 
              
              
              
             } // end of semantic action 
            | T_IF '(' Expr ')' Block
              {
              $$ = ASTCreateNode(A_IFSTMT); // Creates a A_IFSTMT node 
              $$ -> S1 = $3; //  Expr is S1 connected
              $$ -> S2 = ASTCreateNode(A_IFBODY); // Creates A_IFBODY node that will be S2 connected
              $$ -> S2 -> S1 = $5; // Since A_IFBODY is S2 connected IFBODY S1 will be Block 
              $$ -> S2 -> S2 = NULL; // Since this doesnt contain a Else it will remain null 
              } // end of semantic action
             
           ; // end of IfStatement
// The syntanx directed semantic action for While Statment            
WhileStatement :  T_WHILE '(' Expr ')' Block 
                  {$$ = ASTCreateNode(A_WHILESTMT); // Creates a A_WHILESTMT node 
                   $$ -> S1 = $3; // Expr is S1 connected 
                   $$ -> S2 = $5;// Block is S2 connected 
                   }// end of semantic action 
               ; // end of WhileStatement

// The syntanx directed semantic action for ForStatement
ForStatement : T_FOR '(' Assign ';' Expr ';' Assign ')' Block 
                {
                  $$ = ASTCreateNode(A_FORSTMT); // creates a A_FORSTMT node 
                  $$ -> S1 = $3; // Assign  is S1 connected 
                  $$ -> S2 = $5; // Exp is S2
                  $$-> S1 -> S1 = $7; // Assign is S1 connected from S1
                  $$-> S1 -> S2 = $9; // Block is S2 connected from S1
                }// end of semantic action 
                ;// end of ForStatement
// The syntanx directed semantic action for ReturnStatement               
ReturnStatement : T_RETURN '('  Expr  ')'  ';'
             {$$ = ASTCreateNode(A_RETURN); // Creates a A_RETURN node 
              $$ ->S1 = $3; // Expr is S1 connected 
              } // end semantic
          | T_RETURN  '('   ')'  ';'
            {$$ = ASTCreateNode(A_RETURN);} // Creates a A_RETURN node 
          | T_RETURN ';'
            {$$ = ASTCreateNode(A_RETURN);} // Creates a A_RETURN node 
          ; //end of ReturnStatement

// The syntanx directed semantic action for BreakStatement          
BreakStatement : T_BREAK ';'
           { $$ = ASTCreateNode(A_BREAK); // Creates a A_BREAK node 
           }//end of semantic action 
          ; // end BreakStatement

// The syntanx directed semantic action for ContinueStatement
ContinueStatement : T_CONTINUE';' 
           {
             $$ = ASTCreateNode(A_CONTINUE); // Creates a A_CONTINUE node 
           }// end of semantic action 
          ; // end ContinueStatement 

Expr : Simpleexpression {$$ = $1 ; }  // Simpleexpression is store in $1
     ; // end of Expr

// The syntanx directed semantic action for Simpleexpression     
Simpleexpression : Additiveexpression {$$ = $1 ; } // Additiveexpression is store is $1 
                 | Simpleexpression Relop Additiveexpression
                  { if($1 -> A_Declared_Type != $3 -> A_Declared_Type)
                      {
                          yyerror("Both sides must be the same type");
                          exit(1);
                      }
                    
                    $$ = ASTCreateNode (A_EXPR); // Creates a A_EXPR node 
                   $$ -> S1 = $1; // Expr S1 is assigned to $1 
                   $$ -> operator = $2; // opreator gets $2 
                   $$ -> A_Declared_Type = A_Decaf_BOOL; // boolean
                   $$ -> S2 = $3; //S2 is gets $3
                   $$ -> name = Create_Temp(); // assigns name with Create_Temp
                   $$ -> symbol = Insert ($$ -> name, $$ -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // assigns name with Create_Temp
                   offset ++; // increment offset

                  } // end semantic action
                ; // end of Simpleexpression

// The syntanx directed semantic action for Relop
Relop : T_LEQ {$$ = A_LEQ;} 
       | '<' {$$ = A_LT;}
       | T_GT {$$ = A_GT;}
       | T_GEQ {$$ = A_GEQ;}
       | T_EQ {$$ = A_EQ;}
       |  T_NEQ {$$ = A_NEQ;}
      ;//end of Relop
 // The syntanx directed semantic action for Additiveexpression     
Additiveexpression : Term {$$ = $1 ; } // Term is assigned with $1 
                    | Additiveexpression Addop Term 
      { if ($1 -> A_Declared_Type != $3 -> A_Declared_Type || $1 -> A_Declared_Type != A_Decaf_INT)
         {
           yyerror ("Additon and Subtraction need integers only");
           exit(1);

         }
        $$ = ASTCreateNode (A_EXPR); // Creates a A_EXPR node 
      $$ -> S1 = $1; // S1 is assigned with $1 
      $$ -> operator = $2; // operator is assigned with $2 
      $$ -> S2 = $3; // S2 is assigned with $3 
      $$ -> A_Declared_Type = $1 -> A_Declared_Type;
      $$ -> name = Create_Temp(); // assigns name to  Create_Temp to store temporary value in symtab
      $$ -> symbol = Insert ($$ -> name, $$ -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // inserts symbol in a symtable 
            offset ++; // increment offset

      }// end of semantic action 
      ; // end of Additiveexpression
// The syntanx directed semantic action for Addop
Addop : '+' {$$ = A_PLUS;}
      | '-' {$$ = A_MINUS;}
      ;// end of Addop
// The syntanx directed semantic action for Term
Term : Factor {$$ = $1 ; }
     | Term Multop Factor
      {  if ($1 ->  A_Declared_Type != $3 -> A_Declared_Type  )
          {
                yyerror ("type mismatch"); 
                exit(1);
          }
          if (($1 -> A_Declared_Type == A_Decaf_BOOL) && ($2 == A_TIMES || $2 == A_DIVIDE))
          {
            yyerror("Cant use booleans on a arithemetic operation " ); 
            exit(1);
          }

         if (($1 -> A_Declared_Type == A_Decaf_INT) && ($2 == A_AND || $2 == A_OR || $2 == A_LEFTSHIFT || $2 == A_RIGHTSHIFT ))
          {
            yyerror("Cant use  a boolean operator on integers" ); 
            exit(1);
          } 
        $$ = ASTCreateNode (A_EXPR); // Creates a A_EXPR node 
      $$ -> S1 = $1; // S1 is assigned $1 
      $$ -> operator = $2; // operator is assigned $2 
      $$ -> A_Declared_Type = $1 -> A_Declared_Type;
      $$ -> S2 = $3; //S2 is assigned with $3
      
      $$ -> name = Create_Temp(); // assigns name with Create_Temp
      $$ -> symbol = Insert ($$ -> name, $$ -> A_Declared_Type,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // assigns name with Create_Temp
        offset ++; // increment offset 

      
      }
     ;
 // The syntanx directed semantic action for Multop
      
Multop : '*' {$$ = A_TIMES ; }
      | '/'  {$$ = A_DIVIDE ; }
      | T_AND {$$ = A_AND ; }
      | T_OR  {$$ = A_OR ;}
      | T_LEFTSHIFT {$$ = A_LEFTSHIFT ;}
      | T_RIGHTSHIFT {$$ = A_RIGHTSHIFT ;}
       ; // end of Multop

// The syntanx directed semantic action for Factor
        Factor : T_ID {
          struct SymbTab *p;
             p = Search($1,LEVEL,1); // looking to see if the variable has been declared 
             // Barfs if Rvalue is not in the SymbTab
             if (p == NULL)
             {
              yyerror("Rvalue hasnt been defined ");
              yyerror ($1);
              exit(1);
             }
             // Barfs if subType not a scalar
             if (p -> SubType != ID_Sub_Type_Scalar)
             {
              yyerror($1);
              yyerror("Wrong SubType , needs to be a Scalar");
              exit(1);
             }
          $$ = ASTCreateNode(A_VAR_RVALUE) ; // Creates a A_VAR_RVALUE  
          $$ -> name = $1; // T_ID is store in name
          $$ -> symbol = p; // assigns symbol from p 
          $$ -> A_Declared_Type = p -> Type;
          } // end of semantic action 
       |  MethodCall
        {$$ = $1;} 
       | T_ID '[' Expr ']' 
        {
          struct SymbTab *p ;
             p = Search($1,LEVEL,1); // looking to see if the variable has been declared 
              // barfs if array is not in the SymbTab
             if (p == NULL)
             {
              yyerror("Array hasnt been defined ");
              yyerror ($1);
              exit(1);
             }
            // Barfs if subType is not array
             if (p -> SubType != ID_Sub_Type_Array)
             {
              yyerror($1);
              yyerror("Wrong SubType , needs to be an Array");
              exit(1);
             }
          $$ = ASTCreateNode(A_VAR_RVALUE) ; // Creates a A_VAR_RVALUE  
          $$ -> name = $1; // T_ID is sotre in name 
          $$ -> S1 = $3;// S1 is assigned with $3 
          $$ -> symbol = p; // assigns symbol from p 
          $$ -> A_Declared_Type = p -> Type;
        }
       | Constant {$$ = $1 ; }
       | '(' Expr ')' {$$ = $2 ; }
       | '!' Factor {
           // Barfs if assignment value is not a boolean value
           if ($2 ->A_Declared_Type != A_Decaf_BOOL)
             {
              yyerror(" type mismatch, expected a boolean ");
             
              exit(1);
              }

         $$ = ASTCreateNode(A_EXPR); // creates a A_EXPR 
         $$ -> operator = A_NOT; // assigned operator to A_NOT
         $$ -> S1 = $2; // S1 is assigned to $2 
         $$ -> A_Declared_Type = A_Decaf_BOOL; 
         $$ -> name = Create_Temp(); // assigns name to  Create_Temp to store temporary value in symtab
         $$ -> symbol = Insert ($$ -> name, A_Decaf_BOOL,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // Inserts symbol in SymbTab
         offset ++; // increments offset
         } // end of semantic aciton 
      
       | '-' Factor 
       {
         $$ = ASTCreateNode(A_EXPR); // creates a A_EXPR node 
         $$ -> operator =  A_MINUS; // operator is assigned A_Minus 
         $$ -> S1 = $2; // S1 is assigned to $2 
         $$ -> A_Declared_Type = A_Decaf_INT;
         $$ -> name = Create_Temp(); // assigns name to  Create_Temp to get the tempory value of Expr
         $$ -> symbol = Insert ($$ -> name, A_Decaf_INT,ID_Sub_Type_Scalar,LEVEL,1,offset,NULL); // Inserts symbol in SymbTab // change from BOOL to INT
         offset ++;// increments offset
         }// end of semantic action 
       
       ;
 // The syntanx directed semantic action for ExternType      
ExternType : T_STRINGTYPE {$$ = A_Decaf_STRING;}  
           | Type 
           ; // end of extern type 
// The syntanx directed semantic action for Type 
Type : T_INTTYPE {$$ = A_Decaf_INT;}
     | T_BOOLTYPE {$$ = A_Decaf_BOOL;}
     ; // end of semantic action for Type 
 // The syntanx directed semantic action for MethodType      
MethodType : T_VOID { $$ = A_Decaf_VOID; }
           | Type  { $$ = $1;}
           ; // end of semantic action for MethodType
 // The syntanx directed semantic action for BoolConstant               
BoolConstant : T_TRUE  // 
              {$$  = ASTCreateNode(A_CONTSTANT_BOOL) ; // Creates A_CONTSTANT_BOOL node 
              $$ -> value = 1; // value 1 means it is true 
              $$ -> A_Declared_Type = A_Decaf_BOOL;
              
              }
             | T_FALSE 
              {$$  = ASTCreateNode(A_CONTSTANT_BOOL) ; // Creates A_CONTSTANT_BOOL
              $$ -> value = 0;
              $$ -> A_Declared_Type = A_Decaf_BOOL;
              } // value is 0 means it is false 
             ; // end of BoolConstant
// The syntanx directed semantic action for ArrayType             
ArrayType :'[' T_INTCONSTANT ']' Type
          {$$ = ASTCreateNode(A_ARRAYTYPE); // Creates a Node for A_ARRAYTYPE
          $$ -> value = $2; // whatever is in T_INTCONSTANT value is assigned 
          $$ -> size = $2; // size of the array
          $$ -> A_Declared_Type = $4; // The declared type can be INT or BOOL 
          } // end of semantic action 
          ; // end of ArrayType
 // The syntanx directed semantic action for
Constant :  T_INTCONSTANT
              {$$ = ASTCreateNode(A_CONTSTANT_INT); // It can be either a T_INTCONSTANT
              $$ -> value =$1;
              $$ -> A_Declared_Type = A_Decaf_INT;
              }// end of semantic action 
         |  BoolConstant{$$ = $1; 
                          // OR it can BOOL constant 
               }// end of semantic action 
         ;                     				   
 %%	/* end of rules, start of program */

// We assume the input is correct without errors
int main(int argc, char * argv[])
{   
   int i = 1; 
   FILE *fp; 
   char s [100];
    // processes the input line 
    while (i < argc)
    {
       
      if (strcmp (argv[i], "-d") == 0 )
      { // we have a debug 
        mydebug = 1; 
      }

      if (strcmp (argv[i], "-o") == 0 )
      { // we have a input file 
         // Copy argv into a temporary 
         // add suffix 
         // attempt to open the file

         strcpy (s, argv[ i  + 1 ]); 
         strcat (s, ".s");
         
         
         if ( (fp = fopen(s,"w")) == NULL )
         {
            printf(" %s cannot open \n", argv[i + 1]);
            exit(1);
         }
      }
       i = i + 1;  
    }
    yyparse();
    if(mydebug) Display(); // display shows up when in debugging mode 
    if(mydebug) printf("Parsing Complete \n"); // Prints when Parsing is completed in deugging mode 
     if(mydebug) ASTprint(0, Program); // Prints the AST tree in debugging mode
     EMIT(Program,fp);
}
