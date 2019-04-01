#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "semaphores.h"

#define CICLOS 10
int *sem_id;
char *pais[3]={"Peru","Bolvia","Colombia"};

void proceso(int i)
{
	int k;
	int l;
	for(k=0;k<CICLOS;k++)
	{
		semwait(*sem_id);
		printf("Entra %s",pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("- %s Sale\n",pais[i]);
		semsignal(*sem_id);
		sleep(rand()%3);
	}
	exit(0);
}

int main()
{
	int pid;
	int status;
	int args[3];
	int i;
	int shmid;

	shmid=shmget(IPC_PRIVATE,sizeof(sem_id),0666|IPC_CREAT);
	if(shmid==-1)
	{
		perror("Error en la memoria compartida\n");
		exit(1);
	}
	sem_id=shmat(shmid,NULL,0);
	if(sem_id==NULL)
	{
		perror("Error en el shmat\n");
		exit(2);
	}

	*sem_id = createsem(IPC_PRIVATE, 1);
	
	srand(getpid());
	for(i=0;i<3;i++)
	{

		pid=fork();
		if(pid==0)
		proceso(i);
	}
	for(i=0;i<3;i++)
		pid = wait(&status);
	
	erasesem(*sem_id);
}
