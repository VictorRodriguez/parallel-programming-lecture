/*********************************************
 * Luis Humberto Sanchez Vaca
 * A01638029
 * 
 * Program for the factory to initialize
 * the share memory space
 * 
 * To compile:
 * gcc factory.c -o factory -lpthread -lm
 ********************************************/

// Necessary libraries
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

//Define shared memory syze
#define SHMSZ 100
//Define semaphore name
char SEM_NAME[]= "BOTTLES";


int main(){
    // Variables for share memory
    int shmid;
    key_t key;
    int *shm, *bottles;
    sem_t *semaphore;

    // Variable to exit the program
    int out=1;

    //name the shared memory segment
    key = 1000;

    //create & initialize semaphore
    semaphore = sem_open(SEM_NAME,O_CREAT,0644,1);
    if(semaphore == SEM_FAILED){
        perror("unable to create semaphore");
        sem_unlink(SEM_NAME);
        exit(-1);
    }

    //create the shared memory segment with this key
    shmid = shmget(key,SHMSZ,IPC_CREAT|0666);
    if(shmid<0){
        perror("failure in shmget");
        exit(-1);
    }

    //attach this segment to virtual memory
    shm = shmat(shmid,NULL,0);
    
    //start writing into memory
    bottles = shm;

    // Assign the initial number of bottles
    *bottles = 1000;

    // Run the program until 1 is presed
    do{
        sem_wait(semaphore);
        
        printf("Factory correctly initialized\n");
        printf("There are %d bottles of milk\n", *bottles);
        
        sem_post(semaphore);

        printf("Press 0 to exit\n");
        scanf("%d", &out);
    }while(out != 0);

    // Finish the semaphore
    sem_close(semaphore);
    sem_unlink(SEM_NAME);
    shmctl(shmid, IPC_RMID, 0);
    _exit(0);
}