#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){

	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	//  Print off a hello world message
	printf("Hello world \n");

	// Finalize the MPI environment.
	MPI_Finalize();

	return 0;

}
