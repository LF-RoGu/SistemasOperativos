#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	char cmd[80];
	
	do {
		printf("Shell >");
		scanf("%[^\n]%*c",cmd);
		system(cmd);
	} while(strcmp(cmd,"exit")!=0);
	return 0;
}