#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define LEN 16

int main(int argc, char** argv){

	int i = 0;
	int A[LEN], B[LEN], C[LEN];
	double time_spent = 0.0;

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
		if(LEN<16){
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
		if(LEN<16){
			printf("C[%d]=A[%d]+B[%d]=%d\n",
					i, i, i, C[i]);
		}else{
			if(i>LEN-8)
				printf("C[%d]=A[%d]+B[%d]=%d\n",
					i, i, i, C[i]);
		}
	}
	printf("\n");


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
	printf("Execution time of paralle loop: %lf seconds\n", time_spent);
	return 0;
}
