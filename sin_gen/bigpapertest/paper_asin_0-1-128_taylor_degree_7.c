#include <stdio.h>
#include "myhead.h"
#define NUM 8


static const DL
C[NUM] = {
	{.l = 0},
	{.l = 0x3ff0000000000000},//asin 1
	{.l = 0},
	{.l = 0x3fc5555555555555},//asin 1/6
 	{.l = 0},
	{.l = 0x3fb3333333333333},//asin 3/40
	{.l = 0},
	{.l = 0x3fa6db6db6db6db7},//asin 5/112
//	{.l = 0},

};

double asin_gen(double x) {
	double result;
	

	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d +x * (C[6].d + x * (C[7].d)))))));
	return result;
}
