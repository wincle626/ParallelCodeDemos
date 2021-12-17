#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

#define LEN 16
#define LOOP 1

int main(int argc, char** argv){

	int i=0;
	int A[LEN], B[LEN], C[LEN];
	double time_spent = 0.0;

	for(i=0;i<LEN;i++){
		A[i] = i;
		B[i] = LEN - i;
		C[i] = 0;
	}
	omp_set_num_threads(4);

	// OMP clock profile
	printf("Profile using omp_get_wtime():\n");
	double start = omp_get_wtime();
	#pragma omp master
	for(i=0;i<LEN;i++){
		C[i] = A[i] + B[i];
	}
	double stop = omp_get_wtime();
	time_spent = (double)(stop - start)*1000*1000;
	printf("Execution time of sequential loop: %lf us\n", time_spent);
	start = omp_get_wtime();
	#pragma omp parallel 
	{
		#pragma omp for schedule(static, 4) private(A, B, C)
		for(i=0;i<LEN;i++){
			C[i] = A[i] + B[i];
		}
	}
	stop = omp_get_wtime();
	time_spent = (double)(stop - start)*1000*1000;
	printf("Execution time of paralle loop: %lf us\n\n", time_spent);

	return 0;
}
