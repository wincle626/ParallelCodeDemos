/*
 * scheduleparallel.c
 *
 *  Created on: 27 Jan 2022
 *      Author: UDRC
 */

#include <scheduleparallel.h>

void scheduleparallel(){

	int num_steps = 1000000, i;
	double step, pi, sum = 0.0, x;

	step = 1.0/num_steps;
	omp_set_num_threads(4);

	double start = omp_get_wtime();
	#pragma omp parallel shared(step) private(i,x)
	{
		#pragma omp for schedule(static, 64)
		for( i= 0 ; i<num_steps; i ++ ) {
			x = (i+0.5) * step;
			double aux = 4.0/(1.0+x*x);
			#pragma omp atomic
			sum += aux;
		}
	}
	pi = sum * step;
	double stop = omp_get_wtime();
	double exectime = stop - start;
	printf("time used: %lf us\n", exectime*1000*1000);

	printf("OMP For Schedule Parallel PI approximate = %0.20lf\n", pi);

}



