/*
 * OclContextQuery.cpp
 *
 *  Created on: Nov 27, 2021
 *      Author: yunwu
 */

#define CL_TARGET_OPENCL_VERSION 120

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

void QueryContext1(cl_platform_id platform_id);

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

	for(cl_uint i = 0; i < numOfPlatforms; ++i) {

		printf("\nOn the %dth platform \n", i);
		QueryContext1(platforms[i]);

	}

	return 0;
}

void context_notify(const char *notify_message, const void *private_info,
                     size_t cb, void *user_data){
          printf("Notification:\n\t%s\n", notify_message);
}

void QueryContext1(cl_platform_id platform_id) {
    /* OpenCL 1.1 scalar types */
    cl_int error = 0;
    cl_uint numOfDevices = 0;

    /* Determine how many devices are connected to your platform */
    error = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &numOfDevices);
    if (error != CL_SUCCESS ) {
        perror("Unable to obtain any OpenCL compliant device info");
        exit(1);
    }
    cl_device_id* devices = (cl_device_id*)
                     alloca(sizeof(cl_device_id) * numOfDevices);

    /*
     Load the information about your devices into the variable
     'devices'
    */

    error = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, numOfDevices, devices, NULL);
    if (error != CL_SUCCESS ) {
        perror("Unable to obtain any OpenCL compliant device info");
        exit(1);
    }

    printf("Number of detected OpenCL devices: %d\n",
            numOfDevices);

    /*
       We attempt to create contexts for each device we find,
       report it and release the context. Once a context is
       created, its context is implicitly
       retained and so you don't have to invoke
      'clRetainContext'
     */

    for(int i = 0; i < numOfDevices; ++ i ) {
        cl_context context = clCreateContext(NULL, 1,
                                             &devices[i],
                                             NULL, NULL,
                                             &error);
        cl_uint ref_cnt = 0;
        if (error != CL_SUCCESS) {
            perror("Can't create a context");
            exit(1);
        }

        error = clGetContextInfo(context,
                                 CL_CONTEXT_REFERENCE_COUNT,
                                 sizeof(ref_cnt), &ref_cnt,
                                 NULL);

        if (error != CL_SUCCESS) {
            perror("Can't obtain context information");
            exit(1);
        }
        printf("Reference count of device is %d\n", ref_cnt);
        // Release the context
        clReleaseContext(context);
    }
}

