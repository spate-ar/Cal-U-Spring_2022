#include "file_util.h"

bool init(FILE** f0, FILE ** f1, FILE ** f2, FILE ** tempf, char * inpFile, char * outpFile, char * lisFile, char * tempFile)
{
	bool badFlag = false; // flag to be returned to main and used to terminate the program
	bool overWrite = false; // flag to be used for checking file overwrites
	
	// Open input
	while(!(*f0 = fopen(inpFile, "r")) && !badFlag)
	{
		// Prompt for Input file
		printf("No valid input file or file name provided! Please enter in an input file name.\n");
		// get file name
		badFlag = getFileName(inpFile);
		// check the extension
		if(!extCheck(inpFile) && !badFlag)
		{
			// Prompt to verify no extension given
			printf("No ext given. Using default .IN\n");
			// Add the extension .IN
			strcat(inpFile, ".IN");
		}
	}
	// Check if program should terminate from input
	if((!badFlag && (*f1 = fopen(outpFile, "r"))) || outpFile != NULL)
	{
		// Prompt that input file was a success and opened for reading
		printf("\nInput file name success! Opened %s for reading.\n\n", inpFile);
				
		// Prompt for output file
		printf("No valid output file or file name provided!\n");
		if(*f1 != NULL)
		{
			fclose(*f1);
		}
		else
		{
			// Processing menu A
			badFlag = menuOptionsA(inpFile, outpFile);
		}
		// Open output
		while((*f1 = fopen(outpFile, "r"))&& !badFlag && !overWrite)
		{
			if(outputFileCheck(inpFile, outpFile))
			{
				printf("Don't overwrite the input!\n");
			}
			else
			{
				printf("Output file name already exists!\n\n");
			}
			badFlag = menuOptionsB(inpFile, outpFile, &overWrite);
		}
		if(!badFlag)	
			*f1 = fopen(outpFile, "w");	
	}
	if(badFlag)
	{
		printf("\n\nPROGRAM TERMINATED!\n\n");	
	}
	else
	{
		// copy output file name into lis file
		strcpy(lisFile, removeExt(outpFile)); 
		// add the .LIS file extension to lis file
	    strcat(lisFile, ".LIS");  
		// create lis file with extension            
	    *f2 = fopen(lisFile, "w"); 
		printf("Lis file: %s created!\n", lisFile);             
	    
		// copy output file name into lis file
		strcpy(tempFile, removeExt(outpFile)); 
		// add the .LIS file extension to lis file
	    strcat(tempFile, ".PYZDROWSKI");  
		// create lis file with extension            
	    *tempf = fopen(tempFile, "w"); 
		printf("Temp file: %s created!\n\n", tempFile); 
	}
	return badFlag;
}

