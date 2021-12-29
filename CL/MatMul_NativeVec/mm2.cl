#define WIDTH 4
#if WIDTH == 1
    typedef int intX;
#elif WIDTH == 2
    typedef int2 intX;
#elif WIDTH == 4
    typedef int4 intX;
#endif
__kernel void matmul(const int M, 
                    const int N,
                    const int K,
                    const __global intX *A,
                    const __global intX *B,
                    __global intX *C) {
   
    // Thread identifiers
    const int globalRow = get_global_id(0); // Row ID of C (0..M)
    const int globalCol = get_global_id(1); // Col ID of C (0..K)
 
    // Initialise the accumulation registers
    int acc = 0;
 
    // Compute a single element (loop over N)    
    intX vecA, vecB, vecC;
    for (int n=0; n<N/WIDTH; n++) {
        // acc += A[globalRow*N + n] * B[globalCol + n*K];       
        vecA = A[globalRow*N/WIDTH + n]; 
        vecB = B[globalCol + n*K/WIDTH];
        vecC = vecA * vecB;
        #if WIDTH == 1
            acc += vecC;
        #elif WIDTH == 2
            acc += vecC.s0;
            acc += vecC.s1;
        #elif WIDTH == 4
            acc += vecC.s0;
            acc += vecC.s1;
            acc += vecC.s2;
            acc += vecC.s3;
        #endif
    }
 
    // Store the result
    C[globalRow*K/WIDTH + globalCol] = acc;
   
}
