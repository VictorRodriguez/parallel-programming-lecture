#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 27
char SEM_NAME[]= "vik";

int main(){
    char ch;
    int shmid;
    key_t key;
    int *shm,*s;
    sem_t *semaphore;

    int bottlesSell;
    int bottlesAv;

    //name the shared memory segment
    key = 1000;

    //create & initialize semaphore
    semaphore = sem_open(SEM_NAME,O_CREAT,0644,1);
    if(semaphore == SEM_FAILED)
    {
        perror("unable to create semaphore");
        sem_unlink(SEM_NAME);
        exit(-1);
    }

  //create the shared memory segment with this key
    shmid = shmget(key,SHMSZ,IPC_CREAT|0666);
    if(shmid<0)
    {
        perror("failure in shmget");
        exit(-1);
    }

    //attach this segment to virtual memory
    shm = shmat(shmid,NULL,0);
    char bottles[]="four";
    int cont=1;
    s = shm;
    while (cont!=0){
        if (*s<=0){
            printf("No more bottles available today\n");
            cont=0;
        }else{
            printf("Bottles available: %d\nHow many bottles would you want to buy?\nOr enter 0 to exit\n", *s);
            scanf("%d", &bottlesSell);
            if (bottlesSell==0){
                cont=0;
            }else{
                sem_wait(semaphore);
                if (bottlesSell<*s){
                    *s=*s-bottlesSell;
                }else if (bottlesSell==*s){
                    *s=0;
                    cont=0;
                    printf("Purchase succesful!\nNo more milk available. Thank you for your purchase");
                }else{
                    printf("Not enough milk available\n");
                }
                sem_post(semaphore);
            }
        }
    }
    sem_close(semaphore);

    /*
    //start writing into memory
    s = shm;
    for(ch='A';ch<='Z';ch++)
    {
        sem_wait(semaphore);
        *s++ = ch;
        sem_post(semaphore);
    }

  //the below loop could be replaced by binary semaphore
    while(*shm != '*')
    {
        sleep(1);
    }
    sem_close(semaphore);
    sem_unlink(SEM_NAME);
    shmctl(shmid, IPC_RMID, 0);
    */

    exit(0);
}