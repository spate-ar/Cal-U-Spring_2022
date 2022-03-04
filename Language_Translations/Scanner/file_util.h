#ifndef FILE_UTIL
#define FILE_UTIL

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// main file routine
bool init(FILE** f0, FILE ** f1, FILE ** f2, FILE ** tempf, char * inpFile, char * outpFile, char * lisFile, char * tempFile);
// returns a good boolean if a file is entered
bool getFileName(char * fileName);
// returns a file name without the extension
char * removeExt(char * fileName);
// menu option if a file is not read
bool menuOptionsA(char * inpFile, char * outpFile);
// menu option 2 if a file is read
bool menuOptionsB(char * inpFile, char * outpFile,  bool * overWrite);
//checks if output file has the same name as the input file to prevent overwritting
bool outputFileCheck(char * inpFile, char * outpFile);
// looks for open files and closes them.
void fileClose(FILE ** f0, FILE ** f1, FILE ** f2, FILE ** temp, char * inpFile, char * outpFile, char * lisFile, char * tempFile);
// check if a file has an extension
bool extCheck(char* fileName);
#endif
