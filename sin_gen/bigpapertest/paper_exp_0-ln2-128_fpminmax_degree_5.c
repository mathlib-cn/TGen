#include <stdio.h>
#include "myhead.h"
#define NUM 6

static const DL
C[NUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff0000000000001},
	{.l = 0x3fdfffffffffdc17},
	{.l = 0x3fc555555631bb85},
	{.l = 0x3fa5555099ee69d1},
	{.l = 0x3f811d0bfc4aaa45},
};

double exp_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d)))));
 
	return result;
}
