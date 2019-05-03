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
    {

		// Lee el marco virtual al buffer

        // Libera el frame virtual
    }


    // Cuenta los marcos asignados al proceso
    i=countframesassigned();

    // Si ya ocupó todos sus marcos, expulsa una página
    if(i>=RESIDENTSETSIZE)
    {
		// Buscar una página a expulsar
		
		// Poner el bitde presente en 0 en la tabla de páginas
        
        // Si la página ya fue modificada, grábala en disco
        {
			// Escribe el frame de la página en el archivo de respaldo y pon en 0 el bit de modificado
        }
		
        // Busca un frame virtual en memoria secundaria
		// Si no hay frames virtuales en memoria secundaria regresa error
		{
            return(-1);
        }
        // Copia el frame a memoria secundaria, actualiza la tabla de páginas y libera el marco de la memoria principal
    }

    // Busca un marco físico libre en el sistema
	// Si no hay marcos físicos libres en el sistema regresa error
    {
        return(-1); // Regresar indicando error de memoria insuficiente
    }

    // Si la página estaba en memoria secundaria
    {
        // Cópialo al frame libre encontrado en memoria principal y transfiérelo a la memoria física
    }
   
	// Poner el bit de presente en 1 en la tabla de páginas y el frame 


    return(1); // Regresar todo bien
}

