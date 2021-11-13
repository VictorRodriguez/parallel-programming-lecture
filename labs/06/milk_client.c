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

#define SHMSZ 100
char SEM_NAME[]= "Figarola";

int main()
{
   char ch;
  int shmid,a;
  key_t key;

  //// Shared M. varaiables
  int *shm,*milk;
  sem_t *semaphore;

  // Clients op.
  int clientOP = -1;

  // How many bottles?
  int sell;

  //create & initialize existing semaphore
  semaphore = sem_open(SEM_NAME,0);
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
  exit(0);
}
