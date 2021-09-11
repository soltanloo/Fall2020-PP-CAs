CC=g++-10

CFLAGS=-c -fopenmp
LNKFLAGS=-fopenmp 
INCDIR=-I/opt/intel/ipp/include
LIBDIR=-L/opt/intel/ipp/lib
LIBFILE=-lippcore

all: main

main: main.o 
	$(CC) $(LNKFLAGS) $(LIBDIR) $(LIBFILE) main.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCDIR) main.cpp

clean:
	rm -f *o main