#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define LEN 16

int main(int argc, char** argv){

	int i = 0;
	int A[LEN], B[LEN], C[LEN];
	double time_spent = 0.0;

	omp_set_num_threads(8);

//	printf("Initiate the data: \n");
	for(i=0;i<LEN;i++){
		A[i] = i;
		B[i] = LEN - i;
//		printf("A[%d]=%d, B[%d]=%d\n",
//				i, A[i], i, B[i]);
	}
//	printf("\n");

	printf("OpenMP parallel for loop: \n");
	#pragma omp parallel for
	for(i=0;i<LEN;i++){
		C[i] = A[i] + B[i];
		if(LEN<=16){
			printf("C[%d]=A[%d]+B[%d]=%d\n",
					i, i, i, C[i]);
		}else{
			if(i>LEN-8)
				printf("C[%d]=A[%d]+B[%d]=%d\n",
					i, i, i, C[i]);
		}
	}
	printf("\n");
	printf("Sequential for loop: \n");
	for(i=0;i<LEN;i++){
		C[i] = A[i] + B[i];
		if(LEN<=16){
			printf("C[%d]=A[%d]+B[%d]=%d\n",
					i, i, i, C[i]);
		}else{
			if(i>LEN-8)
				printf("C[%d]=A[%d]+B[%d]=%d\n",
					i, i, i, C[i]);
		}
	}
	printf("\n");


	// System clock profile
	printf("Profile using system clock:\n");
	clock_t begin = clock();
	for(i=0;i<LEN;i++){
		C[i] = A[i] + B[i];
	}
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Execution time of sequential loop: %lf second\n", time_spent);
	begin = clock();
	#pragma omp parallel for
	for(i=0;i<LEN;i++){
		C[i] = A[i] + B[i];
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Execution time of paralle loop: %lf seconds\n\n", time_spent);

	// OMP clock profile
	printf("Profile using omp_get_wtime():\n");
	double start = omp_get_wtime();
	for(i=0;i<LEN;i++){
		C[i] = A[i] + B[i];
	}
	double stop = omp_get_wtime();
	time_spent = (double)(stop - start);
	printf("Execution time of sequential loop: %lf second\n", time_spent);
	start = omp_get_wtime();
	#pragma omp parallel for
	for(i=0;i<LEN;i++){
		C[i] = A[i] + B[i];
	}
	stop = omp_get_wtime();
	time_spent = (double)(stop - start);
	printf("Execution time of paralle loop: %lf seconds\n\n", time_spent);

	printf("Profile each thread using omp_get_wtime():\n");
	#pragma omp parallel for num_threads(16)
	for(i=0;i<LEN;i++){
		int tid = omp_get_thread_num();
		start = omp_get_wtime();
		C[i] = A[i] + B[i];
		stop = omp_get_wtime();
		time_spent = (double)(stop - start);
		printf("Execution time of thread %d: %lf seconds\n", tid, time_spent);
	}

	return 0;
}
