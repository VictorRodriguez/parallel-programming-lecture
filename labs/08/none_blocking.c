/******************************************************************************
 * * FILE: mpi_helloNBsend.c
 * * DESCRIPTION:
 * *   MPI tutorial example code: Simple hello world program that uses
 * nonblocking
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
    MPI_Status stats[2];
    MPI_Request reqs[2];

    double t1,t2,deltat;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Get_processor_name(hostname, &len);
    printf ("Hello from task %d on %s!\n", taskid, hostname);

    t1 = MPI_Wtime();

    if (taskid == MASTER)
           printf("MASTER: Number of MPI tasks is: %d\n",numtasks);

    /* determine partner and then send/receive with partner */
    if (taskid < numtasks/2) 
          partner = numtasks/2 + taskid;
    else if (taskid >= numtasks/2) 
          partner = taskid - numtasks/2;

    MPI_Irecv(&message, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(&taskid, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &reqs[1]);

    /* now block until requests are complete */
    MPI_Waitall(2, reqs, stats);

    t2 = MPI_Wtime();
    deltat = t2-t1;

    /* print partner info and exit*/
    printf("Task %d is partner with %d\n",taskid,message);
    printf("Elapsed time %f \n ",deltat);
    MPI_Finalize();

}



