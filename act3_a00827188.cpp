#include <stdio.h>
#include <omp.h>

int main(){
    int threads = 16;

    double left = 1;
    double right = 10;
    int step = 10000000;
    double delta = (right-left)/step;


    double cal = (10/left + 10/right)/2; 

    #pragma omp parallel shared(result) num_threads(threads)
    {
        #pragma omp for
        for(int i = 1; i < step; i++) {
            cal += 5/(left + i*delta);
        }
    }

    cal = cal * delta;


    printf("Resultado de integracion: %.2f \n", cal);

    return 0;
} 