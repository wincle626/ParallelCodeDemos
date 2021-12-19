#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char** argv) {

    int i, num_steps = 16384;
    float sum = 0.0, pi;
    float step = 1.0 / (float)num_steps;

    omp_set_num_threads(4);

    printf("Sequantial PI computation:\n");
    for (i = 0; i < num_steps; ++i) {
        float x = (i + 0.5) * step;
        float aux = 4.0 / (1.0 + x * x);
        sum += aux;
    }
    pi = step * sum;
    printf("PI is %.10f\n\n", pi);
    
    printf("Naive parallel PI computation: \n");
    #pragma omp parallel for 
    for (i = 0; i < num_steps; ++i) {
        float x = (i + 0.5) * step;
        float aux = 4.0 / (1.0 + x * x);
        sum += aux;
    }
    pi = step * sum;
    printf("PI is %.10f\n\n", pi);

    sum = 0.0;
    step = 1.0 / (double) num_steps;
    printf("Parallel PI computation using critical:\n");
    #pragma omp parallel private(i) shared(sum) 
    {
        #pragma omp for schedule(static, 1) 
        for (i=0; i<num_steps; ++i){
            float x = (i + 0.5) * step;
            float aux = 4.0 / (1.0 + x * x);
            #pragma omp critical
            sum += aux;
        }
    }
    pi = step * sum;
    printf("PI is %.10f\n\n", pi);

    sum = 0.0;
    step = 1.0 / (double) num_steps;
    printf("Parallel PI computation using reduction:\n");
    #pragma omp parallel private(i) shared(sum) 
    {
        #pragma omp for reduction(+:sum) schedule(static, 1)
        for (i=0; i<num_steps; ++i){
            float x = (i + 0.5) * step;
            float aux = 4.0 / (1.0 + x * x);
            #pragma omp atomic
            sum += aux;
        }
    }
    pi = step * sum;
    printf("PI is %.10f\n\n", pi);

    return 0;
}