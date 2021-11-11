/*
 * Taken from
 * http://www.linuxdevcenter.com/pub/a/linux/2007/05/24/semaphores-in-linux.html?page=5
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

#define SHMSZ 100
//Semaphore name
char SEM_NAME[]= "MILK";

int main()
{
  int shmid, a;
  key_t key;
  //Shared memory variable
  int *shm,*milk;

  sem_t *semaphore;

    //Server end option
  int serverEnd = 0;

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
  //Initialize the number of milk bottles
  *milk = 1000;

    //Run server until user ends it
    while (!serverEnd){
        //Take semaphore to print initial milk value
        sem_wait(semaphore);
        printf("Initialized with %d milk bottles\n",*milk);
        sem_post(semaphore);
        printf("Close the server? ");
        //Read if want to end server
        //Press 1 to end server
        a = scanf("%d",&serverEnd);
    }

  //Close semaphore and finish memory segment
  sem_close(semaphore);
  sem_unlink(SEM_NAME);
  shmctl(shmid, IPC_RMID, 0);
  _exit(0);
}
