CC=g++

CFLAGS=-c
INCDIR=-I.

all: main

main: main.o 
	$(CC) main.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCDIR) main.cpp

clean:
	rm -f *o main