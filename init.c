#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_FORK 5

#define FALSE 0
#define TRUE 1

int main() 
{
    /*Variable para el control de los procesos hijos*/
    int p_hijos = 6;
    int pid = 0;
    int status = 0;
    /*Almacenamos los pids de los procesos hijos para tener un control*/
    char pid_buffer[MAX_FORK] = {0};
    
    printf("check1\n");
    
    while(1)
    {
        /*Variable donde se almacenara el pid de cada proceso hijo*/
        pid = fork();
        /*Restamos 1 a la variable para el control de los procesos hijos*/
        p_hijos--;
        
        printf("check2\n");
        
        /*Obtenemos el pid del proceso padre para almacenarlo en el buffer*/
        /*snprinf permite almacenar el valor del 3er argumento en el 1er arg limitantolo a un valor de 5 para seguridad*/
        snprintf(pid_buffer,MAX_FORK,"%d",getpid());
        
        printf("check3\n");
        
        if(FALSE == pid)
        {
            /*Mandar a llamar funcion execl*/
            /*Se generan las ventanas y se manda a llamar el programa .getty*/
            //execl("/usr/bin/xterm","/usr/bin/xterm", "-e","./getty", pid_buffer, (void*)NULL);
            printf("Ventana\n");
        }
        else
        {
            if(p_hijos < TRUE)
            {
                /*Revisa el estado del sistema*/
                wait(&status);
                /*makes the calling thread sleep until seconds seconds have elapsed or a signal arrives which is not ignored*/
                /*Hace que el sistema entre en modo hibernacion hasta que le entre una llamada*/
                sleep(1);
                /*Si sale de modo hibernacion se le aumenta a p_hijos para abrir otra ventana*/
                p_hijos++;
            }
        }
    }
}
