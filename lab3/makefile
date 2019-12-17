main:main.o sem.o
	gcc -o main main.o sem.o

get:get.o sem.o
	gcc -o get get.o sem.o

put:put.o sem.o
	gcc -o put put.o sem.o

create_input:create_input.o
	gcc -o create_input create_input.o

%.o:%.c
	gcc -c $< -o $@

clear:
	rm -f *.o
	rm -f main
	rm -f get
	rm -f put