// checks if output file has same name as input file when naming
bool outputFileCheck(char * inpFile, char * outpFile)
{
	bool flag = false;
	
	
	if(strcmp(outpFile, inpFile) == 0)
	{
		flag = true;
	}
	return flag;
}
// menu options for no output file read
bool menuOptionsA(char * inpFile, char * outpFile)
{
	char menuItem[1] = "";	// stores menu item selection
	bool flag = false;
	bool doneFlag = false;
		
	do
	{
		printf("Enter in one of the following commands:\n"
				"1.) Enter 'q' to quit.\n"
		        "2.) Enter 'n' for a new file name\n"
		        "3.) Enter 'Return' for default.\n"
		        );
		gets(menuItem);
		if(menuItem[0] == 'q' || menuItem[0] == 'Q') // q to quit, flag = true
		{
		    flag = true;
		    doneFlag = true;            	       
		}
		else if(menuItem[0] == 'n' || menuItem[0] == 'N') // prompt for new output file
		{
            printf("Enter a new output file name!\n");
			// prompt user for filename
			gets(outpFile);
			// check output for extension
			if(!extCheck(outpFile))
			{
				// add extension 
				strcat(outpFile, ".OUT");
			}
			// set the flag to exit loop
			doneFlag = true;
		}
		else if(menuItem[0] == '\0') // check file extension
		{
			// copy file name from input
			strcpy(outpFile, removeExt(inpFile));
			// add the default extension (.OUT)
			strcat(outpFile, ".OUT");
			// set the flag to exit loop
			doneFlag = true;
		}
		else
		{
		    printf("Please enter in a valid command...\n");
		}
	} while(!doneFlag);					
	return flag;
}
// menu options for output file read
bool menuOptionsB(char * inpFile, char * outpFile, bool * overWrite)
{
	char menuItem[1] = "";	// stores menu item selection
	bool flag = false;
	bool doneFlag = false;
	char backUpFile[25] = "";
	FILE * fp;
	FILE * fp2;
		
	do
	{
	 	printf("Enter in one of the following commands:\n"
	            "1.) Enter 'o' to overwrite the file.\n"
	            "2.) Enter 'q' to quit.\n"
	            "3.) Enter 'n' for a new file name\n"
	            "4.) Enter 'Return' for default.\n"
	            );
		gets(menuItem);
		if(menuItem[0] == 'o' || menuItem[0] == 'O') // overwrite selections
	    {
	    	if(outputFileCheck(inpFile, outpFile))
	    	{
	    		printf("Nice try...");
			}
			else
			{
				// copy file name without extension
				strcpy(backUpFile, removeExt(outpFile));
				// add .BAK extension
				strcat(backUpFile, ".BAK");
				// try to open .BAK
				if((fp = fopen(backUpFile, "r")))
				{
					// if backup exists, prompt user
					printf("Back-up already exists over-writting!\n");
					if(fp != NULL)
					{
						fclose(fp); // close file
					}
					// re-open for writing/overwrite
					printf("Opening back-up file: %s for writing\n", backUpFile);
					fp = fopen(backUpFile, "w");
					// copy contents of original file
					printf("Opening local output file: %s for reading\n\n", outpFile);
					fp2 = fopen(outpFile, "r");
					// create a character storage space
	           	 	char ch;
	            	// loop to cycle through until end of file and transfer data
	            	ch = fgetc(fp2);
					while (ch != EOF)
	            	{
	            		fputc(ch, fp);
	            		// write a character 1 at a time to the new file
	            		ch = fgetc(fp2);	            		
					}
					printf("Back-up has been created and file copied!\n");
					if(fclose(fp) == 0)
						printf("\nBack-up file: %s has been closed successfully!\n", backUpFile);
					else
						printf("Error closing back-up file.\n", backUpFile);
					if(fclose(fp2) == 0)
						printf("Local output file: %s has been closed successfully!\n", outpFile);
					else
						printf("Error closing local output file.\n", outpFile);
					*overWrite = true;
				}	
				else
				{
					printf("Opening back-up file: %s for writing\n", backUpFile);
					fp = fopen(backUpFile, "w");
					// copy contents of original file
					printf("Opening local output file: %s for reading\n\n", outpFile);
					fp2 = fopen(outpFile, "r");
					// create a character storage space
	            	char ch;
	            	// loop to cycle through until end of file and transfer data
	            	ch = fgetc(fp2);
					while (ch != EOF)
	            	{
	            		fputc(ch, fp);
	            		// write a character 1 at a time to the new file
	            		ch = fgetc(fp2);
					}
					printf("Back-up has been created and file copied!\n");
					if(fclose(fp) == 0)
						printf("\nBack-up file: %s has been closed successfully!\n", backUpFile);
					else
						printf("Error closing back-up file.\n", backUpFile);
					if(fclose(fp2) == 0)
						printf("Local output file: %s has been closed successfully!\n", outpFile);
					else
						printf("Error closing local output file.\n", outpFile);
					*overWrite = true;
				}		
				// set flag to exit loop			 
	        	doneFlag = true;	            	     
			}
	    }
		else if(menuItem[0] == 'q' || menuItem[0] == 'Q') // q to quit, flag = true
		{
		    flag = true;
		    doneFlag = true;            	       
		}
		else if(menuItem[0] == 'n' || menuItem[0] == 'N') // prompt for new output file
		{
            printf("Enter a new output file name!\n");
			// prompt user for filename
			gets(outpFile);
			// check output for extension
			if(!extCheck(outpFile))
			{
				// add extension 
				strcat(outpFile, ".OUT");
			}
			// set the flag to exit loop
			doneFlag = true;
		}
		else if(menuItem[0] == '\0') // check file extension
		{
			// copy file name from input
			strcpy(outpFile, removeExt(inpFile));
			// add the default extension (.OUT)
			strcat(outpFile, ".OUT");
			// set the flag to exit loop
			doneFlag = true;
		}
		else
		{
		    printf("Please enter in a valid command...\n");
		}
	} while(!doneFlag);				
	return flag;
}
// returns file name without extension
char * removeExt(char * fileName)
{
	char noExt[25] = "";
	char *e = noExt;
	
	size_t fileSize = strlen(fileName);
	size_t extSize = strlen(strrchr(fileName, '.'));
	
	strcpy(noExt, fileName);
	noExt[fileSize - extSize] = 0;
	
	return e;
}
// returns true if extension is found
bool extCheck(char* fileName)
{
	bool flag = false;
	int i = 0, k = 0;
	char ext [5] = "";
	char *e = ext;
	
	if((e = strrchr(fileName, '.')) != NULL)
	{
		flag = true;
	}

	return flag;
}

// returns true if empty
bool getFileName(char * fileName)
{
	bool flag = false;
	
	gets(fileName);
	if(strcmp(fileName, "") == 0)
	{
		flag = true;
	}	
	return flag;
}

// looks for open files and closes them.
void fileClose(FILE ** f0, FILE ** f1, FILE ** f2, FILE ** tempf, char * inpFile, char * outpFile, char * lisFile, char * tempFile)
{
     // close the files	if they are open
	if(f0 != NULL)
	{
		// checks if fclose returns 0, if so, it has closed successfully
		if(fclose(*f0) == 0)	
			printf("Input file: %s closed successfully!\n", inpFile); // prompt user it was successful
		else
			printf("Error closing input file.\n");
	}
	if(f1 != NULL)
	{
		// checks if fclose returns 0, if so, it has closed successfully
		if(fclose(*f1) == 0)
			printf("Output file: %s closed successfully!\n", outpFile); // prompt user it was successful
		else
			printf("Error closing output file.\n");
	}
	if(f2 != NULL)
	{
		// checks if fclose returns 0, if so, it has closed successfully
		if(fclose(*f2) == 0)
			printf(".Lis file: %s closed successfully!\n", lisFile); // prompt user it was successful
		else
			printf("Error closing lis file.\n");
	}
	if(tempf != NULL)
	{
		// checks if fclose returns 0, if so, it has closed successfully
		if(fclose(*tempf) == 0)
			printf("Temp file: %s closed successfully!\n", tempFile);	// prompt user it was successful
		else
			printf("Error closing temp file.\n");
		//remove(tempName);
	}
}
