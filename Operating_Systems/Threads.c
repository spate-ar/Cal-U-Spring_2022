// Andrew Bissell, Andrew Spate, Nicholas Spudich
// Group 2
// Operating Systems CSC-400
//---------------------------------------------------------------INCLUDE/DEFINE------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#define NUMBEROFTHREADS 8
#define MAXRANGE 1923457200
#define MINRANGE 1923456000
int increment;
//----------------------------------------------------------------THREADTASK---------------------------------------------------------------------//
// DESCRIPTION: Takes created thread and gives it the task to calculate the prime numbers within the MAX/MINRANGE.
// RETURNS: void
// PARAMETERS: Current count of the thread being created (int)
// CALLS: none
void *ThreadTask(int threadCount)
{
	long start= MINRANGE+(threadCount*increment);
	long finish= start+increment;
	time_t startTime, currentTime;
	double endTime;
	
	// Prompt to user on thread creation and processing range
	printf("Thread %d from %ld to %ld\n", threadCount, start, start+increment);
	// Start timer
	time(&startTime);
	// Variable Declarations
	int flag=0;
	long j=0;
	int k=0;
	long long primes[2000];
	// Loop to check number and see if it is prime
	for (start; start<=finish; start++) {	 
	    j = 2;
    	flag = 0;
	    for(j ; j <=start / 2; j++) {
		    // n divisble by j, n is not prime
		    if(start % j == 0) {
    			flag = 1;
    			j = start; // break
		    }
	    }
	    if (flag==0) {
	        primes[k]=start;
	        k++;
	    }  
	}
	// Stop timer
	time(&currentTime);
	// Get Time difference in seconds
	endTime = difftime(currentTime, startTime);
	// Prompt to user thread number when and when it was completed
	printf("Thread %d done in %f seconds\n", threadCount, endTime);
	// Loop to display found prime numbers and if no prime numbers are found
	if(k>0)	{
	    printf("and found: ");
	    for (j=0; j<k; j++) {
	        printf("%ld\t", primes[j]);
	    }
	    printf("\n");    
	}
	else {
	    printf("and found no primes\n");
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------MAIN-----------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DESCRIPTION: creates threads in groups of 1, 2, 3, 4, and 8 then gives the group the task to find the prime numbers
// RETURNS: int
// CALLS: *ThreadTask()
int main(int argc, char *argv[]) {
	// Variable Declarations
	pthread_t threads[25];	
	int threadCount = 0;
	int average = 0;
	int status;
	int i=0;
	int activeThreads=0;
	time_t roundStart, roundEnd;
	double timeTook;
	int joinerror;			// value to hold the success value (0) or error value (anything other than 0)
	// loop to create a group process of 1, 2, 3, 4, and 8 threads that pass the increment step with it
	for (i=0; i<5; i++) {	// toggle i= for threads on current iteration
	    if (i==4) {
	        i=7;
	    }
	    activeThreads=i+1;
	    increment=(MAXRANGE-MINRANGE)/activeThreads;
	    printf("*****%d Threads*****\n",activeThreads);
	    time(&roundStart);
	    for(threadCount = 0; threadCount < activeThreads; threadCount++) {
		    // Start with one thread, process it and add another thread.
		    status = pthread_create(&threads[threadCount], NULL, (void *) ThreadTask, (void *)threadCount); 
		
		    if(status != 0) {
			    printf("Oops. pthread_create returned an error code %d\n", status);
			    exit(-1);
		    }
	    }
    	for(threadCount = 0; threadCount < activeThreads; threadCount++) {
    		joinerror = pthread_join(threads[threadCount], NULL);
    		if(joinerror != 0){
    			printf("Oops. pthread_join returned error code %d\n",status);
    			exit(-1);
    		}
    	}	
    	time(&roundEnd);
    	printf("***** COMPLETE IN %lf*****\n\n", difftime(roundEnd, roundStart));	
	}	
	return 0;
}

