#include <omp.h>
#include <stdio.h>
#define N     100000

int main (){

    int i;
    float a[N], b[N], c[N];

    /* Some initializations */
    for (i=0; i < N; i++)
          a[i] = b[i] = i * 1.0;


    double start = omp_get_wtime();
            for (i=0; i < N; i++)
                    c[i] = a[i] + b[i];

    double end = omp_get_wtime();

    printf("start time = %f\n",start);
    printf("end time = %f\n",end);
    printf("diff time = %f\n",end - start);

	return 0;
}
