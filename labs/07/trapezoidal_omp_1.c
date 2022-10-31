#include <math.h>
#include <stdio.h> // printf()
#include <stdlib.h> // atoi()
#include <omp.h> // OpenMP

/* Demo program for OpenMP:
computes trapezoidal approximation to an integral*/

const double pi = 3.141592653589793238462643383079;

int main(int argc, char** argv) {
    /* Variables */
    double a = 0.0, b = pi; /* limits of integration */;
    int n = 1048576; /* number of subdivisions = 2^20 */
    double h = (b - a) / n; /* width of subdivision */
    double integral; /* accumulates answer */
    int threadcnt = 1;

    double f(double x);

    /* parse command-line arg for number of threads */
    if (argc > 1) {
        threadcnt = atoi(argv[1]);
    }

    #ifdef _OPENMP
        omp_set_num_threads( threadcnt );
        printf("OMP defined, threadct = %d\n", threadcnt);
    #else
        printf("OMP not defined");
    #endif

    integral = (f(a) + f(b))/2.0;
    int i;

    #pragma omp parallel for private(i) shared (a, n, h, integral)
    for(i = 1; i < n; i++) {
        integral += f(a+i*h);
    }

    integral = integral * h;
    printf("With %d trapezoids, our estimate of the integral from \n", n);
    printf("%f to %f is %f\n", a,b,integral);
}

double f(double x) {
    return sin(x);
}
