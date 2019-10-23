#include <stdio.h>
#include "myhead.h"
#define NUM 8

static const DL
C[NUM] = {
	{.l = 0x388f31424811bfa6},
	{.l = 0x3ff0000000000000},
	{.l = 0xbbc14b45eeb5d319},
	{.l = 0xbfc5555555555555},
	{.l = 0xbcf23be22b6678f5},
	{.l = 0x3f81111111184b69},
	{.l = 0xbdd6768e3f97b2c0},
	{.l = 0xbf2a0182aa4a0d41},
};

double sin_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d + x * (C[6].d + x * (C[7].d)))))));

	return result;
}