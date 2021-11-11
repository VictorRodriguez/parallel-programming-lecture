#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Define shared memory syze
#define SHMSZ 100
//Define semaphore name
char SEM_NAME[]= "MILK";

int main()
{
  char ch;
  int shmid,a;
  key_t key;
  //Shared memory variables
  int *shm,*milk;
  sem_t *semaphore;

  //Client option selector
  int clientOP = -1;
  //Number of bottles to sell
  int sell;

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

  //start reading
  milk = shm;

    //While client doesn't want to go out of menu
    while (clientOP != 0){
        printf("What to do?:\n");
        printf("    0. Get Out\n    1. Consult botlles of milk available\n");
        printf("    2. Sell bottles of milk\n");
        printf("Select option: ");
        //Read option
        a = scanf("%d",&clientOP);
        printf("\n");

        //See aviable bottles
        if (clientOP == 1){
            //Take semaphore to print the actual number of bottles
            sem_wait(semaphore);
            printf("Bottles available %d\n", *milk);
            printf("\n");
            sem_post(semaphore);
            
        }
        //Sell bottles of mikl
        else if(clientOP == 2){
            //Take sempahore to compare and do operations with milk variable
            sem_wait(semaphore);
            //Look if there are more than 0 milks
            if (*milk > 0){
                //If there are more than 0, then ask how many the milk seller want to sell
                printf("How many bottles are you going to sell?: ");
                a = scanf("%d",&sell);
                //If he wants to sell more than there are available, then reject
                //  the request and show the number of bottles available
                if ((*milk - sell) < 0){
                    printf("No enough bottles available\n");
                    printf("Bottles of milk available %d\n", *milk);
                }
                //If there is enough mikl, then substract the number of bottles selled
                else{
                    *milk = *milk -sell;    
                }
                
                printf("\n");
            }
            //If there is no more milk available, then print it
            else{
                printf("No more milk available\n");
                printf("\n");

            }
            sem_post(semaphore);
        }
        //Go out of menu
        else{
            printf("Going out\n");
        }
    }
  sem_close(semaphore);
  exit(0);
}