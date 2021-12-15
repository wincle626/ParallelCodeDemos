#define CL_TARGET_OPENCL_VERSION 120

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

void EnqueueKernel(cl_platform_id platform, cl_uint numOfPlatforms);
void loadProgramSource(const char** filename, int filenumber,
		char** filebuffer, size_t* size);

int main(int argc, char** argv){
	
   
	/* OpenCL 1.1 scalar data types */
	cl_uint numOfPlatforms;
	cl_int  error;

	/*
	Get the number of platforms
	Remember that for each vendor's SDK installed on the
	Computer, the number of available platform also
	increased.
	*/
	error = clGetPlatformIDs(0, NULL, &numOfPlatforms);
	if(error < 0) {
		perror("Unable to find any OpenCL platforms");
		exit(1);
	}
	// Allocate memory for the number of installed platforms.
	// alloca(...) occupies some stack space but is
	// automatically freed on return
	cl_platform_id* platforms = (cl_platform_id*) alloca(sizeof(cl_platform_id)
					* numOfPlatforms);

	error = clGetPlatformIDs(numOfPlatforms, platforms, NULL);
	if(error < 0) {
		perror("Unable to find any OpenCL platforms");
		exit(1);
	}
	printf("Number of OpenCL platforms found: %d\n", numOfPlatforms);


	// Search for a CPU/GPU device through the installed
   	// platform. Build a OpenCL program and do not run it.
   	for(cl_uint i = 0; i < numOfPlatforms; i++ ) {
		printf("On the %dth platform. \n");
		EnqueueKernel(platforms[i], numOfPlatforms);
	}

	return 0;
}

