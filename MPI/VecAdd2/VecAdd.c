#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LEN 16

int main(argc,argv) int argc; char *argv[];
{
    int numtasks, rank, dest, source, rc, count, tag=1;  
    int i, *A, *B, *C; 
    MPI_Status Stat;


    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        dest = 1;
        source = 1;
        A = (int*) malloc(sizeof(int)*LEN);
        B = (int*) malloc(sizeof(int)*LEN);
        C = (int*) malloc(sizeof(int)*LEN);
        for(i=0;i<LEN;i++){
            A[i] = i; B[i] = LEN - i;
            C[i] = 0;
        }
        rc = MPI_Send(&A[LEN/2], LEN/2, MPI_INT, dest, tag, MPI_COMM_WORLD);
        rc = MPI_Send(&B[LEN/2], LEN/2, MPI_INT, dest, tag, MPI_COMM_WORLD);
        for(i=0;i<LEN/2;i++){C[i] = A[i] + B[i];}
        rc = MPI_Recv(&C[LEN/2], LEN/2, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
        for(i=0;i<LEN;i++){
            printf("C[%d]=%d\n",i,C[i]);
        }
    } else if (rank == 1) {
        dest = 0;
        source = 0;
        int *tmpA, *tmpB, *tmpC;
        tmpA = (int*) malloc(sizeof(int)*LEN/2);
        tmpB = (int*) malloc(sizeof(int)*LEN/2);
        tmpC = (int*) malloc(sizeof(int)*LEN/2);
        rc = MPI_Recv(tmpA, LEN/2, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
        rc = MPI_Recv(tmpB, LEN/2, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
        for(i=0;i<LEN/2;i++){tmpC[i] = tmpA[i] + tmpB[i];}
        rc = MPI_Send(tmpC, LEN/2, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }

    rc = MPI_Get_count(&Stat, MPI_INT, &count);
    printf("Process %d: Received %d int(s) from task %d with tag %d \n",
           rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
    MPI_Finalize();
}
