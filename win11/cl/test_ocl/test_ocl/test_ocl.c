/*
Sources: http://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/
*/

// openCL headers

#define CL_TARGET_OPENCL_VERSION 120
#define _CRT_SECURE_NO_DEPRECATE

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif



#include <stdio.h>
#include <stdlib.h>


#define MAX_SOURCE_SIZE (0x100000)



int main(int argc, char** argv) {

	int ret;
	int SIZE = 16;

	// Allocate memories for input arrays and output array.
	int* A = (int*)malloc(sizeof(int) * SIZE);
	int* B = (int*)malloc(sizeof(int) * SIZE);

	// Output
	int* C = (int*)malloc(sizeof(int) * SIZE);


	// Initialize values for array members.
	int i = 0;
	for (i = 0; i < SIZE; ++i) {
		A[i] = i;
		B[i] = SIZE - i;
	}

	// Load kernel from file vecAddKernel.cl

	FILE* kernelFile;
	char* kernelSource;
	size_t kernelSize;

	kernelFile = fopen("vecAddKernel.cl", "r");

	if (!kernelFile) {

		fprintf(stderr, "No file named vecAddKernel.cl was found\n");

		exit(-1);

	}
	kernelSource = (char*)malloc(MAX_SOURCE_SIZE);
	kernelSize = fread(kernelSource, 1, MAX_SOURCE_SIZE, kernelFile);
	fclose(kernelFile);

	// Getting platform and device information
	// Getting platform count
	cl_uint platCount;
	clGetPlatformIDs(0, NULL, &platCount);
	printf("there are %d platforms \n", platCount);
	int platind = 1;

	// Allocate memory, get list of platforms
	cl_platform_id* platforms = (cl_platform_id*)malloc(platCount * sizeof(cl_platform_id));
	clGetPlatformIDs(platCount, platforms, NULL);

	cl_uint devCount;
	clGetDeviceIDs(platforms[platind], CL_DEVICE_TYPE_ALL, 0, NULL, &devCount);
	printf("there are %d devices \n", devCount);
	int devind = 1;

	// List of devices in platform
	cl_device_id* devices = (cl_device_id*)malloc(devCount * sizeof(cl_device_id));
	clGetDeviceIDs(platforms[platind], CL_DEVICE_TYPE_ALL, devCount, devices, NULL);

	char buf[256];
	clGetDeviceInfo(devices[devind], CL_DEVICE_NAME, sizeof(buf), buf, NULL);
	printf("device name: '%s'\n", buf);

	// Creating context.
	cl_context context = clCreateContext(NULL, 1, &devices[devind], NULL, NULL, &ret);


	// Creating command queue
	cl_command_queue commandQueue = clCreateCommandQueue(context, devices[devind], 0, &ret);

	// Memory buffers for each array
	cl_mem aMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(int), NULL, &ret);
	cl_mem bMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(int), NULL, &ret);
	cl_mem cMemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, SIZE * sizeof(int), NULL, &ret);


	// Copy lists to memory buffers
	ret = clEnqueueWriteBuffer(commandQueue, aMemObj, CL_TRUE, 0, SIZE * sizeof(int), A, 0, NULL, NULL);;
	ret = clEnqueueWriteBuffer(commandQueue, bMemObj, CL_TRUE, 0, SIZE * sizeof(int), B, 0, NULL, NULL);

	// Create program from kernel source
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, (const size_t*)&kernelSize, &ret);

	// Build program
	ret = clBuildProgram(program, 1, &devices[devind], NULL, NULL, NULL);

	// Create kernel
	cl_kernel kernel = clCreateKernel(program, "addVectors", &ret);


	// Set arguments for kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&aMemObj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&bMemObj);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&cMemObj);


	// Execute the kernel
	size_t globalItemSize = SIZE;
	size_t localItemSize = 64; // globalItemSize has to be a multiple of localItemSize. 1024/64 = 16 
	ret = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, &globalItemSize, &localItemSize, 0, NULL, NULL);

	// Read from device back to host.
	ret = clEnqueueReadBuffer(commandQueue, cMemObj, CL_TRUE, 0, SIZE * sizeof(int), C, 0, NULL, NULL);

	// Write result
	/*
	for (i=0; i<SIZE; ++i) {
		printf("%f + %f = %f\n", A[i], B[i], C[i]);
	}
	*/

	// Test if correct answer
	for (i = 0; i < SIZE; ++i) {
		if (C[i] != SIZE) {
			printf("Something didn't work correctly! Failed test. \n");
			break;
		}
	}
	if (i == SIZE) {
		printf("Everything seems to work fine! \n");
	}

	// Clean up, release memory.
	ret = clFlush(commandQueue);
	ret = clFinish(commandQueue);
	ret = clReleaseCommandQueue(commandQueue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(aMemObj);
	ret = clReleaseMemObject(bMemObj);
	ret = clReleaseMemObject(cMemObj);
	ret = clReleaseContext(context);
	free(A);
	free(B);
	free(C);

	return 0;

}
