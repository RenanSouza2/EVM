FLAGS=-Wall -Werror -Wfatal-errors

UINT_PATH=../../uint
UINT_FILE=$(UINT_PATH)/debug.o

.PHONY: test
test: test.o
	./test.o 

.PHONY: test.o
test.o: test.c $(UINT_FILE)
	gcc $(FLAGS) -o $@ $^ -D DEBUG

.PHONY: $(UINT_FILE)
$(UINT_FILE):
	$(MAKE) debug.o --directory=$(UINT_PATH)

clean:
	rm -rf test.o
