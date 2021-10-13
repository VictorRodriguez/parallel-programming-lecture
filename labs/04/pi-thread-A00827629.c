/******************************************************************************
* * FILE: pi-thread-A00827629.c
* * DESCRIPTION:
* *   This program estimates pi by using multithreading by random number method
**    (also called Montecarlo Method) 
* * AUTHOR: Juan Carlos Triana Vela
* * LAST REVISED: 11/10/2021
* ******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS    5

//Number of points we are going to randomly allocate
int npoints = 1000000;
//Number of points inside the circle
int circle_point = 0;

/*
void *PrintHello(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
	global_variable++;
	printf("Global variable %d\n",global_variable);

    pthread_exit(NULL);
}
*/

void *calculatePi(void* arg){

    while (circle_point < npoints){
        circle_point++ ;
    }
    if (circle_point >= npoints){
        //printf("Thread end\n");
        pthread_exit(NULL);
    }
}

int main(int argc, char *argv[])
{
    //Thread vector
    pthread_t threads[NUM_THREADS];

    //Error variable
    int rc;
    //Thread ID variable
    long tID;
    //Initialize random seed
    srand(time(NULL));

    //random number
    double x;
    double a = 1.0;

    /*
    //Random double number generation betwen 1 and 0
    x = (double)rand()/(double)(RAND_MAX/a);
    printf("Float random number is %lf\n",x);
    */

    for(tID=0;tID<NUM_THREADS;tID++){
        //printf("In main: creating thread %ld\n", tID);
        rc = pthread_create(&threads[tID], NULL, calculatePi, (void*)tID);
        if (rc){
            //printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    while (circle_point < npoints){

    }
    printf("Finished program at %d iterations\n", circle_point);

    /* Last thing that main() should do */
    pthread_exit(NULL);
}
