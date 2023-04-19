         /* simple lex program which removes comments from a source program
            The main key is that a variable "comment" is set when the start of a comment
            is seen and then unset when the ending set is seen.  It is possible to have
            two starts closed by on end.

            Shaun Cooper
            January 2015

            Nhan Le
            September 1, 2022
 
         */
        int comment = 0;
        int debug=1;  /* prints out debug statements if desired */
        int lineCount=1;  /*declare lineCount to count the line */
%%
"/*"	{
	if (comment && debug) fprintf(stderr,
		" >>>>>>>> line %d: Possible Nested comment <<<<<<<<<<\n",lineCount); /*add lineCount variable, so we can see which line has error*/
	comment = 1;
	}

"*/"	{
          if (!comment) printf("%s",yytext); /*print out if it is not in a comment */
  	  comment = 0;
	}

.       { if (!comment) printf("%s",yytext);}

\n {
  lineCount++;
  printf("%s", yytext); /* print new line when we have it */
}

%%

int yywrap(void)
{ return 1;
}

int main()  /*add int */
{
  yylex();
  
}

