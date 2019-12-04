#include <math.h>
#include "myhead.h"

#define BIT 1
#define TABLE_NUM 2
#define DEGREE 13
#define COEFFICIENTS_NUM 7

// static const DL Twopm28 = { .l = 0x3e30000000000000 };

// P = fpminimax(asin(x), [| 1, 3, 5, 7, 9, 11, 13| ], [| D... | ], [0, sqrt(1 - (1 / 2)^2)]); printexpansion(P);
static const DL
C[COEFFICIENTS_NUM] = {
	{.l = 0x3ff00009dc8cfddc},
	{.l = 0x3fc5310c862679b9},
	{.l = 0x3fb8981448cc49e8},
	{.l = 0xbfba3e58e4fd9b75},
	{.l = 0x3fe001b124886772},
	{.l = 0xbfe5f3c137032839},
	{.l = 0x3fdc509f98d0098b},
};

/* asin value: 0~1, step = 1/128 */
static const DL
asin_tab[TABLE_NUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3fe0c152382d7366},
};

/* cos(y) = sqrt(1-[x]^2) value: 0~1, step = 1/128 */
static const DL
sk[TABLE_NUM] = 
{
	{.l = 0x0000000000000000},
	{.l = 0x3fe0000000000000},
};

/* sin(y) = [x] value: 0~1, step = 1/128 */
static const DL
ck[TABLE_NUM] = 
{
	{.l = 0x3ff0000000000000},
	{.l = 0x3febb67ae8584caa},
};

double asin_gen(double x) {
	double result, temp, delta, zsq;
	int order;

	temp = x * (double)TABLE_NUM;
	order = temp;
	
	// delta = x * sqrt(1 - temp * temp) - temp * sqrt(1 - x * x);
	delta = x * ck[order].d - sk[order].d * sqrt(1.0 - x + x * (1.0 - x));

	x = delta;
	// if (delta < Twopm28.d)
	//	x = 0.0;
	zsq = x * x;

	result = C[0].d * delta + (delta * zsq) * (C[1].d + zsq * (C[2].d + zsq * (C[3].d + zsq * (C[4].d + zsq * (C[5].d + zsq * (C[6].d))))));

	result = asin_tab[order].d + result;

	return result;
}
