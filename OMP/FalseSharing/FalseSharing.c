#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

#define LEN 16384
#define LOOP 100
#define PAD 16

int main(int argc, char** argv){

	int i=0,j=0;
	int C[LEN], C_PAD[LEN*PAD];
	double time_spent = 0.0;

	for(i=0;i<LEN;i++){
		C[i] = 0;
		for(j=0;j<PAD;j++){
			C_PAD[i*PAD+j] = 0;
		}
	}

	omp_set_num_threads(4);

	// OMP clock profile
	printf("Profile using omp_get_wtime():\n");
	double start = omp_get_wtime();
	#pragma omp master
	{
		for(i=0;i<LEN;i++){
			for(j=0;j<LOOP;j++){
				C[i] += C[i] + i;
			}
		}
	}
	double stop = omp_get_wtime();
	time_spent = (double)(stop - start)*1000*1000;
	printf("Execution time of sequential loop: %lf us\n", time_spent);

	start = omp_get_wtime();
	#pragma omp parallel shared(C)
	{	
		#pragma omp for private(i, j)
		for(i=0;i<LEN;i++){
			for(j=0;j<LOOP;j++){
				C[i] += C[i] + i;
			}
		}
	}
	stop = omp_get_wtime();
	time_spent = (double)(stop - start)*1000*1000;
	printf("Execution time of false sharing paralle loop: %lf us\n", time_spent);

	start = omp_get_wtime();
	#pragma omp parallel shared(C)
	{	
		#pragma omp for private(i, j)
		for(i=0;i<LEN;i++){
			int tmp = C[i];
			for(j=0;j<LOOP;j++){
				tmp += tmp + i;
			}
			C[i] = tmp;
		}
	}
	stop = omp_get_wtime();
	time_spent = (double)(stop - start)*1000*1000;
	printf("Execution time of tempvar fixed false sharing paralle loop: %lf us\n", time_spent);

	start = omp_get_wtime();
	#pragma omp parallel shared(C)
	{	
		#pragma omp for private(i, j)
		for(i=0;i<LEN;i++){
			for(j=0;j<LOOP;j++){
				C_PAD[i*PAD] += C_PAD[i*PAD] + i;
			}
		}
	}
	stop = omp_get_wtime();
	time_spent = (double)(stop - start)*1000*1000;
	printf("Execution time of padding fixed false sharing paralle loop: %lf us\n", time_spent);

	start = omp_get_wtime();
	#pragma omp parallel shared(C)
	{	
		#pragma omp for private(i, j) schedule(static, LEN/PAD)
		for(i=0;i<LEN;i++){
			for(j=0;j<LOOP;j++){
				C[i] += C[i] + i;
			}
		}
	}
	stop = omp_get_wtime();
	time_spent = (double)(stop - start)*1000*1000;
	printf("Execution time of schedule fixed false sharing paralle loop: %lf us\n", time_spent);

	return 0;
}
