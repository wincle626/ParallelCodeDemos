__kernel void vector_add(__local int *A, 
			__local int *B, 
			__global int *C) {
    
    // Get the index of the current element
    __private int i = get_global_id(0);

    // Do the operation
    C[i] = A[i] + B[i];
}
