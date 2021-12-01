/* 
   bsort.c - Parallel sorting algorithm based on bubblesort
   compile: mpicc -Wall -O -o bsort bsort.c
   run:     mpirun -np num_procs bsort in_file out_file
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


/* swap entries in array v at positions i and j; used by bubblesort */
static inline /* this improves performance; Exercise: by how much? */
void swap(int * v, int i, int j)
{
  int t = v[i];
  v[i] = v[j];
  v[j] = t;
}


/* (bubble) sort array v; array is of length n */
void bubblesort(int * v, int n)
{
  int i, j;
  for (i = n-2; i >= 0; i--)
    for (j = 0; j <= i; j++)
      if (v[j] > v[j+1])
        swap(v, j, j+1);
}


/* merge two sorted arrays v1, v2 of lengths n1, n2, respectively */
int * merge(int * v1, int n1, int * v2, int n2)
{
  int * result = (int *)malloc((n1 + n2) * sizeof(int));
  int i = 0;
  int j = 0;
  int k;
  for (k = 0; k < n1 + n2; k++) {
    if (i >= n1) {
      result[k] = v2[j];
      j++;
    }
    else if (j >= n2) {
      result[k] = v1[i];
      i++;
    }
    else if (v1[i] < v2[j]) { // indices in bounds as i < n1 && j < n2
      result[k] = v1[i];
      i++;
    }
    else { // v2[j] <= v1[i]
      result[k] = v2[j];
      j++;
    }
  }
  return result;
}


int main(int argc, char ** argv)
{
  int n;
  int * data = NULL;
  int c, s;
  int * chunk;
  int o;
  int * other;
  int step;
  int p, id;
  MPI_Status status;
  double elapsed_time;
  FILE * file = NULL;
  int i;

  if (argc!=3) {
    fprintf(stderr, "Usage: mpirun -np <num_procs> %s <in_file> <out_file>\n", argv[0]);
    exit(1);
  }

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  if (id == 0) {
    // read size of data
    file = fopen(argv[1], "r");
    fscanf(file, "%d", &n);
    // compute chunk size
    c = n/p; if (n%p) c++;
    // read data from file
    data = (int *)malloc(p*c * sizeof(int));
    for (i = 0; i < n; i++)
      fscanf(file, "%d", &(data[i]));
    fclose(file);
    // pad data with 0 -- doesn't matter
    for (i = n; i < p*c; i++)
      data[i] = 0;
  }

  // start the timer
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = - MPI_Wtime();

  // broadcast size
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // compute chunk size
  c = n/p; if (n%p) c++;

  // scatter data
  chunk = (int *)malloc(c * sizeof(int));
  MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);
  free(data);
  data = NULL;

  // compute size of own chunk and sort it
  s = (n >= c * (id+1)) ? c : n - c * id;
  bubblesort(chunk, s);

  // up to log_2 p merge steps
  for (step = 1; step < p; step = 2*step) {
    if (id % (2*step)!=0) {
      // id is no multiple of 2*step: send chunk to id-step and exit loop
      MPI_Send(chunk, s, MPI_INT, id-step, 0, MPI_COMM_WORLD);
      break;
    }
    // id is multiple of 2*step: merge in chunk from id+step (if it exists)
    if (id+step < p) {
      // compute size of chunk to be received
      o = (n >= c * (id+2*step)) ? c * step : n - c * (id+step);
      // receive other chunk
      other = (int *)malloc(o * sizeof(int));
      MPI_Recv(other, o, MPI_INT, id+step, 0, MPI_COMM_WORLD, &status);
      // merge and free memory
      data = merge(chunk, s, other, o);
      free(chunk);
      free(other);
      chunk = data;
      s = s + o;
    }
  }

  // stop the timer
  elapsed_time += MPI_Wtime();

  // write sorted data to out file and print out timer
  if (id == 0) {
    file = fopen(argv[2], "w");
    fprintf(file, "%d\n", s);   // assert (s == n)
    for (i = 0; i < s; i++)
      fprintf(file, "%d\n", chunk[i]);
    fclose(file);
    printf("Bubblesort %d ints on %d procs: %f secs\n", n, p, elapsed_time);
    // printf("%d %2d %f\n", n, p, elapsed_time);
  }

  MPI_Finalize();
  return 0;
}
