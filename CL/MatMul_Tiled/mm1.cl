#define TS 32
__kernel void matmul(const int M, 
                    const int N,
                    const int K,
                    const __global int *A,
                    const __global int *B,
                    __global int *C) {
   
    // Thread identifiers
    const int TileSize = get_local_size(0); // tile width is local size dimension
    const int localRow = get_local_id(0); // Local row ID (max TS)
    const int localCol= get_local_id(1); // Local row ID (max TS)
    const int globalRow = TileSize*get_group_id(0)+localRow; // Row ID of C (0..M)
    const int globalCol = TileSize*get_group_id(1)+localCol; // Col ID of C (0..K)

    // Local memory to fit a tile of TS*TS elements of A and B
    __local int Asub[TS*TS];
    __local int Bsub[TS*TS];
 
    // Initialise the accumulation register
    int acc = 0;

     // Loop over all tiles
    const int numTiles = N/TileSize;
    for (int t=0; t<numTiles; t++) {

        // Load one tile of A and B into local memory
        const int tiledRow = TileSize*t + localRow;
        const int tiledCol = TileSize*t + localCol;        
        Asub[localRow*TileSize+localCol] = A[globalRow*N + tiledCol];
        Bsub[localCol*TileSize+localRow] = B[globalCol*N + tiledRow];
 
        // Synchronise to make sure the tile is loaded
        barrier(CLK_LOCAL_MEM_FENCE);

        // Perform the computation for a single tile
        for (int k=0; k<TileSize; k++) {
            acc += Asub[localRow*TileSize+k] * Bsub[localCol*TileSize+k];
        }
 
        // Synchronise before loading the next tile
        barrier(CLK_LOCAL_MEM_FENCE);

    }
 
    // Store the final result in C
    C[globalRow*K+globalCol] = acc;
   
}
