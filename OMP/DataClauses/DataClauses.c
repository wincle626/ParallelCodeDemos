// openmp_private.c
// compile with: /openmp
// #include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

#define NUM_THREADS 4
#define SLEEP_THREAD 1
#define NUM_LOOPS 2

enum Types {
   ThreadPrivate,
   Private,
   FirstPrivate,
   LastPrivate,
   Shared,
   MAX_TYPES
};

int nSave[NUM_THREADS][MAX_TYPES][NUM_LOOPS] = {{0}};
int nThreadPrivate;

#pragma omp threadprivate(nThreadPrivate)
#pragma warning(disable:4700)

int main() {
   int nPrivate = NUM_THREADS;
   int nFirstPrivate = NUM_THREADS;
   int nLastPrivate = NUM_THREADS;
   int nShared = NUM_THREADS;
   int nRet = 0;
   int i;
   int j;
   int nLoop = 0;

   nThreadPrivate = NUM_THREADS;
   printf("These are the variables before entry "
           "into the parallel region.\n");
   printf("nThreadPrivate = %d\n", nThreadPrivate);
   printf("      nPrivate = %d\n", nPrivate);
   printf(" nFirstPrivate = %d\n", nFirstPrivate);
   printf("  nLastPrivate = %d\n", nLastPrivate);
   printf("       nShared = %d\n\n", nShared);
   omp_set_num_threads(NUM_THREADS);

   #pragma omp parallel copyin(nThreadPrivate) private(nPrivate) shared(nShared) firstprivate(nFirstPrivate)
   {
      #pragma omp for schedule(static) lastprivate(nLastPrivate)
      for (i = 0 ; i < NUM_THREADS ; ++i) {
         for (j = 0 ; j < NUM_LOOPS ; ++j) {
            int nThread = omp_get_thread_num();
            assert(nThread < NUM_THREADS);

            if (nThread == SLEEP_THREAD)
               usleep(100*1000);
            nSave[nThread][ThreadPrivate][j] = nThreadPrivate;
            nSave[nThread][Private][j] = nPrivate;
            nSave[nThread][Shared][j] = nShared;
            nSave[nThread][FirstPrivate][j] = nFirstPrivate;
            nSave[nThread][LastPrivate][j] = nLastPrivate;
            nThreadPrivate = nThread;
            nPrivate = nThread;
            nShared = nThread;
            nLastPrivate = nThread;
            --nFirstPrivate;
         }
      }
   }

   for (i = 0 ; i < NUM_LOOPS ; ++i) {
      for (j = 0 ; j < NUM_THREADS ; ++j) {
         printf("These are the variables at entry of "
                  "loop %d of thread %d.\n", i + 1, j);
         printf("nThreadPrivate = %d\n",
                  nSave[j][ThreadPrivate][i]);
         printf("      nPrivate = %d\n",
                  nSave[j][Private][i]);
         printf(" nFirstPrivate = %d\n",
                  nSave[j][FirstPrivate][i]);
         printf("  nLastPrivate = %d\n",
                  nSave[j][LastPrivate][i]);
         printf("       nShared = %d\n\n",
                  nSave[j][Shared][i]);
      }
   }

   printf("These are the variables after exit from "
            "the parallel region.\n");
   printf("nThreadPrivate = %d (The last value in the "
            "master thread)\n", nThreadPrivate);
   printf("      nPrivate = %d (The value prior to "
            "entering parallel region)\n", nPrivate);
   printf(" nFirstPrivate = %d (The value prior to "
            "entering parallel region)\n", nFirstPrivate);
   printf("  nLastPrivate = %d (The value from the "
            "last iteration of the loop)\n", nLastPrivate);
   printf("       nShared = %d (The value assigned, "
            "from the delayed thread, %d)\n\n",
            nShared, SLEEP_THREAD);

   return 0;
}