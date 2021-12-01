/****************************************************
 * Luis Humberto Sanchez Vaca
 * A01638029
 * 
 * Program to calcule area under the curve
 * by with trapezoidal rule using OpenMP
 * Ex. sin(x) from 0 to 1
 * 
 * To compile:
 * gcc -omp main.c -o main -lm
 **************************************************/

// Necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


int main(){
    // Limits of the function
    double min = 0;
    double max = 1;

    // Number of intervals
    int steps = 1000000;

    // Interval size
    double delta;

    // Variables for the result
    double partial_result;
    double total_result;
    
    // Calculate of delta
    delta = (max-min) / steps;

    // Initial aproximation
    partial_result = (sin(min) + sin(max)) / 2.0;

    // Parallelize the calculations
    #pragma omp parallel private(partial_result) shared(total_result)
    {
        #pragma omp for
        for (int i=1; i<steps; i++){
            // Partial results for each trapezoid
            partial_result += sin(min + i * delta);
        }
        // Create a thread
        #pragma omp critical
        {
            // Sum the thread to the final result   
            total_result += partial_result;

            // Final calulation
            total_result *= delta;
        }
    }

    // Display the result
    printf("Result is: %lf\n", total_result);

    return 0;
}