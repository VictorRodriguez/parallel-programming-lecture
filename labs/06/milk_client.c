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
  
  //attach this segment to virtual memory
  shm = shmat(shmid,NULL,0);

  //start reading
  milk = shm;

    // While client is in the menu
    while (clientOP != 0){

        // Print the options
        printf("Option: \n");
        printf("0. Exit\n");
        printf("1. Consult available milk\n");
        printf("2. Sell bottles of milk\n");
        printf("Select: ");

        // Read selected option
        a = scanf("%d",&clientOP);
        printf("\n");

        // See aviable bottles
        if (clientOP == 1){

            // Semaphore prints how many milks available
            sem_wait(semaphore);
            printf("Bottles: %d milk bottles\n",*milk);
            printf("\n");
            sem_post(semaphore);

        }
        // Sell bottles
        else if(clientOP == 2){

            // Compare operations
            sem_wait(semaphore);
            if (*milk > 0){

                // More than 0 bottles
                printf("How many bottles are you going to sell?: ");
                a = scanf("%d",&sell);
                // What if he wants to sell more?
                if ((*milk - sell) < 0){

                    printf("Can't make this execution\n");
                    printf("Not enough bottles available\n");
                    printf("Bottles available: %d\n", *milk);
                }

                // If there is milk to sell
                else{
                    // Substract sold milk to current milk
                    *milk = *milk -sell;    
                }

                printf("\n");
            }
            // No more milk in current key
            else{

                printf("No more milk available\n");
                printf("\n");

            }
            sem_post(semaphore);
        }

        // Returning to menu
        else{
            printf("Exit...\n");
        }
    }
  sem_close(semaphore);
  exit(0);
}
