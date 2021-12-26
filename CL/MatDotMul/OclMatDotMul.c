// This program implements a matrix vector multiplication using OpenCL

// System includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

// Define the OpenCL version
#define CL_TARGET_OPENCL_VERSION 120

// OpenCL includes
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

// Define vector length
#define ROW 4
#define COL 4
#define DEBUG 0

int main() {
    // This code executes on the OpenCL host
    
    // Host data
    int *A = NULL;  // Input matrix
    int *B = NULL;  // Input array
    int *C = NULL;  // Output array
    
    // Compute the size of the data 
    size_t datasize = sizeof(int)*ROW*COL;
    size_t rowdatasize = sizeof(int*)*ROW;
    size_t coldatasize = sizeof(int)*COL;

    // Allocate space for input/output data
    A = (int*)malloc(datasize);
    B = (int*)malloc(datasize);
    C = (int*)malloc(datasize);

    // Initialize the input data
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            A[i*COL+j] = i*COL+j;
            B[i*COL+j] = i*COL+j;
            C[i*COL+j] = 0;
        }
    }
#if DEBUG
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            printf("A[%d][%d]=%d\t", i, j, A[i*COL+j]);
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            printf("B[%d][%d]=%d\t", i, j, B[i*COL+j]);
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            printf("C[%d][%d]=%d\t", i, j, C[i*COL+j]);
        }
        printf("\n");
    }
    printf("\n");
