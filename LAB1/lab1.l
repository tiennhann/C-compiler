     /*  This lex routine uses a counting array to match alphabeticstrings 
         and make a frequency count.
  
         The real item to notice is that yywrap() is called at EOF and then is run
         to do what we need to do.   yywrap() returns true when we have a successful 
         end to the program.  We may want to return false (0) if we want to lexing process 
         to fail 

        Shaun Cooper
        January 2015

        Nhan Le 
        August 22, 2022

        I add int infront of yywrap(), main to do the default type
        Declare countNumber to count number in for loop 
        Print the number, we have counted. 
        lex has 3 things: global var, lex, and c code
   */
	int lgths[100];
     int countNumber = 0;
%%
[a-zA-Z]+	lgths[yyleng]++;    //use to count the letter. 
[0-9]+ countNumber ++;   //declare a countNumber var to count for how many number.
.|\n;                    //ignore everthing and newline.
%%
int yywrap()   //it will be code at the end of the code to wrap up everything
{
	int i;
	printf("Length  No. words\n");
     //for loop to loop through the the array to find how many letter
	for (i=1; i<100; i++) {
          //the length is not zero, print it out
		if (lgths[i] > 0) {
			printf("%5d%10d\n",i,lgths[i]);
		}
	}
          printf("Total number is: %d\n", countNumber);
	return(1);
}

int main()
{  yylex();

}
