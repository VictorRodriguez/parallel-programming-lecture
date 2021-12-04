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
#include <unistd.h>

#define SHMSZ 27
void init();
void end();

char SEM_NAME[] = "Ian";

int *milk, *sellers, *shm1, *shm2;
sem_t *semaphore;

int shmid1, shmid2;

int main()
{
	init();
	char c = NULL;
	//the below loop could be replaced by binary semaphore
	while (c != 'q')
	{
		sem_post(semaphore);
		printf("press 'q' to quit\n");
		scanf("%c",&c);
	}
	end();
	_exit(0);
}

void init()
{
	key_t key1, key2;

	//name the shared memory segment
	key1 = 1000;
	key2 = 2000;
	//create & initialize semaphore
	semaphore = sem_open(SEM_NAME, O_CREAT, 0644, 0);
	if (semaphore == SEM_FAILED)
	{
		perror("unable to create semaphore");
		sem_unlink(SEM_NAME);
		exit(-1);
	}

	//create the shared memory segment with this key
	shmid1 = shmget(key1, SHMSZ, IPC_CREAT | 0666);
	if (shmid1 < 0)
	{
		perror("failure in shmget 1");
		exit(-1);
	}

	//attach this segment to virtual memory
	shm1 = shmat(shmid1, NULL, 0);

	//start writing into memory
	milk = shm1;
	//initialize milk with 1000 bottles
	*milk = 1000;

	//create the shared memory segment with this key
	shmid2 = shmget(key2, SHMSZ, IPC_CREAT | 0666);
	if (shmid2 < 0)
	{
		perror("failure in shmget 2");
		exit(-1);
	}

	//attach this segment to virtual memory
	shm2 = shmat(shmid2, NULL, 0);

	//start writing into memory
	sellers = shm2;
	//initialize milk with a
	*sellers = 1;

}
void end(){
	sem_close(semaphore);
	sem_unlink(SEM_NAME);
	shmctl(shmid1, IPC_RMID, 0);
	shmctl(shmid2, IPC_RMID, 0);
}