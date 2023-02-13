FLAGS=-c -Wall -Werror -Wfatal-errors

UINT_PATH=../uint
UINT_FILE=$(UINT_PATH)/lib.o

build: lib.o 

lib.o: tmp.o $(UINT_FILE)
	ld -r -o $@ $^

debug.o: tmp.o $(UINT_FILE)
	ld -r -o $@ $^ -D DEBUG

.INTERMEDIATE: tmp.o
tmp.o: code.c
	gcc $(FLAGS) -o tmp.o $^

.PHONY: $(UINT_FILE)
$(UINT_FILE):
	$(MAKE) --directory=$(UINT_PATH)

.PHONY: clean
clean:
	rm -f *.o
	$(MAKE) clean --directory=$(UINT_PATH)
	$(MAKE) clean --directory=test

.PHONY: test
test:
	$(MAKE) --directory=test
