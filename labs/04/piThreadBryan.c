/*
*   Bryan Marquez
*   A01562119
*   Link de grafica:
*   https://drive.google.com/file/d/1BAWaelRbxSWJkOyKNGafxcitf8tdb5b1/view?usp=sharing
*   Conclusions: I expected the execution time to be 
*   less the more threads I gave it, but strangely the 
*   time was increasing, although the result of Pi is
*   very close, the threads/time ratio was not 
*   what I expected.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define starting_points 10000000
#define num_threads     8
int circle_count = 0;

void *monte_carlo_pi(void *param);
/* Double precision random number generator*/
double double_random(){
    return random()/((double)RAND_MAX +1);
}

int main (int argc, const char * argv[]){
	/*clock_t begin = clock();*/
    int thread_points = starting_points/num_threads;
    double pi;

    pthread_t processors[num_threads];

    /* Random number generator */
    srandom((unsigned)time(NULL));

    for(int i = 0; i < num_threads; i++)
        pthread_create(&processors[i], 0, monte_carlo_pi, &thread_points);

    for(int i = 0; i < num_threads; i++)
        pthread_join(processors[i], NULL);

    /* calculating pi*/
    pi = 4.0 * circle_count/starting_points;
	/*clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    */
    printf("Number of Points: %d\n", starting_points);
    printf("Calculated Pi: %f\n", pi);
	/*printf("-------------------------------------------------\n"
           "Time used with %d threads --> %.3f\n", num_threads, time_spent);
	*/
    return 0;
}

void *monte_carlo_pi(void *param)
{
    int points;
    points = *((int *)param);
    int hit_count = 0;
    double x, y;

    for(int i=0; i < points; i++){
        /* Monte Carlo Calculation*/
        /*Random numbers between -1.0 and +1.0 (x,y) point*/
        x = double_random() * 2.0 - 1.0;
        y = double_random() * 2.0 - 1.0;
        /* if (x,y) point is in the circle then */
        if(sqrt(x*x +y*y) < 1.0)
            ++hit_count; /* (x,y) point is IN the circle */
    }

    circle_count += hit_count;
    pthread_exit(0);
}