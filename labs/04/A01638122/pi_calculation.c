// Owen Jáuregui Borbón
// A01638122
// Code to aproximate the value of Pi
// Report: 
// https://docs.google.com/document/d/1UhGYKqAK5UYAwrDvu6jLoQVTe5fRgckY6e-VOV4EELw/edit?usp=sharing

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int             numThreads;
int             threadPoints;
long            pointsInside = 0;
long            totalPoints  = 1000000;
unsigned int    seed;
pthread_mutex_t lock;
clock_t         start, end;

double randPos() {
    return (double)rand_r(&seed) / (double)((unsigned)RAND_MAX+1);
}

void* monteCarlo_pi (void* t_id) {
    double x, y;

    for(int i = 0; i < threadPoints; i++) {
        x = randPos();
        y = randPos();
        if (sqrt(pow((x-0.5),2)+pow((y-0.5),2)) <= 0.5) {
            pthread_mutex_lock(&lock);
            pointsInside++;
            pthread_mutex_unlock(&lock);
        }
    }    
    pthread_exit(NULL);
}

int main(int argc, const char* argv[]) {
    seed = time(NULL);
    
    numThreads   = atoi(argv[1]);
    threadPoints = totalPoints/numThreads;

    pthread_t threads[numThreads];
    double    pi, cpu_time;
    int rc;

    if (pthread_mutex_init(&lock, NULL) != 0) {
        exit(1);
    }

    start = clock();

    for(long i = 0; i < numThreads; i++) {
        rc = pthread_create(&threads[i], NULL, monteCarlo_pi, (void*)i);
        if(rc) {
            exit(-1);
        }
    }

    for(long i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    end = clock();

    pi  = (4.00 * (float) pointsInside) / (float) totalPoints;
    cpu_time = ((double) (end-start)) / CLOCKS_PER_SEC;

    pthread_mutex_destroy(&lock);

    printf("%lf \n", pi);
    printf("Time: %lf seconds \n", cpu_time);

    pthread_exit(NULL);

    return 0;
}
