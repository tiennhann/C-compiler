/*
 * Nhan Le 
 * CS370 
 * September 5, 2022
 * Purpose of the program: using a symbol table to test its function by 5 mehthods:insert, modify,
 * search, delete, display
 * Thing that I edited in this code: I added the symtable.h, modified the declare variable parts,
 * added address variable in main methods, declare sym array for symbol, added request to get user input
 * for symbol and address to switch case 1. 
 *
 * 
 * */

#include<stdio.h>
/* #include<conio.h> */
//#include<malloc.h> /*malloc gives me an error when I run the program*/
#include<string.h>
#include<stdlib.h>
#include "symtable.h" /*make a symtable.h file which include the function in h file*/

int size=0;
void Display();
void Delete();
void Modify();

struct SymbTab *first,*last;

void main() {
   int op, address;
   struct SymbTab * y;
   char sym[10]; //sym array for symbol with char type

   do {
      printf("\n\tSYMBOL TABLE IMPLEMENTATION\n");
      printf("\n\t1.INSERT\n\t2.DISPLAY\n\t3.DELETE\n\t4.SEARCH\n\t5.MODIFY\n\t6.END\n");
      printf("\n\tEnter your option : ");
      scanf("%d",&op);

      switch(op) {
	 //move get input from user part from insert method to main method
         case 1:
         printf("\n\tEnter the symbol : "); //get symbol input from user
         scanf("%s", sym);
         
         printf("\n\tEnter the address : "); //get address input from user
         scanf("%d", &address);
         
         Insert(sym, address); //call Insert method for symbol and address 
         break;


         case 2:
         Display();
         break;


         case 3:
         Delete();
         break;

         case 4:
         printf("\n\tEnter the symbol to be searched : ");
         scanf("%s",sym); //replace label by sym
         
         y = Search(sym); //call Search method to find symbol from user's request
         
         printf("\n\tSearch Result:");
         if(y != NULL) //change y ==1 to y!=NULL which mean the symbol was found
            printf("\n\tThe symbol is present in the symbol table\n");
         else //the symbol was not found
            printf("\n\tThe symbol is not present in the symbol table\n");
         break;


         case 5:
         Modify();
         break;

         case 6:
         exit(0);
      } //end switch
   } //end do

   while(op<6);

}  //end main

//change the void Insert() to struct SymbTab * Insert(char *mySymbol, int address)
struct SymbTab * Insert(char * mySymbol, int address) {
   struct SymbTab * n;	//change int n to struct SymbTab * n;

   n = Search(mySymbol); //call Search method to find mySymbol

   //if n!= Null which means we found the symbol in the table
   if(n != NULL)
      printf("\n\tThe symbol exists already in the symbol table\n\tDuplicate can't be inserted");
   else {
      struct SymbTab *p;
      p = malloc(sizeof(struct SymbTab)); //locate the memory the heap of linked list
      p->symbol = strdup(mySymbol); //change strcypy into strdup

      p->addr = address; //point to the address
      p->next = NULL; //chek to see the  end of linked list points to null

      //printf("\n\tEnter the symbol : ");
      //scanf("%s",p->symbol);
      //printf("\n\tEnter the address : ");
      //scanf("%d",&p->addr);

      if(size == 0) {
         first = p;
         last = p;
      }//end if
      else {
         last->next = p;
         last = p;
      }//end else

      size++;
   }//end  else

   printf("\n\tSymbol inserted\n");

   }//end Insert method


//prints contents of linked list 
void Display() {
   int i;
   struct SymbTab *p;
   p=first;
   printf("\n\tSYMBOL\t\tADDRESS\n");

   for(i = 0; i < size; i++) {
      printf("\t%s\t\t%d\n", p->symbol, p->addr); //remove label
      p=p->next;
   } //end for

} //end Display method


//searches for requested symbol from user
//change void Search() to struct SymTab *Search()
struct SymbTab * Search(char * mySymbol) {
   int i;
   struct SymbTab *p;
   p = first;

   for(i = 0; i < size; i++) { 
      if(strcmp(p->symbol, mySymbol) == 0) //compare current symbol to mysymbol
         return p; //if symbol was found, return it 
      p = p->next; //if not, we keep looking for it
   } //end for

