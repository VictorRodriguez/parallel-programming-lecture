/*************************************************************
 * Luis Humberto Sánchez Vaca 
 * A01638029
 * Code to calculate pi using Monte Carlo's method
 * Graph of time results: 
 * https://docs.google.com/document/d/1Pq0xLKnLjPou9h_hYO7O67_w8DA2Ivw_K4J3FQpSkgc/edit?usp=sharing
 ************************************************************/

// Necessary librarys
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

// Variables for the calculation
int numberOfThreads;
int pointsInThread;
long toltalPoints = 1000000;
long insidePoints = 0;

// Variable for random values
unsigned int seed;

// Mutex
pthread_mutex_t lock;

// Function for getting random values
double getRandom(){
    double num;
    num = (double) rand_r(&seed) / ((unsigned) RAND_MAX + 1);
    return num;
}

// Function for calculating pi
void* monteCarlo(void* t_id){
    // For each point
    for (int j=0; j<pointsInThread; j++){
        // Get random coordinates
        double x = getRandom();
        double y = getRandom();
        // Check if is inside the circle
        if (sqrt(pow((x-0.5),2)+pow((y-0.5),2)) <= 0.5){
            // If so, add it to the result
            pthread_mutex_lock(&lock);
            insidePoints++;
            pthread_mutex_unlock(&lock);
        }
    }
    // Finish the thread
    pthread_exit(NULL);
}

// Main function
int main(int argc, const char* argv[]){
    // Check that you have the correct number of arguments
    if (argc < 2){
        printf("\nSpecify number of threads\n");
        return 1;
    }
    if (argc != 2){
        printf("\nToo many input arguments\n");
        return 1;
    }

    // Check the time at the start of the algorithm
    clock_t t;
    t = clock();

    // Seed for random values
    seed = time(NULL);

    // Get the number of threads from the arguments
    numberOfThreads = atoi(argv[1]);
    // Calculate the number of points per thread
    pointsInThread = toltalPoints / numberOfThreads;

    // Array with the number of threads requierd
    pthread_t threads[numberOfThreads];

    // Variable for the reult of pi
    double pi;

    // Creation of the mutex and cheking that it was correctly done
    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\nmutex init failed\n");
        return 1;
    }

    // Call the monteCarlo function for each thread
    for (long i=0; i < numberOfThreads; i++){
        // Creation of the thread and checking that it was correctly done
        if(pthread_create(&threads[i], NULL, monteCarlo, (void*)i)){
            printf("\npthread create failed\n");
            return 1;
        }
    }

    // Chek that the process of the threads has finished
    for (int i=0; i < numberOfThreads; i++){
        pthread_join(threads[i], NULL);
    }

    // Calculate the value of pi
    pi = (4.0 * insidePoints) / toltalPoints;

    // Finish the mutex
    pthread_mutex_destroy(&lock);

    // Print the value of pi
    printf("\nPi = %lf\n", pi);

    // Check the final time
    t = clock() - t;
    // Calculate the time taken to finish the algorithm
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    // Print the that time
    printf("\nTime taken: %lf\n", time_taken);

    // Finish the pthread
    pthread_exit(NULL);

    return 0;
}