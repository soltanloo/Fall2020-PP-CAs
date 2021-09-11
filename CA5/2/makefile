CC=g++
CCFLAGS =-Wall
LNKFLAGS=-pthread
INCDIR=-I/opt/intel/ipp/include
LIBDIR=-L/opt/intel/ipp/lib
LIBFILE=-lippcore

all: main

main: main.o
	$(CC) $(CCFLAGS) $(LIBDIR) $(LIBFILE) -o main main.o -pthread

main.o: main.cpp
	$(CC) $(CCFLAGS) $(INCDIR) -c main.cpp -pthread

clean:
	- rm *.o
	- rm main