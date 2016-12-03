# all

all: tunnel64d.o iftun.o extremite.o
	gcc tunnel64d.o iftun.o extremite.o -o tunnel64d
 
tunnel64d.o: tunnel64d.c iftun.h extremite.h
	gcc -c tunnel64d.c -o tunnel64d.o -W -Wall
 
iftun.o: iftun.c
	gcc -c iftun.c -o iftun.o -W -Wall


extremite.o: extremite.c
	gcc -c extremite.c -o extremite.o -W -Wall


# suppression des fichiers temporaires
clean:
	rm -rf *.o
