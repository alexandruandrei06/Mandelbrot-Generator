CC = gcc
CFLAGS = -g -Wall -Wextra -fopenmp
LOADLIBES=-lm

EXECUTABLES = mandelbrot-openmp

.PHONY: all
all: $(EXECUTABLES)

$(EXECUTABLES): util.o

util.o: util.h

.PHONY: clean
clean:
	rm -vf *.o $(EXECUTABLES) ../output/*

run:
	./mandelbrot-openmp ../input/mandelbrot1.in ../output/mandelbrot1-openmp.ppm Test1