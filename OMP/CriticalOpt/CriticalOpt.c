#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv){

	int sum = 0;

	printf("Initial sum is %d\n", sum);
	#pragma omp parallel num_threads(4)
	{		
		int tid = omp_get_thread_num();
		// #pragma omp critical
		// {
			sum += tid;
			printf("sum is %d thread %d\n", sum, tid);
		// }
	}
	printf("Final sum is %d\n", sum);

	return 0;
}
