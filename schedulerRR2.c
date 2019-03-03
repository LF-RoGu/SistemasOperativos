#include "scheduler.h"


extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

int Timeflag=0;
QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];

void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;
   
	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		threads[callingthread].status=READY;
		_enqueue(&ready,callingthread);
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
		changethread=1;
	}
	
	if(event==UNBLOCKTHREAD)
	{
			threads[callingthread].status=READY;
			_enqueue(&ready,callingthread);
	}
    
     if(event==TIMER)        //Se crea un nuevo evento
	{
        if(threads[callingthread].status=RUNNING){
            Timeflag++;
            if(Timeflag==2){                                 //Verificamos que exista un thread corriendo
            threads[callingthread].status=READY;            //Lo dejamos con status listo
            _enqueue(&ready,callingthread);                 //Lo guardamos en la cola
            changethread=1;
            Timeflag=0;
            }                                 //Cambiamos de thread
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
