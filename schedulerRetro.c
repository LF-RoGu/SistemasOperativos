#include "scheduler.h"
#include <stdio.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

QUEUE ready;
QUEUE Priority[5];
QUEUE *actual = &Priority[0];
QUEUE waitinginevent[MAXTHREAD];
int Readyflag = 0;

void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;

	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		threads[callingthread].status=READY;
        QUEUE *new = &Priority[0];         //Guardamos nuevo hilo en prioridad 0
		_enqueue(new,callingthread);
        Readyflag = 1;                           //Bandera para indicar la existencia de hilos
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
        if(threads[callingthread].status==RUNNING && Readyflag==1 ){      //Verificamos que exista un thread corriendo
             
         
        int i;
        for(i = 0;i<5;i++){
            if(_emptyq(&Priority[i])==0){             //Si la cola no esta vacia

                if(i<4){
                    actual = &Priority[i+1];
                    threads[callingthread].status=READY;            //Lo dejamos con status listo
                     _enqueue(actual,callingthread);   //Lo guardamos en la cola siguiente
                    actual = &Priority[i];
                }
                else{                                  //Si se llegó a la última cola
                    actual = &Priority[i];
                    threads[callingthread].status=READY;            //Lo dejamos con status listo
                     _enqueue(actual,callingthread);   //Lo guardamos en la cola siguiente
                }

                
                break;                                 //Salimos del ciclo una vez que ya se haya cambiado 
            }
        }
        changethread=1;                                 //Cambiamos de thread        
        }     
	}

	
	if(changethread)
	{
        if(threads[callingthread].status!=END){
            old=currthread;
		    next=_dequeue(actual); 
		    threads[next].status=RUNNING;
		    _swapthreads(old,next);
        }
        else{
            old=currthread;
		    next=_dequeue(&ready); 
		    threads[next].status=RUNNING;
		    _swapthreads(old,next);   
        }
	    
	}

}
