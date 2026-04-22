#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int rank, size;
    const int N = 100;
    int local_n;
    int *data = NULL;
    int *local_data = NULL;
    int local_sum = 0;
    int global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) {
            fprintf(stderr, "Error: N=%d debe ser divisible entre el numero de procesos (%d)\n", N, size);
        }
        MPI_Finalize();
        return 1;
    }

    local_n = N / size;
    local_data = (int *)malloc((size_t)local_n * sizeof(int));
    if (!local_data) {
        fprintf(stderr, "Error al reservar memoria local\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        data = (int *)malloc((size_t)N * sizeof(int));
        if (!data) {
            fprintf(stderr, "Error al reservar memoria global\n");
            free(local_data);
            MPI_Finalize();
            return 1;
        }

        for (int i = 0; i < N; i++) {
            data[i] = 1;
        }
    }

    MPI_Scatter(data, local_n, MPI_INT,
                local_data, local_n, MPI_INT,
                0, MPI_COMM_WORLD);

    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < local_n; i++) {
        local_sum += (local_data[i] * 100)/(5);
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Suma global = %d\n", global_sum);
        free(data);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}
