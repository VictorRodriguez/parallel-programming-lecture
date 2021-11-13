/*

Author: Miguel Figarola Prado A01632557
This program implements server-clients case in order to
make a program to "sell" milk from a factory with a key
of 1000 milk bottles.
Each client shall ask if they want to buy, how much and if so
tell if there is available milk or not.

*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 27
char SEM_NAME[]= "Figarola";

int main()
{
  char ch;
  int shmid;
  key_t key;
  char *shm,*s;
  sem_t *mutex;

  // Declare semaphore
  sem_t *semaphore;

  key_t key;

  //Shared M. varaiables
  int *shm,*milk;

  int shmid, n;

  //Shared key
  key = 1000;

  // End the program
  int end = 0;

  //create & initialize semaphore
  mutex = sem_open(SEM_NAME,O_CREAT,0644,1);
  if(mutex == SEM_FAILED)
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
  // Initialize the number of milk bottles
  *milk = 1000;

  //the below loop could be replaced by binary semaphore
  while(*shm != '*')
    {
        // Semaphore prints how many milks available
        sem_wait(semaphore);
        printf("Bottles: %d milk bottles\n",*milk);
        // Then asks to exit
        sem_post(semaphore);
        printf("Exit? ");
        // Press 1 to exit
        n = scanf("%d",&end);
    }
  sem_close(mutex);
  sem_unlink(SEM_NAME);
  shmctl(shmid, IPC_RMID, 0);
  _exit(0);
}
