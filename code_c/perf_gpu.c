/* Teste la peformance de Q_rsqrt sur un grand nombre de valeurs aléatoires
 * À compiler avec `gcc -o test perf_gpu.c -lm -fopenacc -foffload=-lm -fno-fast-math -fno-associative-math -Ofast`.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N_FLOAT 1000000
#define MAX_FLOAT 1000

int main() {
    float *the_floats = NULL, *results_rsqrt = NULL;
    clock_t before, t_qrsqrt, t_rsqrt;
    float error = .0f;
    int i;
    

    srand((unsigned) time(NULL));
    
    // créee le tableau  de flottants
    the_floats = malloc(N_FLOAT *sizeof(float));
    
    if(the_floats == NULL) {
        printf("error allocating the_floats");
        return -1;
    }
    
    for(i=0; i < N_FLOAT; i++) // génère N_FLOAT float entre 1 et MAX_FLOAT
        the_floats[i] = ((float) (1 + rand())) / RAND_MAX * MAX_FLOAT;
    
    // performance de Q_rsqrt()
    results_rsqrt = malloc(N_FLOAT *sizeof(float));
    
    if(results_rsqrt == NULL) {
        printf("error allocating results_qrsqrt");
        return -1;
    }
    
    before = clock();
    
    # pragma acc parallel
    # pragma acc loop
    for(i=0; i < N_FLOAT; i++)
        results_rsqrt[i] = 1.f / sqrtf(the_floats[i]);
        
    t_rsqrt = clock() - before;
    printf("rsqrt: %.5f nsecs/op\n",  (double) t_rsqrt / CLOCKS_PER_SEC / N_FLOAT * 1e9);
    
    free(the_floats);
    free(results_rsqrt);
}
