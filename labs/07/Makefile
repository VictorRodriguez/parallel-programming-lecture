GCC=gcc -fopenmp

all:
	$(GCC) hello_omp.c -o hello_omp
	$(GCC) hello_omp_private.c -o hello_omp_private
	$(GCC) omp_array.c -o omp_array
	$(GCC) omp_for.c -o omp_for
	$(GCC) omp_loop.c -o omp_loop

clean:
	rm hello_omp hello_omp_private omp_array omp_for omp_loop
