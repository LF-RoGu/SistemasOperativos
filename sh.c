#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#define FALSE 0
#define TRUE 1

#define XTERM_PATH "/usr/bin/xterm"
#define XTERM "xterm"

int main(int argc, char **argv)
{
	char exit[]="exit";
	char shutdown[]="shutdown";
	char comando[15];
	int FlagShutdown;
	int FlagExit;
	int Option;
	int i;
	int status;
	FILE *fp;	
	pid_t pid;
	//fp = popen("clear","w");//limpia la pantalla
	while(1)
	{
		FlagShutdown = 0;
		FlagExit = 0;
		Option = 0;
		printf("sh >");
		scanf("%s",comando);
		getchar();
		for(i=0;i<strlen(shutdown);i++)
		{
			if(shutdown[i]!=comando[i])
			{
				FlagShutdown = 1;	
			}

		}

		if (FlagShutdown == 0)
		{
			Option = 1;
		}else
		{
			for(i=0;i<strlen(exit);i++)
			{
				if(exit[i]!=comando[i])
				{
					FlagExit = 1;
				}
			}
			if (FlagExit == 0)
			{
				Option = 2;
			}
		}
		if (Option == 0)
		{
			pid = fork();
			if(pid==0)
			{
				fp = popen(comando,"w");
				return 0;
			}else
			{
				wait(&status);

			}
		}else if(Option == 1)
		{
			
			execl("/usr/bin/killall", "killall","./init","./getty","/usr/bin/xterm", (char*)NULL);
			return 0;
		}else
		{
			fp = popen("clear","w");//limpia la pantalla
			pclose(fp);
			execlp("./getty","getty",NULL); //llama getty
			printf("exit");
			return 0;
		}
	}
	return 0;
}
