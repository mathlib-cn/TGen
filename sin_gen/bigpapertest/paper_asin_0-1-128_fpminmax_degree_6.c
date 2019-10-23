
#include <stdio.h>
#include "myhead.h"
#define NUM 7

static const DL
C[NUM] = {
	{.l = 0xb9efbfce2e0f22a3},
	{.l = 0x3ff0000000000000},
	{.l = 0x3d228ef2e2d46be7},
	{.l = 0x3fc555555534d7e2},
	{.l = 0x3e6291cc88541b63},
	{.l = 0x3fb3329ea0ada4f8},
	{.l = 0x3f50e247125e2f06},
};

double asin_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d + x * (C[6].d))))));

 

	return result;
}