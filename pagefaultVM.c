#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mmu.h>

#define NUMPROCS 4
#define PAGESIZE 4096
#define PHISICALMEMORY 12*PAGESIZE 
#define TOTFRAMES PHISICALMEMORY/PAGESIZE
#define RESIDENTSETSIZE PHISICALMEMORY/(PAGESIZE*NUMPROCS)

extern char *base;
extern int framesbegin;
extern int idproc;
extern int systemframetablesize;
extern int ptlr;

extern struct SYSTEMFRAMETABLE *systemframetable;
extern struct PROCESSPAGETABLE *ptbr;

/*Funciones provenientes de mmu.c*/
/** Descripcion de la funcion
 * La función copyframe(int sframe,int dframe) realiza la copia de un marco a otro
	marco dentro del archive de intercambio donde sframe es el marco fuente y dframe es el
	marco destino.
 */
extern int copyframe(int sframe,int dframe);
/** Descripcion de la funcion
 * La función writeblock(char *buffer, int dblock) escribe un bloque que es del
	mismo tamaño de los marcos y páginas en el archivo de intercambio. El apuntador buffer
	apunta a la memoria lo que se tiene que escribir y dblock es el bloque a escrinir que
	corresponde a un marco en el archivo de intercambio.
 */
extern int writeblock(char *buffer, int dblock);
/** Descripcion de la funcion
 * La función readblock(char *buffer, int dblock) lee un bloque que es del mismo
	tamaño de los marcos y páginas del archivo de intercambio. El apuntador buffer apunta a la
	memoria donde se almacenarán los datos leídos y sblock es el bloque a leer que corresponde a
	un marco en el archivo de intercambio.
 */
extern int readblock(char *buffer, int sblock);
/** Descripcion de la funcion
 * La función loadframe(int frame) transfiere al marco físico de la memoria física el
	contenido que corresponde al marco del archivo de intercambio. El parámetro frame es el
	marco físico que será cargado de su respaldo en el archivo de intercambio.
 */
extern int loadframe(int frame);
/** Descripcion de la funcion
 * La función saveframe(int frame) transfiere del marco frame de la memoria física al
	bloque del archivo de intercambio que le corresponde a ese mismo marco físico. El parámetro
	frame siempre debe ser un marco físico.
 */
extern int saveframe(int frame);
/*Fin de funciones provenientes de mmu.c*/

int getfreeframe();
int searchvirtualframe();
int getfifo();

int pagefault(char *vaddress)
{
    int i;
    int frame,vframe; 
    long pag_a_expulsar;
    int fd;
    char buffer[PAGESIZE];
    int pag_del_proceso;

    // A partir de la dirección que provocó el fallo, calculamos la página
    pag_del_proceso=(long) vaddress>>12;


    // Si la página del proceso está en un marco virtual del disco
    if((ptbr + pag_del_proceso)->presente != 1 && (ptbr + pag_del_proceso)->framenumber != -1)
    {
        frame = (ptbr + pag_del_proceso)->framenumber;
		// Lee el marco virtual al buffer
        readblock(buffer,frame);
        // Libera el frame virtual
        systemframetable[frame].assigned=0;
    }


    // Cuenta los marcos asignados al proceso
    i=countframesassigned();

    // Si ya ocupó todos sus marcos, expulsa una página
    if(i>=RESIDENTSETSIZE)
    {
		// Buscar una página a expulsar
		pag_a_expulsar = getfifo();
		// Poner el bitde presente en 0 en la tabla de páginas
        (ptbr + pag_a_expulsar)->presente=0;
        // Si la página ya fue modificada, grábala en disco
        if((ptbr + pag_a_expulsar)->modificado == 1)        
        {
            frame = (ptbr + pag_a_expulsar)->framenumber;
            saveframe(frame);
            pag_a_expulsar = getfifo();
			// Escribe el frame de la página en el archivo de respaldo y pon en 0 el bit de modificado
            (ptbr + pag_a_expulsar)->modificado=0;
        }
		
        // Busca un frame virtual en memoria secundaria
        vframe = searchvirtualframe();
		// Si no hay frames virtuales en memoria secundaria regresa error
        if(vframe == -1)
		{
            return(-1);
        }
        // Copia el frame a memoria secundaria, actualiza la tabla de páginas y libera el marco de la memoria principal
        copyframe(frame,vframe);
        (ptbr + pag_a_expulsar)->presente=0;
        (ptbr + pag_a_expulsar)->framenumber = vframe;
        systemframetable[frame].assigned = 0;
        systemframetable[vframe].assigned = 1;
    }

    // Busca un marco físico libre en el sistema
    frame = getfreeframe();

	if(ptbr[pag_del_proceso].framenumber==-1){
    // Si no hay marcos físicos libres en el sistema regresa error
    if (frame == -1)
    {
        return (-1); // Regresar indicando error de memoria insuficiente
    }
	}
    // Si la página estaba en memoria secundaria
    else{
        // Cópialo al frame libre encontrado en memoria principal y transfiérelo a la memoria física
       // copyframe(vframe,frame);
	writeblock(buffer,frame);
    }
   
	// Poner el bit de presente en 1 en la tabla de páginas y el frame 
    (ptbr+pag_del_proceso)->presente=1;
    (ptbr+pag_del_proceso)->framenumber=frame;
    return(1); // Regresar todo bien
}


int getfreeframe()
{
	int count_frame;

	// Recorrer la tabla de frames para encontrar uno disponible
	for(count_frame = framesbegin ; count_frame < (systemframetablesize + framesbegin); count_frame++)
	{
		//Si ecuentra un frame que está sin asignar, termina la búsqueda        	
		if(0 == systemframetable[count_frame].assigned)
        	{
			systemframetable[count_frame].assigned=1;
			break;
        	}
	}

	if(count_frame < (systemframetablesize + framesbegin))
        	systemframetable[count_frame].assigned = 1;
	else
        	count_frame = -1;

	return(count_frame);
}

int searchvirtualframe()
{
int i;
    for ( i = framesbegin + systemframetablesize; i < 2*systemframetablesize  + framesbegin; i++)

        if (!systemframetable[i].assigned)
        {
            systemframetable[i].assigned = 1;
            break;
        }

    if (i >= systemframetablesize + framesbegin && i < (systemframetablesize * 2) + framesbegin)
    {
        systemframetable[i].assigned = 1;
    }
    else
        i = -1;
    return (i);
}
int getfifo()
{
    int ret, tiempo=-1,i;
    
    for (i = 0; i < ptlr ; i++)
    {

        if ((ptbr + i)->presente == 1)
        {

            if ((ptbr + i)->tarrived <= tiempo)
            {
                ret = i;
                tiempo = (ptbr + i)->tarrived;
            }
        }
    }
    return ret;
}

