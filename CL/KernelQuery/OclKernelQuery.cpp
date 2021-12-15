#define CL_TARGET_OPENCL_VERSION 120

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

void DisplayKernelWorkGroupInfo(cl_kernel kernel, cl_device_id device);
void KernelQuery(cl_platform_id platform, cl_uint numOfPlatforms);
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
		KernelQuery(platforms[i], numOfPlatforms);
	}

	return 0;
}

void DisplayKernelWorkGroupInfo(cl_kernel kernel, cl_device_id device){

	cl_int error;
	size_t maxglobalworksize[3], maxworkgroupsize;
	error = clGetKernelWorkGroupInfo(kernel, device,
			CL_KERNEL_GLOBAL_WORK_SIZE,
			sizeof(size_t)*3, maxglobalworksize,
			NULL);
	printf("Maximum global_work_size to clEnqueueNDRangeKernel():\n"
		"%ld-%ld-%ld\n",
		maxglobalworksize[0],maxglobalworksize[1],maxglobalworksize[2]);
	
	error = clGetKernelWorkGroupInfo(kernel, device,
			CL_KERNEL_WORK_GROUP_SIZE,
			sizeof(size_t), maxglobalworksize,
			NULL);
	printf("Maximum workgroup size: %d\n", maxworkgroupsize);
	
}

void KernelQuery(cl_platform_id platform, cl_uint numOfPlatforms) {
   
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
	printf("\nCreate multiple kernel from built program object:\n");
	cl_uint numOfKernels;
        error = clCreateKernelsInProgram(program, 0, NULL,
                                         &numOfKernels);
        if (error != CL_SUCCESS) {
            perror("Unable to retrieve kernel count from program");
            exit(1);
        }
	printf("There %d kernels in the program.\n");

	cl_kernel* kernels = (cl_kernel*)
                             alloca(sizeof(cl_kernel) * numOfKernels);
        error = clCreateKernelsInProgram(program, numOfKernels,
                                         kernels, NULL);

        for(cl_uint i = 0; i < numOfKernels; i++) {
		printf("\n%dth kernel:");
        	char kernelName[32];
            	cl_uint argCnt;
            	clGetKernelInfo(kernels[i],
                	        CL_KERNEL_FUNCTION_NAME,
                        	sizeof(kernelName),
                            	kernelName, NULL);
            	clGetKernelInfo(kernels[i], CL_KERNEL_NUM_ARGS,
                	        sizeof(argCnt), &argCnt, NULL);
            	printf("Kernel name: %s with arity: %d\n",
                    	kernelName,
                    	argCnt);
        }
        /* Release the kernels */
        for(cl_uint i = 0; i < numOfKernels; i++)
            clReleaseKernel(kernels[i]);

	/* Create single kernel with specific function name
	 * */
	printf("\nCreate single kernel using kernel name from program\n");
	cl_uint argCnt1;
	cl_kernel kernel1, kernel2, kernel3, kernel4;
	kernel1 = clCreateKernel(program,"vector_add",&error);
        clGetKernelInfo(kernel1, CL_KERNEL_NUM_ARGS,
                        sizeof(argCnt1), &argCnt1, NULL);
        printf("\nKernel1 name: vector_add  with arity: %d\n", argCnt1);
	DisplayKernelWorkGroupInfo(kernel1, device);
	kernel2 = clCreateKernel(program,"vector_sub",&error);
        clGetKernelInfo(kernel2, CL_KERNEL_NUM_ARGS,
                        sizeof(argCnt1), &argCnt1, NULL);
        printf("\nKernel2 name: vector_sub  with arity: %d\n", argCnt1);
	DisplayKernelWorkGroupInfo(kernel2, device);
	kernel3 = clCreateKernel(program,"vector_mul",&error);
        clGetKernelInfo(kernel3, CL_KERNEL_NUM_ARGS,
                        sizeof(argCnt1), &argCnt1, NULL);
        printf("\nKernel3 name: vector_mul  with arity: %d\n", argCnt1);
	DisplayKernelWorkGroupInfo(kernel3, device);
	kernel4 = clCreateKernel(program,"vector_div",&error);
        clGetKernelInfo(kernel4, CL_KERNEL_NUM_ARGS,
                        sizeof(argCnt1), &argCnt1, NULL);
        printf("\nKernel4 name: vector_div  with arity: %d\n", argCnt1);
	DisplayKernelWorkGroupInfo(kernel4, device);

	clReleaseKernel(kernel1);
	clReleaseKernel(kernel2);
	clReleaseKernel(kernel3);
	clReleaseKernel(kernel4);

       	/* Clean up */
       	for(int i=0; i< NUMBER_OF_FILES; i++) { 
		free(buffer[i]);
	}
       	clReleaseProgram(program);
       	clReleaseContext(context);
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
