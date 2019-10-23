#include <stdio.h>
#include "myhead.h"
#define NUM 7

static const DL
C[NUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff0000000000000},
	{.l = 0x3fe0000000000001},
	{.l = 0x3fc5555555553ced},
	{.l = 0x3fa5555556351b25},
	{.l = 0x3f81110d40415ef6},
	{.l = 0x3f56d16f7657ba80},
};

double exp_gen(double x) {
	double result;

	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d + x * (C[6].d))))));

	return result;
}
