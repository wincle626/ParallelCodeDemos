__kernel void vector_add(__global int *A, __global int *B, __global int *C) {
    
    // Get the index of the current element
    int i = get_global_id(0);

    // Do the operation
    C[i] = A[i] + B[i];
}

__kernel void vector_mul(__global int *A, __global int *B, __global int *C){
	// Get the index of current element
	int i = get_global_id(0);

	// Do the operaiton
	C[i] = A[i] * B[i];
}

__kernel void vector_div(__global int *A, __global int *B, __global int *C){
	// Get the index of current element
	int i = get_global_id(0);

	// Do the operation
	C[i] = A[i] / B[i];
}
