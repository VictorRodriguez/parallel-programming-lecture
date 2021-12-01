#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    // Limits of the integral
    int a = 0;
    int b = 1;

    // Approximation value
    double aprox = 0;

    // Number of divisions
    int n = 100000;

    // Calculate h (delta x)
    double h = 0;
    h = (double)(b-a)/n;

    // Calculate aproximation
    for(int i = 0; i < n; i++){
        aprox += sin(a + i*h);
    }
    aprox = h*aprox;
    printf("Integral is: %1f \n",aprox);

    return 0;
}