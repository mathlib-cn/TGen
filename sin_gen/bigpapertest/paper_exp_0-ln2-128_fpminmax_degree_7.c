#include <stdio.h>
#include "myhead.h"
#define NUM 8

static const DL
C[NUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff0000000000000},
	{.l = 0x3fe0000000000000},
	{.l = 0x3fc5555555555556},
	{.l = 0x3fa5555555554678},
	{.l = 0x3f811111119057bf},
	{.l = 0x3f56c167f90022e5},
	{.l = 0x3f2a12305f6902c9},
};

double exp_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d + x * (C[6].d + x * (C[7].d))))))); 



	return result;
}
