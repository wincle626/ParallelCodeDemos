#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#define ROW 256
#define COL 64
#define PAD 16

void check(int c1[ROW], int c2[ROW], int pad){
    int i = 0;
    for(i=0;i<ROW;i++){
        if((c1[i]-c2[i*pad])*(c1[i]-c2[i*pad])>0.00001){
            printf("Check failed!\n");
            break;
        }else{
            if(i==ROW-1)
                printf("Check succeeded!\n");
        }
    }
}

int main(int argc, char** argv){

    int i = 0, j = 0;
    int A[ROW][COL], b[COL], c[ROW], c1[ROW], c2[ROW*PAD];
    double start, stop, exe_time;

    for(i=0;i<COL;i++){
        for(j=0;j<ROW;j++){
            A[j][i] = j*ROW+i;
        }
        b[i] = i;
    }

    printf("Maximum processor number: %d\n", omp_get_num_procs());
    omp_set_num_threads(4);

    printf("Sequential matrix vector multiplication:\n");
    start = omp_get_wtime();
    #pragma omp master
    {
        for (i = 0; i < ROW; ++i) {
            c[i] = 0 ;
            for (j = 0; j < COL; ++j){
                c[i] += A[i][j]*b[j];
            }
        }
    }
    stop = omp_get_wtime();
    exe_time = stop - start;
    printf("Time of sequential Execution: %lf us\n", exe_time*1000*1000);

    printf("Naive parallel matrix vector multiplication:\n");
    start = omp_get_wtime();
    #pragma omp parallel 
    {
        #pragma omp for
        for (i = 0; i < ROW; ++i) {
            c1[i] = 0 ;
            for (j = 0; j < COL; ++j){
                c1[i] += A[i][j]*b[j];
            }
        }
    }
    stop = omp_get_wtime();
    exe_time = stop - start;
    printf("Time of naive parallel Execution: %lf us\n", exe_time*1000*1000);
    check(c, c1, 1);

    printf("Improved naive parallel matrix vector multiplication:\n");
    start = omp_get_wtime();
    #pragma omp parallel shared(A,b,c) private(i,j)
    {
        #pragma omp for 
        for (i = 0; i < ROW; ++i) {
            c1[i] = 0 ;
            for (j = 0; j < COL; ++j){
                c1[i] += A[i][j]*b[j];
            }
        }
    }
    stop = omp_get_wtime();
    exe_time = stop - start;
    printf("Time of naive parallel Execution: %lf us\n", exe_time*1000*1000);
    check(c, c1, 1);

    printf("Optimal parallel matrix vector multiplication:\n");
    // omp_set_nested(2);
    start = omp_get_wtime();
    #pragma omp parallel shared(A,b,c) private(i,j)
    {
        #pragma omp for schedule(static) nowait
        for (i = 0; i < ROW; ++i) {
            int sum = 0;
            for (j = 0; j < COL; ++j){
                int mul = A[i][j]*b[j];
                sum += mul;
            }
            #pragma omp atomic 
            c2[i] += sum;
        }
    }
    stop = omp_get_wtime();
    exe_time = stop - start;
    printf("Time of naive parallel Execution: %lf us\n", exe_time*1000*1000);
    check(c, c2, 1);


    return 0;
}