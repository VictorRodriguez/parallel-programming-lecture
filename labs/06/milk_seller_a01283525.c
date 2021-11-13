//Ian De La Garza González A01283525
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 27

void init();
void end();
char SEM_NAME[] = "Ian";
int shmid;
int *milk, seller, *shm1, *shm2;
sem_t *semaphore;

int shmid1, shmid2;

int main()
{
    init();
	int cond = 0, opc = 0, quantity =0;
	printf("\033c");
	while (!cond)
	{
		printf(
			"\n"
			"-------------------------------\n"
			"     Main Menu | Seller %d     \n"
			"-------------------------------\n"
			"1) Status				     	\n"
			"2) Sell milk				    \n"
			"3) Exit   					    \n"
			"-------------------------------\n"
			, seller);
		scanf("%d", &opc);
		switch (opc)
		{
		case 1:
			sem_wait(semaphore);
			printf("\033c");
			printf("There are %d milk bottles left.\n", *milk);
	        sem_post(semaphore);		
			break;
		case 2:
			sem_wait(semaphore);
			printf("\033c");
			printf("Quantity of bottles to sell (Max 500 units, min 1 unit): \n");
			scanf("%d",&quantity);
			int res =*milk - quantity; 
			if (res < 0){
				printf("No enough bottles\n");
				printf("Please check status\n");
			}else if(quantity<1 || quantity>500){
				(quantity<1)?
					printf("Value to small\n"):
					printf("Exceeds maximum value for the input(Max 500)\n");
			}
			else{
				*milk -= quantity;    
			}
			sem_post(semaphore);		
			break;
		case 3:
			cond = 1;
			break;	
		default:
			break;
		}
	}

	end();
}

void init()
{
	key_t key1, key2;

	//name the shared memory segment
	key1 = 1000;
	key2 = 2000;
	//create & initialize existing semaphore
	semaphore = sem_open(SEM_NAME, 0, 0644, 0);
	if (semaphore == SEM_FAILED)
	{
		perror("reader:unable to execute semaphore");
		sem_close(semaphore);
		exit(-1);
	}

	//create the shared memory segment with this key
	shmid1 = shmget(key1, SHMSZ, 0666);
	if (shmid1 < 0)
	{
		perror("reader:failure in shmget 1");
		exit(-1);
	}

	//attach this segment to virtual memory
	shm1 = shmat(shmid1, NULL, 0);

	milk = shm1;
	//create the shared memory segment with this key
	shmid2 = shmget(key2, SHMSZ, 0666);
	if (shmid2 < 0)
	{
		perror("reader:failure in shmget 2");
		exit(-1);
	}

	//attach this segment to virtual memory
	shm2 = shmat(shmid2, NULL, 0);
	
	if (*shm2>5)
	{
		perror("reader:Limit of 5 vendors exceeded");
		exit(-1);
	}
	seller = *shm2;
	sem_wait(semaphore);
	*shm2=*shm2+1;
	sem_post(semaphore);
}

void end(){
	sem_wait(semaphore);
	*shm2=*shm2-1;
	sem_post(semaphore);
	sem_close(semaphore);
	//shmctl(shmid1, IPC_RMID, 0);
	//shmctl(shmid2, IPC_RMID, 0);
	exit(0);
}