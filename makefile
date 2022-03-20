CC = gcc
CFLAGS = -std=gnu99 -Wdepantic

all: holes

holes: holes.o
	$(CC) $(CFLAGS) holes.o -o holes

holes.o: holes.c
	$(CC) $(CFLAGS) -c holes.c -o holes.o