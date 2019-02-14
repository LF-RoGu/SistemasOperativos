all: init getty sh
init:init.c
	gcc -Wall -g -o init init.c
getty:getty.c
	gcc -Wall -g  -o getty getty.c
sh:sh.c
	gcc -Wall -g  -o sh sh.c
