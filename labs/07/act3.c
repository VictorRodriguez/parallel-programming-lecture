/*
    Owen Jauregui Borbon - A01638122

    Program for calculating theapproximate area under 
    the curve for a function using the trapezoidal
    rule. Parallelized using openMP.
*/

#include <stdio.h>
#include <omp.h>

int main() {
    // Number of threads for openMP
    int    threads    = 8;

    // Integration limits
    double left       = 1;
    double right      = 20;

    // Amount and size of steps for the approximation
    double stepAmount = 1000000;
    double diff       = (right-left)/stepAmount;

    // Starting value for the approximation 
    // using 5/x as example function
    double result     = (5/left + 5/right)/2;

    // Use of openMP for parallelizing operations
    #pragma omp parallel shared(result) num_threads(threads)
    {
        // Parallelizing iterations
        #pragma omp for
        for(int i = 1; i < stepAmount; i++) {
            // Adding Y value for each step to te approximation
            result += 5/(left + i*diff);
        }
    }

    // Multiply the approximation and the step size for final result
    result *= diff;

    // Display result
    printf("Approximate result: %.4f \n", result);

    return 0;
}