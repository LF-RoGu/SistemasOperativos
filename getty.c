/*
 * getty.c
 * 
 * Copyright 2019 Luis <luis@luis-VirtualBox>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


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
	char input_user [10] = {0},
		 input_password [10] = {0},
		 user_validate[30] = {0},
		 file_password[30] = {0};
	int length_userValidate = 0,
		pid = 0,
		status = 0;
	FILE *fp;
	
	/*Capturamos user y psswd*/
	printf("User:");
	fflush(stdout);
	scanf("%s",input_user);
	fflush(stdout);
	
	printf("Password:");
	fflush(stdout);
	scanf("%s",input_password);
	fflush(stdout);
	
	/*concatenamos el texto para que sea comparable con el del archivo passwd.txt*/
	strcat(user_validate,input_user);
	strcat(user_validate,":");
	strcat(user_validate,input_password);
	
	length_userValidate = strlen(user_validate);
	
	/*abre el archivo en modo lectura para comparar opciones*/
	fp = fopen("passwd.txt","r");
	
	while(fgets(file_password,21,fp) != NULL)
	{
		if(FALSE == strncmp(file_password,user_validate,length_userValidate))
		{
			fclose(fp);
			pid = fork();
			if(FALSE == pid)
			{
				execlp("./sh","sh",(char*)NULL);
			}
			else
			{
				wait(&status);
			}
		}
	}

	return 0;
}

