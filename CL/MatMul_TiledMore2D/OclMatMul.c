// This program implements a matrix multiplication using OpenCL

// System includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>   // for gettimeofday()
#include <omp.h>

// Define the OpenCL version
#define CL_TARGET_OPENCL_VERSION 120

// OpenCL includes
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

// Define matrix dimension
// A is of DIM1 x DIM2
// B is of DIM2 x DIM3
// C is of DIM1 x DIM3
#define DIM1 4096 // row of A, row of C
#define DIM2 1024 // col of A, row of B
#define DIM3 256 // col of B, col of C
#define TRANSPOSE 1

void read_csv(int row, int col, 
              char *filename, int *data);
void check_data(int row, int col, 
                int *data1, int *data2);
void seqmm(int *A, int *B, int *C);
void ompmm(int *A, int *B, int *C);
void oclmm(int *A, int *B, int *C);

typedef enum{
    seq=1, omp=2, ocl=3
}options;

int main(int argc, char** argv) {

    options opt;
    if(argc<2){
        printf("Please input choice: 1-seq, 2-omp, or 3-ocl. \n");
        exit(1);
    }else{
        opt = atoi(argv[1]);
    }

    // Intializes random number generator
    time_t t;
    srand((unsigned) time(&t));

    // Intializes data
    int *A, *B, *C, *Cseq, *Comp, *Cocl;
    A = (int *) malloc(sizeof(int)*DIM1*DIM2);
    B = (int *) malloc(sizeof(int)*DIM2*DIM3);
    C = (int *) malloc(sizeof(int)*DIM1*DIM3);
    Cseq = (int *) malloc(sizeof(int)*DIM1*DIM3);
    Comp = (int *) malloc(sizeof(int)*DIM1*DIM3);
    Cocl = (int *) malloc(sizeof(int)*DIM1*DIM3);
    read_csv(DIM1, DIM2, "A.csv", A);
    read_csv(DIM2, DIM3, "B.csv", B);
    read_csv(DIM1, DIM3, "C.csv", C);
	// for(int i=0; i<DIM1; i++){
	// 	for(int j=0; j<DIM2; j++){
	// 		A[i * DIM2 + j] = rand() % 50;
	// 	}
	// }
	// for(int i=0; i<DIM2; i++){
	// 	for(int j=0; j<DIM3; j++){
	// 		B[i * DIM3 + j] = rand() % 50;
	// 	}
	// }
	for(int i=0; i<DIM1; i++){
		for(int j=0; j<DIM3; j++){
			Cseq[i * DIM3 + j] = 0;
			Comp[i * DIM3 + j] = 0;
			Cocl[i * DIM3 + j] = 0;
		}
	}

    switch(opt){
        case seq:
            seqmm(A, B, Cseq);
            check_data(DIM1, DIM3, Cseq, C);
            free(Cseq);
            break;
        case omp:
            ompmm(A, B, Comp);
            check_data(DIM1, DIM3, Comp, C);
            free(Comp);
            break;
        case ocl:
            oclmm(A, B, Cocl);
            check_data(DIM1, DIM3, Cocl, C);
            free(Cocl);
            break;
        default:
            printf("NULL \n");break;
    }
    free(C);

    return 0;
}

void read_csv(int row, int col, 
              char *filename, int *data){

	FILE *file;
	file = fopen(filename, "r");

	int i = 0;
    char line[4098];
	while (fgets(line, 4098, file) && (i < row))
    {
    	// int row[ssParams->nreal + 1];
        char* tmp = strdup(line);

	    int j = 0;
	    const char* tok;
	    for (tok = strtok(line, "\t"); tok && *tok; j++, tok = strtok(NULL, "\t\n"))
	    {
	        data[i*col+j] = atoi(tok);
	        //printf("%f\t", data[i*col+j]);
	    }
	    //printf("\n");

        free(tmp);
        i++;
    }

}

void check_data(int row, int col, 
                int *data1, int *data2){
    int i, j;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++){
            if(data1[i*col+j]!=data2[i*col+j]){
                printf("@ %dth row %dth col, the result is %d, should be %d. \n",
                        i, j, data1[i*col+j], data2[i*col+j]);
                exit(1);
            }
        }
    }
    printf("Computation is correct. \n");
}

