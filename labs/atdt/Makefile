CC=gcc
MPICC=/usr/lib64/openmpi/bin/mpicc
CFLAGS=-O2 -Wall -Wextra -std=c11
OMPFLAGS=-fopenmp

TARGETS=hello_openmp sum_openmp hello_mpi reduce_mpi hybrid_hello hybrid_sum \
bfs_compare_large

all: $(TARGETS)

hello_openmp: hello_openmp.c
	$(CC) $(CFLAGS) $(OMPFLAGS) -o $@ $<

sum_openmp: sum_openmp.c
	$(CC) $(CFLAGS) $(OMPFLAGS) -o $@ $<

hello_mpi: hello_mpi.c
	$(MPICC) $(CFLAGS) -o $@ $<

reduce_mpi: reduce_mpi.c
	$(MPICC) $(CFLAGS) -o $@ $<

hybrid_hello: hybrid_hello.c
	$(MPICC) $(CFLAGS) $(OMPFLAGS) -o $@ $<

hybrid_sum: hybrid_sum.c
	$(MPICC) $(CFLAGS) $(OMPFLAGS) -o $@ $<

bfs_compare_large:
	gcc -O2 -fopenmp bfs_compare_large.c -o bfs_compare_large

clean:
	rm -f $(TARGETS)

.PHONY: all clean
