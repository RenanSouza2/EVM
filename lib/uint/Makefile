FLAGS = -c -Wall -Werror -Wfatal-errors

LIB_ROOT = ..

build: lib.o
debug: debug.o

lib.o: code.c
	gcc -o $@ $^ $(FLAGS)

debug.o: code.c
	gcc -o $@ $^ $(FLAGS) -D DEBUG

clean:
	rm -f *.o
	$(MAKE) clean --directory=test

.PHONY: test
test:
	$(MAKE) --directory=test

export
