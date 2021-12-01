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
    int upper = 1;
    int interval = 6;


    // step size or delta x
    float delta = (float)(upper - lower) / interval;

    // Compute integration values
    float integration = f(lower) + f(upper);

    // Number of threas EXPORT NUM_OF_THREADS=x
    int thread_count = 8; 

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