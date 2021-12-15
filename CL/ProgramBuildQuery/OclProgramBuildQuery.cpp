#define CL_TARGET_OPENCL_VERSION 200

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>


void ProgramBuildQuery(cl_program program, cl_device_id device);
void loadProgramSource(const char** filename, int filenumber,
		char** filebuffer, size_t* size);

int main(int argc, char** argv) {
   
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
		cl_device_id device;
       		// Get the GPU device
		error = clGetDeviceIDs(platforms[i], 
        	                      CL_DEVICE_TYPE_GPU, 1,
                	              &device, NULL);
		if(error != CL_SUCCESS) {
          	// Otherwise, get the CPU
          		error = clGetDeviceIDs(platforms[i],
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

		ProgramBuildQuery(program, device);

        	/* Clean up */
        	for(i=0; i< NUMBER_OF_FILES; i++) { 
			free(buffer[i]);
		}
        	clReleaseProgram(program);
        	clReleaseContext(context);
	}
}

void ProgramBuildQuery(cl_program program, cl_device_id device){

	cl_uint error;
       	/*
       	Build OpenCL program object and dump the error
       	message, if any
       	*/
       	char *program_log;
       	const char options[] = "-cl-finite-math-only \
               	                -cl-no-signed-zeros";
       	size_t log_size;
	size_t globalvariable_size;
       	//error = clBuildProgram(program, 1, &device, options,
	//		NULL,NULL);
       	// Uncomment the line below, comment the line above;
       	// build the program to use build options dynamically
       	error = clBuildProgram(program, 1, &device, NULL,
			NULL, NULL);

	if(error != CL_SUCCESS) {
       		// If there's an error whilst building the program,
       		// dump the log
       		clGetProgramBuildInfo(program, device,
               	                  	CL_PROGRAM_BUILD_LOG, 0,
                       	          	NULL,
                               	  	&log_size);
       		program_log = (char*) malloc(log_size+1);
       		program_log[log_size] = '\0';
       		clGetProgramBuildInfo(program, device,
               	                	  CL_PROGRAM_BUILD_LOG,
                       		          log_size+1, program_log,
                	               	  NULL);
       		printf("\n=== ERROR ===\n\n%s\n=============\n",
               		program_log);
       		free(program_log);
	}else{
		// If no error return the global variable size
		clGetProgramBuildInfo(program, device,
				CL_PROGRAM_BUILD_GLOBAL_VARIABLE_TOTAL_SIZE,
				sizeof(size_t), &globalvariable_size,
				NULL);
		printf("%d bytes are used for global memory \n");
	}
	
}

#define MAX_SOURCE_SIZE (0x100000)

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
		fclose( fp );
	}
}
