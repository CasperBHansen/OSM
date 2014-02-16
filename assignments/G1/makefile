# compilation macros
OUT=doubly-linked-list
CC=gcc
LDFLAGS=-Wall -pedantic -std=c99
SRC=*.c
OBJ=*.o

# package macros
PACK_NAME=hansen_hansen_hovgaard
PACK_FILES=makefile *.{c,h} report/{makefile,*.{tex,pdf}} ../../src

build:
	$(CC) -c $(LDFLAGS) $(SRC)
	$(CC) $(OBJ) -o $(OUT)

run:
	./$(OUT)

pack:
	zip $(PACK_NAME)_$(shell basename ${PWD}) -r $(PACK_FILES)

clean:
	rm -rf *.o $(PACK_NAME).zip .*.*.{swp,swo} .*.{swp,swo}

.PHONY: clean run

