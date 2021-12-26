__kernel void matrix_dot_mul(const int RowSize, const int ColSize,
                             const __global int *A,
                             const __global int *B,
                             __global int *C) {
   
   // Indexing
   const int col = get_global_id(0);   
   const int row = get_global_id(1);

   // Computation
   C[row*ColSize+col] = A[row*ColSize+col] * B[row*ColSize+col];

}