#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#define FALSE 0
#define TRUE 1
#define ERROR -1

#define XTERM_PATH "/usr/bin/xterm"
#define XTERM "xterm"

int main() 
{
	/**/
	int exec_status = 0;
    /*Variable para el control de los procesos hijos*/
    int p_hijos = 0;
    int pid = 0;
    int status;
    /*Almacenamos los pids de los procesos hijos para tener un control*/
    char pid_buffer[20] = {0};
    
    while(TRUE)
    {
        /*Variable donde se almacenara el pid de cada proceso hijo*/
        pid = fork();
        /*Restamos 1 a la variable para el control de los procesos hijos*/
        p_hijos++;
        
        /*Obtenemos el pid del proceso padre para almacenarlo en el buffer*/
        /*snprinf permite almacenar el valor del 3er argumento en el 1er arg limitantolo a un valor de 5 para seguridad*/
        /*getppid() obtiene el pid del proceso padre del proceso actual*/
        sprintf(pid_buffer,"%d",getppid());
        
        if(FALSE == pid)
        {
            /*Mandar a llamar funcion execl*/
            /*Se generan las ventanas y se manda a llamar el programa .getty*/
            exec_status = execl(XTERM_PATH,XTERM,"-hold","-e","./getty","getty",(char*)NULL);
            if(ERROR == exec_status)
            {
				perror("Failed to execute");
			}
        }
        else
        {
            if(p_hijos >= 6)
            {
                /*Revisa el estado del sistema*/
                wait(&status);
                /*makes the calling thread sleep until seconds seconds have elapsed or a signal arrives which is not ignored*/
                /*Hace que el sistema entre en modo hibernacion hasta que le entre una llamada*/
                sleep(1);
                /*Si sale de modo hibernacion se le aumenta a p_hijos para abrir otra ventana*/
                p_hijos--;
            }
        }
    }
}
