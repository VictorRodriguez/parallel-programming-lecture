/*H**********************************************************************
* FILENAME :        montecarlo.c             
*
* DESCRIPTION :
*       Multithreading montecarlo pi calculation 

* AUTHOR :    Oswaldo Hernandez        START DATE :    12 oct 21
*H*/

#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

#define n_threads 4     // Num of threads
#define n_points 100000  // Num of iterations

pthread_t tid[n_threads];
sem_t mutex;

int count = 0;
double pi;
int iter_per_thread = n_points / n_threads;


/*
 * Function:  generateRandom
 * --------------------
 * Generates a random decimal number between 0 and 1
 * 
 * Taken from https://stackoverflow.com/a/3067387
 *
 *  returns: random double
 */
double generateRandom()
{
    srand( time(NULL) );
    return (double) rand() / RAND_MAX; 
}

/*
 * Function:  calculatePi()
 * --------------------
 * Generates a random coordinate and if it is inside the circle it increments the counter
 * 
 *  returns: void
 */
void *calculatePi(void *arg) 
{    
    for (int i = 0; i < iter_per_thread; i++)
    {
        sem_wait(&mutex);
        double x = generateRandom();
        double y = generateRandom();

        double result = ( x * x ) + ( y * y );

        if (result <= 1)
        {
            sem_post(&mutex);
            count++;
        }
    }
    return NULL;
}

int main()
{

    sem_init(&mutex, 0, 1); /* Initialize semaphore */

    /* Create threads */
    for (int i = 0; i < n_threads; i++)
    {
        pthread_create(&tid[i], NULL, calculatePi, NULL);
    }

    /* Join threads */
    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pi= (double) count / n_points * 4;

    printf("# of trials = %d , estimate of pi is %1.16f AND an absolute error of %g\n", n_points, pi, fabs(pi - M_PI));
            
    return 0;
}
