#include <stdio.h>
#include "myhead.h"
#define NUM 5

static const DL
C[NUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3fefffffffffedee},
	{.l = 0x3fe00000007d30f8},
	{.l = 0x3fc555511976763b},
	{.l = 0x3fa563cf3c344453},
};

double exp_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * C[4].d)));

	return result;
}
