CC=gcc
CFLAGS=-Wall -Werror
PROG=main

all: $(PROG)

stack.o: stack.h stack.c
	$(CC) $(CFLAGS) -c stack.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

main: stack.o main.o
	$(CC) $(CFLAGS) -o main	stack.o main.o

# examples
PThread_1_hello: PThread_1_hello.o
	$(CC) $(CFLAGS) -o PThread_1_hello stack.o PThread_1_hello.o

PThread_condvar: PThread_condvar.o
	$(CC) $(CFLAGS) -o PThread_condvar stack.o PThread_condvar.o

PThread_matmult: PThread_matmult.o
	$(CC) $(CFLAGS) -o PThread_matmult stack.o PThread_matmult.o

run:
	./$(PROG)

clean:
	rm main PThread_1_hello PThread_condvar PThread_matmult *.o

.PHONY: clean run
