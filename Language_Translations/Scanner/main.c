
/******************************************************************************

Nicholas Spudich	: spu8504@calu.edu
Andrew Spate		: spa3195@calu.edu
Dr. Pzydrowski
CSC 460 - Language Translation
2/15/2022 - 2/18/2022 (ONE DAY LATE)
Group #6
Group Assignment #2 : Scanner

*******************************************************************************/
#include "file_util.h"
#include "scanner.h"

int main(int argc, char * argv[])
{
    FILE *f0 = NULL;  			// input file handle
    FILE *f1 = NULL;  			// output file handle
    FILE *f2 = NULL;  			// list file handle
    FILE *tempf = NULL; 		// temp file handle
		
	char buffer[100] = "";		// buffer for storing characters read in from file 
	char *buff = buffer;

	char inpFile[25] = "";		// stores input file name
	char *ip = inpFile;			// ptr to input file
	char outpFile[25] = "";		// stores output file name
	char *op = outpFile;		// ptr to output file
	char lisFile[25] = ""; 		// stores lis file name
	char *lis = lisFile;		// ptr to lis file
	char tempFile[25] = ""; 	// temp file name
	char * t = tempFile;		// ptr to temp file

	if(argc == 2)				// check arguments for input file
	{
		strcpy(inpFile, argv[1]);	// copy it to input file name
	}
	if(argc == 3)				// check arguments for input and output file
	{
		strcpy(inpFile, argv[1]);	// copy it to input file name
		strcpy(outpFile, argv[2]);	// copy it to output file name
	}	
	if(!(init(&f0, &f1, &f2, &tempf, ip, op, lis, t)))	// run init to get and assign filenames
	{	
		fileClose(&f0, &f1, &f2, &tempf, ip, op, lis, t);	// close the files 
		printf("\n\n----- PROGRAM 1 COMPLETED -----\n");
		startUp(&f0, &f1, &f2, ip, op, lis);				// re-open files for scanner
		tokenScanner(buff, &f0, &f1, &f2, ip, op, lis);		// run token scanner on input file, storing contents in output and lis files
		fileCloseScanner(&f0, &f1, &f2, ip, op, lis);		// close the files used by the scanner
		printf("\n\n----- PROGRAM 2 COMPLETED -----\n");
	}	
}
