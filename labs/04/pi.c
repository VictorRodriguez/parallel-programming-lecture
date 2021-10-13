/* This program compues pi
using Monte Carlo algorithm and the implementation of
Threads */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define POINTS 10000000
// Threads to be used
#define nThreads 4 

// Work to be done per thread
int work_thread = POINTS/nThreads;

double x,y; // coordinates
int count = 0; /* # of points in the 1st quadrant of unit circle */
double z; // distance between points
double pi;

pthread_t thread[nThreads];

unsigned int SEED;

// Function to generate a random number
double GenerateRAND(){
    srand(time(NULL));
    return (double) rand()/RAND_MAX;
}

/* Function to compute the value of pi
given the random numbre of the coordinates */
void *ComputePI( void* arg){

    for (int i = 0; i<work_thread; i++) {
        x = GenerateRAND();
        y = GenerateRAND();
        z = (x*x) + (y*y);

        if (z <= 1){
           count++;
        }
    }
}

// int argc, char* argv
int main(){

    // Start the clock
    clock_t begin = clock();
    SEED = time(NULL);

    printf("Doing %d points per thread\n", work_thread);

    // Create the threads requiered 
    for(int j=0; j<nThreads; j++){
        pthread_create(&thread[j],NULL,ComputePI,NULL);
    }

    // Join the threads
    for(int i=0; i<nThreads; i++){
        pthread_join(thread[i], NULL);
    }

    pi = (double)count/POINTS*4;

    // Time the process
    clock_t end = clock();
    double time_spent = (double)(end-begin) / CLOCKS_PER_SEC; 

    // Print results
    printf("time = %f\n",time_spent);

    printf("# of points = %d , estimate of pi is %g \n",POINTS,pi);

    return 0;

}