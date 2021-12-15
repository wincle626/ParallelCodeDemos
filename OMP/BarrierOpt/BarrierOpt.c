#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv){

	int a[5], i;

    	#pragma omp parallel
    	{
        	// Perform some computation.
        	#pragma omp for
        	for (i = 0; i < 5; i++){
			int j = omp_get_thread_num();
            		a[i] = i * i;
			printf("a[%d]=%d*%d, on thread %d\n", i, i, i, j);
		}

        	// Print intermediate results.
        	#pragma omp master
            	for (i = 0; i < 5; i++){
			int j = omp_get_thread_num();
                	printf("a[%d] = %d, on thread %d\n", i, a[i], j);
		}

        	// Wait.
        	#pragma omp barrier

        	// Continue with the computation.
        	#pragma omp for
        	for (i = 0; i < 5; i++){
			int j = omp_get_thread_num();
			printf("a[%d]=%d+%d, on thread %d\n", i, a[i], i, j);
			a[i] += i;
		}

        	// Wait.
        	#pragma omp barrier

        	// Print intermediate results.
		#pragma omp single
		{
		    	for (i = 0; i < 5; i++){
				int j = omp_get_thread_num();
				printf("a[%d] = %d, on thread %d\n", i, a[i], j);
			}
		}	

		// Wait.
		#pragma omp barrier

        	// Continue with the computation.
        	#pragma omp for
        	for (i = 0; i < 5; i++){
			int j = omp_get_thread_num();
			printf("a[%d]=%d+%d, on thread %d\n", i, a[i], i, j);
			a[i] += i;
		}
	}

	return 0;

}
