lab2:main.o sem.o
	gcc -o lab2 main.o sem.o -lpthread

main.o:main.c
	gcc -c main.c -o main.o

sem.o:sem.c
	gcc -c sem.c -o sem.o

clear:
	rm -f *.o
	rm -f lab2
