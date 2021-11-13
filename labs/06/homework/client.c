
/****************************************************
 * Luis Humberto Sanchez Vaca
 * A01638029
 * 
 * Program for the seller to check the bottles
 * available and see if the operation can be done 
 * to sell the number of bottles from the input
 * 
 * To compile:
 * gcc client.c -o client -lpthread -lm
 **************************************************/

// Necessary libraries
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
char SEM_NAME[]= "BOTTLES";

int main(){
    // Variables for share memory
    int shmid;
    key_t key;
    int *shm, *bottles;
    sem_t *semaphore;

    // Variable for the menu option selected
    int option;
    // Variable for the amount of bottles to sell
    int amount;
    
    //name the shared memory segment
    key = 1000;

    //create & initialize existing semaphore
    semaphore = sem_open(SEM_NAME,0,0644,0);
    if(semaphore == SEM_FAILED){
        perror("reader:unable to execute semaphore");
        sem_close(semaphore);
        exit(-1);
        }

    //create the shared memory segment with this key
    shmid = shmget(key,SHMSZ,0666);
    if(shmid<0){
        perror("reader:failure in shmget");
        exit(-1);
        }

    //attach this segment to virtual memory
    shm = shmat(shmid,NULL,0);

    //start reading
    bottles = shm;

    do{
        // Print the menu
        printf("Select an option:\n");
        printf("(1) Check number of available bottles\n");
        printf("(2) Sell bottles\n");
        printf("(0) Exit\n");
        // Ask for an option
        scanf("%d", &option);

        // Check the action according to the option
        switch (option){
            case 1:
                // Print number of bottles
                sem_wait(semaphore);
                printf("There are %d bottles of milk available\n\n", *bottles);
                sem_post(semaphore);
                break;

            case 2:
                // Try to sell bottles
                sem_wait(semaphore);
                if(*bottles > 0){
                    // Check that there are bottles available
                    printf("Type amount of bottles to sell: ");
                    // Ask for the amount to sell
                    scanf("%d", &amount);
                    
                    if ((*bottles - amount) < 0){
                        // Check if there are enough bottles
                        printf("There are not enough bottles for this sell\n");
                        printf("There are %d bottles of milk available\n\n", *bottles);
                    }
                    else{
                        // Subtract the amount sold
                        *bottles -= amount;
                    }
                }
                else{
                    // Indicate if there are 0 bottles
                    printf("There's no more bottles of milk\n\n");
                }
                sem_post(semaphore);
                break;
            
            case 0:
                printf("Exiting\n");
                break;

            default:
                printf("Select a valid option\n\n");
                break;
        }


    }while(option != 0);

    // Finish the semaphore
    sem_close(semaphore);
    exit(0);
}