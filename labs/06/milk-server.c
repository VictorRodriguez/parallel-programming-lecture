/*
 * Taken from
 * http://www.linuxdevcenter.com/pub/a/linux/2007/05/24/semaphores-in-linux.html?page=5
 */
/*
Bryan Marquez - A01562119
To execute, run the following 2 lines in your terminal
$ gcc milk-server.c -o milk-server -lpthread -lrt
$ ./milk-server
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
#include <unistd.h>

#define SHMSZ 54
char SEM_NAME[]= "sync";

int main()
{
  char ch;
  int shmid;
  key_t key;
  int *shm,*s;
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
  s = shm;
  *s = 1000; //Milk bottles produced daily

  //Code thing to run server until users stops it
  //the below loop could be replaced by binary semaphore
  while(*shm != '*')
    {
      sem_wait(semaphore);
      sem_post(semaphore);
      sleep(1);
    }
  //ALL THE WAY TO HERE I GUESS

  sem_close(semaphore);
  sem_unlink(SEM_NAME);
  shmctl(shmid, IPC_RMID, 0);
  _exit(0);
}
