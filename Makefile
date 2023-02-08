SRC = src/main.c
TGT = bin/main.o

FLAGS = -Wall -Werror -Wfatal-errors

run: $(TGT)
	./$(TGT)

build: $(TGT)

$(TGT): $(SRC)
	gcc -o $@ $^ $(FLAGS)

clean:
	rm -f bin/*.o;