/* H3 Bryan Marquez
* Purpose: Estimate definite integral (or area under curve)
* using the trapezoidal rule.
*
*
* Compile: gcc -fopenmp -o trapomp trapomp.c
* Usage: ./trapomp <number of threads>
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

void Trap(double a, double b, int n, double* global_result_p);
double f(double x);

int main(int argc, char* argv[]) {
    double global_result = 0.0;
    double a=0, b=15;
    int n=100000;
    int thread_count;

    thread_count = strtol(argv[1], NULL, 10);
#   pragma omp parallel num_threads(thread_count)
    Trap(a, b, n, &global_result);

    printf("Threads: %d.\n", thread_count);
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.10e.\n", a, b, global_result);
    return 0;
}

void Trap(double a, double b, int n, double* global_result_p) {
    double h, x, my_result;
    double local_a, local_b;
    int i, local_n;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    h = (b-a)/n;  // The height of the trapezoid.
    local_n = n/thread_count;
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    my_result = (f(local_a) + f(local_b))/2.0;
    for ( i = 1; i <= local_n-1; i++)
    {
        x = sin(local_a+ i*h);
        my_result += f(x);
    }
    my_result = my_result*h;

#   pragma omp critical
    *global_result_p += my_result;
    
}

double f(double x) {
    double return_val;
    return_val = x*x;
    return return_val;
}
