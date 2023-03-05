FLAGS = -c -Wall -Werror -Wfatal-errors

LIBS = bytes32

LIB_ROOT = ..
LIB_DIR = $(foreach LIB,$(LIBS),$(LIB_ROOT)/$(LIB))
LIB_FILES = $(foreach LIB,$(LIB_DIR),$(LIB)/lib.o)
DBG_FILES = $(foreach LIB,$(LIB_DIR),$(LIB)/debug.o)

build: lib.o
alone: aln.o
debug: debug.o
alone_debug: aln_dbg.o

lib.o: aln.o $(LIB_FILES)
	ld -r -o $@ $^

aln.o: code.c
	gcc -o $@ $^ $(FLAGS)

.PHONY: $(LIB_FILES)
$(LIB_FILES):
	$(MAKE) --directory=$(dir $@)




debug.o: aln_dbg.o $(DBG_FILES)
	ld -r -o $@ $^

aln_dbg.o: code.c
	gcc -o $@ $^ $(FLAGS) -D DEBUG

.PHONY: $(DBG_FILES)
$(DBG_FILES):
	$(MAKE) debug.o --directory=$(dir $@)



clean:
	rm -f *.o
	$(MAKE) clean --directory=test
	for DIR in $(LIB_DIR) ; do \
		$(MAKE) clean --directory=$$DIR ; \
	done

.PHONY: test
test:
	$(MAKE) --directory=test
