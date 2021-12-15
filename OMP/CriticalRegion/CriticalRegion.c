#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 10

int main(int argc, char** argv){

	#pragma omp parallel
	{
		int i = omp_get_thread_num();
		printf("I am a parallel region from thread %d.\n", i);
		#pragma omp critical 
		{
			printf("I am a critical region from thread %d.\n", i);
		}
	}
	printf("\n");

	#pragma omp parallel num_threads(4)
	{
		int i = omp_get_thread_num();
		printf("I am a parallel region from thread %d.\n", i);
		#pragma omp critical 
		{
			printf("I am a critical region from thread %d.\n", i);
		}
	}
	printf("\n");

	#pragma omp critical
	{
		int i = omp_get_thread_num();
		printf("I am a critical region from thread %d.\n", i);
		#pragma omp parallel num_threads(10)
		{
			int j = omp_get_thread_num();
			printf("I am a parallel region from thread %d.\n", j);
		}
	}
	printf("\n");

	int i;
	int max;
	int a[SIZE];

	for (i = 0; i < SIZE; i++)
	{
		a[i] = rand()%100;
		printf("%d\n", a[i]);
	}

	max = a[0];
	#pragma omp parallel for num_threads(4)
	for (i = 1; i < SIZE; i++)
	{
		if (a[i] > max)
		{
			#pragma omp critical
			{
				// compare a[i] and max again because max
				// could have been changed by another thread after
				// the comparison outside the critical section
				if (a[i] > max)
					max = a[i];
			}
		}
	}

	printf("max = %d\n", max);

	return 0;
}
