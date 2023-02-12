BIN = bin
TGT = $(BIN)/main.o

run: $(TGT)
	./$(TGT)

build: $(TGT)
	
.PHONY: $(TGT)
$(TGT):
	$(MAKE) --directory=src

clean:
	rm -rf $(BIN)/*.o
	$(MAKE) clean --directory=lib
	
.PHONY: test
test: 
	$(MAKE) test --directory=lib
