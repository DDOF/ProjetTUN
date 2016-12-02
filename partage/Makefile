# all

all: main.o iftun.o extremite.o
	gcc main.o iftun.o extremite.o -o main
 
main.o: main.c iftun.h extremite.h
	gcc -c main.c -o main.o -W -Wall -v
 
iftun.o: iftun.c
	gcc -c iftun.c -o iftun.o -W -Wall -v


extremite.o: extremite.c
	gcc -c extremite.c -o extremite.o -W -Wall -v


# suppression des fichiers temporaires
clean:
	rm -rf *.o