void seqmm(int *A, int *B, int *C){

    int i, j, k;

#if TRANSPOSE==1
    int *Aseq, *Bseq;
    Aseq = (int *) malloc(sizeof(int)*DIM1*DIM2);
    Bseq = (int *) malloc(sizeof(int)*DIM2*DIM3);
    // transform matrix
	for(i=0; i<DIM1; i++){
		for(j=0; j<DIM2; j++){
			Aseq[i*DIM2+j] = A[i*DIM2+j];
		}
	}
	for(i=0; i<DIM2; i++){
		for(j=0; j<DIM3; j++){
			Bseq[j*DIM2+i] = B[i*DIM3+j];
		}
	}
    clock_t tstart = clock(); 
    for(i=0;i<DIM1;i++){
        for(k=0;k<DIM3;k++){
            int acc = 0;
            for(j=0;j<DIM2;j++){
                acc += Aseq[i*DIM2+j] * Bseq[k*DIM2+j];
            }
            C[i*DIM3+k] = acc;
        }
    }
    clock_t tend = clock() ;
    double elapsed_time = 1000 * (double)(tend-tstart)/(double)CLOCKS_PER_SEC ;
    printf("Sequential execution time: %0.5lf ms \n", 
           elapsed_time);
    free(Aseq);
    free(Bseq);
 #else   
    clock_t tstart = clock();
    for(i=0;i<DIM1;i++){
        for(k=0;k<DIM3;k++){
            C[i*DIM3+k] = 0;
            for(j=0;j<DIM2;j++){
                C[i*DIM3+k] += A[i*DIM2+j] * B[j*DIM3+k];
            }
        }
    }
    clock_t tend = clock() ;
    double elapsed_time = 1000 * (double)(tend-tstart)/(double)CLOCKS_PER_SEC ;
    printf("Sequential execution time: %0.5lf ms \n", 
           elapsed_time);
    free(A);
    free(B);
#endif
	// struct timeval start, end;
	// gettimeofday(&start, NULL);
    // for(i=0;i<DIM1;i++){
    //     for(k=0;k<DIM3;k++){
    //         C[i*DIM3+k] = 0;
    //         for(j=0;j<DIM2;j++){
    //             C[i*DIM3+k] += A[i*DIM2+j] * B[j*DIM3+k];
    //         }
    //     }
    // }
	// gettimeofday(&end, NULL);
	// long seconds = (end.tv_sec - start.tv_sec);
	// long microseconds = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    // printf("Sequential execution time: %0.5lf ms \n", 
    //        microseconds/1000);

}

void ompmm(int *A, int *B, int *C){

    int i, j, k;
#if TRANSPOSE==1
    int *Aomp, *Bomp;
    Aomp = (int *) malloc(sizeof(int)*DIM1*DIM2);
    Bomp = (int *) malloc(sizeof(int)*DIM2*DIM3);
    // transform matrix
	for(i=0; i<DIM1; i++){
		for(j=0; j<DIM2; j++){
			Aomp[i*DIM2+j] = A[i*DIM2+j];
		}
	}
	for(i=0; i<DIM2; i++){
		for(j=0; j<DIM3; j++){
			Bomp[j*DIM2+i] = B[i*DIM3+j];
		}
	}
    // OpenMP accelerated matrix multiplication
    double start = omp_get_wtime();
    #pragma omp parallel private(i, j, k) shared(Aomp, Bomp, C) num_threads(4)
    {
        #pragma omp for schedule(static) //collapse(2) 
        for(i=0;i<DIM1;i++){
            for(k=0;k<DIM3;k++){
                int acc = 0;
                for(j=0;j<DIM2;j++){
                    acc += Aomp[i*DIM2+j] * Bomp[k*DIM2+j];
                }
                C[i*DIM3+k] = acc;
            }
        }
    }
    double stop = omp_get_wtime();
    double elapsed_time = (double)(stop - start)*1000;
    printf("OMP execution time: %0.5lf ms\n", elapsed_time);
    free(Aomp);
    free(Bomp);
#else
    // OpenMP accelerated matrix multiplication
    double start = omp_get_wtime();
    #pragma omp parallel private(i, j, k) shared(A, B, C) num_threads(4)
    {
        #pragma omp for schedule(static, 16) collapse(2) 
        for(i=0;i<DIM1;i++){
            for(k=0;k<DIM3;k++){
                int ind1 = i*DIM3+k;
                int acc = 0;
                for(j=0;j<DIM2;j++){
                    int ind2 = i*DIM2+j;
                    int ind3 = j*DIM3+k;
                    acc += A[ind2] * B[ind3];
                }
                C[ind1] = acc;
            }
        }
    }
    double stop = omp_get_wtime();
    double elapsed_time = (double)(stop - start)*1000;
    printf("OMP execution time: %0.5lf ms\n", elapsed_time);
    free(A);
    free(B);
#endif

}

