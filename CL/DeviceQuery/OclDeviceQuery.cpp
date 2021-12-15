/*
 * OclDeviceQuery.cpp
 *
 *  Created on: Nov 27, 2021
 *      Author: yunwu
 */

#define CL_TARGET_OPENCL_VERSION 120

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>


void displayDeviceDetails(cl_device_id platform_id,
                          cl_device_info param_name,
                          const char* paramNameAsStr);
cl_uint DeviceQuery(cl_platform_id platform_id);
void displayPlatformInfo(cl_platform_id id,
                         cl_platform_info param_name,
                         const char* paramNameAsStr);
cl_uint PlatformQuery(cl_platform_id* platforms);

int main(int argc, char** argv){

	//	/* OpenCL 1.2 data structures */
	cl_platform_id platforms;
	/* OpenCL 1.1 scalar data types */
	cl_uint numOfPlatforms;

	numOfPlatforms = PlatformQuery(&platforms);

	return 0;
}

void displayDeviceDetails(cl_device_id device_id,
                          cl_device_info param_name,
                          const char* paramNameAsStr) {
  cl_int error = 0;
  size_t paramSize = 0;

  error = clGetDeviceInfo( device_id, param_name, 0, NULL, &paramSize );
  if (error != CL_SUCCESS ) {
    perror("Unable to obtain device info for param\n");
    exit(1);
  }

  /*
    The cl_device_info are preprocessor directives defined in cl.h
  */

  switch (param_name) {
    case CL_DEVICE_TYPE: {
            cl_device_type* devType = (cl_device_type*)
                        alloca(sizeof(cl_device_type) * paramSize);
            error = clGetDeviceInfo( device_id, param_name, paramSize,
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
            }break;
    case CL_DEVICE_VENDOR_ID: {
    		cl_uint* devType = (cl_uint*)
						alloca(sizeof(cl_uint) * paramSize);
			error = clGetDeviceInfo( device_id, param_name, paramSize,
					devType, NULL );

            if (error != CL_SUCCESS ) {
                perror("Unable to obtain device info for param\n");
                return;
            }

            printf("Device Vendor ID: %d \n", *devType);
    		}break;
    case CL_DEVICE_MAX_COMPUTE_UNITS: {
    		cl_uint* devType = (cl_uint*)
						alloca(sizeof(cl_uint) * paramSize);
			error = clGetDeviceInfo( device_id, param_name, paramSize,
					devType, NULL );

            if (error != CL_SUCCESS ) {
                perror("Unable to obtain device info for param\n");
                return;
            }

            printf("Device maximum compute unit number: %d \n", *devType);
    		}break;
    case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS: {
    		cl_uint* devType = (cl_uint*)
						alloca(sizeof(cl_uint) * paramSize);
			error = clGetDeviceInfo( device_id, param_name, paramSize,
					devType, NULL );

            if (error != CL_SUCCESS ) {
                perror("Unable to obtain device info for param\n");
                return;
            }

            printf("Device maximum work item dimension: %d \n", *devType);
    		}break;
    case CL_DEVICE_MAX_WORK_ITEM_SIZES: {
    		cl_uint* devType = (cl_uint*)
						alloca(sizeof(cl_uint) * paramSize);
			error = clGetDeviceInfo( device_id, param_name, paramSize,
					devType, NULL );

            if (error != CL_SUCCESS ) {
                perror("Unable to obtain device info for param\n");
                return;
            }

            printf("Device maximum work item size: %d \n", *devType);
    		}break;
    case CL_DEVICE_MAX_WORK_GROUP_SIZE: {
    		cl_uint* devType = (cl_uint*)
						alloca(sizeof(cl_uint) * paramSize);
			error = clGetDeviceInfo( device_id, param_name, paramSize,
					devType, NULL );

            if (error != CL_SUCCESS ) {
                perror("Unable to obtain device info for param\n");
                return;
            }

            printf("Device maximum work group size: %d \n", *devType);
    		}break;
    case CL_DEVICE_MAX_CLOCK_FREQUENCY: {
    		cl_uint* devType = (cl_uint*)
						alloca(sizeof(cl_uint) * paramSize);
			error = clGetDeviceInfo( device_id, param_name, paramSize,
					devType, NULL );

            if (error != CL_SUCCESS ) {
                perror("Unable to obtain device info for param\n");
                return;
            }

            printf("Device maximum clock frequency size: %d \n", *devType);
    		}break;
    case CL_DEVICE_GLOBAL_MEM_SIZE: {
    		cl_uint* devType = (cl_uint*)
						alloca(sizeof(cl_uint) * paramSize);
			error = clGetDeviceInfo( device_id, param_name, paramSize,
					devType, NULL );

            if (error != CL_SUCCESS ) {
                perror("Unable to obtain device info for param\n");
                return;
            }

            printf("Device maximum global memory size: %d \n", *devType);
    		}break;
    case CL_DEVICE_LOCAL_MEM_SIZE: {
    		cl_uint* devType = (cl_uint*)
						alloca(sizeof(cl_uint) * paramSize);
			error = clGetDeviceInfo( device_id, param_name, paramSize,
					devType, NULL );

            if (error != CL_SUCCESS ) {
                perror("Unable to obtain device info for param\n");
                return;
            }

            printf("Device maximum local memory size: %d \n", *devType);
    		}break;
   } //end of switch
}

cl_uint DeviceQuery(cl_platform_id platform_id, cl_device_id* devices) {
    /* OpenCL 1.1 device types */

    cl_int error = 0;
    cl_uint numOfDevices = 0;

    /* Determine how many devices are connected to your
       platform */
    error = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, NULL,
                           &numOfDevices);
    if (error != CL_SUCCESS ) {
        perror("Unable to obtain any OpenCL compliant device info");
        exit(1);
    }

    devices = (cl_device_id*)
			  alloca(sizeof(cl_device_id) * numOfDevices);

    /* Load the information about your devices into the
       variable 'devices'
    */
    error = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, numOfDevices, devices,
                           NULL);
    if (error != CL_SUCCESS ) {
        perror("Unable to obtain any OpenCL compliant device info");
        exit(1);
    }

    printf("Number of detected OpenCL devices: %d\n",numOfDevices);

    /*
      We attempt to retrieve some information about the
      devices.
    */

    for(int i = 0; i < numOfDevices; ++ i ) {
        displayDeviceDetails( devices[i], CL_DEVICE_TYPE,
        		"CL_DEVICE_TYPE" );
        displayDeviceDetails( devices[i], CL_DEVICE_VENDOR_ID,
        		"CL_DEVICE_VENDOR_ID" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_COMPUTE_UNITS,
        		"CL_DEVICE_MAX_COMPUTE_UNITS" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
        		"CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_WORK_ITEM_SIZES,
        		"CL_DEVICE_MAX_WORK_ITEM_SIZES" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_WORK_GROUP_SIZE,
        		"CL_DEVICE_MAX_WORK_GROUP_SIZE" );
        displayDeviceDetails( devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
        		"CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR" );
        displayDeviceDetails( devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
        		"CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT" );
        displayDeviceDetails( devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
        		"CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT" );
        displayDeviceDetails( devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
        		"CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG" );
        displayDeviceDetails( devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
        		"CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT" );
        displayDeviceDetails( devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
        		"CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY,
        		"CL_DEVICE_MAX_CLOCK_FREQUENCY" );
        displayDeviceDetails( devices[i], CL_DEVICE_ADDRESS_BITS,
        		"CL_DEVICE_ADDRESS_BITS" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_READ_IMAGE_ARGS,
        		"CL_DEVICE_MAX_READ_IMAGE_ARGS" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_WRITE_IMAGE_ARGS,
        		"CL_DEVICE_MAX_WRITE_IMAGE_ARGS" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_MEM_ALLOC_SIZE,
        		"CL_DEVICE_MAX_MEM_ALLOC_SIZE" );
        displayDeviceDetails( devices[i], CL_DEVICE_IMAGE2D_MAX_WIDTH,
        		"CL_DEVICE_IMAGE2D_MAX_WIDTH" );
        displayDeviceDetails( devices[i], CL_DEVICE_IMAGE2D_MAX_HEIGHT,
        		"CL_DEVICE_IMAGE2D_MAX_HEIGHT" );
        displayDeviceDetails( devices[i], CL_DEVICE_IMAGE3D_MAX_WIDTH,
        		"CL_DEVICE_IMAGE3D_MAX_WIDTH" );
        displayDeviceDetails( devices[i], CL_DEVICE_IMAGE3D_MAX_HEIGHT,
        		"CL_DEVICE_IMAGE3D_MAX_HEIGHT" );
        displayDeviceDetails( devices[i], CL_DEVICE_IMAGE3D_MAX_DEPTH,
        		"CL_DEVICE_IMAGE3D_MAX_DEPTH" );
        displayDeviceDetails( devices[i], CL_DEVICE_IMAGE_SUPPORT,
        		"CL_DEVICE_IMAGE_SUPPORT" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_PARAMETER_SIZE,
        		"CL_DEVICE_MAX_PARAMETER_SIZE" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_SAMPLERS,
        		"CL_DEVICE_MAX_SAMPLERS" );
        displayDeviceDetails( devices[i], CL_DEVICE_MEM_BASE_ADDR_ALIGN,
        		"CL_DEVICE_MEM_BASE_ADDR_ALIGN" );
        displayDeviceDetails( devices[i], CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE,
        		"CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE" );
        displayDeviceDetails( devices[i], CL_DEVICE_SINGLE_FP_CONFIG,
        		"CL_DEVICE_SINGLE_FP_CONFIG" );
        displayDeviceDetails( devices[i], CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
        		"CL_DEVICE_GLOBAL_MEM_CACHE_TYPE" );
        displayDeviceDetails( devices[i], CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
        		"CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE" );
        displayDeviceDetails( devices[i], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
        		"CL_DEVICE_GLOBAL_MEM_CACHE_SIZE" );
        displayDeviceDetails( devices[i], CL_DEVICE_GLOBAL_MEM_SIZE,
        		"CL_DEVICE_GLOBAL_MEM_SIZE" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
        		"CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE" );
        displayDeviceDetails( devices[i], CL_DEVICE_MAX_CONSTANT_ARGS,
        		"CL_DEVICE_MAX_CONSTANT_ARGS" );
        displayDeviceDetails( devices[i], CL_DEVICE_LOCAL_MEM_TYPE,
        		"CL_DEVICE_LOCAL_MEM_TYPE" );
        displayDeviceDetails( devices[i], CL_DEVICE_LOCAL_MEM_SIZE,
        		"CL_DEVICE_LOCAL_MEM_SIZE" );
        displayDeviceDetails( devices[i], CL_DEVICE_ERROR_CORRECTION_SUPPORT,
        		"CL_DEVICE_ERROR_CORRECTION_SUPPORT" );
        displayDeviceDetails( devices[i], CL_DEVICE_PROFILING_TIMER_RESOLUTION,
        		"CL_DEVICE_PROFILING_TIMER_RESOLUTION" );
        displayDeviceDetails( devices[i], CL_DEVICE_ENDIAN_LITTLE,
        		"CL_DEVICE_ENDIAN_LITTLE" );
        displayDeviceDetails( devices[i], CL_DEVICE_AVAILABLE,
        		"CL_DEVICE_AVAILABLE" );
        displayDeviceDetails( devices[i], CL_DEVICE_COMPILER_AVAILABLE,
        		"CL_DEVICE_COMPILER_AVAILABLE" );
        displayDeviceDetails( devices[i], CL_DEVICE_EXECUTION_CAPABILITIES,
        		"CL_DEVICE_EXECUTION_CAPABILITIES" );
    }

    return numOfDevices;
}

