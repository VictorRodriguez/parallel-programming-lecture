//Ian De La Garza González      A01283525
//To compile "gcc piA01283525.c -o pi -pthread -lm"
//To run "./pi <number of threads>"
#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int NUM_THREADS = 1;

int TOTAL_DOTS = 10000000;
int DOTS_PER_THREAD =0;;
struct Coordinates
{
    double x;
    double y;
};
struct calculation
{
    int inside;
};

struct Coordinates getRandomPosition(){
    struct Coordinates c;
    c.x = (double)rand() / RAND_MAX;
    c.y = (double)rand() / RAND_MAX;
    return c;
}

void *calculatePi(void *arg)
{
    struct Coordinates coord;;
    struct calculation *t = (struct calculation *)arg;
    double inside=0;
    //printf("%d %ld\n", dots, tid);
    for (int i = 0; i<DOTS_PER_THREAD; i++)
    {
        coord=getRandomPosition();
        //printf("%f %f %ld\n", coord.x, coord.y, tid);
        if(sqrt(coord.x*coord.x+coord.y*coord.y)<=1){
            inside++;
        }
    }
    t->inside+=inside;
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    NUM_THREADS=atoi(argv[1]);
    srand( time(NULL));
    pthread_t threads[NUM_THREADS];
    int total = TOTAL_DOTS;
    DOTS_PER_THREAD = TOTAL_DOTS/NUM_THREADS;
    int rc;
    struct calculation t;
    clock_t start, end;
    start = clock();
    for (int i = 0; i < NUM_THREADS; i++)
    {
        t.inside=0;
        printf("In main: creating thread %d\n", i);
        rc = pthread_create(&threads[i], NULL, calculatePi, (void *)&t);
        if(rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    int *data;
    int inside=0;
    for (int t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], NULL);
    }
    end = clock();
    printf("PI = %f\nTime to execute: %f seconds.", (double)t.inside/TOTAL_DOTS*4, (double)(end-start)/CLOCKS_PER_SEC);
    
}