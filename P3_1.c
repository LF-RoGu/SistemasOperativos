/*
 * P3_1.c
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

#define ITE 2000000000

int main(int argc, char **argv)
{
	struct timeval ts;
	
	long long start_ts;
	long long stop_ts;
	
	long long elapsed_ts;
	
	float pi_sum;
	
	gettimeofday(&ts,NULL);
	
	start_ts = ts.tv_sec;
	
	for(int i = 0;i < ITE; i++)
	{
		pi_sum = pi_sum + pow(-1,i)*(4.0/((2.0*i+1)));
	}
	puts("*************************");
	printf("Pi value: %2.20f \n",pi_sum);
	
	gettimeofday(&ts,NULL);
	
	stop_ts = ts.tv_sec;
	
	elapsed_ts = stop_ts - start_ts;
	puts("*************************");
	printf("Time passed: %lld sec \n",elapsed_ts);
	return 0;
}

/*
 * Pi value: 3.14159679
 * t = 81 seg
 * Problema, solo saca el valor correcto unas veces, otras falla
 */
