#include <stdio.h>
#include "myhead.h"
#define NUM 4

static const DL
C[NUM] = {
	{.l = 0xbc0a74cd5ad926e7},
	{.l = 0x3ff000000000d48c},
	{.l = 0xbe4f18d8e33f63ee},
	{.l = 0xbfc555423f60a2a6},
};

double sin_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d)));
	
	return result;
}