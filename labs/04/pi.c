/* This program computes pi (3.14)
using Monte Carlo algorithm and the implementation of Threads 
Jose Miguel Figarola Prado

https://docs.google.com/document/d/1hi8iKU3lW-wkWX8r4UpvseC2qjNW-LhhU3bnosipi2I/edit?usp=sharing

*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

// Point to be utilized
#define POINTS 1000000
// Threads to be used
#define nThreads 2

// Work to be done per thread
 int work_thread = POINTS/nThreads;
 pthread_t thread[nThreads];

double x,y; // coordinates
int count = 0; /* # of points in the 1st quadrant of unit circle */
double z; // distance between points
double pi;

unsigned int SEED; // rand and time

// Function to generate a random number
double GenerateRAND(){
    srand(time(NULL));
    double rnum = (double)rand_r(&SEED) / (double)((unsigned)RAND_MAX + 1);
    return rnum;

}

/* Function to compute the value of pi
given the random numbers of the coordinates */
void *ComputePI(void* arg){

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

    printf("Doing %d points per thread --> %d\n", work_thread,nThreads);

    // Create the threads requiered 
    for(int j=0; j<nThreads; j++){
        pthread_create(&thread[j],NULL,ComputePI,NULL);
    }

    // Join the threads
    for(int i=0; i<nThreads; i++){
        pthread_join(thread[i], NULL);
    }

    // Calculate pi
    pi = (double)count/POINTS*4;

    // Time the process
    clock_t end = clock();
    double time_spent = (double)(end-begin) / CLOCKS_PER_SEC; 

    // Print results
    printf("time = %f\n",time_spent);

    printf("# of points = %d , estimate of pi is %g \n",POINTS,pi);

    return 0;

}