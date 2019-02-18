/*
 * P3_2.c
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
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <math.h>

#include <pthread.h>

#define ITE 2000000000
#define N_THREADS 4

void *pi_value(void *args);

pthread_mutex_t lock;

float pi_total = 0;

int main(int argc, char **argv)
{
	struct timeval ts;
	
	long long start_ts;
	long long stop_ts;
	long long elapsed_ts;
	
	float pi_sum = 0;
	
	int args[N_THREADS];
	pthread_t tid[N_THREADS];
	
	gettimeofday(&ts,NULL);
	
	start_ts = ts.tv_sec;
	
	/**/
	pthread_mutex_init(&lock,NULL);
	
	for(int i = 0; i < N_THREADS; i++)
	{
		args[i] = i;
		/**/
		pthread_create(&tid[i],NULL,pi_value,(void *)&args[i]);
	}
	
	for(int i = 0; i < N_THREADS; i++)
	{
		pthread_join(tid[i],NULL);
	}
	
	gettimeofday(&ts,NULL);
	
	stop_ts = ts.tv_sec;
	
	elapsed_ts = stop_ts - start_ts;
	
	pi_sum = pi_total;
	
	puts("*************************");
	printf("Pi value: %2.20f \n",pi_sum);
	
	puts("*************************");
	printf("Time passed: %lld sec \n",elapsed_ts);
	
	
	return 0;
}

void *pi_value(void *args)
{
	int temp = *((int *)args);
	int r_init = temp*(ITE/N_THREADS);
	int r_end = (temp+1)*(ITE/N_THREADS);
	float temp_sum = 0;
	
	for(int i = r_init; i < r_end; i++)
	{
		temp_sum = temp_sum + pow(-1,i)*(4.0/((2.0*i+1)));
	}
	
	pthread_mutex_lock(&lock);
	
	pi_total = pi_total + temp_sum;
	
	pthread_mutex_unlock(&lock);
	
	return 0;
}
