CC=gcc
CFLAGS=-Wall -Werror
PROG=main

build:
	$(CC) $(CFLAGS) $(PROG).c stack.c -o program

example:
	$(CC) $(CFLAGS) examples/$(PROG).c stack.c -o program

run:
	./program

clean:
	rm program *.o
