/*
 * P5_procesos.c
 * 
 * Copyright 2018 Luis <luis@luis-VirtualBox>
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
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

// Macro que incluye el código de la instrucción máquina xchg
#define atomic_xchg(A,B) __asm__ __volatile__(\
"lock xchg %1,%0 ;\n"	\
: "=ir" (A)	\
: "m" (B), "ir" (A)	\
);

#define CICLOS 10

char *pais[3] = {"Peru","Bolivia","Colombia"};

int *g;

void proceso(int i);

int main(int argc, char **argv)
{
	int pid;
	int status;
	int shmid;
	int i;
	
	shmid = shmget(0x1234,sizeof(g),0666|IPC_CREAT);
	if(shmid == -1)
	{
		perror("Error en la memoria compartida\n");
		exit(1);
	}
	
	g = shmat(shmid,NULL,0);
	
	if(g == NULL)
	{
		perror("Error en el shmat\n");
		exit(2);
	}
	*g = 0;
	srand(getpid());
	
	for(i = 0;i<3;i++)
	{
		// Crea un nuevo proceso hijo que ejecuta la función proceso()
		pid=fork();
		if(pid==0)
			proceso(i);
	}
	for(i = 0;i<3;i++)
		pid = wait(&status);
	shmdt(g);
}

void proceso(int i)
{	
	int k,l;
	
	for(k = 0;k < CICLOS;k++)
	{
		l = 1;
		do
		{
			atomic_xchg(l,*g);
		}while(l != 0);
		/*Empieza la SC*/
		printf("Entra %s\n",pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("-%s Sale\n",pais[i]);
		/*Termina SC*/
		l = 1;
		*g = 0;
		sleep(rand()%3);
	}
	/*Termina el proceso*/
	exit(0);
}

