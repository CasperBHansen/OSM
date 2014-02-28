CC=gcc
CFLAGS=-Wall -Werror

all: main

stack.o: stack.h stack.c
	$(CC) $(CFLAGS) -c stack.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

main: stack.o main.o
	$(CC) $(CFLAGS) -o main	stack.o main.o

clean:
	rm main *.o
