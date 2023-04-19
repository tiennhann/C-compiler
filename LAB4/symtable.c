//Name: Nhan Le 
//Date: September 16, 2022 
//Program Name: symtable.c
//Description: Creates a symbol table 

#include "symtable.h"

//declare variables for our methods to use 
int size = 0;
struct SymbTab *first,*last;

//method for inserting in the table
//check to see if symbol is in table, if not insert .0
void Insert(char *sym, int address) {

	//declare variables 
	//we store the character(string) from user into a simple integer 
	//whihc make it is easier to check 
	int n;	
	n = Search(sym);

	//if the symbol is there, print out message  
	if(n == 1)
		printf("\n\tThe symbol exists already in the symbol table\n\tDuplicate can.t be inserted");
	else
	{
		struct SymbTab *p;
		p = malloc(sizeof(struct SymbTab));
		p->symbol = strdup(sym);
		p-> addr = address;
		p-> next = NULL;

		//if Symbol Table is empty, set first symbol to null 
		if(size==0){
			first=p;
			last=p;

		}//end if
		//assign the last character pointing to null since its at the end 
		else{
			last->next=p;
			last=p;
		}//end else
	size++;
	}//end else
	printf("\n\tThe symbol inserted\n");
}//end Insert method 


//Display method will print contents of the symbol table
void Display(){
	int i;
	struct SymbTab *p;
	p = first;
	printf("\n\tSYMBOL\t\tADDRESS\n");
	
	//for loop is used to go through the linked list to check each node 	
	for(i=0;i<size;i++){
		printf("\t%s\t\t%d\n", p->symbol,p->addr);
		p=p->next;
	}//end for

}//end Display method 

//Search method will search through the linked list for specific 
//symbol that user need to search in the Symbol Table 
int Search(char *s) {
	int i, flag = 0;
	struct SymbTab *p;
	p=first;
	
	//for loop to go through each node Strcmp compares
	//character (string) to character (string)
	//to find specific value of the symbol that resquest by user	
	for(i = 0;i < size;i++){
		if(strcmp(p->symbol,s)==0)
		flag = 1;
		p = p->next;
	}//end for 
    return flag;

}//end Search method 

//Delete method delete node with Symbol and Address to it from Symbol Table
//Checks to see if it is there, if it is deleted, else print a message
void Delete(char *sym){
	int a;
	struct SymbTab *p,*q;
	p = first;
	q = NULL;

	//printf("\n\tEnter the label to be deleted : ");
	//check to see if it is there
	//scanf("%s",l);
	a=Search(sym);

	if(a==0)
		printf("\n\tSymbol not found\n");
	else{
		//it is there, replumb first and last 
		while(strcmp(p->symbol,sym) !=0){
			q = p;
			p = p->next;
		}//end while

		if (q == NULL)  //then we are at the head
			first = p-> next;
		else if (p == last){ //we are at the tail 
			last = q;
			last -> next = NULL;
		}//end else if 
		else { // we are in the middle 	
			q->next = p->next;
		}//end second else 
		
	size--;
	Display();

	}//end else 

}//end Delete method

//FetchAddress method looks at the list and find the match address 
int FetchAddr(char *s){
	struct SymbTab *p;
	p = first;

	for( int i = 0; i < size; i++){
		if(strcmp(p->symbol, s) == 0){
			break;
		}//end if
		p = p ->next;
	}//end for
	if(p != NULL)
		return p-> addr;
	return 1;
}//end FetchAddr
