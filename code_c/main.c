#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

float rsqrt(float number) {
    return 1. / sqrt(number);
}

int main() {
    printf("%d", sizeof(long));
    printf("%.5f %.5f\n", Q_rsqrt(152.f), rsqrt(152.f));
    return 0;
}
