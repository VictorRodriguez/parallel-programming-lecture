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
#include <limits.h>
#include <semaphore.h>
#include <time.h>

#define n_threads 16            /* Num of threads */
#define n_points 100000         /* Num of iterations */

int iter_per_thread = n_points / n_threads;

pthread_t tid[n_threads];       /* Threads IDs */
sem_t mutex;                    /* Sync bit */

int count = 0;                  /* Total points inside the circle */
double pi = 0;                  /* Variable for calculated pi vavlue */

unsigned int seed = 676767676 ; /* Seed for random number */


/*
 * Function:  generateRandom
 * --------------------
 * Generates a random decimal number between 0 and 1
 * 
 * Random number generator with linear congruential generator
 * 
 * Taken from “Numerical Recipes” by William H. Press, Chapter 7: Random Numbers
 *
 *  returns: random double
 */
double generateRandom()
{
    seed = seed * 1103515245 + 123456;
    return seed / (double)UINT_MAX; 
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
        double x = generateRandom();
        double y = generateRandom();

        double result = ( x * x ) + ( y * y );

        if (result <= 1)
        {
            sem_wait(&mutex);
            count++;
            sem_post(&mutex);
        }
    }
    return NULL;
}

int main()
{
    clock_t begin = clock();
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

    pi = ( (double)count / (double) n_points) * 4.0;

    printf("# of trials = %d, estimate of pi is %1.16f and an absolute error of %g\n", n_points, pi, fabs(pi - M_PI));
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed time: %4f seconds || Threads: %d threads\n", time_spent, n_threads);
    return 0;
}