void EnqueueKernel(cl_platform_id platform, cl_uint numOfPlatforms) {

    	// Create host variables
    	int i;
    	const int LIST_SIZE = 16;
    	int *A = (int*)malloc(sizeof(int)*LIST_SIZE);
    	int *B = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
    	for(i = 0; i < LIST_SIZE; i++) {
        	A[i] = i + 1;
        	B[i] = LIST_SIZE - i + 1;
	}

   
	cl_device_id device;
	cl_int error;
       	// Get the GPU device
	error = clGetDeviceIDs(platform, 
                              CL_DEVICE_TYPE_GPU, 1,
               	              &device, NULL);
	if(error != CL_SUCCESS) {
        	// Otherwise, get the CPU
        	error = clGetDeviceIDs(platform,
               		                 CL_DEVICE_TYPE_CPU,
                       		         1, &device, NULL);
		printf("Found a CPU.\n");
       	}else{
		printf("Found a GPU.\n");
	}
        if(error != CL_SUCCESS) {
        	perror("Can't locate any OpenCL compliant device");
        	exit(1);
        }
        /* Create a context */
        cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL,
	       	                   &error);
        if(error != CL_SUCCESS) {
		perror("Can't create a valid OpenCL context");
        	exit(1);
        }

    	// Create memory buffers on the device for each vector
    	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
        	LIST_SIZE * sizeof(int), NULL, &error);
    	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
        	LIST_SIZE * sizeof(int), NULL, &error);
    	cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
       		LIST_SIZE * sizeof(int), NULL, &error);

        /* Load the two source files into temporary 
        datastores */
        const char *file_names[] = {"kernel1.cl",
        	                     "kernel2.cl"};
        const int NUMBER_OF_FILES = 2;
        char* buffer[NUMBER_OF_FILES];
        size_t sizes[NUMBER_OF_FILES];
        loadProgramSource(file_names, NUMBER_OF_FILES, buffer,
               	          sizes);
        /* Create the OpenCL program object */
       	cl_program program = clCreateProgramWithSource(context,
               	                            NUMBER_OF_FILES,
                       	                    (const
                               	             char**)buffer,
                                       	    sizes, &error);      
	if(error != CL_SUCCESS) {
       		perror("Can't create the OpenCL program object");
       		exit(1);   
	}
       	error = clBuildProgram(program, 1, &device, NULL,
			NULL, NULL);
	if(error != CL_SUCCESS) {
       		perror("Failed to build the program\n");
       		exit(1);
	}

	/* Create multiple kernel from program without knowing
	 * the function name
	 * */
	cl_uint numOfKernels;
        error = clCreateKernelsInProgram(program, 0, NULL,
                                         &numOfKernels);
        if (error != CL_SUCCESS) {
            perror("Unable to retrieve kernel count from program");
            exit(1);
        }

	cl_kernel* kernels = (cl_kernel*)
                             alloca(sizeof(cl_kernel) * numOfKernels);
        error = clCreateKernelsInProgram(program, numOfKernels,
                                         kernels, NULL);

        /* Release the kernels */
        for(cl_uint i = 0; i < numOfKernels; i++)
            clReleaseKernel(kernels[i]);

	/* Create single kernel with specific function name
	 * */
	cl_uint argCnt1;
	cl_kernel kernel1, kernel2, kernel3, kernel4;
	kernel1 = clCreateKernel(program,"vector_add",&error);
	kernel2 = clCreateKernel(program,"vector_sub",&error);
	kernel3 = clCreateKernel(program,"vector_mul",&error);
	kernel4 = clCreateKernel(program,"vector_div",&error);
	

    	// Set the arguments of the kernel
    	clSetKernelArg(kernel1, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    	clSetKernelArg(kernel1, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    	clSetKernelArg(kernel1, 2, sizeof(cl_mem), (void *)&c_mem_obj);

	/* Create a command queue */
	cl_command_queue command_queue = clCreateCommandQueue(
						   context,
						   device,
						   0,
						   &error);
	if ( error != CL_SUCCESS ){
		printf("Can't create a command queue\n");
		exit(1);
	}

    	// Copy the lists A and B to their respective memory buffers
    	clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
        	LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
    	clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0,
        	LIST_SIZE * sizeof(int), B, 0, NULL, NULL);
	
	// Execute the OpenCL kernel on the list
    	size_t global_item_size = LIST_SIZE; // Process the entire lists
    	size_t local_item_size = 4; // Divide work items into groups of 
	clEnqueueNDRangeKernel(command_queue, kernel1, 1, NULL,
            	&global_item_size, &local_item_size, 0, NULL, NULL);


    	// Read the memory buffer C on the device to the local variable C
    	clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0,
        	LIST_SIZE * sizeof(int), C, 0, NULL, NULL);

    	// Display the result to the screen
    	for(i = 0; i < LIST_SIZE; i++)
        	printf("%d + %d = %d\n", A[i], B[i], C[i]);

       	/* Clean up */
       	for(int i=0; i< NUMBER_OF_FILES; i++) { 
		free(buffer[i]);
	}
    	clFlush(command_queue);
    	clFinish(command_queue);
       	clReleaseProgram(program);
    	clReleaseKernel(kernel1);
	clReleaseKernel(kernel2);
	clReleaseKernel(kernel3);
	clReleaseKernel(kernel4);
    	clReleaseMemObject(a_mem_obj);
    	clReleaseMemObject(b_mem_obj);
    	clReleaseMemObject(c_mem_obj);
    	clReleaseCommandQueue(command_queue);
    	clReleaseContext(context);
    	free(A);
    	free(B);
    	free(C);
      
}

void loadProgramSource(const char** filename, int filenumber, 
		char** filebuffer, size_t* filesize){
	for(cl_uint i=0;i<filenumber;i++){		
		FILE *fp;
		char *source_str;
    		size_t source_size;
    		fp = fopen(filename[i], "r");
    		if (!fp) {
        		fprintf(stderr, "Failed to load kernel.\n");
        		exit(1);
    		}
    		filebuffer[i] = (char*)malloc(MAX_SOURCE_SIZE);
    		filesize[i] = fread( filebuffer[i], 1, MAX_SOURCE_SIZE, fp);
		//printf("%s\n", filebuffer[i]);
		fclose( fp );
	}
}
