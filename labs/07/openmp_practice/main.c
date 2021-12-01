/**
 * @file main.c
 * @author Oswaldo Hernandez 
 * @brief Calculates trapezoidal AUC using OPENMP
 * @version 0.1
 * @date 2021-12-01
 */
#include <stdio.h>
#include <omp.h>
#include <math.h>

#define f(x) 1/(1+pow(x,2))

int main(){

    /* Variables */
    float k;
    int i;
    int lower = 0;
    int upper = 1;
    int subInterval = 6;

    /* Calculation */

    /* Calculate step size */
    float stepSize = (float)(upper - lower) / subInterval;

    /* Finding integration Value */
    float integration = f(lower) + f(upper);

    int thread_count = 16; 

    int iterPerThread = subInterval / thread_count;

	# pragma omp parallel num_threads(thread_count) shared(integration, k) private(i)
	{
        #pragma omp for
            for (i = 1; i <= subInterval - 1; i++)
            {
                k = lower + i * stepSize;
                integration = integration + 2 * f(k);
            }
	}
    
    integration = integration * stepSize/2;
    printf("AUC approximation: %.5f\n", integration);
    return 0;
}
