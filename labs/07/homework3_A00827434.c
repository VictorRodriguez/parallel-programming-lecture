
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double square(double x); 
double valorActual;
void trapecio(double a, double b, int n, double* totalPointer);


int main(int argc, char* argv[]) {

	double total = 0; 
	double a, b; 
	int n, contador; 
	valorActual=0;

	printf("Escribir limite izquierdo, limite derecho, y numero de trapecios\n");
	scanf("%lf %lf %d", &a, &b, &n);
	# pragma omp parallel num_threads(contador)
	trapecio(a, b, n, &total);
	printf("Con n = %d trapezoides, área =~%14e \n", n, total);
	return 0;
}


double square(double x) {
	double funcion;
	funcion = x*x;
	return funcion;
} 

void trapecio(double a, double b, int n, double* totalPointer) {
	double h, x, resultado;
	double local_a, local_b;
	int i, local_n;
	int threadNum = omp_get_thread_num();
	int contador = omp_get_num_threads();
	h = (b-a)/n;
	local_n = n/contador;
	local_a = a + threadNum*local_n*h;
	local_b = local_a + local_n*h;
	resultado = (square(local_a) + square(local_b))/2.0;
	for (i = 1; i <= local_n-1; i++) {
		x = local_a + i*h;
		resultado += square(x);
	}
	resultado = resultado*h;
	valorActual += resultado;
	# pragma omp critical
	{
	*totalPointer += resultado;
	}
}
