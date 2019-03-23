all: P5_hilos P5_procesos P5_semaforos
P5_hilos:P5_hilos.c
	gcc -Wall -g -o P5_hilos P5_hilos.c -pthread
P5_procesos:P5_procesos.c
	gcc -Wall -g -o P5_procesos P5_procesos.c
P5_semaforos:P5_semaforos.c
	gcc -Wall -g -o P5_semaforos P5_semaforos.c
