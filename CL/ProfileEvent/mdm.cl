__kernel void matrix_dot_mul(const int RowSize, const int ColSize,
                             const __global int *A,
                             const __global int *B,
                             __global int *C) {
   
   // Indexing
   const int col = get_global_id(0);   
   const int row = get_global_id(1);
   const int wgdim1 = get_global_size(0);
   const int wgdim2 = get_global_size(1);

   // Computation
   // C[row*ColSize+col] = A[row*ColSize+col] * B[row*ColSize+col];
   C[row*wgdim2+col] = A[row*wgdim2+col] * B[row*wgdim2+col];

}