#include <stdio.h>
#include "myhead.h"
#define NUM 6

static const DL
C[NUM] = {
	{.l = 0xb9d4fc64a5efeb6a},
	{.l = 0x3ff0000000000000},
	{.l = 0x3d0847eb23176753},
	{.l = 0xbfc5555555599c6c},
	{.l = 0x3e1cb882e949c36c},
	{.l = 0x3f811100f32d3a83},

};

double sin_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d)))));
	


	return result;
}