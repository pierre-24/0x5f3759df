/* Teste la peformance de Q_rsqrt sur un grand nombre de valeurs aléatoires
 * À compiler avec `gcc -o test perf.c -lm`.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N_FLOAT 100000
#define SEED 4242
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
    return 1.f / sqrtf(number);
}

int main() {
    float the_floats[N_FLOAT], results_qrsqrt[N_FLOAT], results_rsqrt[N_FLOAT];
    clock_t before, t_qrsqrt, t_rsqrt;
    float error = .0f;
    int i;
    

    srand((unsigned) time(NULL));
    
    for(i=0; i < N_FLOAT; i++) // génère N_FLOAT float entre 1 et MAX_FLOAT
        the_floats[i] = ((float) (1 + rand())) / RAND_MAX * MAX_FLOAT;
        
    // performance de Q_rsqrt()
    before = clock();
    for(i=0; i < N_FLOAT; i++)
        results_qrsqrt[i] = Q_rsqrt(the_floats[i]);
    t_qrsqrt = clock() - before;
    
    // performances de rsqrt()
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
}
