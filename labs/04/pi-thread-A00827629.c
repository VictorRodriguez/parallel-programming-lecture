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
#include <math.h>

//Number of threads
const int num_threads = 10;
//Number of points we are going to randomly place
const long npoints = 450000000;
//Number of points inside the circle
long circle_point = 0;
//Point we are going to place 
int points_per_thread = npoints / num_threads;
//Max number 
double a = 1.0;

pthread_mutex_t lock;

//Counting points inside circle for pi calcultation thread function
void *calculatePi(void *arg){
    //Thread id
    long tid;
    tid = (long) arg;

    //Points placed
    int i = 0;

    //random coordinates
    double x;
    double y;
    /*
    //Random double number generation betwen 1 and 0
    x = (double)rand()/(double)(RAND_MAX/a);
    printf("Float random number is %lf\n",x);
    */
    //

    pthread_mutex_lock(&lock);
    for ( i=0; i < points_per_thread; i++){ 

        //Random coordinate generation
        x = (double)rand()/(double)(RAND_MAX/a);
        y = (double)rand()/(double)(RAND_MAX/a);
        //
        double comp = sqrt(((x-0.5)*(x-0.5))+((y-0.5)*(y-0.5)));

        if ( comp <= 0.5){
            circle_point++;
        }
        
    }
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    //Thread vector
    pthread_t threads[num_threads];

    //Error variable
    int rc;
    //Thread ID variable
    long tID;
    //Initialize random seed
    srand(time(NULL));

    //pi
    float pi = 0;

    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init failed\n");
            exit(1);
    }

    printf("Using %ld points\n", npoints);
    printf("Using %d threads\n", num_threads);
    printf("Placing %d points per thread\n", points_per_thread);
    
    for(tID=0;tID<num_threads;tID++){
        //printf("In main: creating thread %ld\n", tID);
        rc = pthread_create(&threads[tID], NULL, calculatePi, (void*)tID);
        if (rc){
            //printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    //Wait until every thread end

    for(tID = 0; tID < num_threads; tID++){
        pthread_join(threads[tID],  NULL);
    }

    pi = (4.00 * (float)circle_point) / (float)npoints;

    printf("\nPi = %f\n", pi);

    //End
    pthread_mutex_destroy(&lock);
    pthread_exit(NULL);
}
