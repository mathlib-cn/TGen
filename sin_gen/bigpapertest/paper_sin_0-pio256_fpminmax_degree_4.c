#include <stdio.h>
#include "myhead.h"
#define NUM 5

static const DL
C[NUM] = {
	{.l = 0x3bda18d9de49ab89},
	{.l = 0x3fefffffffffcb7b},
	{.l = 0x3e3094ecf9a8384e},
	{.l = 0xbfc5556281d0b611},
	{.l = 0x3f2ad18e4f0827b1},
};

double sin_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * C[4].d)));



	return result;
}