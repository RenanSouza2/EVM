FLAGS =  -c -Wall -Werror -Wfatal-errors

build: lib.o

lib.o: code.c
	gcc $(FLAGS) -o $@ code.c

debug.o: code.c
	gcc $(FLAGS) -o $@ code.c -D DEBUG

.PHONY: clean
clean:
	rm -f *.o
	$(MAKE) clean --directory=test

.PHONY: test
test:
	$(MAKE) --directory=test
