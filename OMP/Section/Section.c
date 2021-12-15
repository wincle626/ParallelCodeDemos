#include <stdio.h>
#include <omp.h>

void funcA() {
	printf("In funcA: this section is executed by thread %d\n",
	       omp_get_thread_num());
}

void funcB() {
	printf("In funcB: this section is executed by thread %d\n",
	       omp_get_thread_num());
}

void funcC() {
	printf("In funcC: this section is executed by thread %d\n",
	       omp_get_thread_num());
}

void funcD() {
	printf("In funcD: this section is executed by thread %d\n",
	       omp_get_thread_num());
}

void funcE() {
	printf("In funcE: this section is executed by thread %d\n",
	       omp_get_thread_num());
}

int main(int argc, char** argv){

	printf("No OpenMP Section:\n");
	#pragma omp parallel num_threads(4)
	{
		(void)funcA();
		(void)funcB();
		(void)funcC();
		(void)funcD();
	}

	printf("Using OpenMP Section:\n");
	#pragma omp parallel sections num_threads(4)
	{
		#pragma omp section 
		{
			(void)funcA();
		}	
		#pragma omp section 
		{
			(void)funcB();
		}	
		#pragma omp section 
		{
			(void)funcC();
		}
		#pragma omp section 
		{
			(void)funcD();
		}

	}

	return 0;
}
