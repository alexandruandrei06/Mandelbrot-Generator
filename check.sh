#! /bin/bash


make clean -C ./Mandelbrot_Serial/
make clean -C ./Mandelbrot_MPI/
make clean -C ./Mandelbrot_OpenMP/
make clean -C ./Mandelbrot_Pthread/
make -C ./Mandelbrot_Serial/
make -C ./Mandelbrot_MPI/
make -C ./Mandelbrot_OpenMP/
make -C ./Mandelbrot_Pthread/

echo -e "\nRunning Mandelbrot secvential algorithm ..."

for i in {1..5}
do
    time ./Mandelbrot_Serial/mandelbrot ./input/mandelbrot$i.in ./output/mandelbrot$i.ppm Test$i
done

echo -e "\nRunning Mandelbrot-MPI algorithm ..."

for i in {1..5}
do
    time mpirun --oversubscribe -np 4 ./Mandelbrot_MPI/mandelbrot-mpi ./input/mandelbrot$i.in ./output/mandelbrot-mpi$i.ppm Test$i
done

echo -e "\nRunning Mandelbrot-OpenMP algorithm ..."

for i in {1..5}
do
    time ./Mandelbrot_OpenMP/mandelbrot-openmp ./input/mandelbrot$i.in ./output/mandelbrot-openmp$i.ppm Test$i
done

echo -e "\nRunning Pthreads algorithm ..."

for i in {1..5}
do
    time ./Mandelbrot_Pthread/mandelbrot-pthread ./input/mandelbrot$i.in ./output/mandelbrot-pthread$i.ppm 8 Test$i
done

echo -e "\nChecking outputs for MPI..."
for i in {1..5}
do
    DIFF=$(diff ./output/mandelbrot$i.ppm ./output/mandelbrot-mpi$i.ppm) 
    if [ "$DIFF" != "" ] 
    then
        echo "Test$i failed"
    else
        echo "Test$i passed"
    fi
done

echo -e "\nChecking outputs for OpenMP..."
for i in {1..5}
do
    DIFF=$(diff ./output/mandelbrot$i.ppm ./output/mandelbrot-openmp$i.ppm) 
    if [ "$DIFF" != "" ] 
    then
        echo "Test$i failed"
    else
        echo "Test$i passed"
    fi
done

echo -e "\nChecking outputs for Phreads..."
for i in {1..5}
do
    DIFF=$(diff ./output/mandelbrot$i.ppm ./output/mandelbrot-pthread$i.ppm) 
    if [ "$DIFF" != "" ] 
    then
        echo "Test$i failed"
    else
        echo "Test$i passed"
    fi
done