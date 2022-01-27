/*
 * ompforparallel.c
 *
 *  Created on: 27 Jan 2022
 *      Author: UDRC
 */

#include "ompforparallel.h"

void ompforparallel(){

	int num_steps = 1000000;
	double step, pi, sum;

	step = 1.0/num_steps;
	omp_set_num_threads(4);

	double start = omp_get_wtime();
	#pragma omp parallel
	{
		int i;
		double x;
		sum = 0;
		#pragma omp for
		for(i=0; i<num_steps; i++) {
			x = (i+0.5) * step;
			sum += 4.0/(1.0+x*x);
		}
	}
	pi = sum * step;
	double stop = omp_get_wtime();
	double exectime = stop - start;
	printf("time used: %lf us\n", exectime*1000*1000);

	printf("OMP For Parallel PI approximate = %0.20lf\n", pi);

}


