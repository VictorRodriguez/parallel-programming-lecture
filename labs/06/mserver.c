/* Kevin Vega Rodriguez - A01612430
Lab 06: Create a milk factory (mserver.c) and milk sellers (client.c) using semaphores
The server is based on server.c only to get the milk stock
*/
#include  <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SHMSZ 27
//Semaphore name
char SEM_NAME[]= "stock";

int main()
{
  int shmid, a;
  key_t key;
  //Shared memory variable
  int *shm,*milk;

  sem_t *semaphore;

 

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

  //start writing into memory
  milk = shm;
  //Milk stock inicializer
  *milk = 1000;
  int control = 1;
    while (control){
        sem_wait(semaphore);
        printf("Day stock is %d bottles of milk\n",*milk);
        sem_post(semaphore);
        printf("press any number to upload the stock or 0 to quit\n");
        a = scanf("%d",&control);
    }

  //Close semaphore and finish memory segment
  sem_close(semaphore);
  sem_unlink(SEM_NAME);
  shmctl(shmid, IPC_RMID, 0);
  _exit(0);
}