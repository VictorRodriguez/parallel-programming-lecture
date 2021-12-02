/*
Calculation of pi
//Marco Alexis López Cruz 
//A01638021
//14/10/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

//Default points and threads
#define POINTS 100000
#define num_Threads 2

//Variables
double x,y;     //coordinates
int num_points = 0; //number of points
double distance; //distance between two points
double pi; //final pi estimation
unsigned int Seed; //for random and time

//Thread and thread array
int thread_work = POINTS/num_Threads;
pthread_t Threads[num_Threads];

//function to calculate the value of pi
void *calculatePi(void* arg){

    for (int i = 0; i<thread_work; i++) {
        srand(time(NULL));
        //randomly calculate the coordinates
        x = (double)rand_r(&Seed) / (double)((unsigned)RAND_MAX + 1);
        y = (double)rand_r(&Seed) / (double)((unsigned)RAND_MAX + 1);
        distance = (x*x) + (y*y);

        if (distance <= 1){
           num_points++;
        }
    }
}

int main(){
    //Starting the clock and seed
    clock_t time_at_begin = clock();
    Seed = time(NULL);

    //Creating the threads
    for(int j=0; j<num_Threads; j++){
        pthread_create(&Threads[j],NULL,calculatePi,NULL);
    }

    //Join the threads
    for(int i=0; i<num_Threads; i++){
        pthread_join(Threads[i], NULL);
    }

    //Estimate pi
    pi = (double)num_points/POINTS*4;

    // Time the process
    clock_t time_at_end = clock();
    double time_spent = (double)(time_at_end-time_at_begin) / CLOCKS_PER_SEC; 

    // Print results
    printf("time = %f\n",time_spent);
    printf("No. of points = %d , estimate of pi is %g \n",POINTS,pi);

    return 0;
}
