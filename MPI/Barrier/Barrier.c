#include <stdio.h>   // printf()
#include <mpi.h>     // MPI

#define BUFFER_SIZE 200
#define MASTER      0

int main(int argc, char** argv) {
    int id = -1, numProcesses = -1, length = -1;
    char hostName[MPI_MAX_PROCESSOR_NAME] = {'\0'};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Get_processor_name (hostName, &length);

    
    char buffer[BUFFER_SIZE] = {'\0'};;
    MPI_Status status;
    if (id != MASTER) {
        // Worker: Build a message and send it to the Master
        int length = sprintf(buffer,
                              "Process #%d of %d on %s is %s the barrier.\n",
                                id, numProcesses, hostName, "Before Barrier");
        MPI_Send(buffer, length+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        // Master: Receive and print the messages from all Workers
        for(int i = 0; i < numProcesses-1; i++) {
           MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, &status);
           printf("%s", buffer);
        }
    }

    // MPI_Barrier(MPI_COMM_WORLD);
    
    if (id != MASTER) {
        // Worker: Build a message and send it to the Master
        int length = sprintf(buffer,
                              "Process #%d of %d on %s is %s the barrier.\n",
                                id, numProcesses, hostName, "After Barrier");
        MPI_Send(buffer, length+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        // Master: Receive and print the messages from all Workers
        for(int i = 0; i < numProcesses-1; i++) {
           MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, &status);
           printf("%s", buffer);
        }
    }

    MPI_Finalize();
    return 0;
}