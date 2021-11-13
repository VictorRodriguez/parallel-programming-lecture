/*
Bryan Marquez - A01562119
To execute, first check that milk-server.c is running, then run the following 2 lines in your terminal
$ gcc milk-client.c -o milk-client -lpthread -lrt
$ ./milk-client
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
#include <time.h>
//SHared Memory SiZe
#define SHMSZ 54
char SEM_NAME[]= "sync"; //Semaphore name

int main()
{
  char ch;
  int shmid;
  key_t key;
  int *shm,*s;
  sem_t *semaphore;
  srand(time(NULL));

  //name the shared memory segment
  key = 1000;

  //create & initialize existing semaphore
  semaphore = sem_open(SEM_NAME,0,0644,0);
  //semaphore = sem_open(SEM_NAME,0);
  if(semaphore == SEM_FAILED)
    {
      perror("reader:unable to execute semaphore");
      sem_close(semaphore);
      exit(-1);
    }

  //create the shared memory segment with this key
  shmid = shmget(key,SHMSZ,0666);
  if(shmid<0)
    {
      perror("reader:failure in shmget");
      exit(-1);
    }

  //attach this segment to virtual memory
  shm = shmat(shmid,NULL,0);

  //start reading
  s = shm;
  //Here we'll do all the milk selling thing code
  int option=-1, x;
  while(option != 3)
  {
    printf("\nSelect an option for the milk selling simulation:\n  1 - Milk bottles available\t 2 - Sell milk bottles\t 3 - End sell\n");
    scanf("%d", &option);
    
    if(option == 1){
      sem_wait(semaphore);
      if(*s <= 0){
        printf("\nNo more bottles left for today..\n Forcing simulation exit\n");
        option = 3;
      }else{
      printf("\nMilk bottles available: %d\n", *s);
      }
      sem_post(semaphore);
    }
    else if(option == 2){
      sem_wait(semaphore);
      //Test-case #1. There needs to be milk bottles available to sell
      if(*s <= 0){
        printf("\nNo more bottles left for today..\n Forcing simulation exit\n");
        option = 3;
      }
      else{
        //Each seller can sell random from 1 to 500 milk bottles, but if we have left
        // less then 80 milk bottles, sell the 80 directly to the first seller who asks for them.
        if(*s > 80){
        x = 1+rand()%500;
        printf("\nMilk bottles to be sold: %d\n",x);
        }else{
          x = *s;
          printf("\nMilk bottles to be sold: %d\n",x);
          printf("\nAnd thats the last pile of milk bottles for today\n");
        }

        if((*s - x) < 0){
          printf("\nExceeds bottles available, check bottles available with option 1\n");
        }
        else{
          *s -= x;
        }
      }
      sem_post(semaphore);
    }
    else{
      printf("\nExiting\n");
    }
  }
  //Stop selling thingy here

  //once done signal exiting of reader:This can be replaced by another semaphore
  *shm = '*';
  sem_close(semaphore);
  //shmctl(shmid, IPC_RMID, 0); //we don't want to detach memory
  exit(0);
}