void displayPlatformInfo(cl_platform_id id,
                         cl_platform_info param_name,
                         const char* paramNameAsStr) {
    cl_int error = 0;
    size_t paramSize = 0;

    error = clGetPlatformInfo( id, param_name, 0, NULL,
                               &paramSize );
    char* moreInfo = (char*)alloca( sizeof(char) * paramSize);
    error = clGetPlatformInfo( id, param_name, paramSize,
                               moreInfo, NULL );
    if (error != CL_SUCCESS ) {
        perror("Unable to find any OpenCL platform information");
        exit(1);
    }
    printf("%s: %s\n", paramNameAsStr, moreInfo);
}

cl_uint PlatformQuery(cl_platform_id* platforms){

	/* OpenCL 1.1 scalar data types */
	cl_uint numOfPlatforms;
	cl_uint numOfDevices;
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
	platforms = (cl_platform_id*) alloca(sizeof(cl_platform_id)
			   * numOfPlatforms);
	error = clGetPlatformIDs(numOfPlatforms, platforms, NULL);
	if(error < 0) {
		perror("Couldn't identify a platform");
		exit(1);
	} 
	printf("Number of OpenCL platforms found: %d\n\n",
		   numOfPlatforms);

	// We invoke the API 'clPlatformInfo' twice for each
	// parameter we're trying to extract
	// and we use the return value to create temporary data
	// structures (on the stack) to store
	// the returned information on the second invocation.
	cl_device_id* devices;
	for(cl_uint i = 0; i < numOfPlatforms; ++i) {
	
		printf("Platform Index: %d\n", i);
		
		displayPlatformInfo( platforms[i],
							 CL_PLATFORM_NAME,
							 "CL_PLATFORM_NAME" );

		displayPlatformInfo( platforms[i],
							 CL_PLATFORM_VERSION,
							 "CL_PLATFORM_VERSION" );

		displayPlatformInfo( platforms[i],
							 CL_PLATFORM_PROFILE,
							 "CL_PLATFORM_PROFILE" );

		displayPlatformInfo( platforms[i],
							 CL_PLATFORM_VENDOR,
							 "CL_PLATFORM_VENDOR" );

		displayPlatformInfo( platforms[i],
							 CL_PLATFORM_EXTENSIONS,
							 "CL_PLATFORM_EXTENSIONS" );

		numOfDevices = DeviceQuery(platforms[i], devices);
		
		printf("\n");
	} 
	printf("\n");

	return numOfPlatforms;
}

