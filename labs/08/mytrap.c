/*
Kevin Vega Rodriguez - A01612430
Implementacion basica del metodo del trapecio para aprocimar la integral de una funcion con 
el metodo de el calculo del area de trapecio utilizando programacion en paralelo
La aproximacion es de aproximadamente 1.97 con un step de 1000000000
*/

#include <stdio.h>
#include <omp.h>
#include <math.h>

int main(){ 
    int threads = 6;

    // Integration limits
    double izq = 1;
    double der = 10;
    int step = 1000000000;
    double delta = (der-izq)/step;


    double result = (3/izq + 9/der)/2; // Ejemplo de una funcion arbitraria

    // Utilizamos openmp para paralelizar el calculo
    #pragma omp parallel shared(result) num_threads(threads)
    {
        // Paralelizamos nuestro ciclo for
        #pragma omp for
        for(int i = 1; i < step; i++) {
            result += 5/(izq + i*delta);
        }
    }
    //Aproximamos el resultado final
    result = result * delta;


    printf("Aproximacion = %.4f \n", result);

    return 0;
}