FLAGS = -Wall -Werror -Wfatal-errors
DEBUG_FILE = ../$(LIB_ROOT)/debug.o

test: test.o
	./test.o 

test.o: test.c $(DEBUG_FILE)
	gcc -o $@ $^ $(FLAGS) -D DEBUG

.PHONY: $(DEBUG_FILE)
$(DEBUG_FILE):
	$(MAKE) debug.o --directory=$(dir $@)

clean:
	rm -rf test.o
