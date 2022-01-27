/*
 * naiveparallel.c
 *
 *  Created on: 27 Jan 2022
 *      Author: UDRC
 */

#include <parallel.h>

void parallel(){

	int num_steps = 1000000;
	double step, pi, sum[1000];
	int num_t=0, i=0;

	step = 1.0/num_steps;
	omp_set_num_threads(4);

	double start = omp_get_wtime();
	#pragma omp parallel
	{
		int i, id;
		int num_threads;
		double x;
		id = omp_get_thread_num();
		num_threads = omp_get_num_threads();
		if(id==0)
			num_t = num_threads;
		sum[id] = 0;
		for(i=id; i<num_steps; i+=num_threads) {
			x = (i+0.5) * step;
			sum[id] += 4.0/(1.0+x*x);
		}
	}
	double stop = omp_get_wtime();
	double exectime = stop - start;
	printf("time used: %lf us\n", exectime*1000*1000);

	pi = 0;
	for(i=0; i<num_t; i++)
		pi += sum[i] * step;
	printf("Parallel PI approximate = %0.20lf\n", pi);

}


