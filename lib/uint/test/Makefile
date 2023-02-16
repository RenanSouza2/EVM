FLAGS = -Wall -Werror -Wfatal-errors

test: test.o
	./test.o 

test.o: test.c
	gcc $(FLAGS) test.c -o test.o

clean:
	rm -rf test.o
