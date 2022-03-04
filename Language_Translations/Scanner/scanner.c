#include "scanner.h"

void tokenScanner(char * buffer, FILE** inFile, FILE ** outFile, FILE ** listFile, char * inpFile, char * outpFile, char * lisFile)
{
	token temp;					// type defined token for identifying
	char c;						// char for building the token
	bool flag = false;			// flag for exiting alpha char
	bool newLineFlag = false; 	// flag for inserting new lines
	bool commentFlag = false; 	// flag for grabbing a whole line of a comment
	bool intLitFlag = false; 	// flag for grabbing int literals
	bool idFlag = false; 		// flag for id/variables
	bool errorFlag = false; 	// flag for lex errors
	bool totError = false; 		// flag for printing total lex errors
	bool eofFlag = false;
	char str[25] = "";			// char array for file printing
	char *s = str;				// ptr to str array
	char errTkn[25] = "";		// char array for storing errors
	char *eT = errTkn;			// ptr to err array
	int lineNumber = 0;			// int for storing the line number
	char lexArr[25] = ""; 		// char array for storing lex errors
	char *lA = lexArr;			// ptr for lex array
	
	// print first line number to list file	
	fprintf(*listFile, "%d\t", lineNumber+1);
	// increment the line number for next line
	lineNumber++;
	// clear the buffer to ensure it is empty
	clearBuffer(buffer);

	// main loop, grabbing a char until end of file
	while((c = getc(*inFile)) != EOF) 					
	{
		
		// check for whitespace until we have something
		while(isspace(c)!=0) 						
		{
			if(c==10||c==13)							// if that space character is a newline
				newLineFlag=true;						// set a flag for list file processing
			c=getc(*inFile);							// grab next character
			
		}
		if (c==-1)
		{
			eofFlag=true;
		}
		// check if is alpha char
		if(isalpha(c) != 0)
		{ 
			// add character to the buffer
			addChar(buffer, c);
		
			// peek at next char
			do
			{		
				if(isalpha(c) != 0 || isdigit(c) == 1) // if that char is a letter or number
				{				
					c = getc(*inFile);		// grab the next char
					addChar(buffer, c); 	// add it to the buffer
				}
				else									// space detected to end string
				{
					ungetc(c, *inFile);					// not a good letter, put it back
					flag = true; 						// flip flag to exit do...while
				}
			} while(!flag);
			idFlag=true;	// flip id flag to process token
			flag = !flag; 	// flip the flag back 
		}
		// If not alpha, check for hyphen
		else if (c == '-')	
		{
			// add it to the buffer
			addChar(buffer, c);
			// check next char for hyphen and if so it is a comment
			c = getc(*inFile);
			if(c == '-')
			{
				// add the next hyphen
				addChar(buffer, c);
				// comment grab whole line
				do
				{
					// keep grabbing until newline
					c = getc(*inFile);
					if(c == EOF)
					{
						c = 10; // set the char to leave
					}
					else
					{
						// keep building the token
						addChar(buffer, c);
					}
				} while (c!= 10);
				commentFlag=true; // flip flag to process token
			}
			else if(isdigit(c) == 1) // else if digit
			{
				// add it to buffer
				addChar(buffer, c);
				
				do	// look for more
				{
					// grab next char
					c = getc(*inFile);
					// keep building token
					addChar(buffer, c);		
				} while (isdigit(c) == 1);
				intLitFlag=true; // flip flag to process token
			}
			else // else the second character is put back into the input file and the token is identified as a MINUSOP
			{
				// put it back
				//addChar(buffer, c);
				ungetc(c, *inFile);	
				// MINUSOP
			}	
		}
		else if(isdigit(c)==1)
		{
			addChar(buffer, c);
			do	// look for more
			{
				c = getc(*inFile);
				addChar(buffer, c);		
			} while (isdigit(c) == 1);
			intLitFlag=true;
		}
		else
		{
			switch(c)
			{
				case '<':
				{
					addChar(buffer, c);
					c = getc(*inFile);
					if(c=='='||c=='>')			//test for LESSEQUAL or NOTEQUAL
					{
						addChar(buffer, c);
					}
					else
					{
						ungetc(c, *inFile);
					}
					break;
				}
				case '>':
				{
					addChar(buffer, c);
					c = getc(*inFile);			//peek for next
					if(c=='=')					// test >=
					{
						addChar(buffer, c);
					}
					else
					{
						ungetc(c, *inFile);
					}
					
					break;
				}
				case ':':
				{
					addChar(buffer, c);
					c = getc(*inFile);
					if (c=='=')						// test for assignop
					{
						addChar(buffer, c);
					}
					else
					{
						ungetc(c, *inFile);
					}	
					break;
				}	
				case '(': // left paren	
				case ')': // right paren		
				case ';': // semicolon
				case ',': // comma
				case '+': // plusop
				case '*': // multop
				case '/': //divideop
				case '!': // notop
				//case EOF: //endofFile
				default:	
				
					addChar(buffer, c);
				
			}
		
		}
		
		// copy everything to listing file
		if(newLineFlag)				// if move to new line in input file, do same with list file
		{

			if(errorFlag)
			{
				fprintf(*listFile, "Error. %s not recognized\n", errTkn);
				clearBuffer(errTkn);
				errorFlag=false;
				totError=true;
			}
			fprintf(*listFile, "%d. \t%s ", lineNumber+1, buffer); // finish last buffer print out then set up for new line
			lineNumber++;
			newLineFlag=false;
		}
		else 												// if not a new line, add buffer to list file
		{
			fprintf(*listFile, "%s ", buffer);
		}
		
		if (commentFlag==false)
		{
			
			// Print to Output
			temp = checkReserved(buffer);
			if (intLitFlag)									// if number
			{
				temp=INTLITERAL;
				intLitFlag=false;
			}else if(idFlag && temp == ERROR)					// if alpha numeric that was not keyword
			{
				temp= ID;
				idFlag=false;
			}
			if (temp==ERROR)
			{
				errorFlag=true;
				addChar(eT, c);								// recieved error as token, saves for future print
				addChar(lA, c);
			}
			if(eofFlag)
			{
				temp = SCANEOF;
			}
			tokenIdent(temp, s);
			fprintf(*outFile, "Token Number: %2d\tToken Type: ", temp);
			fputs(str, *outFile);
			fprintf(*outFile, "\tActualToken: %s\n", buffer);
			commentFlag=false;
			idFlag=false;
			
		}
		else
		{
			newLineFlag=true;
		}
		commentFlag=false;
		
		// Clear the buffers
		clearBuffer(buffer);
		clearBuffer(str);
	}
	c=getc(*inFile);
	
	if(totError)
	{
	
		fprintf(*listFile, "\n\n%d\tLexical Errors", lexicalError(lA, totError));	// using for .lis
	}
}

