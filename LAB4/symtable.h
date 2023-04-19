/*
Name: Nhan Le
Date: September 16, 2022 

Symbol Table changes:
Remove label. and modify completely from program
Change search to have one parameter, return 1 if there 0 if not 
Change insert to have two parameters, label and addr change SymTab struct to have char*

Variables are char *sym, address, *symbol, int addr, struct SymbTab *next, size
The Structure is built having an insert, display, delete, and search methods 
for our Linked List, the user can manually add nodes, deletes nodes, search up a node 
or display the information in a node 
*/


#include<stdio.h>
//#include<malloc/malloc.h>
#include<string.h>
#include<stdlib.h>

#ifndef SYMTABLE_H
#define SYMTABLE_H

//Phototypes for our methods in the h file 
void Insert(char *sym, int address); //takes a symbol and address and inserts it if symbol is not there 
void Display();
void Delete(char *s); //takes a symbol and removes it if present 
int Search(char *s); //takes a symbol and states if present or not returns 0 or 1 

int FetchAddress(char *s);


struct SymbTab{

	char *symbol;
	int addr;
	struct SymbTab *next;
};

#endif
