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

#define milkCant 1000

#define SHMSZ 27
char SEM_NAME[]= "vik";

int main(){
    char ch;
    int shmid;
    key_t key;
    int *shm,*s;
    sem_t *semaphore;

    key = 1000;

  //create & initialize existing semaphore
    semaphore = sem_open(SEM_NAME,O_CREAT,0644,1);
    if(semaphore == SEM_FAILED)
    {
        perror("reader:unable to execute semaphore");
        sem_close(semaphore);
        exit(-1);
    }

  //create the shared memory segment with this key
    shmid = shmget(key,SHMSZ,IPC_CREAT|0666);
    if(shmid<0)
    {
        perror("reader:failure in shmget");
        exit(-1);
    }

    //attach this segment to virtual memory
    shm = shmat(shmid,NULL,0);

    s = shm;
    *s=milkCant;
    while(*s>0);
    printf("Milks Sold Out!!!");
    sem_close(semaphore);
    sem_unlink(SEM_NAME);
    shmctl(shmid, IPC_RMID, 0);
    /*
    //start reading
    s = shm;
    for(s=shm;*s!=NULL;s++)
    {
        sem_wait(semaphore);
        putchar(*s);
        sem_post(semaphore);
    }

    //once done signal exiting of reader:This can be replaced by another semaphore
    *shm = '*';
    sem_close(semaphore);
    shmctl(shmid, IPC_RMID, 0);
    */

    exit(0);
}