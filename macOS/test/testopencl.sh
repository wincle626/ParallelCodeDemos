/usr/local/opt/llvm/bin/clang -o test_opencl test_opencl.c -framework OpenCL
./test_opencl
rm -rf test_opencl
