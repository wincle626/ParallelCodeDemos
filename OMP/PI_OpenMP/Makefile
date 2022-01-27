all: compile
	gcc -fopenmp -o pi_openmp main.c  -I./headers ./sources/atomicparallel.o ./sources/criticalparallel.o ./sources/datastructparallel.o ./sources/ompforparallel.o ./sources/padparallel.o ./sources/parallel.o ./sources/reductionparallel.o ./sources/scheduleparallel.o ./sources/sequential.o
	
compile:
	gcc -fopenmp -O0 -Wall -c -fmessage-length=0 -o ./sources/atomicparallel.o ./sources/atomicparallel.c -I./headers
	gcc -fopenmp -O0 -Wall -c -fmessage-length=0 -o ./sources/criticalparallel.o ./sources/criticalparallel.c -I./headers
	gcc -fopenmp -O0 -Wall -c -fmessage-length=0 -o ./sources/datastructparallel.o ./sources/datastructparallel.c -I./headers
	gcc -fopenmp -O0 -Wall -c -fmessage-length=0 -o ./sources/ompforparallel.o ./sources/ompforparallel.c -I./headers
	gcc -fopenmp -O0 -Wall -c -fmessage-length=0 -o ./sources/padparallel.o ./sources/padparallel.c -I./headers
	gcc -fopenmp -O0 -Wall -c -fmessage-length=0 -o ./sources/parallel.o ./sources/parallel.c -I./headers
	gcc -fopenmp -O0 -Wall -c -fmessage-length=0 -o ./sources/reductionparallel.o ./sources/reductionparallel.c -I./headers
	gcc -fopenmp -O0 -Wall -c -fmessage-length=0 -o ./sources/scheduleparallel.o ./sources/scheduleparallel.c -I./headers
	gcc -fopenmp -O0 -Wall -c -fmessage-length=0 -o ./sources/sequential.o ./sources/sequential.c -I./headers

clean:
	rm -rf ./sources/*.o
	rm -rf pi_openmp
