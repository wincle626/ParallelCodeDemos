#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv){

	int i    = 0;
	int a[5] = {1, 3, 5, 7, 9};
	int b[5] = {2, 4, 6, 8, 10};
	int c[5] = {};


	printf("Total thread number is %d\n\n", 
		omp_get_num_threads());
	
	printf("Fork: 3 threads\n");
	#pragma omp parallel for num_threads(3)
	for(i=0;i<5;i++){
		int tid = omp_get_thread_num();
		c[i] = a[i] + b[i];
		if(tid==0)
			printf("c[%d]=a[%d]+b[%d] is %d (main thread: %d of %d threads).\n",
				i, i, i, 
				c[i], tid, omp_get_num_threads());
		else
			printf("c[%d]=a[%d]+b[%d] is %d (split thread: %d of %d threads).\n",
				i, i, i,
				c[i], tid, omp_get_num_threads());
	}

	printf("\nJoin:\n");
	printf("Total thread number is %d\n\n", 
		omp_get_num_threads());
	
	printf("Fork: 4 threads\n");
	#pragma omp parallel for num_threads(4)
	for(i=0;i<5;i++){
		int tid = omp_get_thread_num();
		c[i] = a[i] + b[i];
		if(tid==0)
			printf("c[%d]=a[%d]+b[%d] is %d (main thread: %d of %d threads).\n",
				i, i, i, 
				c[i], tid, omp_get_num_threads());
		else
			printf("c[%d]=a[%d]+b[%d] is %d (split thread: %d of %d threads).\n",
				i, i, i,
				c[i], tid, omp_get_num_threads());
	}

	printf("\nJoin:\n");
	printf("Total thread number is %d\n\n", 
		omp_get_num_threads());
	
	printf("Fork: 2 threads\n");
	#pragma omp parallel for num_threads(2)
	for(i=0;i<5;i++){
		int tid = omp_get_thread_num();
		c[i] = a[i] + b[i];
		if(tid==0)
			printf("c[%d]=a[%d]+b[%d] is %d (main thread: %d of %d threads).\n",
				i, i, i, 
				c[i], tid, omp_get_num_threads());
		else
			printf("c[%d]=a[%d]+b[%d] is %d (split thread: %d of %d threads).\n",
				i, i, i,
				c[i], tid, omp_get_num_threads());
	}

	printf("\nJoin:\n");
	printf("Total thread number is %d\n\n", 
		omp_get_num_threads());
	
	return 0;
}
