CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =

all: TCP_Sender TCP_Receiver

TCP_Sender: TCP_Sender.o
	$(CC) $(LDFLAGS) -o TCP_Sender TCP_Sender.o

TCP_Receiver: Receiver.o
	$(CC) $(LDFLAGS) -o TCP_Receiver TCP_Receiver.o

Sender.o: TCP_Sender.c
	$(CC) $(CFLAGS) -c TCP_Sender.c -o TCP_Sender.o

Receiver.o: TCP_Receiver.c
	$(CC) $(CFLAGS) -c TCP_Receiver.c -o TCP_Receiver.o

.PHONY: clean

clean:
	rm -f sender receiver *.o
