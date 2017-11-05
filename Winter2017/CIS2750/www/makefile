all: a1 post addauthor view.py
	chmod +x view.py
a1: ClassStorage.o parsing.o
	gcc -o a1 ClassStorage.o parsing.o -ansi -g -Wall
ClassStorage.o: ClassStorage.c
	gcc -Wall -g -c ClassStorage.c
parsing.o: parsing.c
	gcc -Wall -g -c parsing.c
post: output.c stream 
	gcc -Wall -g output.c -lstream -o post -L.
output.c: a1 post.cc 
	./a1 post.cc
stream: stream.o
	ar -cvq libstream.a stream.o
stream.o: stream.c 
	gcc -c stream.c -o stream.o
addauthor: addAuthor.o stream
	gcc addAuthor.o -lstream -o addauthor -L.
addAuthor.o: addAuthor.c
	gcc -Wall -g -c addAuthor.c -o addAuthor.o
clean:
	rm *.o a1 post addauthor tmp.txt output.c libstream.a
cleanStreams:
	rm messages/*