__kernel void matrix_dot_div(const int RowSize, const int ColSize,
                             const __global float *A,
                             const __global float *B,
                             __global float *C) {
   
   // Indexing
   const int col = get_global_id(0);   
   const int row = get_global_id(1);

   // Computation
   C[row*ColSize+col] = A[row*ColSize+col] / B[row*ColSize+col];

}
