/*
 * P5_hilos.c
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
#include <pthread.h>

// Macro que incluye el código de la instrucción máquina xchg
#define atomic_xchg(A,B) __asm__ __volatile__(\
"lock xchg %1,%0 ;\n"	\
: "=ir" (A)	\
: "m" (B), "ir" (A)	\
);

#define CICLOS 10

char *pais[3] = {"Peru","Bolivia","Colombia"};

int g = 0;

void *hilo1(void *args);


int main(int argc, char **argv)
{
	pthread_t tid[3];
	int res;
	int args[3];
	int i;
	void *thread_result;
	
	srand(getpid());
	
	// Crea los hilos
	for(i=0;i<3;i++)
	{
		args[i]=i;
		res = pthread_create(&tid[i], NULL, hilo1, (void *) &args[i]);
	}
	// Espera que terminen los hilos
	for(i=0;i<3;i++)
		res = pthread_join(tid[i], &thread_result);
	return 0;
}

void *hilo1(void *args)
{
	/*Cast de la var args*/
	int *myNum = (int *)args;
	int i = *myNum;
	
	int k,l;
	
	for(k = 0;k < CICLOS;k++)
	{
		l = 1;
		do
		{
			atomic_xchg(l,g);
		}while(l != 0);
		/*Empieza la SC*/
		printf("Entra %s\n",pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("-%s Sale\n",pais[i]);
		/*Termina SC*/
		g = 0;
		l = 1;
		sleep(rand()%3);
	}
}
