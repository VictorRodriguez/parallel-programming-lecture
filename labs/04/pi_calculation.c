#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int numberOfThreads;
int pointsInThread;
long toltalPoints = 1000000;
long insidePoints = 0;

unsigned int seed;
pthread_mutex_t lock;

double getRandom(){
    double num;
    num = (double) rand_r(&seed) / ((unsigned) RAND_MAX + 1);
    return num;
}

void* monteCarlo(void* t_id){
    for (int j=0; j<pointsInThread; j++){
        double x = getRandom();
        double y = getRandom();
        if (sqrt(pow((x-0.5),2)+pow((y-0.5),2)) <= 0.5){
            pthread_mutex_lock(&lock);
            insidePoints++;
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(NULL);
}


int main(int argc, const char* argv[]){
    
    if (argc < 2){
        printf("\nSpecify number of threads\n");
        return 1;
    }
    if (argc != 2){
        printf("\nToo many input arguments\n");
        return 1;
    }

    clock_t t;
    t = clock();

    seed = time(NULL);

    numberOfThreads = atoi(argv[1]);
    pointsInThread = toltalPoints / numberOfThreads;

    pthread_t threads[numberOfThreads];

    double pi;

    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\nmutex init failed\n");
        return 1;
    }

    for (long i=0; i < numberOfThreads; i++){
        if(pthread_create(&threads[i], NULL, monteCarlo, (void*)i)){
            printf("\npthread create failed\n");
            return 1;
        }
    }

    for (int i=0; i < numberOfThreads; i++){
        pthread_join(threads[i], NULL);
    }

    pi = (4.0 * insidePoints) / toltalPoints;

    pthread_mutex_destroy(&lock);

    printf("\nPi = %lf\n", pi);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("\nTime taken: %lf\n", time_taken);

    pthread_exit(NULL);

    return 0;
}