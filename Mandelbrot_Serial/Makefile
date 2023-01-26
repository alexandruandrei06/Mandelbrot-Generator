CC = gcc
CFLAGS = -g -Wall -Wextra
LOADLIBES=-lm

EXECUTABLES = mandelbrot

.PHONY: all
all: $(EXECUTABLES)

$(EXECUTABLES): util.o

util.o: util.h

.PHONY: clean
clean:
	rm -vf *.o $(EXECUTABLES) ../output/*
