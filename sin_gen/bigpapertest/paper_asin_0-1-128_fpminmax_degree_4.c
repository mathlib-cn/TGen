
#include <stdio.h>
#include "myhead.h"
#define NUM 5

static const DL
C[NUM] = {
	{.l = 0x3be350dd2bc19f38},
	{.l = 0x3fefffffffffb222},
	{.l = 0x3e4344ae142757d0},
	{.l = 0x3fc55525433e41ff},
	{.l = 0x3f5335ed9c808b72},
};

double asin_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * C[4].d)));



	return result;
}