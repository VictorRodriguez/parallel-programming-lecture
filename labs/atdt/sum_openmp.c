#include <stdio.h>
#include <omp.h>

int main(void) {
    const int N = 100;
    int sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= N; i++) {
        sum += i;
    }

    printf("Suma de 1 a %d = %d\n", N, sum);
    return 0;
}
