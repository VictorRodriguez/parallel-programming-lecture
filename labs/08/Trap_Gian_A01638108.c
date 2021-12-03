
/**
 * @file Trap_Gian_A01638108.c
 * @author Giancarlo Franco Carrillo
 * @brief Trapezoidal aproximation for integral calculation using omp library.
 * @date 2021-12-3
 */

#include <stdio.h>
#include <omp.h>
#include <math.h>

// Function to compute integral
#define f(x) sin(x)

int main(){ 

    // main variables
    float k;
    int i;
    int lower = 0;
    int upper = 5;
    int interval = 10;


    // step size or delta x
    float delta = (float)(upper - lower) / interval;

    // Compute integration values
    float integration = f(lower) + f(upper);

    // Number of threas EXPORT NUM_OF_THREADS=x
    int thread_count = 12; 

	# pragma omp parallel num_threads(thread_count) shared(integration, k) private(i)
	{
        #pragma omp for
            for (i = 1; i <= interval - 1; i++)
            {
                k = lower + i * delta;
                integration = integration + 2 * f(k);
            }
	}

    integration = integration * delta/2;
    printf("Integral pproximation: %.5f\n", integration);
    return 0;
}