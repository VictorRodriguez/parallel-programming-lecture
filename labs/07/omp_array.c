
/******************************************************************************
 * * FILE: omp_array.c
 * * DESCRIPTION:
 * *   Array addition - C/C++ Version
 * *    This is a simple array adition running with omp
 * * AUTHOR: Victor Rodriguez
 * * LAST REVISED: 04/06/05
 * ******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{

    int x = 0;
    long int sz2G=2000000000;       // c dynamic array
    long int * myarray_1 = malloc(sizeof(long int) *sz2G);
    long int * myarray_2 = malloc(sizeof(long int) *sz2G);
    long int * myarray_3 = malloc(sizeof(long int) *sz2G);

    #pragma omp parallel shared(myarray_1,myarray_2,myarray_3) private(x)
    {
        for ( x = 0; x < sz2G ; x++ ) {
            myarray_1[x]= myarray_2[x] +  myarray_3[x];
        }
    }

    free(myarray_1);
    free(myarray_2);
    free(myarray_3);
}


