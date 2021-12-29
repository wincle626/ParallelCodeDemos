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
    int acc[WT][WT];
    for (int w1=0;w1<WT;w1++) {
        for (int w2=0;w2<WT;w2++) {
            acc[w1][w2] = 0;
        }
    }   

     // Loop over all tiles
    const int numTiles = N/TS;
    for (int t=0; t<numTiles; t++) {

        // Load one tile of A and B into local memory
        for(int w1=0;w1<WT;w1++){
            const int tiledRow = TS*t + localRow;
            const int tiledCol = TS*t + localCol;   
            for(int w2=0;w2<WT;w2++){     
                Asub[(localRow+w2*RTS)*TS+localCol+w1*RTS] = A[(globalRow+w2*RTS)*N+tiledCol+w1*RTS];
                Bsub[(localCol+w1*RTS)*TS+localRow+w2*RTS] = B[(globalCol+w1*RTS)*N+tiledRow+w2*RTS];
            }
        }
 
        // Synchronise to make sure the tile is loaded
        barrier(CLK_LOCAL_MEM_FENCE);

        // Perform the computation for a single tile
        for (int k=0; k<TS; k++) {
            for(int w1=0;w1<WT;w1++){
                for(int w2=0;w2<WT;w2++){
                    acc[w1][w2] += Asub[(localRow+w2*RTS)*TS+k] * Bsub[(localCol+w1*RTS)*TS+k];
                }
            }
        }
 
        // Synchronise before loading the next tile
        barrier(CLK_LOCAL_MEM_FENCE);

    }
 
    // Store the final result in C
    for(int w1=0;w1<WT;w1++){
        for(int w2=0;w2<WT;w2++){
            C[(globalRow+w2*RTS)*K+globalCol+w1*RTS] = acc[w1][w2];
        }
    }
   
}
