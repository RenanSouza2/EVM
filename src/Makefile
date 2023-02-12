SRC  = main.c
ROOT = ../
OBJ  = $(ROOT)/bin/main.o

FLAGS=-Wall -Wall -Werror -Wfatal-errors

LIB = $(ROOT)/lib

UINT_PATH = $(LIB)/uint
UINT_FILE = $(UINT_PATH)/lib.o

build: $(OBJ)

$(OBJ): main.c $(UINT_FILE)
	gcc $(FLAGS) -o $(OBJ) $^

.PHONY: $(UINT_FILE)
$(UINT_FILE):
	$(MAKE) --directory $(UINT_PATH)

clean:
	rm -rf $(OBJ)
