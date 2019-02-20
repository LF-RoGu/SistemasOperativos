#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sched.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sched.h>

#define STACK_SIZE (1024*1024)
#define ITE 2000000000
#define THREADS 4 //Numero de procesadores

pthread_mutex_t lock;

static int pi_value(void *args);
float pi_total = 0;
long double *SharedVar = 0;

static int pi_value(void *args)
{
	int temp = *(int *)args;
	int r_init = temp*(ITE/THREADS);
	int r_end = (temp+1)*(ITE/THREADS);
	long double temp_sum = 0;
	for(int i = r_init; i < r_end; i++)
	{
		temp_sum += (pow(-1,i))/((2.0*i+1));
	}
	*SharedVar += temp_sum;
	return 1;
}

int main(void)
{
	long long Start;
	long long Stop;
	int shmid;
	int Value[THREADS];
	float Time;
	struct timeval ts;
	pid_t tid[THREADS];
	pid_t pid;
	char *stack[THREADS];
	
	shmid = shmget(IPC_PRIVATE, sizeof(SharedVar), IPC_CREAT | 0666);
	SharedVar = shmat(shmid,NULL,0);

	gettimeofday(&ts,NULL);
	Start = ts.tv_sec*1000000 + ts.tv_usec;	
	
	for (int i = 0; i < (THREADS); i++)
	{
		pid = fork();
		if (pid ==0)
		{
			pi_value(&i);
			return 0;
		}
		
	}

	int status;
	for (int i = 0; i < (THREADS); i++)
	{
		if(pid != 0)

		{
			wait(&status);
		}
	}
	
	printf("Value: %Lf\n", (*SharedVar)*4);
	gettimeofday(&ts, NULL);
	Stop = ts.tv_sec*1000000.0 + ts.tv_usec;
	Time = (float)(Stop - Start)/1000000;
	printf("%f segundos\n", Time);
	return 0;
	
}

