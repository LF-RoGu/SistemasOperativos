#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int ret;
	int i=1;
	
	printf("Hijo: %d\n",i);
	i++;
	pid=fork();
	while (i < 6 && pid != 0 )
	{
		printf("Hijo: %d\n",i);
		pid=fork();
		i++;
	}	

	if (pid ==0)
	{
		printf("Calling excel...\n");
		//ret = execl("hello", "progr",NULL);
		ret = execl("/usr/bin/xterm", "xterm","-hold", "-e","./hello", "progr",NULL);
		printf("failed execl....ret = %d\n",ret);
	}else
	{
		printf("Padre Close\n");
	}
	return 0;
	
}