   return NULL; //if we can not found the symbol, return null
   
} //end Search

// Modify method uses to modify the symbol that already insert  and its address
void Modify() {
   char l[10],nl[10];
   int add, choice, i, secondAddress; //add the secondAddress which is the address the user wwant to replace the old address
   struct SymbTab *p, *s; //declare s as a struct SymbTab, not int likes the original code
   p=first;
   
   printf("\n\tWhat do you want to modify?\n");
   printf("\n\t1.Only the symbol\n\t2.Only the address\n\t3.Both the symbol and address\n");
   printf("\tEnter your choice : ");
   scanf("%d",&choice);

   switch(choice) {
      case 1: //changes symbol only
      printf("\n\tEnter the old symbol : ");
      scanf("%s",l);
      s = Search(l); //we call Search() because the symbol must be exists to be replaced
      if(s == 0) //if not, print the sentence not found
      	printf("\n\tSymbol not found\n");
      else { //if the symbol exist, we will replace it
         printf("\n\tEnter the new symbol : ");
         scanf("%s",nl);

         for(i = 0; i < size; i++) {
            if(strcmp(p->symbol,l) == 0)
               strcpy(p->symbol,nl); //replace old symbol with new one

            p=p->next; //move to next character
         } //end for

      	printf("\n\tAfter Modification:\n");
      	Display(); //we want to display the table after modifying

      } //end else
      break;

      case 2: //changes address only
      printf("\n\tEnter the symbol where the address is to be modified : ");
      scanf("%s",l);
      s=Search(l); //search for the symbol which locate its address

      if(s==0) //if not, print the sentence not found
         printf("\n\tSymbol not found\n");
      else { //if symbol exist, we will enter the new address to replace
         printf("\n\tEnter the new address : ");
         scanf("%d",&secondAddress);

         for(i=0;i<size;i++) {
            if(strcmp(p->symbol, l) == 0)
               p->addr = secondAddress; //replace address
               
            p=p->next;
         } // end for

      	printf("\n\tAfter Modification:\n");
      	Display(); //display the table after replacing address
      } // end else
      break;

      case 3: //changes both symbol and address
      printf("\n\tEnter the old symbol : ");
      scanf("%s",l);
      s=Search(l); //search to make sure the symbol exists
      
      if(s==0) //if not, print the sentence not found
         printf("\n\tSymbol not found\n");
      else { //else continue to replace address and symbol
         printf("\n\tEnter the new symbol : ");
         scanf("%s",nl);
         printf("\n\tEnter the new address : ");
         scanf("%d",&add);

         for(i=0;i<size;i++) {
            if(strcmp(p->symbol,l)==0) {
               strcpy(p->symbol,nl); //replace symbol
               
               p->addr=add; //replace address
            } //end if

            p=p->next;
         } //end for

         printf("\n\tAfter Modification:\n");
         Display(); //display new tabel with new address and new symbol
       } //end else
       break;
   } //end switch
} //end modify mothod

//delete method is using to remove symbol 
void Delete() {
   char l[10];
   struct SymbTab *p, *q, *a;
   p = first;
   
   printf("\n\tEnter the symbol to be deleted : ");
   scanf("%s",l);
   
   a = Search(l); //search for symbol by search method

   if(a == 0) //if not, print the sentence not found
   printf("\n\tSymbol not found\n");
   else { // else delete the symbol
      if(strcmp(first->symbol,l) == 0)
         first=first->next;
      
      else if(strcmp(last->symbol,l)==0) {
         q = p->next;
         while(strcmp(q->symbol,l) != 0) {
            p=p->next;
            q=q->next;
         } //end while

         p->next = NULL;
         last=p;
      } //end else if

      else {
         q=p->next;
         while(strcmp(q->symbol,l) != 0) {
            p = p->next;
            q = q->next;
         } //end while

         p->next = q->next;
      } //end inner else
      

      size--;
      printf("\n\tAfter Deletion:\n");
      Display(); //display new linked list to user after deletion
   } //end else
} //end Delete method
