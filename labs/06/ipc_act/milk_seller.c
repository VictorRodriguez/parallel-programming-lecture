/**
 * @file milk_seller.c
 * @author Oswaldo Hernandez
 * @brief Client from milk factory, it requests 0 <= x <= 500 number of bottles
 * @version 0.1
 * @date 2021-11-13
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SHM_SIZE 1024 /* 1K shared memory segment */

char sem_name[] = "sem";
sem_t *semaphore;
key_t key = 1000; /* Shared memory segment name */
int shmid;        /* Shared memory segment */
char *shm;        /* Virtual memory */
int *num_bottles; /* Number of bottles */

void create_shared_memory();
void create_semaphore();

/**
 * @brief main function
 * 
 * @return int 
 */
int main()
{
    printf("Hello world\n");
    create_semaphore();
    create_shared_memory();

    /* Attach segment to virtual memory */
    shm = shmat(shmid, NULL, 0);

    num_bottles = shm;

    int bottle_request;

    while (*num_bottles <= 1000 && *num_bottles > 0)
    {
        bottle_request = rand() % 500;
        sem_wait(semaphore);
        if (*num_bottles - bottle_request > 0)
        {
            *num_bottles = *num_bottles - bottle_request;
            printf("Selling %d bottles\n", bottle_request);
        }
        else
        {
            printf("%d bottles are not available\n", bottle_request);
        }
        sem_post(semaphore);
        sleep(5);
    }

    *shm = '*';
    sem_close(semaphore);
    shmctl(shmid, IPC_RMID, 0);
    exit(0);

    return 0;
}

/**
 * @brief Create a semaphore object
 * 
 */
void create_semaphore()
{
    semaphore = sem_open(sem_name, O_CREAT, 0644, 1); /* Open semaphore with Oflags */
    if (semaphore == SEM_FAILED)
    {
        perror("Unable to create semaphore"); /* Print error value*/
        sem_unlink(sem_name);                 /* Remove named semaphore name */
        exit(-1);                             /* Finish with error */
    }
}

/**
 * @brief Create a shared memory object
 * 
 */
void create_shared_memory()
{
    //create the shared memory segment with this key
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("failure in shmget"); /* Print error value */
        exit(-1);                    /* Finish with error */
    }
}