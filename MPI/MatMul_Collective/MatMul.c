/*
 *  mmult.c: matrix multiplication using MPI.
 * There are some simplifications here. The main one is that matrices B and C 
 * are fully allocated everywhere, even though only a portion of them is 
 * used by each processor (except for processor 0)
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DIM1 8
#define DIM2 6
#define DIM3 4

void fill_matrix(int row, int col, int *m)
{
    int n=0;
    int i, j;
    for (i=0; i<row; i++)
        for (j=0; j<col; j++)
            m[i*col+j] = n++;
}

void transpose_matrix(int row, int col, int*m, int *tm){
    int i, j;
    for (i=0; i<row; i++)
        for (j=0; j<col; j++)
            tm[j*row+i] = m[i*col+j];
}

void print_matrix(int row, int col, int *m)
{
    int i, j = 0;
    for (i=0; i<row; i++) {
        printf("\n\t| ");
        for (j=0; j<col; j++)
            printf("%2d \t", m[i*col+j]);
        printf("|");
    }
}

int main(int argc, char *argv[])
{
    int myrank, np, from, to, i, j, k;
    int tag = 666;		/* any value will do */
    MPI_Status status;

    int *A, *B, *TB, *C;

    A = (int *) malloc(sizeof(int)*DIM1*DIM2);
    B = (int *) malloc(sizeof(int)*DIM2*DIM3);
    TB = (int *) malloc(sizeof(int)*DIM2*DIM3);
    C = (int *) malloc(sizeof(int)*DIM1*DIM3);

    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);	/* who am i */
    MPI_Comm_size(MPI_COMM_WORLD, &np); /* number of processors */

    /* Just to use the simple variants of MPI_Gather and MPI_Scatter we */
    /* impose that SIZE is divisible by np. By using the vector versions, */
    /* (MPI_Gatherv and MPI_Scatterv) it is easy to drop this restriction. */

    if (DIM1%np!=0) {
        if (myrank==0) printf("Matrix size not divisible by number of processors\n");
            MPI_Finalize();
        exit(-1);
    }

    /* Process 0 fills the input matrices and broadcasts them to the rest */
    /* (actually, only the relevant stripe of A is sent to each process) */
    if(myrank==0){        
        fill_matrix(DIM1, DIM2, A);
        fill_matrix(DIM2, DIM3, B);
        transpose_matrix(DIM2, DIM3, B, TB);
    }
    int *Apart = (int *) malloc(sizeof(int)*DIM1*DIM2/2);
    int *Cpart = (int *) malloc(sizeof(int)*DIM1*DIM3/2);

    MPI_Bcast (TB, DIM2*DIM3, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter (A, DIM1*DIM2/np, MPI_INT, 
                 Apart, DIM1*DIM2/np, MPI_INT, 
                 0, MPI_COMM_WORLD);
                 
    for (i=0; i<DIM1/np; i++) {
        for (j=0; j<DIM3; j++) {
            Cpart[i*DIM3+j]=0;
            for (k=0; k<DIM2; k++)
                Cpart[i*DIM3+j] += Apart[i*DIM2+k]*TB[j*DIM2+k];
        }
    }  
    
    MPI_Gather(Cpart, DIM1*DIM3/np, MPI_INT, 
               C, DIM1*DIM3/np, MPI_INT, 
               0, MPI_COMM_WORLD);		
	MPI_Barrier(MPI_COMM_WORLD);

    if(myrank==0){
        printf("\n\n");
        print_matrix(DIM1, DIM2, A);
        printf("\n\n\t       * \n");
        print_matrix(DIM2, DIM3, B);
        printf("\n\n\t       = \n");
        print_matrix(DIM1, DIM3, C);
        printf("\n\n");
    }

    MPI_Finalize(); 

    free(A);free(B);free(C);
    free(Apart);free(Cpart);

    return 0;
}

