CC = gcc
CFLAGS = -std=gnu99 -Wpedantic -ggdb3

all: holes

holes: holes.o holes_func.o
	$(CC) $(CFLAGS) holes.o holes_func.o -o holes

holes.o: holes.c
	$(CC) $(CFLAGS) -c holes.c -o holes.o

holes_func.o: holes_func.c
	$(CC) $(CFLAGS) -c holes_func.c -o holes_func.o