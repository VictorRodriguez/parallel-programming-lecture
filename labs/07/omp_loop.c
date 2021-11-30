#include <omp.h>
#include <stdio.h>
#define N     100000

int main () {

    int i,tid,NTHR;
    float a[N], b[N], c[N];

    /* Some initializations */
    for (i=0; i < N; i++)
          a[i] = b[i] = i * 1.0;

    double start = omp_get_wtime();

    //omp_set_dynamic(0);
    //omp_set_num_threads(4);

    #pragma omp parallel shared(a,b,c) private(i)
      {
            #pragma omp for schedule(static)
            for (i=0; i < N; i++)
                    c[i] = a[i] + b[i];

              }  /* end of parallel section */



    double end = omp_get_wtime();

    printf("start time = %f\n",start);
    printf("end time = %f\n",end);
    printf("diff time = %f\n",end - start);
	return 0;
}
