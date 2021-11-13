/**
 * @file milkFactory_server.c
 * @author Giancarlo Franco Carrillo 
 * @brief A Milk factory that only sells 1000 bottles a day, implemented with semaphores.
 * @date 2021-11-13 
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_SIZE 1024 // 1K of shared memory segment

char sem_name[] = "Gian";
sem_t *semaphore;
key_t key = 1000;    /* Shared memory segment name */
int shmid;           /* Shared memory segment */
char *shm;           /* Virtual memory */
int *num_bottles;    /* Number of bottles */
int *client_command; /* Client command */

void create_semaphore();
void create_shared_memory();

int main(){

    printf("Hello World\n");
    /* Initialize the semaphore */
    create_semaphore();

    /* Create shared memory */
    create_shared_memory();

    /* Attach segment to virtual memory */
    shm = shmat(shmid, NULL, 0);

    num_bottles = shm; /* Write into memory */

    *num_bottles = 1000; /* Assign 1000 bottles to the factory */

    while (*shm != '*'){

        printf("%d bottles available\n", *num_bottles);
        sleep(1);
    }

    sem_close(semaphore);
    sem_unlink(sem_name);
    shmctl(shmid, IPC_RMID, 0);
    _exit(0);

    return 0;
}

// Create a semaphore object 
void create_semaphore(){

    semaphore = sem_open(sem_name, O_CREAT, 0644, 1); /* Open semaphore with Oflags */
    if (semaphore == SEM_FAILED){

        perror("Unable to create semaphore"); /* Print error value*/
        sem_unlink(sem_name);                 /* Remove named semaphore name */
        exit(-1);                             /* Finish with error */
    }
}

//Create a shared memory object
void create_shared_memory(){

    //create the shared memory segment with this key
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);

    if (shmid < 0){

        perror("failure in shmget"); /* Print error value */
        exit(-1);                    /* Finish with error */
    }
}
