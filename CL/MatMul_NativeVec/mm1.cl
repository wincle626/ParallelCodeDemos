#define WIDTH 2
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

    // Check if we are within valid area of matrix C
    if( globalRow < M && globalCol < K ) {
        // Compute a single element (loop over N)  
        int NN = N/WIDTH;
        for (int n=0; n<NN; n++) {
            // acc += A[globalRow*N + n] * B[globalCol*N + n];  
            intX vecA = A[globalRow*NN + n]; 
            intX vecB = B[globalCol*NN + n];
            intX vecC = vecA * vecB;
            #if WIDTH == 1
                acc += vecC;
            #elif WIDTH == 2
                acc += vecC.x;
                acc += vecC.y;
            #elif WIDTH == 4
                acc += vecC.x;
                acc += vecC.y;
                acc += vecC.x;
                acc += vecC.w;
            #endif
        }
    }
 
    // Store the result
    C[globalRow*K + globalCol] = acc;
   
}
