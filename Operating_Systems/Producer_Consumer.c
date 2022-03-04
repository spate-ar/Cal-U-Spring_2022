// Andrew Bissell, Andrew Spate, Nicholas Spudich
// Group 2
// Operating Systems CSC-400
//---------------------------------------------------------------INCLUDE/DEFINE------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#define NUMBEROFTHREADS 8
#define MAXRANGE 1923457200
#define MINRANGE 1923456000

pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
long int buffer[2] = {0};
bool Flag = false;

void ProcessPrimes(long start)
{
	long j = 0;
	bool flag = false;
	
	for(j = 2; j <= (start / 2); j++) 
	{
		// n divisble by j, n is not prime
		if((start % j) == 0) 
		{
			
    		flag = true;
    		j = start; // break
		}
	}
	if (flag == false) 
	{
	    printf("%ld\n", start);
	}  
}	

void * Producer(void *ptr)
{
	int i;
	for(i = MINRANGE; i <= MAXRANGE; i++) 
	{
		pthread_mutex_lock(&the_mutex);								// get exclusive access to buffer
		//printf("\nproducer start %d , buffer[0] = %ld , buffer[1] = %ld", i, buffer[0], buffer[1]);
		
		while(buffer[0] != 0 && buffer[1] != 0)
			pthread_cond_wait(&condp, &the_mutex);
			
		if(buffer[0] == 0)
		{
			buffer[0] = i;
		}	
		else
		{
			buffer[1] = i;
		}
		pthread_cond_signal(&condc);								// wake up consumer
		//printf("\nproducer end   %d , buffer[0] = %ld , buffer[1] = %ld\n" , i, buffer[0], buffer[1]);
		pthread_mutex_unlock(&the_mutex);							// release access to buffer
	}
	pthread_exit(0);
}

void * Consumer(void *ptr)
{
	int i, l = 0;
	long temp = 0;
	for(i = MINRANGE; i <= MAXRANGE; i++) 
	{
		pthread_mutex_lock(&the_mutex);								// get exclusive access to buffer
		//printf("\nconsumer start %d ,buffer[0] = %ld , buffer[1] = %ld" , i, buffer[0], buffer[1]);
		while((buffer[0] == 0) && (buffer[1] == 0) && Flag == false)
			pthread_cond_wait(&condc, &the_mutex);
		
		if(buffer[1] != 0)
		{
			temp = buffer[1];
			buffer[1] = 0;
		}
		else
		{
			temp = buffer[0];
			buffer[0] = 0;
		}	
		if(buffer[1] == 0 && buffer[2] == 0)
			Flag = true;
		pthread_cond_signal(&condp);								// wake up producer
		//printf("\nconsumer end   %d , buffer[0] = %ld , buffer[1] = %ld\n", i, buffer[0], buffer[1]);
		pthread_mutex_unlock(&the_mutex);							// release access to buffer
		if(temp != 0)	
			ProcessPrimes(temp);			
	}
	pthread_exit(0);	
}


// DESCRIPTION: creates threads in groups of 1, 2, 3, 4, and 8 then gives the group the task to find the prime numbers
// RETURNS: int
// CALLS: *ThreadTask()
int main(int argc, char *argv[]) 
{
	pthread_t threads[25];
	
	pthread_mutex_init(&the_mutex, 0);
	pthread_cond_init(&condc, 0);
	pthread_cond_init(&condp, 0);
	
	time_t roundStart, roundEnd;
	int threadCount = 0, average = 0, status, i = 0, activeThreads = 0, joinerror; // value to hold the success value (0) or error value (anything other than 0)
	double timeTook;
	
	// loop to create a group process of 1, 2, 3, 4, and 8 threads that pass the increment step with it
	for (i = 2; i <= 5; i++) 
	{	
		// toggle i= for threads on current iteration
	    if (i == 5) 
	    	i = 8;	
	    activeThreads = i;
	
	    printf("     *****%d Threads*****\n\n", activeThreads);
	    //system("pause");
	    printf("Primes Found\n");
	    printf("----------------------------- \n\n");
		
		time(&roundStart);
	    
	   	// Create Producer Thread
		status = pthread_create(&threads[0], 0, * Producer, 0);
		if(status != 0) 
		{
		    printf("Oops. pthread_create returned an error code %d\n", status);
		    exit(-1);
		}
		
	    for(threadCount = 1; threadCount <= activeThreads; threadCount++) 
		{
		    // Start with one thread, process it and add another thread.
		    status = pthread_create(&threads[threadCount], 0, * Consumer, 0); 
		    if(status != 0) 
			{
			    printf("Oops. pthread_create returned an error code %d\n", status);
			    exit(-1);
		    }
	    }
	    
    	for(threadCount = 0; threadCount <= activeThreads; threadCount++) 
		{
    		joinerror = pthread_join(threads[threadCount], NULL);		
    		if(joinerror != 0)
			{
    			printf("Oops. pthread_join returned error code %d\n",status);
    			exit(-1);
    		}
    	}	
    	time(&roundEnd);
		printf("***** COMPLETE IN %lf*****\n\n", difftime(roundEnd, roundStart));
		system("pause");
		Flag = false;
	}	
	pthread_cond_destroy(&condc);
	pthread_cond_destroy(&condp);
	pthread_mutex_destroy(&the_mutex);	
	return 0;
}
