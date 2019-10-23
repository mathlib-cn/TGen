
#include <stdio.h>
#include "myhead.h"
#define NUM 6

static const DL
C[NUM] = {
	{.l = 0x3a1ebaae5b28ede9},
	{.l = 0x3ff0000000000000},
	{.l = 0xbd51d9c92cd5c5fe},
	{.l = 0x3fc5555555f36c2f},
	{.l = 0xbe7a0d617a1249e6},
	{.l = 0x3fb333eaea475ef4},
};

double asin_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d)))));

 


	return result;
}