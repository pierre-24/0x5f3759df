/* Teste la précision de Q_rsqrt, en affichant le nombre obtenu pour différentes valeurs d'entrées
 * À compiler avec `gcc -o test prec.c -lm`.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float Q_rsqrt(float number, int second_step)
{
	int32_t i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( int32_t * ) &y; 
	i  = 0x5f3759df - ( i >> 1 ); 
	y  = * ( float * ) &i;
	
	if(second_step)
	    y  = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}

float rsqrt(float number) {
    return 1.f / sqrtf(number);
}

int main() {
    float n = .0f;
    int mult, i;
    
    for(int mult=-4; mult < 4; mult++) {
        for(int i=100; i < 1000; i++) {
            float n = i * pow(10, mult - 1);
            printf("%f\t%f\t%f\t%f\n", n, rsqrt(n), Q_rsqrt(n, 0), Q_rsqrt(n, 1));
        }
    }
}
