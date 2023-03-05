FLAGS = -c -Wall -Werror -Wfatal-errors

LIBS = bytes
ALONE = stack

LIB_ROOT = ..
LIB_DIR = $(foreach LIB,$(LIBS),$(LIB_ROOT)/$(LIB))
ALN_DIR = $(foreach LIB,$(ALONE),$(LIB_ROOT)/$(LIB))

LIB_FILES = $(foreach LIB,$(LIB_DIR),$(LIB)/lib.o)
ALN_FILES = $(foreach LIB,$(ALN_DIR),$(LIB)/aln.o)

DBG_FILES = $(foreach LIB,$(LIB_DIR),$(LIB)/debug.o)
ALN_DBG_FILES = $(foreach LIB,$(ALN_DIR),$(LIB)/aln_dbg.o)


build: lib.o



lib.o: tmp.o $(LIB_FILES) $(ALN_FILES)
	ld -r -o $@ $^

.INTERMEDIATE: tmp.o
tmp.o: code.c
	gcc -o $@ $^ $(FLAGS)

.PHONY: $(LIB_FILES)
$(LIB_FILES):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(ALN_FILES)
$(ALN_FILES):
	$(MAKE) alone --directory=$(dir $@)



debug.o: dbg.o $(DBG_FILES) $(ALN_DBG_FILES)
	ld -r -o $@ $^

.INTERMEDIATE: dbg.o
dbg.o: code.c
	gcc -o $@ $^ $(FLAGS) -D DEBUG

.PHONY: $(DBG_FILES)
$(DBG_FILES):
	$(MAKE) debug.o --directory=$(dir $@)

.PHONY: $(ALN_DBG_FILES)
$(ALN_DBG_FILES):
	$(MAKE) alone_debug --directory=$(dir $@)



clean:
	rm -f *.o
	$(MAKE) clean --directory=test
	for DIR in $(LIB_DIR) $(ALN_DIR) ; do \
		( $(MAKE) clean --directory=$$DIR ) || exit $$? ; \
	done

.PHONY: test
test:
	$(MAKE) --directory=test
