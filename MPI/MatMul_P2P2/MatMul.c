/*
 *  mmult.c: matrix multiplication using MPI.
 * There are some simplifications here. The main one is that matrices B and C 
 * are fully allocated everywhere, even though only a portion of them is 
 * used by each processor (except for processor 0)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define DIM1 8
#define DIM2 6
#define DIM3 4

int *allocatevector(int len);
int **allocatematrix(int row, int col);
void matrix2vector(int row, int col, int **m, int *v);
void vector2matrix(int row, int col, int *v, int **m);
void matrixtranspose(int row, int col, int **m, int **tm);
void fill_matrix(int row, int col, int **m);
void print_matrix(int row, int col, int **m);
void print_vector(int len, int *v);
void matrixmult(int dim1, int dim2, int dim3, 
                int **A, int **B, int **C);
void matrixmult2(int dim1, int dim2, int dim3, 
                int **A, int **TB, int **C);

int main(int argc, char *argv[])
{
    int myrank, np, i, j, k;
    int tag = 666;		/* any value will do */
    MPI_Status status;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);	/* who am i */
    MPI_Comm_size(MPI_COMM_WORLD, &np); /* number of processors */

    /* Just to use the simple variants of MPI_Gather and MPI_Scatter we */
    /* impose that SIZE is divisible by P. By using the vector versions, */
    /* it is easy to drop this restriction. */

    if (DIM3%np!=0) {
        if (myrank==0) printf("Matrix size not divisible by number of processors\n");
            MPI_Finalize();
        exit(-1);
    }

    /* Process 0 fills the input matrices and send A to each process */
    /* and only the relevant stripe of B is sent */

    if (myrank==0) {

        // define data containers
        int **A=NULL, **B=NULL, **TB=NULL, **C=NULL, **C_check=NULL;
        int *vA=NULL, *vB=NULL, *vC=NULL;

        // allocate memory 
        A = allocatematrix(DIM1, DIM2);
        B = allocatematrix(DIM2, DIM3);
        TB = allocatematrix(DIM3, DIM2);
        C = allocatematrix(DIM1, DIM3);
        C_check = allocatematrix(DIM1, DIM3);

        // initiate the data
        fill_matrix(DIM1, DIM2, A);
        fill_matrix(DIM2, DIM3, B);
        matrixtranspose(DIM2, DIM3, B, TB);

        printf("%d process in total.\n", np);
        printf("\n\n");
        print_matrix(DIM1, DIM2, A);
        printf("\n\n\t       * \n");
        print_matrix(DIM2, DIM3, B);

        // matrix vectorization
        vA = allocatevector(DIM1*DIM2);
        vB = allocatevector(DIM2*DIM3);
        vC = allocatevector(DIM1*DIM3);
        matrix2vector(DIM1, DIM2, A, vA);
        matrix2vector(DIM3, DIM2, TB, vB);
        
        // send to workers
        for(i=1;i<np;i++){
            int dest = i;
            MPI_Send(vA, DIM1*DIM2, MPI_INT, 
                    dest, tag, MPI_COMM_WORLD);
            MPI_Send(&vB[i*DIM2*DIM3/np], DIM2*DIM3/np, MPI_INT, 
                    dest, tag, MPI_COMM_WORLD);
        }
        // do the matrix multiplication on host
        for(i=0;i<DIM1;i++){
            for(j=0;j<DIM3/np;j++){
                C[i][j] = 0;
                for(k=0;k<DIM2;k++){
                    C[i][j] += vA[i*DIM2+k] * vB[j*DIM2+k];
                }
            }
        }
        // receive data from workers
        int *vCpart = (int *) malloc(sizeof(int)*DIM1*DIM3/np);
        for(i=1;i<np;i++){
            int sour = i;
            MPI_Recv(vCpart, DIM1*DIM3/np, MPI_INT,
                    sour, tag, MPI_COMM_WORLD, &status);
            for(k=0;k<DIM1;k++){
                for(j=0;j<DIM3/np;j++){
                    C[k][i*DIM3/np+j] = vCpart[k*DIM3/np+j];
                }
            }
        }
        free(vCpart);

        // check with sequential version
        matrixmult(DIM1, DIM2, DIM3, A, B, C_check);
        // matrixmult2(DIM1, DIM2, DIM3, A, TB, C_check);

        printf("\n\n\t       = \n");
        print_matrix(DIM1, DIM3, C);
        printf("\n\n");
        printf("\tShould be:\n");
        print_matrix(DIM1, DIM3, C_check);
        printf("\n\n");

        // free up memory on host
        for(i=0;i<DIM1;i++){
            free(A[i]);
            free(C[i]);
            free(C_check[i]);
        }
        for(i=0;i<DIM2;i++){
            free(B[i]);
        }
        for(i=0;i<DIM3;i++){
            free(TB[i]);
        }
        free(A);
        free(B);
        free(TB);
        free(C);
        free(C_check);
        free(vA);
        free(vB);
        free(vC);

    }else{

        // define data containers
        int *vA=NULL, *vB=NULL, *vC=NULL;
        
        // allocate memory 
        vA = allocatevector(DIM1*DIM2);
        vB = allocatevector(DIM2*DIM3/np);
        vC = allocatevector(DIM1*DIM3/np);
        
        // receive data from host
        int sour = 0;
        int dest = 0;
        MPI_Recv(vA, DIM1*DIM2, MPI_INT, 
                sour, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(vB, DIM2*DIM3/np, MPI_INT, 
                sour, tag, MPI_COMM_WORLD, &status);
                
        // do the matrix multiplication on worker
        for(i=0;i<DIM1;i++){
            for(j=0;j<DIM3/np;j++){
                vC[i*DIM3/np+j] = 0;
                for(k=0;k<DIM2;k++){
                    vC[i*DIM3/np+j] += vA[i*DIM2+k] * vB[j*DIM2+k];
                }
            }
        }
        
        // send to host
        MPI_Send(vC, DIM1*DIM3/np, MPI_INT, 
                dest, tag, MPI_COMM_WORLD);

        // free up memory on worker
        free(vA);
        free(vB);
        free(vC);

    }

    MPI_Finalize();
    return 0;
}


