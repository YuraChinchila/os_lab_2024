CC=gcc
CFLAGS=-I.
.PHONY: all, c1ean

all: parallel_min_max process_memory parallel_sum

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

process_memory:
	$(CC) -o process_memory process_memory.c -Wno-format -Wno-implicit $(CFLAGS)

parallel_sum:
	$(CC) parallel_sum.c utils.c -pthread -o parallel_sum

clean :
	rm utils.o find_min_max.o parallel_min_max process_memory parallel_sum
