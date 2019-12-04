#include <math.h>
#include "myhead.h"

#define BIT 2
#define TABLE_NUM 4
#define DEGREE 9
#define COEFFICIENTS_NUM 5

// static const DL Twopm28 = { .l = 0x3e30000000000000 };

// P = fpminimax(asin(x), [| 1, 3, 5, 7, 9| ], [| D... | ], [0, sqrt(1 - (3 / 4)^2)]); printexpansion(P);
static const DL
C[COEFFICIENTS_NUM] = {
	{.l = 0x3ff000026f616c53},
	{.l = 0x3fc54d2fde90fce9},
	{.l = 0x3fb44689a0037e5c},
	{.l = 0x3f95281ce214111c},
	{.l = 0x3fb4beebede70f2d},
};

/* asin value: 0~1, step = 1/128 */
static const DL
asin_tab[TABLE_NUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3fd02be9ce0b87cd},
	{.l = 0x3fe0c152382d7366},
	{.l = 0x3feb235315c680dc},
};

/* cos(y) = sqrt(1-[x]^2) value: 0~1, step = 1/128 */
static const DL
sk[TABLE_NUM] = 
{
	{.l = 0x0000000000000000},
	{.l = 0x3fd0000000000000},
	{.l = 0x3fe0000000000000},
	{.l = 0x3fe8000000000000},
};

/* sin(y) = [x] value: 0~1, step = 1/128 */
static const DL
ck[TABLE_NUM] = 
{
	{.l = 0x3ff0000000000000},
	{.l = 0x3feefbdeb14f4eda},
	{.l = 0x3febb67ae8584caa},
	{.l = 0x3fe52a7fa9d2f8ea},
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
