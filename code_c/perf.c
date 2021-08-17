/* Teste la peformance de Q_rsqrt sur un grand nombre de valeurs aléatoires
 * À compiler avec `gcc -o test perf.c -lm`.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N_FLOAT 1000000
#define MAX_FLOAT 10000

float Q_rsqrt(float number)
{
	int32_t i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( int32_t * ) &y; 
	i  = 0x5f3759df - ( i >> 1 ); 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}

float rsqrt(float number) {
    float res;
#ifdef USE_rsqrtss
    asm ("rsqrtss %1, %0" : "=x" (res) : "xm" (number));
#else
    res = 1.f / sqrtf(number);
#endif
    return res;
}

int main() {
    float *the_floats = NULL, *results_qrsqrt = NULL, *results_rsqrt = NULL;
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
    results_qrsqrt = malloc(N_FLOAT *sizeof(float));
    
    if(results_qrsqrt == NULL) {
        printf("error allocating results_qrsqrt");
        return -1;
    }
    before = clock();
    for(i=0; i < N_FLOAT; i++)
        results_qrsqrt[i] = Q_rsqrt(the_floats[i]);
    t_qrsqrt = clock() - before;
    
    // performances de rsqrt()
    results_rsqrt = malloc(N_FLOAT *sizeof(float));
    
    if(results_rsqrt == NULL) {
        printf("error allocating results_rsqrt");
        return -1;
    }
    
    before = clock();
    for(i=0; i < N_FLOAT; i++)
        results_rsqrt[i] = rsqrt(the_floats[i]);
    t_rsqrt = clock() - before;
    
    // calcul de l'erreur relative
    for(i=0; i < N_FLOAT; i++) {
        error += fabs((results_qrsqrt[i] - results_rsqrt[i]) / results_rsqrt[i]);
    }
    
    printf("Q_rsqrt: %.5f nsecs/op\n", (double) t_qrsqrt / CLOCKS_PER_SEC / N_FLOAT * 1e9);
    printf("rsqrt: %.5f nsecs/op\n",  (double) t_rsqrt / CLOCKS_PER_SEC / N_FLOAT * 1e9);
    printf("erreur moyenne de Q_rsqrt: %.3f pourcents\n", error / N_FLOAT * 100);
    
    free(the_floats);
    free(results_qrsqrt);
    free(results_rsqrt);
    
    return 0;
}
