all:
	mpicc -Wall -o MatMul MatMul.c
run:
	mpirun -np 4 ./MatMul 2
clean:
	rm -rf MatMul