void startUp(FILE** f0, FILE ** f1, FILE ** f2, char * inpFile, char * outpFile, char * lisFile)
{
		printf("\n\nOpening input file for reading: %s\n", inpFile);
		*f0 = fopen(inpFile, "r");

		printf("Opening output file for writing: %s\n", outpFile);
		*f1 = fopen(outpFile, "w");

		printf("Opening lis file for writing: %s\n\n", lisFile);
		*f2 = fopen(lisFile, "w");
}

void clearBuffer(char * buffer) // clears the buffer
{
	buffer[0] = '\0';
}

token checkReserved(char * ch) // returns the token of the string
{
	token t; 
	
	if(strcmp(ch, "begin") == 0)	// BEGIN
	{
		t = BEGIN;
	}
	else if(strcmp(ch, "end") == 0)	// END
	{
		t = END;
	}
	else if(strcmp(ch, "read") == 0)	// READ
	{
		t = READ;
	}
	else if(strcmp(ch, "write") == 0)	// WRITE
	{
		t = WRITE;
	}
	else if(strcmp(ch, "if") == 0)	// IF
	{
		t = IF;
	}
	else if(strcmp(ch, "then") == 0)	// THEN
	{
		t = THEN;
	}
	else if(strcmp(ch, "else") == 0)	// ELSE
	{
		t = ELSE;
	}
	else if(strcmp(ch, "endif") == 0)	// ENDIF
	{
		t = ENDIF;
	}
	else if(strcmp(ch, "while") == 0)	// WHILE
	{
		t = WHILE;
	}
	else if(strcmp(ch, "endwhile") == 0)	// ENDWHILE
	{
		t = ENDWHILE;
	}
	else if(strcmp(ch, "variable") == 0)	// variable
	{
		t = ID;
	}
	else if(strcmp(ch, "integer") == 0)	// INTLITERAL
	{
		t = INTLITERAL;
	}
	else if(strcmp(ch, "false") == 0)	// FALSEOP
	{
		t = FALSEOP;
	}
	else if(strcmp(ch, "true") == 0)	// TRUEOP
	{
		t = TRUEOP;
	}
	else if(strcmp(ch, "null") == 0)	// NULLOP
	{
		t = NULLOP;
	}
	else if(ch[0] == '(')			// LPAREN
	{
		t = LPAREN;
	}
	else if(ch[0] == ')')			// RPAREN
	{
		t = RPAREN;
	}
	else if(ch[0] == ';')			// SEMICOLON
	{
		t = SEMICOLON;
	}
	else if(ch[0] == ',')			// COMMA
	{
		t = COMMA;
	}
	else if(strcmp(ch,":=")==0)		//ASSIGNOP
	{
		t = ASSIGNOP;
	}
	else if(ch[0] == '+')			// PLUSOP
	{
		t = PLUSOP;
	}
	else if(ch[0] == '-')			// MINUSOP
	{
		t = MINUSOP;
	}
	else if(ch[0] == '*')			// MULTOP
	{
		t = MULTOP;
	}
	else if(ch[0] == '/')			// DIVOP
	{
		t = DIVOP;
	}
	else if(ch[0] == '!')			// NOTOP
	{
		t = NOTOP;
	}
	else if(ch[0] == '<')			// LESSOP
	{
		t = LESSOP;
	}
	else if(strcmp(ch, "<=") == 0)	// LESSEQUALOP
	{
		t = LESSEQUALOP;
	}
	else if(ch[0] == '>')			// GREATEROP
	{
		t = GREATEROP;
	}
	else if(strcmp(ch, ">=") == 0)	// GREATEREQUALOP
	{	
		t = GREATEREQUALOP;
	}
	else if(ch[0] == '=')			// EQUALOP
	{
		t = EQUALOP;
	}
	else if(strcmp(ch, "<>") == 0)	// NOTEQUALOP
	{
		t = NOTEQUALOP;
	}
	else if(strcmp(ch, "eof") == 0)	// SCANEOF
	{
		t = SCANEOF;
	}
	else	// ERROR
	{
		t = ERROR;
	}
	return t;
}
	
