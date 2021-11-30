/*
    AUTOR: Juan Carlos Triana Vela
    Program: A00827620_trapezoid_omp
        This program achieves the calculation of the area under the curve of some function
        (in this case sin(x)), using paralelization by open mp
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


int main(){
    //Define limits
    int a = 0;
    int b = 5;
    //Aproximation
    double aprox_par = 0.0;
    double aprox_total = 0.0;
    //Define number of division
    int n = 1000000;
    //Calculate h (delta of x)
    double h;
    h = (double)(b-a)/n;
    //Calculate first aproximation
    aprox_total = (double) (sin(a) + sin(b)) / 2.0;

    //Parallel area
    #pragma omp parallel private(aprox_par) shared(a,b,h,n,aprox_total)
    {

        for (int i=1; i<n; i++){
            aprox_par += sin(a+ i*h);
        }
        //Create thread safe region.
        #pragma omp critical
        {
            //add each threads partial sum to the total sum
            aprox_total += aprox_par;
            //Calculate the total aproximation
            aprox_total = h*aprox_total;
        }
    

    }
    printf("Integral is: %lf \n",aprox_total);
    return 0;
}
