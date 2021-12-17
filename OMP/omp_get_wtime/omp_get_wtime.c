#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

#define LEN 16
#define LOOP 1

int main(int argc, char** argv){

	double time_spent = 0.0;

	omp_set_num_threads(4);

	// OMP clock profile
	printf("Profile using omp_get_wtime():\n");
	double start = omp_get_wtime();
	usleep(1000);
	usleep(1000);
	usleep(1000);
	usleep(1000);
	double stop = omp_get_wtime();
	time_spent = (double)(stop - start)*1000*1000;
	printf("Execution time of sequential loop: %lf us\n", time_spent);
	start = omp_get_wtime();
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			usleep(1000);
		}
		#pragma omp section
		{
			usleep(1000);
		}
		#pragma omp section
		{
			usleep(1000);
		}
		#pragma omp section
		{
			usleep(1000);
		}
	}
	stop = omp_get_wtime();
	time_spent = (double)(stop - start)*1000*1000;
	printf("Execution time of paralle loop: %lf us\n\n", time_spent);

	return 0;
}
