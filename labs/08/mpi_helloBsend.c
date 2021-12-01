/******************************************************************************
 * * FILE: mpi_helloBsend.c
 * * DESCRIPTION:
 * *   MPI tutorial example code: Simple hello world program that uses blocking
 * *   send/receive routines.
 * * AUTHOR: Blaise Barney
 * * LAST REVISED: 07/09/12
 * ******************************************************************************/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  MASTER     0

int main (int argc, char *argv[])
{
    int  numtasks, taskid, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int  partner, message;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Get_processor_name(hostname, &len);
    printf ("Hello from task %d on %s!\n", taskid, hostname);
    if (taskid == MASTER)
           printf("MASTER: Number of MPI tasks is: %d\n",numtasks);

    if (numtasks < 2){ 

        printf("Numtask must be grather than 2 \n");
        return -1; 
    }

    int number;
    if (taskid == 0) {
    // If we are rank 0, set the number to -1 and send it to process 1
     number = -1;
     MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
     } else if (taskid == 1) {
         MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         printf("Process 1 received number %d from process 0\n", number);
     }
     MPI_Finalize();

}


