all: P3_1 P3_2 P3_3 P3_4
P3_1:P3_1.c
	gcc -Wall -g -o P3_1 P3_1.c -lm -pthread
P3_2:P3_2.c
	gcc -Wall -g -o P3_2 P3_2.c -lm -pthread
P3_3:P3_3.c
	gcc -Wall -g -o P3_3 P3_3.c -lm -pthread
P3_4:P3_4.c
	gcc -Wall -g -o P3_4 P3_4.c -lm -pthread
