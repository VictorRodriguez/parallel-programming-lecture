#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Hello();

int main() 
{
    int thread_count = 12;


    # pragma omp parallel num_threads(thread_count)
    {
    Hello();
    }
    
    return 0;
}

void Hello()
{
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    printf("Hello from thread %d of %d\n", my_rank, thread_count);
}
