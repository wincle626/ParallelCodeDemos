__kernel void matmul(const int M, 
                    const int N,
                    const int K,
                    const __global int *A,
                    const __global int *B,
                    __global int *C) {
   
    // Thread identifiers
    const int globalRow = get_global_id(0); // Row ID of C (0..M)
    const int globalCol = get_global_id(1); // Col ID of C (0..K)
 
    // Compute a single element (loop over N)
    int acc = 0;
    for (int n=0; n<N; n++) {
        acc += A[globalRow*N + n] * B[globalCol*N + n];
    }
 
    // Store the result
    C[globalRow*K + globalCol] = acc;
   
}
