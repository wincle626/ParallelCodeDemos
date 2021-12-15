#define CL_TARGET_OPENCL_VERSION 120

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

void DisplayProgramInfo(cl_program program, cl_program_info param_name);
void ProgramQuery(cl_platform_id platform, cl_uint numOfPlatforms);
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
		ProgramQuery(platforms[i], numOfPlatforms);
	}

	return 0;
}
void DisplayProgramInfo(cl_program program, cl_program_info param_name){
	cl_uint err;
	switch(param_name) {
		case CL_PROGRAM_REFERENCE_COUNT:
			cl_uint ref_cnt;
            		err = clGetProgramInfo(program, CL_PROGRAM_REFERENCE_COUNT,
                        	           sizeof(cl_uint), &ref_cnt, NULL);
			if (err == CL_SUCCESS)
				printf("CL_PROGRAM_PREFERENCE_COUNT - %d\n", ref_cnt);
			else
				printf("Failed to query CL_PROGRAM_PREFERENCE_COUNT\n");
			break;
        	case CL_PROGRAM_NUM_DEVICES:
			cl_uint numofdev;
            		err = clGetProgramInfo(program, CL_PROGRAM_NUM_DEVICES, 
                        	           sizeof(cl_uint), &numofdev, NULL);
            		if (err == CL_SUCCESS)
				printf("CL_PROGRAM_NUM_DEVICES - %d\n", numofdev);
			else
				printf("Failed to query CL_PROGRAM_NUM_DEVICES\n");
            		break;
		case CL_PROGRAM_NUM_KERNELS:
			size_t numofkernel;
            		err = clGetProgramInfo(program, CL_PROGRAM_NUM_KERNELS, 
                        	           sizeof(size_t), &numofkernel, NULL);
            		if (err == CL_SUCCESS)
				printf("CL_PROGRAM_NUM_KERNELS - %ld\n", numofkernel);
			else
				printf("Failed to query CL_PROGRAM_NUM_KERNELS\n");
            		break;
		case CL_PROGRAM_KERNEL_NAMES:
			char kernelname[MAX_SOURCE_SIZE];
            		err = clGetProgramInfo(program, CL_PROGRAM_KERNEL_NAMES, 
                        	           MAX_SOURCE_SIZE, &kernelname, NULL);
            		if (err == CL_SUCCESS)
				printf("CL_PROGRAM_KERNEL_NAMES - %s\n", kernelname);
			else
				printf("Failed to query CL_PROGRAM_KERNEL_NAMES\n");
            		break;
        	case CL_PROGRAM_SOURCE:
			char kernelsource[MAX_SOURCE_SIZE];
            		err = clGetProgramInfo(program, CL_PROGRAM_SOURCE, 
                        	           MAX_SOURCE_SIZE, kernelsource, NULL);
            		if (err == CL_SUCCESS)
				printf("CL_PROGRAM_SOURCE - %s\n", kernelsource);
			else
				printf("Failed to query CL_PROGRAM_SOURCE\n");
            		break;
        	case CL_PROGRAM_CONTEXT:
			cl_context context;
            		err = clGetProgramInfo(program, CL_PROGRAM_CONTEXT, 
                        	           sizeof(context), &context, NULL);
            		if (err == CL_SUCCESS){
				cl_uint error;
				int ref_cnt;
				error = clGetContextInfo(context,
						         CL_CONTEXT_REFERENCE_COUNT,
						         sizeof(ref_cnt), &ref_cnt,
						         NULL);
				if (error != CL_SUCCESS) {
				    perror("Can't obtain context information");
				    exit(1);
				}
				printf("Context reference count of device is %d\n", ref_cnt);
			}
			else
				printf("Failed to query CL_PROGRAM_CONTEXT\n");
            		break;
        	case CL_PROGRAM_DEVICES:
			cl_device_id device;
            		err = clGetProgramInfo(program, CL_PROGRAM_DEVICES, 
                        	           sizeof(cl_device_id), &device, NULL);
            		if (err == CL_SUCCESS){
				cl_uint error;
				size_t paramSize = 0;
				error = clGetDeviceInfo( device, CL_DEVICE_TYPE, 
							0, NULL, &paramSize );
				if (error != CL_SUCCESS ) {
					perror("Unable to obtain device info for CL_DEVICE_TYPE\n");
					exit(1);
				}
				cl_device_type* devType = (cl_device_type*)
						alloca(sizeof(cl_device_type) * paramSize);
				error = clGetDeviceInfo( device, param_name, paramSize,
							devType, NULL );
				if (error != CL_SUCCESS ) {
					perror("Unable to obtain device info for param\n");
					return;
				}
				switch (*devType) {
					case CL_DEVICE_TYPE_CPU :
						printf("CPU detected\n");break;
					case CL_DEVICE_TYPE_GPU :
						printf("GPU detected\n");break;
					case CL_DEVICE_TYPE_DEFAULT :
						printf("default detected\n");break;
				}
			}
			else
				printf("Failed to query CL_​PROGRAM_​KERNEL_​NAMES\n");
            		break;
        default:
            	printf("Error: UNSUPPORTED program Info type = %d ", param_name);
    }	
}

void ProgramQuery(cl_platform_id platform, cl_uint numOfPlatforms) {
   
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

	DisplayProgramInfo(program, CL_PROGRAM_REFERENCE_COUNT);
	DisplayProgramInfo(program, CL_PROGRAM_NUM_DEVICES);
	DisplayProgramInfo(program, CL_PROGRAM_NUM_KERNELS);
	DisplayProgramInfo(program, CL_PROGRAM_CONTEXT);
	DisplayProgramInfo(program, CL_PROGRAM_DEVICES);

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
		fclose( fp );
	}
}
