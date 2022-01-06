/usr/local/opt/llvm/bin/clang -o test_openmp test_openmp.c -fopenmp -L/usr/local/opt/llvm/lib
./test_openmp
rm -rf test_openmp
