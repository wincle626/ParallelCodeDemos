/*
 * seq.c
 *
 *  Created on: 27 Jan 2022
 *      Author: UDRC
 */

#include <sequential.h>

void sequential()
{

	int num_steps = 1000000, i;
	double step, pi, x, sum;

	step = 1.0/num_steps;
	double start = omp_get_wtime();
	sum = 0;
	for(i=0; i<num_steps; i++) {
		x = (i+0.5) * step;
		sum += 4.0/(1.0+x*x);
	}
	pi = sum * step;
	double stop = omp_get_wtime();
	double exectime = stop - start;
	printf("time used: %lf us\n", exectime*1000*1000);
	printf("Sequantial PI approximate = %0.20lf\n", pi);

}


