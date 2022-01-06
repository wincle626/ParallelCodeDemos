/usr/local/opt/mpich/bin/mpicc -o test_mpich test_mpich.c
/usr/local/opt/mpich/bin/mpirun -np 4 ./test_mpich
rm -rf test_mpich
rm -rf test_mpich.dSYM
