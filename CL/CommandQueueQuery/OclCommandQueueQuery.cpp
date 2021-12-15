#define CL_TARGET_OPENCL_VERSION 120

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

void CommandQueueQuery(cl_platform_id platform, cl_uint numOfPlatforms);
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
		CommandQueueQuery(platforms[i], numOfPlatforms);
	}

	return 0;
}

void CommandQueueQuery(cl_platform_id platform, cl_uint numOfPlatforms) {
   
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

        /* Release the kernels */
        for(cl_uint i = 0; i < numOfKernels; i++)
            clReleaseKernel(kernels[i]);

	/* Create single kernel with specific function name
	 * */
	printf("\nCreate single kernel using kernel name from program\n");
	cl_uint argCnt1;
	cl_kernel kernel1, kernel2, kernel3, kernel4;
	kernel1 = clCreateKernel(program,"vector_add",&error);
	kernel2 = clCreateKernel(program,"vector_sub",&error);
	kernel3 = clCreateKernel(program,"vector_mul",&error);
	kernel4 = clCreateKernel(program,"vector_div",&error);

	/* Create a command queue */
/*	const cl_command_queue_properties properties[] = {
		CL_QUEUE_PROPERTIES,
		CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE |
		CL_QUEUE_PROFILING_ENABLE,
		CL_QUEUE_SIZE,
		CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE
		};
	#ifdef CL_VERSION_2_0
		cl_command_queue command_queue = clCreateCommandQueueWithProperties(
						   context,
		       				   device,
    						   properties,
    						   &error);
	#else
		cl_command_queue command_queue = clCreateCommandQueue(
						   context,
        	                                   device,
                	                           properties,
                        	                   &error);
	#endif*/
	cl_command_queue command_queue = clCreateCommandQueue(
						   context,
						   device,
						   0,
						   &error);
	if ( error != CL_SUCCESS ){
		printf("Can't create a command queue\n");
		exit(1);
	}

	cl_uint refCnt;
	error = clGetCommandQueueInfo(command_queue,
			CL_QUEUE_REFERENCE_COUNT,
			sizeof(cl_uint),
			&refCnt,
			NULL);
	if ( error != CL_SUCCESS ){
		printf("Can't get reference count info from queue\n");
		exit(1);
	}
	printf("The reference count of command queue is %ld.\n",
			refCnt);

	cl_device_id devicefromqueue;
	error = clGetCommandQueueInfo(command_queue, 
			CL_QUEUE_DEVICE,
			sizeof(cl_device_id), 
			&devicefromqueue,
			NULL);
	if ( error != CL_SUCCESS ){
		printf("Can't get device info from queue\n");
		exit(1);
	}
	cl_uint *devType;
	error = clGetDeviceInfo( devicefromqueue, CL_DEVICE_TYPE, sizeof(cl_uint),
            		devType, NULL );
        if ( error != CL_SUCCESS ) {
                perror("Unable to obtain device info for param\n");
                exit(1);
        }
        switch (*devType) {
              case CL_DEVICE_TYPE_CPU :
                   printf("CPU detected from queue\n");break;
              case CL_DEVICE_TYPE_GPU :
                   printf("GPU detected from queue\n");break;
              case CL_DEVICE_TYPE_DEFAULT :
                   printf("default detected from queue\n");break;
        }

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
