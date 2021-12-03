
#Uncomment your choice of compiler
CC    =   mpicc
#CC    =   mpigcc
#CC    =   mpipgcc

#Add your choice of flags
FLAGS   =   -O1 -w

all:    hello       \
        mpi_helloBsend \
		mpi_ringtopo \
		blocking \
		none_blocking

clean:
	find . -executable -type f -exec rm -rf {} \;
	find . -iname "*~" -exec rm -rf {} \;

hello:  hello.c
	$(CC) $(FLAGS) hello.c -o hello

helloBsend:  mpi_helloBsend.c
	$(CC) $(FLAGS) mpi_helloBsend.c -o mpi_helloBsend

mpi_bandwidth: mpi_ringtopo.c
	$(CC) $(FLAGS) mpi_ringtopo.c -o mpi_ringtopo

mpi_omp:  mpi_omp.c
	$(CC) $(FLAGS) -fopenmp mpi_omp.c -o  mpi_omp

mpi_omp_matrix:  mpi_omp_matrix.c
	$(CC) $(FLAGS)  -fopenmp  mpi_omp_matrix.c -o  mpi_omp_matrix

mpi_omp_buble:  mpi_omp_buble.c
	$(CC) $(FLAGS) -fopenmp mpi_omp_buble.c -o  mpi_omp_buble

blocking:  blocking.c
	$(CC) $(FLAGS) blocking.c -o  blocking

none_blocking:  none_blocking.c
	$(CC) $(FLAGS) none_blocking.c -o  none_blocking
