// omp_set_dynamic.cpp
// compile with: /openmp
#include <stdio.h>
#include <omp.h>

int main()
{
    int i, num_dym = 3;
    omp_set_dynamic(num_dym);
    omp_set_num_threads(5);
    printf("omp_set_dynamic(%d), omp_set_num_threads(5).\n", num_dym);
    printf("omp_get_dynamic() in sequential region  returns %d "
            "in thread %d.\n", omp_get_dynamic(), omp_get_thread_num());
    #pragma omp parallel
    {
        #pragma omp for 
        for(i=0;i<5;i++){
            printf("omp_get_dynamic() in parallel region  returns %d "
                   "in thread %d.\n", omp_get_dynamic( ), omp_get_thread_num());
        }
    }
}