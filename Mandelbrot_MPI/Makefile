CC = mpicc
CFLAGS = -g -Wall -Wextra
LOADLIBES=-lm

EXECUTABLES = mandelbrot-mpi

.PHONY: all
all: $(EXECUTABLES)

$(EXECUTABLES): util.o

util.o: util.h

.PHONY: clean
clean:
	rm -vf *.o $(EXECUTABLES) ../output/*

run:
	mpirun --oversubscribe -np 4 mandelbrot-mpi ../input/mandelbrot1.in ../output/mandelbrot1-mpi.ppm