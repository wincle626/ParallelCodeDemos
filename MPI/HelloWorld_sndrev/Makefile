nnum = 4
npnum = 4
ppnnum = 2
hostf = ./hosts

all:
	mpicc -o HelloWorld HelloWorld.c
runn:
	mpirun -n $(nnum) ./HelloWorld
runnp:
	mpirun -np $(npnum) ./HelloWorld
runppn:
	mpirun -ppn $(ppnnum) ./HelloWorld
runhostf:
	mpirun -f $(hostf) ./HelloWorld
clean:
	rm -rf HelloWorld