#endif
    printf("Data done. \n");

    // Use this to check the output of each API call
    cl_int status;  
     
    //-----------------------------------------------------
    // STEP 1: Discover and initialize the platforms
    //-----------------------------------------------------
    
    cl_uint numPlatforms = 0;
    cl_platform_id *platforms = NULL;
    
    // Use clGetPlatformIDs() to retrieve the number of 
    // platforms
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
 
    // Allocate enough space for each platform
    platforms =   
        (cl_platform_id*)malloc(
            numPlatforms*sizeof(cl_platform_id));
 
    // Fill in platforms with clGetPlatformIDs()
    status |= clGetPlatformIDs(numPlatforms, platforms, 
                NULL);
    if(status==CL_SUCCESS){
        printf("clGetPlatformIDs done!\n");
    }else{
        printf("clGetPlatformIDs failed!\n");
        exit(1);
    }

    //-----------------------------------------------------
    // STEP 2: Discover and initialize the devices
    //----------------------------------------------------- 
    
    cl_uint numDevices = 0;
    cl_device_id *devices = NULL;

    // Use clGetDeviceIDs() to retrieve the number of 
    // devices present
    status = clGetDeviceIDs(
        platforms[0], 
        CL_DEVICE_TYPE_ALL, 
        0, 
        NULL, 
        &numDevices);

    // Allocate enough space for each device
    devices = 
        (cl_device_id*)malloc(
            numDevices*sizeof(cl_device_id));

    // Fill in devices with clGetDeviceIDs()
    status |= clGetDeviceIDs(
        platforms[0], 
        CL_DEVICE_TYPE_ALL,        
        numDevices, 
        devices, 
        NULL);
    if(status==CL_SUCCESS){
        printf("clGetDeviceIDs done!\n");
    }else{
        printf("clGetDeviceIDs failed!\n");
        exit(1);
    }

    //-----------------------------------------------------
    // STEP 3: Create a context
    //----------------------------------------------------- 
    
    cl_context context = NULL;

    // Create a context using clCreateContext() and 
    // associate it with the devices
    context = clCreateContext(
        NULL, 
        numDevices, 
        devices, 
        NULL, 
        NULL, 
        &status);
    if(status==CL_SUCCESS){
        printf("clCreateContext done!\n");
    }else{
        printf("clCreateContext failed!\n");
        exit(1);
    }

    //-----------------------------------------------------
    // STEP 4: Create a command queue
    //----------------------------------------------------- 
    
    cl_command_queue cmdQueue;

    // Create a command queue using clCreateCommandQueue(),
    // and associate it with the device you want to execute 
    // on
    cmdQueue = clCreateCommandQueue(
        context, 
        devices[0], 
        0, 
        &status);
    if(status==CL_SUCCESS){
        printf("clCreateCommandQueue done!\n");
    }else{
        printf("clCreateCommandQueue failed!\n");
        exit(1);
    }

    //-----------------------------------------------------
    // STEP 5: Create device buffers
    //----------------------------------------------------- 
    
    cl_mem bufferA;  // Input array on the device
    cl_mem bufferB;  // Input array on the device
    cl_mem bufferC;  // Output array on the device

    // Use clCreateBuffer() to create a buffer object (d_A) 
    // that will contain the data from the host array A
    bufferA = clCreateBuffer(
        context, 
        CL_MEM_READ_ONLY,                         
        datasize, 
        NULL, 
        &status);
    if(status==CL_SUCCESS){
        printf("clCreateBuffer A done!\n");
    }else{
        printf("clCreateBuffer A failed!\n");
        exit(1);
    }

    // Use clCreateBuffer() to create a buffer object (d_B)
    // that will contain the data from the host array B
    bufferB = clCreateBuffer(
        context, 
        CL_MEM_READ_ONLY,                         
        datasize, 
        NULL, 
        &status);
    if(status==CL_SUCCESS){
        printf("clCreateBuffer B done!\n");
    }else{
        printf("clCreateBuffer B failed!\n");
        exit(1);
    }

    // Use clCreateBuffer() to create a buffer object (d_C) 
    // with enough space to hold the output data
    bufferC = clCreateBuffer(
        context, 
        CL_MEM_WRITE_ONLY,                 
        datasize, 
        NULL, 
        &status);
    if(status==CL_SUCCESS){
        printf("clCreateBuffer C done!\n");
    }else{
        printf("clCreateBuffer C failed!\n");
        exit(1);
    }
    
    //-----------------------------------------------------
    // STEP 6: Write host data to device buffers
    //----------------------------------------------------- 
    
    // Use clEnqueueWriteBuffer() to write input array A to
    // the device buffer bufferA
    status = clEnqueueWriteBuffer(
        cmdQueue, 
        bufferA, 
        CL_FALSE, 
        0, 
        datasize,                         
        A, 
        0, 
        NULL, 
        NULL);
    
    // Use clEnqueueWriteBuffer() to write input array B to 
    // the device buffer bufferB
    status = clEnqueueWriteBuffer(
        cmdQueue, 
        bufferB, 
        CL_FALSE, 
        0, 
        datasize,                                  
        B, 
        0, 
        NULL, 
        NULL);

    //-----------------------------------------------------
    // STEP 7: Create and compile the program
    //----------------------------------------------------- 
     
    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;
    fp = fopen("mdm.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
    // Create a program using clCreateProgramWithSource()
    cl_program program = clCreateProgramWithSource(
        context, 
        1, 
        (const char**)&source_str,                                 
        NULL, 
        &status);
    if(status==CL_SUCCESS){
        printf("clCreateProgramWithSource done!\n");
    }else{
        printf("clCreateProgramWithSource failed!\n");
        exit(1);
    }

    // Build (compile) the program for the devices with
    // clBuildProgram()
    status = clBuildProgram(
        program, 
        numDevices, 
        devices, 
        NULL, 
        NULL, 
        NULL);
    if(status==CL_SUCCESS){
        printf("clBuildProgram done!\n");
    }else{
        size_t loglen = 0;
        status = clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &loglen);
        char *log = calloc(loglen, sizeof(char));
        status = clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, loglen, log, NULL);
        printf("%s\n", log);
        printf("clBuildProgram failed!\n");
        exit(1);
    }
   
    //-----------------------------------------------------
    // STEP 8: Create the kernel
    //----------------------------------------------------- 

    cl_kernel kernel = NULL;

    // Use clCreateKernel() to create a kernel from the 
    // vector addition function (named "vecadd")
    kernel = clCreateKernel(program, "matrix_dot_mul", &status);
    if(status==CL_SUCCESS){
        printf("clCreateKernel done!\n");
    }else{
        printf("clCreateKernel failed!\n");
        exit(1);
    }

    //-----------------------------------------------------
    // STEP 9: Set the kernel arguments
    //----------------------------------------------------- 
    
    // Associate the input and output buffers with the 
    // kernel 
    // using clSetKernelArg()
    int RowSize = ROW, ColSize = COL;
    status  = clSetKernelArg(
        kernel, 
        0, 
        sizeof(int), 
        (void*)&RowSize);
    status  |= clSetKernelArg(
        kernel, 
        1, 
        sizeof(int), 
        (void*)&ColSize);
    status  |= clSetKernelArg(
        kernel, 
        2, 
        sizeof(cl_mem), 
        &bufferA);
    status |= clSetKernelArg(
        kernel, 
        3, 
        sizeof(cl_mem), 
        &bufferB);
    status |= clSetKernelArg(
        kernel, 
        4, 
        sizeof(cl_mem), 
        &bufferC);
    if(status==CL_SUCCESS){
        printf("clSetKernelArg done!\n");
    }else{
        printf("clSetKernelArg failed!\n");
        exit(1);
    }

    //-----------------------------------------------------
    // STEP 10: Configure the work-item structure
    //----------------------------------------------------- 
    
    // Define an index space (global work size) of work 
    // items for 
    // execution. A workgroup size (local work size) is not 
    // required, 
    // but can be used.
    int gloablWorkDim = 2;
    size_t globalWorkSize[2];    
    // There are '2D' work-items 
    globalWorkSize[0] = ROW;
    globalWorkSize[1] = COL;

    //-----------------------------------------------------
    // STEP 11: Enqueue the kernel for execution
    //----------------------------------------------------- 
    
    // Execute the kernel by using 
    // clEnqueueNDRangeKernel().
    // 'globalWorkSize' is the 1D dimension of the 
    // work-items
    status = clEnqueueNDRangeKernel(
        cmdQueue, 
        kernel, 
        gloablWorkDim, 
        NULL, 
        globalWorkSize, 
        NULL, 
        0, 
        NULL, 
        NULL);
    if(status==CL_SUCCESS){
        printf("clEnqueueNDRangeKernel done!\n");
    }else{
        printf("clEnqueueNDRangeKernel failed!\n");
        exit(1);
    }

    //-----------------------------------------------------
    // STEP 12: Read the output buffer back to the host
    //----------------------------------------------------- 
    
    // Use clEnqueueReadBuffer() to read the OpenCL output  
    // buffer (bufferC) 
    // to the host output array (C)
    clEnqueueReadBuffer(
        cmdQueue, 
        bufferC, 
        CL_TRUE, 
        0, 
        datasize, 
        C, 
        0, 
        NULL, 
        NULL);

    // Verify the output
    bool result = true;
    for(int i = 0; i < ROW; i++) {
        for(int j=0;j<COL;j++){
#if DEBUG
            printf("C[%d][%d]=%d - %d\n",
                    i,j,C[i*COL+j],(i*COL+j)*(i*COL+j));
#endif
            if(C[i*COL+j] != (i*COL+j)*(i*COL+j)) {
                result = false;
                break;
            }
        }
    }
    if(result) {
        printf("Output is correct\n");
    } else {
        printf("Output is incorrect\n");
    }

    //-----------------------------------------------------
    // STEP 13: Release OpenCL resources
    //----------------------------------------------------- 
    
    // Free OpenCL resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseContext(context);

    // Free host resources
    free(A);
    free(B);
    free(C);
    free(platforms);
    free(devices);
}