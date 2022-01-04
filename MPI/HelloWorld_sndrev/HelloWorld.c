#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main( int argc, char *argv[] )
{
    int  numprocs, myrank, namelen, i;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    char greeting[MPI_MAX_PROCESSOR_NAME + 80];
    MPI_Status status;

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
    MPI_Get_processor_name( processor_name, &namelen );

	sprintf( greeting, "Hello, world, from process %d of %d on %s",
			myrank, numprocs, processor_name );

    if ( myrank == 0 ) {

		for ( i = 1; i < numprocs; i++ ) {
			MPI_Recv( greeting, sizeof( greeting ), MPI_CHAR,
						i, 1, MPI_COMM_WORLD, &status );
			printf( "%s\n", greeting );
		}
		
	}else{
		
		MPI_Send( greeting, strlen( greeting ) + 1, MPI_CHAR,
					0, 1, MPI_COMM_WORLD );

	}

    MPI_Finalize( );
    return 0;
}
