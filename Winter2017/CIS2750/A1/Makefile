all: a1
a1: ClassStorage.o parsing.o
	gcc -o a1 ClassStorage.o parsing.o -ansi -g -Wall
ClassStorage.o: ClassStorage.c
	gcc -Wall -g -c ClassStorage.c
parsing.o: parsing.c
	gcc -Wall -g -c parsing.c

clean:
	rm *.o a1