#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define LEN 4

int main(int argc, char** argv){

	int i = 0;
	int a[LEN], b[LEN], c[LEN];

	for(i=0;i<LEN;i++){
		a[i] = i;
		b[i] = LEN - i;
	}

	printf("Within paralle region: \n");
	#pragma omp parallel 
	{
		#pragma omp for
		for(i=0;i<LEN;i++){
			c[i] = a[i] + b[i];	
			int tid = omp_get_thread_num();	
			if(tid==0)			
					printf("c[%d]=a[%d]+b[%d] is %d (main thread: %d of %d threads).\n", 
					i, i, i,  c[i], tid, omp_get_num_threads());		
			else			
					printf("c[%d]=a[%d]+b[%d] is %d (split thread: %d of %d threads).\n", 
					i, i, i, c[i], tid, omp_get_num_threads());	
		}
	}
	printf("\n");
	printf("Without paralle region: \n");
	#pragma omp for
	for(i=0;i<LEN;i++){
		c[i] = a[i] + b[i];
		int tid = omp_get_thread_num();	
		if(tid==0)			
				printf("c[%d]=a[%d]+b[%d] is %d (main thread: %d of %d threads).\n", 
				i, i, i,  c[i], tid, omp_get_num_threads());		
		else			
				printf("c[%d]=a[%d]+b[%d] is %d (split thread: %d of %d threads).\n", 
				i, i, i, c[i], tid, omp_get_num_threads());	
	}
	printf("\n");

	return 0;
}
