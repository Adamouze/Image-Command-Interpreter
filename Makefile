prog : projet.o
	gcc -Wall -o prog projet.o

projet.o : projet.c projet.h
	gcc -Wall -c projet.c

clean :
	rm -v *.o
	rm -v prog