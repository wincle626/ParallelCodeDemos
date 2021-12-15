#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv){

	#pragma omp parallel num_threads(4)
   	{
		#pragma omp single
		{
			int i = omp_get_thread_num();
			// Only a single thread.
			printf("Single thread operations on thread %d\n", i);
		}

      		// Multiple threads in the team compute the results.
		{
			int j = omp_get_thread_num();
			printf("Do some thing in parallel on thread %d\n", j);
		}

		// #pragma omp single
		// {
		// 	int i = omp_get_thread_num();
		// 	// Only a single thread.
		// 	printf("Single thread operations on thread %d\n", i);
		// }

      	// 	// Multiple threads in the team compute the results.
		// {
		// 	int j  = omp_get_thread_num();
		// 	printf("Do some thing in parallel on thread %d\n", j);
		// }

		// #pragma omp single
		// {
		// 	int i = omp_get_thread_num();
		// 	// Only a single thread.
		// 	printf("Single thread operations on thread %d\n", i);
		// }

      	// 	// Multiple threads in the team compute the results.
		// {
		// 	int j  = omp_get_thread_num();
		// 	printf("Do some thing in parallel on thread %d\n", j);
		// }
	}

	return 0;

}
