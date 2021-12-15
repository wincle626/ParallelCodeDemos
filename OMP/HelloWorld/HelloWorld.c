#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv){

	if(argc==2){
		int tnum = atoi(argv[1]);
		omp_set_num_threads(tnum);
	}
	
	printf("There are %d of threads in the sequential region. \n", 
		omp_get_num_threads());
	printf("There are maximum %d of threads in the system. \n",
		omp_get_max_threads());
	printf("There are %d of processor in the system. \n\n",
		omp_get_num_procs());

	#pragma omp parallel
	{	
		int tid = omp_get_thread_num();
		if(tid==0)
			printf("There are %d of threads in the parallel region. \n",
				omp_get_num_threads());
		printf("Hello World !! I am on %dth thread. \n", tid);
	}

	return 0;
}
