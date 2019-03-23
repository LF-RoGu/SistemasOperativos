/*
 * Queue_P5.h
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
#include <unistd.h>
#include <ucontext.h>

typedef struct 	QUEUE
{
	int pFirst;
	int pLast;
	int arr[3];
}Queue_bloq;

void init(Queue_bloq *s)
{
	s->pFirst = 0;
	s->pLast = 0;
}

void put(Queue_bloq *s,int pid)
{
	s->arr[s->pFirst] = pid;
	s->pFirst++;
}
int pull(Queue_bloq *s)
{
	int bloq;
	bloq = s->arr[s->pLast];
	s->pLast++;
	return bloq;
}



