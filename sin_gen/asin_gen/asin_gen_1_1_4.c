#include <math.h>
#include "myhead.h"

#define BIT 1
#define TABLE_NUM 2
#define DEGREE 9
#define COEFFICIENTS_NUM 5

// static const DL Twopm28 = { .l = 0x3e30000000000000 };

// P = fpminimax(asin(x), [| 1, 3, 5, 7, 9| ], [| D... | ], [0, sqrt(1 - (1 / 2)^2)]); printexpansion(P);
static const DL
C[COEFFICIENTS_NUM] = {
	{.l = 0x3ff000904a0053b2},
	{.l = 0x3fc44f59a8a20711},
	{.l = 0x3fc2e2b7ff93691e},
	{.l = 0xbfc5f50bdb53d77d},
	{.l = 0x3fd00893946bae86},
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

	result = C[0].d * delta + (delta * zsq) * (C[1].d + zsq * (C[2].d + zsq * (C[3].d + zsq * (C[4].d))));

	result = asin_tab[order].d + result;

	return result;
}