int lexicalError(char * lexArr, bool flag) // prints lexical error if flag, returns count
{
	size_t count;
	int acc=0;
	count=sizeof(lexArr);
	int i=0;
	for(i; i<count;i++)
	{
		if (lexArr[i]!=0)
		{
			acc++;
		}
	}
	return acc;
	
}

void tokenIdent(token t, char * s) // identifies the string for the token
{	
	if(t == BEGIN)	// BEGIN
	{
		strcpy(s, "BEGIN");
	}
	else if(t == END)	// END
	{
		strcpy(s, "END\t");
	}
	else if(t == READ)	// READ
	{
		strcpy(s, "READ");
	}
	else if(t == WRITE)	// WRITE
	{
		strcpy(s, "WRITE");
	}
	else if(t == IF)	// IF
	{
		strcpy(s, "IF\t");
	}
	else if(t == THEN)	// THEN
	{
		strcpy(s, "THEN");
	}
	else if(t == ELSE)	// ELSE
	{
		strcpy(s, "ELSE");
	}
	else if(t == ENDIF)	// ENDIF
	{
		strcpy(s, "ENDIF");
	}
	else if(t == WHILE)	// WHILE
	{
		strcpy(s, "WHILE");
	}
	else if(t == ENDWHILE)	// ENDWHILE
	{
		strcpy(s, "ENDWHILE");
	}
	else if(t == ID)	// ID
	{
		strcpy(s, "ID\t");
	}
	else if(t == INTLITERAL)	// INTLITERAL
	{
		strcpy(s, "INTLITERAL");
	}
	else if(t == FALSEOP)	// FALSEOP
	{
		strcpy(s, "FALSEOP");
	}
	else if(t == TRUEOP)	// TRUEOP
	{
		strcpy(s, "TRUEOP");
	}
	else if(t == NULLOP)	// NULLOP
	{
		strcpy(s, "NULLOP");
	}
	else if(t == LPAREN)	// LPAREN
	{
		strcpy(s, "LPAREN");
	}
	else if(t == RPAREN)	// RPAREN
	{
		strcpy(s, "RPAREN");
	}
	else if(t == SEMICOLON)			// SEMICOLON
	{
		strcpy(s, "SEMICOLON");
	}
	else if(t == COMMA)			// COMMA
	{
		strcpy(s, "COMMA");
	}
	else if(t == ASSIGNOP)		//ASSIGNOP
	{
		strcpy(s, "ASSIGNOP");
	}
	else if(t == PLUSOP)			// PLUSOP
	{
		strcpy(s, "PLUSOP");
	}
	else if(t == MINUSOP)			// MINUSOP
	{
		strcpy(s, "MINUSOP");
	}
	else if(t == MULTOP)			// MULTOP
	{
		strcpy(s, "MULTOP");
	}
	else if(t == DIVOP)			// DIVOP
	{
		strcpy(s, "DIVOP");
	}
	else if(t == NOTOP)			// NOTOP
	{
		strcpy(s, "NOTOP");
	}
	else if(t == LESSOP)			// LESSOP
	{
		strcpy(s, "LESSOP");
	}
	else if(t == LESSEQUALOP)	// LESSEQUALOP
	{
		strcpy(s, "LESSEQUALOP");
	}
	else if(t == GREATEROP)			// GREATEROP
	{
		strcpy(s, "GREATEROP");
	}
	else if(t == GREATEREQUALOP)	// GREATEREQUALOP
	{	
		strcpy(s, "GREATEREQUALOP");	
	}
	else if(t == EQUALOP)			// EQUALOP
	{
		strcpy(s, "EQUALOP");
	}
	else if(t == NOTEQUALOP)	// NOTEQUALOP
	{
		strcpy(s, "NOTEQUALOP");
	}
	else if(t == SCANEOF)	// SCANEOF
	{
		strcpy(s, "SCANEOF");	
	}
	else	// ERROR
	{
		strcpy(s, "ERROR");
	}
}
void addChar(char * buffer, char newChar) // adds a character to the buffer
{
	char c = newChar;
	char ch[1] = "";
	ch[0] = c;
	ch[1] = '\0';
	
	strcat(buffer, ch);
}
// looks for open files and closes them.
void fileCloseScanner(FILE ** f0, FILE ** f1, FILE ** f2, char * inpFile, char * outpFile, char * lisFile)
{
     // close the files	if they are open
	if(*f0 != NULL)
	{
		// checks if fclose returns 0, if so, it has closed successfully
		if(fclose(*f0) == 0)	
			printf("Input file: %s closed successfully!\n", inpFile); // prompt user it was successful
		else
			printf("Error closing input file.\n");
	}
	if(*f1 != NULL)
	{
		// checks if fclose returns 0, if so, it has closed successfully
		if(fclose(*f1) == 0)
			printf("Output file: %s closed successfully!\n", outpFile); // prompt user it was successful
		else
			printf("Error closing output file.\n");
	}
	if(*f2 != NULL)
	{
		// checks if fclose returns 0, if so, it has closed successfully
		if(fclose(*f2) == 0)
			printf(".Lis file: %s closed successfully!\n", lisFile); // prompt user it was successful
		else
			printf("Error closing lis file.\n");
	}
}

/*
FILE* fileOpen(char * fileName, char c)
{
	FILE* file;
	
	if(c == 'r')
	{
		printf("\n\nOpening file for reading: %s\n", fileName);
		file = fopen(fileName, "r");
	}
	else
	{
		printf("\n\nOpening file for writing: %s\n", fileName);
		file = fopen(fileName, "w");
	}
	return file;
}
*/
