CC = gcc
CFLAGS = -w -O1 -std=c11

all: main.c shmcom.c shmcom.h
	$(CC) $(CFLAGS) main.c shmcom.c -o main

clean:
	rm main