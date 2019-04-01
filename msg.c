#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define CICLOS 10

struct msgbuffer
{
    long    msgtype;
    char    msgtext[1];
}mssg;

char *pais[3]={"Peru","Bolvia","Colombia"};

void proceso(int i, int msg_id)
{
	int k;
	int l;
	for(k=0;k<CICLOS;k++)
	{

        msgrcv(msg_id, &mssg, sizeof(mssg.msgtext), 1, 0);
        
		printf("Entra %s",pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("- %s Sale\n",pais[i]);
		
		msgsnd(msg_id, &mssg, sizeof(mssg.msgtext), IPC_NOWAIT);

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
    int msg_id;
    
	msg_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666 | IPC_EXCL);
   
    mssg.msgtype = 1;
    msgsnd(msg_id, &mssg, sizeof(mssg.msgtext), IPC_NOWAIT);
	srand(getpid());
	for(i=0;i<3;i++)
	{
		pid=fork();
		if(pid==0)
            proceso(i, msg_id);
	}
	for(i=0;i<3;i++)
		pid = wait(&status);
	msgctl(msg_id, IPC_RMID, NULL);
}
