#include <stdio.h>
#include "myhead.h"
#define NUM 7

static const DL
C[NUM] = {
	{.l = 0x39a5b32c373a103b},
	{.l = 0x3ff0000000000000},
	{.l = 0xbcd93d23df47f8f2},
	{.l = 0xbfc5555555547450},
	{.l = 0xbe0477ee9a14db48},
	{.l = 0x3f81111e194f108f},
	{.l = 0xbede2bfcaa5c4d71},
};

double sin_gen(double x) {
	double result;

	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d + x * (C[6].d))))));

	return result;
}