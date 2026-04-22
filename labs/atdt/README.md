# OpenMP, MPI y MPI+OpenMP Samples

Este paquete contiene ejemplos básicos para Linux de:

- OpenMP
- MPI
- MPI + OpenMP (híbrido)

## Requisitos

- GCC con soporte OpenMP
- Una implementación de MPI, por ejemplo:
  - OpenMPI
  - MPICH

## Compilación

```bash
make
```

## Ejecución

### OpenMP
```bash
export OMP_NUM_THREADS=4
./hello_openmp
./sum_openmp
```

### MPI
```bash
mpirun -np 4 ./hello_mpi
mpirun -np 4 ./reduce_mpi
```

### Híbrido MPI + OpenMP
```bash
export OMP_NUM_THREADS=4
mpirun -np 2 ./hybrid_hello
mpirun -np 2 ./hybrid_sum
```

## Limpieza

```bash
make clean
```
