/*
    Owen Jauregui Borbon
    A01638122
*/

#include <semaphore.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdio.h>

#define SEM      "BOTTLES"
#define MEM_SIZE 100

int main() {
    int   *bottles;
    int    shmid;
    int    sell;
    int    option     = 2;
    key_t  key        = 1000;
    sem_t *semaphore;

    semaphore = sem_open(SEM,0);

    shmid   = shmget(key,MEM_SIZE,0666);
    bottles = shmat(shmid,NULL,0);

    while (option != 0) {
        printf("Select an option:\n1. Ask factory for bottles\n0. End\n");
        scanf("%d",&option);

        if(option == 1) {
            sem_wait(semaphore);

            if (*bottles > 0) {
                printf("Enter the amount of bottles desired:");
                scanf("%d",&sell);

                if (*bottles - sell < 0){
                    printf("No enough bottles available\n");
                    printf("Only %d bottles were given\n", *bottles);
                    *bottles = 0;
                } else {
                    *bottles = *bottles -sell;    
                }
            } else {
                printf("There is no more bottles of milk left\n");
            }
            printf("\n");

            sem_post(semaphore);
        } else {
            printf("Closing the program\n");
        }
    }
    sem_close(semaphore);
    return 0;
}