#include <stdio.h>
#include <omp.h>

int main(){
    int threads = 16;

    // Integration limits
    double izquierda;
    double derecha;
    printf("limite izquierdo: ");
    scanf("%lf",&izquierda);
    printf("limite derecho: ");
    scanf("%lf",&derecha);
    int step = 10000000;
    double delta = (derecha-izquierda)/step;


    double integracion = (10/izquierda + 10/derecha)/2; 

    // Utilizamos openmp para paralelizar el calculo
    #pragma omp parallel shared(result) num_threads(threads)
    {
        // Paralelizamos nuestro ciclo for
        #pragma omp for
        for(int i = 1; i < step; i++) {
            integracion += 5/(izquierda + i*delta);
        }
    }
    //Aproximamos el resultado final
    integracion = integracion * delta;


    printf("Resultado de integracion: %.2f \n", integracion);

    return 0;
}