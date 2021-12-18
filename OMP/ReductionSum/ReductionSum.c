#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv){

   int i, sum=0;
   omp_set_num_threads(4);

   #pragma omp parallel
   {
      // #pragma omp for reduction(+:sum)
      #pragma omp for 
      for(i=1;i<=10;i++){
         int tid = omp_get_thread_num();
         sum+=i;
         printf("sum on thread %d is %d\n", tid, sum);
      }
   }
   printf("Sum of 1 to 10 is %d\n", sum);

   return 0;
}