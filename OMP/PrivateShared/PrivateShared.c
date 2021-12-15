#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NUM_THREADS 4
#define NUM_START 1
#define NUM_END 10

int main(int argc, char** argv){

	int i, nRet = 0, nSum = 0, nStart = NUM_START, nEnd = NUM_END;
	int nThreads = 0, nTmp = nStart + nEnd;
	unsigned uTmp = ((unsigned)(abs(nStart - nEnd) + 1) *
			(unsigned)abs(nTmp)) / 2;
	int nSumCalc = uTmp;

	if (nTmp < 0)
		nSumCalc = -nSumCalc;

	omp_set_num_threads(NUM_THREADS);

	#pragma omp parallel default(none) private(i) shared(nSum, nThreads, nStart, nEnd)
	{
		#pragma omp master
		nThreads = omp_get_num_threads();

		#pragma omp for
		for (i=nStart; i<=nEnd; ++i) {
			#pragma omp atomic
			nSum += i;
		}
	}

	if  (nThreads == NUM_THREADS) {
		printf("%d OpenMP threads were used.\n", NUM_THREADS);
		nRet = 0;
	}
	else {
		printf("Expected %d OpenMP threads, but %d were used.\n",
			NUM_THREADS, nThreads);
		nRet = 1;
	}

	if (nSum != nSumCalc) {
		printf("The sum of %d through %d should be %d, "
			"but %d was reported!\n",
			NUM_START, NUM_END, nSumCalc, nSum);
		nRet = 1;
	}
	else
		printf("The sum of %d through %d is %d\n",
			NUM_START, NUM_END, nSum);

	return 0;

}
