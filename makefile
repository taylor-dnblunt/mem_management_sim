CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

all: hole

hole: hole.o hole_func.o min_heap.o
	$(CC) $(CFLAGS) hole.o hole_func.o min_heap.o -o hole

hole.o: hole.c
	$(CC) $(CFLAGS) -c hole.c -o hole.o

hole_func.o: hole_func.c
	$(CC) $(CFLAGS) -c hole_func.c -o hole_func.o

min_heap.o: min_heap.c
	$(CC) $(CFLAGS) -c min_heap.c -o min_heap.o

clean:
	rm *.o hole