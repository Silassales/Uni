all: a1 post addauthor configfile view.py db
	chmod +x view.py
db: db.o
	gcc db.o -o db -lmysqlclient -L/usr/lib/x86_64-linux-gnu/ -I/usr/include/msql
db.o: db.c
	gcc db.c -o db.o -c 
a1: ClassStorage.o parsing.o
	gcc -o a1 ClassStorage.o parsing.o -ansi -g -Wall
ClassStorage.o: ClassStorage.c
	gcc -Wall -g -c ClassStorage.c
parsing.o: parsing.c
	gcc -Wall -g -c parsing.c
post: output.c stream 
	gcc -Wall -g output.c -lstream -o post -L. `mysql_config --cflags --libs`
output.c: a1 post.cc 
	./a1 post.cc
stream: stream.o
	ar -cvq libstream.a stream.o 
stream.o: stream.c 
	gcc -c stream.c -o stream.o -L/usr/lib/mysql -lmysqlclient
addauthor: addAuthor.o stream
	gcc addAuthor.o -lstream -o addauthor -L. `mysql_config --cflags --libs`
addAuthor.o: addAuthor.c
	gcc -Wall -g -c addAuthor.c -o addAuthor.o
configfile: webConfig.c 
	gcc webConfig.c -o configfile
clean:
	rm *.o a1 db post configfile addauthor tmp.txt output.c libstream.a