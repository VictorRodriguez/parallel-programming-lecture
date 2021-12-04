//Ian De La Garza González      A01283525
//Este programa calcula el integral de la funcion de f(x)=3*sin(2x)+4
//De 1 a 4
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>

double function(double x) {
  return 3*sin(2*x)+4;
}

int main(int argc, char* argv[]){


    //limites
    double l = atof(argv[1]), r = atof(argv[2]);
    //pasos
    int nsteps = 1000000;

    double step = (r-l)/nsteps;
    double partial_Sum = 0, total_Sum = 0;

    #pragma omp parallel private(partial_Sum) shared(l, r, h, nsteps, step, total_Sum) 
    {
        
        #pragma omp for
            for(int i = 1; i <= nsteps; i++){
                partial_Sum += function(l + i*step);
            }

        //Create thread safe region.
        #pragma omp critical
        {
                total_Sum += partial_Sum;
                total_Sum = step*total_Sum;
        }
    }
    printf("Total Sum: %f\n", total_Sum);
    return 0;
}
