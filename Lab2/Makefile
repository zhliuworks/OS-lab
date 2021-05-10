CC = gcc
CFLAGS = -w -O1 -std=c11

all: pccom/pccom msgcom/msgclient msgcom/msgserver
	make pccom
	make msgclient
	make msgserver
	make -C shmcom
	
pccom: pccom/pccom.c
	$(CC) $(CFLAGS) pccom/pccom.c -o pccom/pccom

msgclient: msgcom/msgclient.c msgcom/msgcom.h
	$(CC) $(CFLAGS) msgcom/msgclient.c -o msgcom/msgclient

msgserver: msgcom/msgserver.c msgcom/msgcom.h
	$(CC) $(CFLAGS) msgcom/msgserver.c -o msgcom/msgserver

clean:
	rm pccom/pccom
	rm msgcom/msgclient
	rm msgcom/msgserver
	make -C shmcom clean