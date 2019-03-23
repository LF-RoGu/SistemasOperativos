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
/*Referencias
 * http://www.mailxmail.com/curso-informatica-sincronizacion/procesos-c-funcion-kill
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
/*Biblioteca generada en base a archivos de materias pasadas para las colas*/
#include "P5_semaforos.h"

// Macro que incluye el código de la instrucción máquina xchg
#define atomic_xchg(A,B) __asm__ __volatile__(	\
						"lock xchg %1,%0 ;\n"	\
						: "=ir" (A)				\
						: "m" (B), "ir" (A)		\
);

#define CICLOS 10

char *pais[3] = {"Peru","Bolivia","Colombia"};

int *g = 0;
int *j = 0;
struct SEMAPHORE* shmid_sem;

void *hilo1(void *args);

typedef struct SEMAPHORE
{
	int cont;
	Queue_bloq bloqueados;
	
}SEMAPHORE;

void Init(struct SEMAPHORE* s, int n);
void Wait(struct SEMAPHORE* s);
void Signal(struct SEMAPHORE* s);
void proceso(int i);


int main(int argc, char **argv)
{	
	int pid,status;
	int shmid_g,
		shmid_j,
		shmid_semaphore;
	/*Solicitamos memoria compartida para el semaforo*/
	shmid_semaphore = shmget(0x1234,sizeof(shmid_sem),0666|IPC_CREAT);
	/*Solicitamos memoria para la variable g*/
	shmid_g = shmget(0x1235,sizeof(g),0666|IPC_CREAT);
	/*Solicitamos memoria para la variable j*/
	shmid_j = shmget(0x1236,sizeof(j),0666|IPC_CREAT);
	/*Revisamos si se solicito correctamente la memoria compartida*/
	if(shmid_semaphore == -1 || shmid_g == -1 || shmid_j == -1)
	{
		perror("Error con SHMID\n");
		exit(1);
	}
	/*Conectamos las var de la memoria compartida*/
	shmid_sem = shmat(shmid_semaphore,NULL,0);
	g = shmat(shmid_g,NULL,0);
	j = shmat(shmid_j,NULL,0);
	/*Revisamos si se conecto correctamente la memoria*/
	/*Si no se conecto correctamente, mandamos mensaje de error*/
	if(shmid_sem == NULL || g == NULL || j == NULL)
	{
		perror("Error en conectar la memoria\n");
		exit(2);
	}
	/*Inicializamos la cola de procesos*/
	Init(shmid_sem,1);
	/*Obtenemos un pid aleatorio tal como en el ejemplo de procesos*/
	srand(getpid());
	for(int i = 0; i < 3;i++)
	{
		pid = fork();
		if(pid == 0)
			proceso(i);
	}
	for(int i = 0; i < 3;i++)
	{
		pid = wait(&status);
	}
	/*liberamos la memoria compartida
	  De lo contrario podriamos tener problemas a la hora de que se vuela a ejecutar, ya que la memoria
	  de los procesos permanece si no la liberamos.	
	*/
	shmdt(shmid_sem);
	shmdt(g);
	shmdt(j);
	return 0;
}

void Init(struct SEMAPHORE* s, int n)
{
	s->cont = n;
	init(&s->bloqueados);
}

void Wait(struct SEMAPHORE* s)
{
	int l = 1;
	/*Aseguramos la atomicidad por medio de una interrupcion*/
	do { atomic_xchg(l,*j); } while(l!=0);
	s->cont--;
	/*Empezamos el chequeo de las condicionales para ver si metemos un proceso a la 
	 * cola de bloqueados o no*/
	 if(s->cont < 0)
	 {
		 /*Guardamos el pid del proceso, ya que el proceso como tal no se puede guardar
		  * */
		 put(&s->bloqueados,getpid());
		 /*Usamos funcion kill para bloquear el proceso
		  * Kill es una funcion del sistema la cual detendra el proceso con SIGSTOP*/
		  kill(getpid(),SIGSTOP);
	 }
	 *g = 0;
}

void Signal(struct SEMAPHORE* s)
{
	/*variable temporal para extraer el proceso de la cola*/
	int temp;
	int l = 1;
	/*Aseguramos la atomicidad por medio de una interrupcion*/
	do { atomic_xchg(l,*g); } while(l!=0);
	s->cont++;
	if(s->cont <= 0)
	{
		/*Extraemos el proceso por medio de una indireccion*/
		temp = pull(&s->bloqueados);
		/*Le indicamos con la funcion kill al sistema que libere el proceso*/
		kill(temp,SIGCONT);
	}
	*j = 0;
}

void proceso(int i)
{	
	int k;
	
	for(k = 0;k < CICLOS;k++)
	{
		Wait(shmid_sem);
		/*Empieza la SC*/
		printf("Entra %s\n",pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("-%s Sale\n",pais[i]);
		fflush(stdout);
		Signal(shmid_sem);
		sleep(rand()%3);
	}
	/*Termina el proceso*/
	exit(0);
}
