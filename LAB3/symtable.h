// Nhan Le
// Sep 5,2022
// Purpose: using h file to show Insert(), Search(), struct{} definition
//

#ifndef SYMTABLE_H
#define SYMTABLE_H
	
	struct SymbTab * Insert(char *symbol, int address);
	struct SymbTab * Search(char *symbol); 

	struct SymbTab {
   		char * symbol;
   		int addr;
   		struct SymbTab * next;
	};
#endif
