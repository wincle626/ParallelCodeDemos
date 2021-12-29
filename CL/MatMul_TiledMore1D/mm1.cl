#define TS 32
#define WT 4
__kernel void matmul(const int M, 
                    const int N,
                    const int K,
                    const __global int *A,
                    const __global int *B,
                    __global int *C) {
   
    // Thread identifiers
    const int RTS = TS/WT; 
    const int localRow = get_local_id(0); // Local row ID (max TS)
    const int localCol= get_local_id(1); // Local row ID (max TS/WT)
    const int globalRow = TS*get_group_id(0)+localRow; // Row ID of C (0..M)
    const int globalCol = TS*get_group_id(1)+localCol; // Col ID of C (0..K)

    // Local memory to fit a tile of TS*TS elements of A and B
    __local int Asub[TS*TS];
    __local int Bsub[TS*TS];
 
    // Initialise the accumulation register
    int acc[WT];
    for (int w=0;w<WT;w++) {
        acc[w] = 0;
    }   

     // Loop over all tiles
    const int numTiles = N/TS;
    for (int t=0; t<numTiles; t++) {

        // Load one tile of A and B into local memory
        for(int w=0;w<WT;w++){
            const int tiledRow = TS*t + localRow;
            const int tiledCol = TS*t + localCol;        
            Asub[localRow*TS+localCol+w*RTS] = A[globalRow*N+tiledCol+w*RTS];
            Bsub[(localCol+w*RTS)*TS+localRow] = B[(globalCol+w*RTS)*N + tiledRow];
        }
 
        // Synchronise to make sure the tile is loaded
        barrier(CLK_LOCAL_MEM_FENCE);

        // Perform the computation for a single tile
        for (int k=0; k<TS; k++) {
            for(int w=0;w<WT;w++){
                acc[w] += Asub[localRow*TS+k] * Bsub[(localCol+w*RTS)*TS+k];
            }
        }
 
        // Synchronise before loading the next tile
        barrier(CLK_LOCAL_MEM_FENCE);

    }
 
    // Store the final result in C
    for(int w=0;w<WT;w++){
        C[globalRow*K+globalCol+w*RTS] = acc[w];
    }
   
}
