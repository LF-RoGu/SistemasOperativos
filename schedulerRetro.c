#include <scheduler.h>
#include <stdio.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

#define QUANTUM 1
#define Q_LEV 10 // nivel de prioridad usados

//Estructura para el uso de colas

typedef struct queue {
	int elem[MAXTHREAD];
	int tope;
	int cola;
} queuef;

void init(queuef *q);
void push(queuef *q,int val);
int pop(queuef *q);
int isempty(queuef *q);

void init(queuef *q)
{
	q->tope=0;
	q->cola=0;
}

void push(queuef *q,int val) 
{
	q->elem[q->cola]=val;
	q->cola++;
	q->cola=q->cola%MAXTHREAD;
}

int pop(queuef *q)
{
	int valret;
	valret=q->elem[q->tope];
	q->tope++;
	q->tope=q->tope%MAXTHREAD;
	return(valret);
}

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];

queuef feedback[Q_LEV];

int quantum = 0;
int threadC = 0;
int threadP[MAXTHREAD];
int init_flag = 1; // 
void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;
	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;
	
	if(init_flag == 1)
	{
		int counter;
		init_flag == 0;
		for(counter = 0; counter<Q_LEV; counter++)
		{
			init(&feedback[counter]);
			threadP[counter] = Q_LEV;
		}
	}

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		threads[callingthread].status=READY;
		threadP[callingthread] = 0;
		threadC++;
		_enqueue(&ready,callingthread);
		printf("%d\n", callingthread);
	}
	
	if(event==BLOCKTHREAD)
	{

		threads[callingthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],callingthread);

		changethread=1;
	}

	if(event==ENDTHREAD)
	{
		threads[callingthread].status=END;
		threadP[callingthread] = Q_LEV;
		threadC--;
		changethread=1;
	}
	
	if(event==UNBLOCKTHREAD)
	{
			threads[callingthread].status=READY;
			_enqueue(&ready,callingthread);
	}

	if(event==TIMER)
	{
		quantum++;
		if(quantum==QUANTUM)
		{
			if(threadsPriority[callingthread]< QUEUE_LEVELS)
			{
				threadP[callingthread]++;
			}
			if(threadC == 1) 			{
				threadP[callingthread] = 0;
			}

			push(&feedback[threadP[callingthread]], callingthread);
			int counter, threadIndex = 0;
			for(counter = Q_LEV-1; counter>=0; counter--)
			{
				if((threadP[counter] <= threadP[threadIndex] && (threads[counter].status==READY || threads[counter].status==RUNNING)))
				{
					threadIndex = counter;
				}
			}
			_enqueue(&ready,pop(&feedback[threadP[threadIndex]]));

			quantum = 0;
			changethread=1;
		}
	}
	if(changethread)
	{
		old=currthread;
		next=_dequeue(&ready);
		
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}

}
