#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX 10

int main(int argc, char** argv) {

	int sharedmem = 0;
	int count = 0;	
	#pragma omp parallel num_threads(MAX)
	{
		int tid = omp_get_thread_num();
        	#pragma omp atomic
		sharedmem += tid;
        	#pragma omp atomic
        	count++;
	}
        printf("Number of threads: %d\n", count);
        printf("Sum of thread index is: %d\n", sharedmem);
	
	return 0;
	
}
