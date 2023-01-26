# Mandelbrot Set Generator

Colloquially, a fractal is a fragmented or broken geometric figure that
can be divided into parts, so that each of them is
(at least approximately) a miniature copy of the whole".

More about fractals: https://en.wikipedia.org/wiki/Fractal

# Mandelbrot set generator using parallel programing methods

The project aims to parallelize Mandelbrot set calculations,
starting from the sequential implementation, with the aim of observing the differences in the different parallel programming methods and their performances.

-   Pthread
-   MPI
-   OpenMP

# Input file format

First line : the complex subspace <br />
Second line : resolution <br />
Third line : numărul maxim de pași de calcul (MAX_STEPS)​ <br />

# Output file format

The result is written as a picture in .ppm format

# MPI installation

```bash
sudo apt install openmpi-bin openmpi-common openmpi-doc libopenmpi-dev
```

# Usage

To compile, run and verify the correctness of the algorithms, you can run the 'check.sh'

```bash
./check.sh
```

The script runs tests on a different number of threads.

How to run specific implementation:

```bash
cd FileOfImplementation
make
```

-   Serial

```bash
./mandelbrot input_file output_file Test0
```

-   Pthreads

```bash
./mandelbrot-pthread input_file output_file threads_no Test0
```

-   MPI

```bash
mpirun --oversubscribe -np process_no ./mandelbrot-mpi input_file output_file Test0
```

-   OpenMP

```bash
./mandelbrot-openmp input_file output_file Test0
```
