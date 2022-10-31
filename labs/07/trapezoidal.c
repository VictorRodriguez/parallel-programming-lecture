#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//function we are calculating the area under
double f(double x) {
    return sin(x);
}

//estimation of pi
const double pi = 3.141592653589793238462643383079;

int main(int argc, char** argv) {
    //Variables
    double a = 0.0, b = pi;         //limits of integration
    int n = 1048576;                //number of subdivisions = 2^20
    double h = (b - a) / n;         //width of each subdivision
    double integral;                // accumulates answer

    integral = (f(a) + f(b))/2.0;  //initial value

    //sum up all the trapezoids
    int i;
    for(i = 1; i < n; i++) {
        integral += f(a+i*h);
    }

    integral = integral * h;
    printf("With %d trapezoids, our estimate of the integral from \n", n);
    printf("%f to %f is %f\n", a,b,integral);

    return 0;
}
