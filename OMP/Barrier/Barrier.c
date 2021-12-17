// omp_master.cpp
// compile with: /openmp
#include <omp.h>
#include <stdio.h>

int main( )
{
    int i, a[5]={0,0,0,0,0};
    #pragma omp parallel num_threads(4)
    {
		int tid = omp_get_thread_num();
		a[tid] = tid;
		printf("a[%d]=%d, on thread %d\n", 
				tid, a[tid], tid);
		#pragma omp barrier
		a[tid] += tid;
		printf("a[%d]=%d, on thread %d\n", 
				tid, a[tid], tid);
    }
}