int *allocatevector(int len){
    int *vec = (int *) malloc(sizeof(int)*len);
    memset(vec, 0, len*sizeof(int));
    return vec;
}

int **allocatematrix(int row, int col){
    int **m = (int **) malloc(sizeof(int*)*row);
    for(int i=0;i<row;i++){
        m[i] = allocatevector(col);
    }
    return m;
}

void matrix2vector(int row, int col, int **m, int *v){
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            v[i*col+j] = m[i][j];
        }
    }
}

void vector2matrix(int row, int col, int *v, int **m){
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            m[i][j] = v[i*col+j];
        }
    }
}

void matrixtranspose(int row, int col, int **m, int **tm){
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            tm[j][i] = m[i][j];
        }
    }
}

void fill_matrix(int row, int col, int **m)
{
    int n=0;
    int i, j;
    for (i=0; i<row; i++){
        n = i*col;
        for (j=0; j<col; j++)
            m[i][j] = n++;
    }
}

void print_matrix(int row, int col, int **m)
{
    int i, j = 0;
    for (i=0; i<row; i++) {
        printf("\n\t| ");
        for (j=0; j<col; j++)
            printf("%2d\t", m[i][j]);
        printf("|");
    }
}

void print_vector(int len, int *v){
    printf("\n\t| ");
    for (int i=0; i<len; i++) {
        printf("%2d\t", v[i]);
    }
    printf("|");
}


void matrixmult(int dim1, int dim2, int dim3, 
                int **A, int **B, int **C){
    for(int i=0;i<dim1;i++){
        for(int j=0;j<dim3;j++){
            C[i][j] = 0;
            for(int k=0;k<dim2;k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void matrixmult2(int dim1, int dim2, int dim3, 
                int **A, int **TB, int **C){
    for(int i=0;i<dim1;i++){
        for(int j=0;j<dim3;j++){
            C[i][j] = 0;
            for(int k=0;k<dim2;k++){
                C[i][j] += A[i][k] * TB[j][k];
            }
        }
    }
}