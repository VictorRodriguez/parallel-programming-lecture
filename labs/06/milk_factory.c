/*
    Owen Jauregui Borbon
    A01638122
*/

#include <semaphore.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdio.h>

#define SEM         "BOTTLES"
#define MEM_SIZE    100
#define BOTTLE_CANT 1000

int main() {
    int   *bottles;
    int    shmid;
    char   end = 0;
    key_t  key = 1000;
    sem_t *semaphore;

    semaphore = sem_open(SEM,O_CREAT,0644,1);

    shmid = shmget(key,MEM_SIZE,IPC_CREAT|0666);
    bottles  = shmat(shmid,NULL,0);

    *bottles = BOTTLE_CANT;

    printf("Amount of bottles that can be selled: %d\n", BOTTLE_CANT);

    printf("---Enter 0 to close server, 1 to show bottles---\n");
    while(end != '0') {
        scanf("%c", &end);
        if (end == '1') {
            sem_wait(semaphore);
            printf("Amount of bottles available: %d\n", *bottles);
            sem_post(semaphore);
        }
    }

    shmctl(shmid, IPC_RMID, 0);

    sem_close(semaphore);
    sem_unlink(SEM);
    
    return 0;
}