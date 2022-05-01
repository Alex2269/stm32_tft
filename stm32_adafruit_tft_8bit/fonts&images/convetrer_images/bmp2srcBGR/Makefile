BIN = bin/bmp2src
SRC = bmp2src.c

all: $(BIN)

$(BIN): $(SRC)
# 	@echo 
	rm -rf bin
	mkdir  bin
	gcc $^ -o $@

clean:
	-rm -fR bin
