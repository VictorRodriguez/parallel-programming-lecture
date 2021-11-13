/* Kevin Vega Rodriguez - A01612430
Lab 06: Create a milk factory (mserver.c) and milk sellers (client.c) using semaphores
The client can be executed multiple times representing multiple sellers and the semaphore will lock the 
sells when they accept the selling value and in this critical section you cant access to the stock
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

//Define shared memory syze
#define SHMSZ 27
//Define semaphore name
char SEM_NAME[]= "stock";


int main()
{
  srand(time(NULL));  
  char ch;
  int shmid,a;
  key_t key;
  //Shared memory variables
  int *shm,*milk;
  sem_t *semaphore;
  //name the shared memory segment
  key = 1000;

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
  milk = shm;
  int control = 1;
  printf("Welcome to milk factory seller system \n");
  // initialize some control varibles
  int nMax = 500;
  int nMin = 1;
  int selled;
  while (control != 0){
    printf("0 = Exit\n");
    printf("1 = Consult bottles\n");
    printf("2 = Sell bottles \n");
    a = scanf("%d",&control);
    printf("\n");
    if (control == 1)
    {
        sem_wait(semaphore);// we need to wait the semaphore
        printf("Bottles available %d\n", *milk); //print the bottles of milk
        printf("\n");
        sem_post(semaphore);// release semaphore
        
    }

    else if(control == 2)
    {
      int *aux;
      sem_wait(semaphore);// we need to wait the semaphore
      
      if (*milk > 0){// control if

        selled = rand()%((nMax+1)-nMin) + nMin; // random selling between 1 and 500
        printf("selling %d bottles of milk\n", selled);
        printf("Press any key to confirm the purchase, enter 0 to cancel ");
        scanf("%d",aux);

        if ((*milk - selled) < 0){ // We can't sell more than the stock
          printf("We are out of stock we only have %d bottlrd\n", *milk);
        }
        else if (*aux == 0){ //cancel option
          printf("Cancelled \n");
        }
        else{
          *milk = *milk -selled;//update the stock     
        }
      printf("\n");
      }
      //If there is no more milk available, then print it
      else{
        printf("Sorry, We are out of stock, try again tomorrow\n");
      }
      sem_post(semaphore);// release semaphore
    }
  }
  //closing the semaphore
  sem_close(semaphore);
  exit(0);
}