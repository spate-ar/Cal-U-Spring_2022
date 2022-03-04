#ifndef SCANNER
#define SCANNER

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum tokenType{
           BEGIN = 0,
           END = 1,
           READ = 2,
           WRITE = 3,
           IF = 4,
           THEN = 5,
           ELSE = 6,
           ENDIF = 7,
           WHILE = 8,
           ENDWHILE = 9,
           ID = 10,
           INTLITERAL = 11,
           FALSEOP = 12,
           TRUEOP = 13, 
           NULLOP = 14,
           LPAREN = 15,
           RPAREN = 16,
           SEMICOLON = 17,
           COMMA = 18,
           ASSIGNOP = 19,
           PLUSOP = 20,
           MINUSOP = 21,
           MULTOP = 22,
           DIVOP = 23,
           NOTOP = 24,
           LESSOP = 25,
           LESSEQUALOP = 26,
           GREATEROP = 27,
           GREATEREQUALOP = 28,
           EQUALOP = 29,
           NOTEQUALOP = 30,
           SCANEOF = 31,
           ERROR = 32,
};
typedef unsigned char token;

// parses chars for token
void tokenScanner(char * buffer, FILE** inFile, FILE ** outfile, FILE ** listFile, char * inpFile, char * outpFile, char * lisFile);
// initializes program returns infile, outfile, listing file
void startUp(FILE ** inFile, FILE ** outfile, FILE ** listFile, char * inpFile, char * outpFile, char * lisFile);
// clears the buffer
void clearBuffer(char * buff); 
// returns the token of the string
token checkReserved(char * token);
// prints lexical error if flag, returns count
int lexicalError(char * lexArr, bool flag); 
// identifies the string for the token
void tokenIdent(token t, char * s); 
// adds a character to the buffer
void addChar(char * buff, char newChar); 
// closes files used in the scanner
void fileCloseScanner(FILE ** f0, FILE ** f1, FILE ** f2, char * inpFile, char * outpFile, char * lisFile);
// opens a file, returns file handle
#endif