void checkstatus(cl_int status, char* func){

    if(status==CL_SUCCESS){
        printf("%s done!\n", func);
    }else{
        printf("%s failed!\n", func);
        exit(1);
    }

}

void printint(char* str, int value){
    printf("%s is %d\n", str, value);
}

void readkenel(char* kernelfile, char *source_str){    
    FILE *fp;
    size_t source_size;
    fp = fopen(kernelfile, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
    printf("Kernel %s read. \n", kernelfile);
}

void oclmm(int *A, int *B, int *C){   

	struct timeval start, end;
	gettimeofday(&start, NULL);

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
    checkstatus(status, "clGetPlatformIDs");
    
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
    checkstatus(status, "clGetDeviceIDs");

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
    checkstatus(status, "clCreateContext");

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
        /*0,*/CL_QUEUE_PROFILING_ENABLE, /*enable profiling*/
        &status);
    checkstatus(status, "clCreateCommandQueue");

    //-----------------------------------------------------
    // STEP 5: Create device buffers
    //-----------------------------------------------------    
    #if TRANSPOSE==1
        int *Aocl, *Bocl;
        Aocl = (int *) malloc(sizeof(int)*DIM1*DIM2);
        Bocl = (int *) malloc(sizeof(int)*DIM2*DIM3);
        // transform matrix
        for(int i=0; i<DIM1; i++){
            for(int j=0; j<DIM2; j++){
                Aocl[i*DIM2+j] = A[i*DIM2+j];
            }
        }
        for(int i=0; i<DIM2; i++){
            for(int j=0; j<DIM3; j++){
                Bocl[j*DIM2+i] = B[i*DIM3+j];
            }
        }
    #endif
    cl_mem bufferA;  // Input array on the device
    cl_mem bufferB;  // Input array on the device
    cl_mem bufferC;  // Output array on the device
    // Use clCreateBuffer() to create a buffer object (d_A) 
    // that will contain the data from the host array A
    bufferA = clCreateBuffer(
        context, 
        CL_MEM_READ_ONLY,                         
        sizeof(int)*DIM1*DIM2, 
        NULL, 
        &status);
    checkstatus(status, "clCreateBuffer A");
    // Use clCreateBuffer() to create a buffer object (d_B)
    // that will contain the data from the host array B
    bufferB = clCreateBuffer(
        context, 
        CL_MEM_READ_ONLY,                      
        sizeof(int)*DIM2*DIM3, 
        NULL, 
        &status);
    checkstatus(status, "clCreateBuffer B");
    // Use clCreateBuffer() to create a buffer object (d_C) 
    // with enough space to hold the output data
    bufferC = clCreateBuffer(
        context, 
        CL_MEM_WRITE_ONLY,                    
        sizeof(int)*DIM1*DIM3, 
        NULL, 
        &status);
    checkstatus(status, "clCreateBuffer C");
    
    //-----------------------------------------------------
    // STEP 6: Write host data to device buffers
    //----------------------------------------------------- 
    // Use clEnqueueWriteBuffer() to write input array A to
    // the device buffer bufferA  
    #if TRANSPOSE==1
        status = clEnqueueWriteBuffer(
            cmdQueue, 
            bufferA, 
            CL_FALSE, 
            0,                  
            sizeof(int)*DIM1*DIM2,                   
            Aocl, 
            0, 
            NULL, 
            NULL);
        // Use clEnqueueWriteBuffer() to write input array B to 
        // the device buffer bufferB
        status |= clEnqueueWriteBuffer(
            cmdQueue, 
            bufferB, 
            CL_FALSE, 
            0,                 
            sizeof(int)*DIM2*DIM3,                                
            Bocl, 
            0, 
            NULL, 
            NULL);
    #else
        status = clEnqueueWriteBuffer(
            cmdQueue, 
            bufferA, 
            CL_FALSE, 
            0,                  
            sizeof(int)*DIM1*DIM2,                   
            A, 
            0, 
            NULL, 
            NULL);
        // Use clEnqueueWriteBuffer() to write input array B to 
        // the device buffer bufferB
        status |= clEnqueueWriteBuffer(
            cmdQueue, 
            bufferB, 
            CL_FALSE, 
            0,                 
            sizeof(int)*DIM2*DIM3,                                
            B, 
            0, 
            NULL, 
            NULL);
    #endif
    checkstatus(status, "clEnqueueWriteBuffer");

    //-----------------------------------------------------
    // STEP 7: Create and compile the program
    //-----------------------------------------------------    
    // Read kernel source 
    char *source_str=NULL;
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    #if TRANSPOSE==1
        readkenel("mm1.cl", source_str);
    #else
        readkenel("mm2.cl", source_str);
    #endif
    // Create a program using clCreateProgramWithSource()
    cl_program program = clCreateProgramWithSource(
        context, 
        1, 
        (const char**)&source_str,                                 
        NULL, 
        &status);
    checkstatus(status, "clCreateProgramWithSource");
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
    kernel = clCreateKernel(program, "matmul", &status);
    checkstatus(status, "clCreateKernel");

    //-----------------------------------------------------
    // STEP 9: Set the kernel arguments
    //----------------------------------------------------- 
    int M = DIM1, N = DIM2, K = DIM3;
    status  = clSetKernelArg(
        kernel, 
        0, 
        sizeof(int), 
        (void*)&M);
    status  |= clSetKernelArg(
        kernel, 
        1, 
        sizeof(int), 
        (void*)&N);
    status  |= clSetKernelArg(
        kernel, 
        2, 
        sizeof(int), 
        (void*)&K);
    status  |= clSetKernelArg(
        kernel, 
        3, 
        sizeof(cl_mem), 
        &bufferA);
    status |= clSetKernelArg(
        kernel, 
        4, 
        sizeof(cl_mem), 
        &bufferB);
    status |= clSetKernelArg(
        kernel, 
        5, 
        sizeof(cl_mem), 
        &bufferC);
    checkstatus(status, "clSetKernelArg");

    //-----------------------------------------------------
    // STEP 10: Configure the work-item structure
    //----------------------------------------------------- 
    // Define an index space (global work size) of work 
    // items for 
    // execution. A workgroup size (local work size) is not 
    // required, 
    // but can be used.
    // There are '2D' work-items 
    int gloablWorkDim = 2; 
    int TileSize = 32;
    int WrappedTiles = 4;
    size_t localWorkSize[2] = {TileSize/WrappedTiles, TileSize/WrappedTiles};  
    size_t globalWorkSize[2] = {M/WrappedTiles, K/WrappedTiles}; 

    //-----------------------------------------------------
    // STEP 11: Enqueue the kernel for execution
    //----------------------------------------------------- 
    // Create the user event
    cl_event user_event = clCreateUserEvent(context, &status);
    checkstatus(status, "clCreateUserEvent");
    // Reserve space for kernel events
    cl_event kernel_event;
    // Execute the kernel by using 
    // clEnqueueNDRangeKernel().
    // 'globalWorkSize' is the 2D dimension of the 
    // work-items
    status = clEnqueueNDRangeKernel(
        cmdQueue, 
        kernel, 
        gloablWorkDim, 
        NULL, 
        globalWorkSize, 
        localWorkSize, 
        1, 
        /*NULL*/&user_event, 
        /*NULL*/&kernel_event);
    checkstatus(status, "clEnqueueNDRangeKernel");
    // Start all kernels by completing the user event
    clSetUserEventStatus(user_event, CL_COMPLETE);
    // Block until all kernels have run to completion
    clWaitForEvents(1, &kernel_event);
    // Calculate the timing
    cl_ulong time_start, time_end;
    clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_START,
                            sizeof(time_start), &time_start, NULL);
    clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_END,
                            sizeof(time_end), &time_end, NULL);
    printf("Event: start=%llu, end=%llu\n", 
            (long long unsigned int)time_start, 
            (long long unsigned int)time_end);
    double nanoSeconds = time_end-time_start;
    printf("OCL kernel execution time: %0.5lf ms \n", nanoSeconds / 1000000.0);

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
        sizeof(int)*DIM1*DIM3, 
        C, 
        0, 
        NULL, 
        NULL);

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
    clReleaseEvent(user_event);
    clReleaseEvent(kernel_event);

    // Free host resources
#if TRANSPOSE==1
    free(Aocl);
    free(Bocl);
#else
    free(A);
    free(B);
#endif
    free(platforms);
    free(devices);

	gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
	long microseconds = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("OCL library execution time: %0.5lf ms \n", 
            microseconds/1000-nanoSeconds/1000000.0
            );

}