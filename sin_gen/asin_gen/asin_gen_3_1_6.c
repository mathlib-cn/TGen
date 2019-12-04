#include <math.h>
#include "myhead.h"

#define BIT 3
#define TABLE_NUM 8
#define DEGREE 13
#define COEFFICIENTS_NUM 7

// static const DL Twopm28 = { .l = 0x3e30000000000000 };

// P = fpminimax(asin(x), [| 1, 3, 5, 7, 9, 11, 13| ], [| D... | ], [0, sqrt(1 - (7 / 8)^2)]); printexpansion(P);
static const DL
C[COEFFICIENTS_NUM] = {
	{.l = 0x3ff00000000a77b1},
	{.l = 0x3fc55554cfdcccde},
	{.l = 0x3fb3337873b67e2d},
	{.l = 0x3fa6ce0b26da1552},
	{.l = 0x3fa029e25ad5f130},
	{.l = 0x3f8faf2cb8e3a357},
	{.l = 0x3fa260a861023846},
};

/* asin value: 0~1, step = 1/128 */
static const DL
asin_tab[TABLE_NUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3fc00abe0c129e1e},
	{.l = 0x3fd02be9ce0b87cd},
	{.l = 0x3fd899f4edc962d3},
	{.l = 0x3fe0c152382d7366},
	{.l = 0x3fe59aad71ced00f},
	{.l = 0x3feb235315c680dc},
	{.l = 0x3ff10c066d3e6932},
};

/* cos(y) = sqrt(1-[x]^2) value: 0~1, step = 1/128 */
static const DL
sk[TABLE_NUM] = 
{
	{.l = 0x0000000000000000},
	{.l = 0x3fc0000000000000},
	{.l = 0x3fd0000000000000},
	{.l = 0x3fd8000000000000},
	{.l = 0x3fe0000000000000},
	{.l = 0x3fe4000000000000},
	{.l = 0x3fe8000000000000},
	{.l = 0x3fec000000000000},
};

/* sin(y) = [x] value: 0~1, step = 1/128 */
static const DL
ck[TABLE_NUM] = 
{
	{.l = 0x3ff0000000000000},
	{.l = 0x3fefbfbf7ebc755f},
	{.l = 0x3feefbdeb14f4eda},
	{.l = 0x3fedaa2fefaae1d8},
	{.l = 0x3febb67ae8584caa},
	{.l = 0x3fe8fae0c15ad38a},
	{.l = 0x3fe52a7fa9d2f8ea},
	{.l = 0x3fdefbdeb14f4eda},
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
