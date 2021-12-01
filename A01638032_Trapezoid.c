/*
Programa que calcula el area bajo la curva usando paralelización con open mp
Marco Alexis López Cruz A01638032
1/12/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

//Limites
int a = 0;
int b = 5;

//Numero de divisiones
int n = 1000000;

//Aproximaciones
double aprox_p = 0.0;
double aprox_t = 0.0;

int main(){
    double h = (double)(b-a)/n; //Calcular h (Delta de x)
    aprox_t = (double) (sin(a) + sin(b)) / 2.0; //Calculación de la primera aproximacion

    #pragma omp parallel private(aprox_par) shared(a,b,h,n,aprox_total) //Area de paralelo
    {
        for (int i=1; i<n; i++){
            aprox_p += sin(a+ i*h);
        }
        #pragma omp critical //Creamos un thread 
        {
            aprox_t += aprox_p; //Agregamos cada thread de la suma parcial a la suma
            aprox_t = h*aprox_t; //Calculo total de la aproximacion
        }
    }
    printf("El resultado es: %lf \n",aprox_t);
    return 0;
}
