//#include <stdio.h>
//#include <math.h>
#include "myhead.h"

#define BIT 6
#define BITNUM 64
#define BITNUM_1 63
#define DEGREE 6
// #define SUBTRACTOR 0x0060000000000000

static const DL
ln2by64HI = { .l = 0x3f862e42fee00000 }, // 6.93147180369123816490e-01 / 64
ln2by64LO = { .l = 0x3d8a39ef35793c76 }, // 1.90821492927058770002e-10 / 64
invln2by64 = { .l = 0x40571547652b82fe }; // 1.44269504088896338700e+00 * 64

// fpminmax: [-1/64*ln2, 1/64*ln2]
// 0x3ff0000000000000 + x * (0x3feffffffffffffc + x * (0x3fdffffffffdb3b3 + x * (0x3fc55555555cfcec + x * (0x3fa5555cfc82edbb + x * 0x3f8111079c6ade53))))
//static const DL
//coefficient[DEGREE] = {
//	{.l = 0x3ff0000000000000},
//	{.l = 0x3feffffffffffffc},
//	{.l = 0x3fdffffffffdb3b3},
//	{.l = 0x3fc55555555cfcec},
//	{.l = 0x3fa5555cfc82edbb},
//	{.l = 0x3f8111079c6ade53}
//	//{.l = 0x3f56c1728d739765},
//};

// 0x3ff0000000000000 + x * (0x3ff0000000000000 + x * (0x3fdffffffffffffe + x * (0x3fc5555555548ba1 + x * (0x3fa55555555b9e25 + x * (0x3f811115c090cf10 + x * 0x3f56c15ce3289cc3)))))
static const DL
coefficient[7] = {
	//{.l = 0x3ff0000000000000},
	//{.l = 0x3ff0000000000000},
	//{.l = 0x3fdffffffffffffe},
	//{.l = 0x3fc5555555548ba1},
	//{.l = 0x3fa55555555b9e25},
	//{.l = 0x3f811115c090cf10},
	//{.l = 0x3f56c15ce3289cc3},
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff0000000000000},
	{.l = 0x3fe0000000000000},
	{.l = 0x3fc5555555548f7c},
	{.l = 0x3fa5555555545d4e},
	{.l = 0x3f811115b7aa905e},
	{.l = 0x3f56c1728d739765},
};

// 0x3ff0000000000000 + x * (0x3ff0000000000000 + x * (0x3fe0000000000000 + x * (0x3fc5555555555555 + x * (0x3fa555555554e422 + x * (0x3f81111111130215 + x * (0x3f56c170ff5e6b4b + x * 0x3f2a01980c56f623))))))
//static const DL
//coefficient[8] = {
//	{.l = 0x3ff0000000000000},
//	{.l = 0x3ff0000000000000},
//	{.l = 0x3fe0000000000000},
//	{.l = 0x3fc5555555555555},
//	{.l = 0x3fa555555554e422},
//	{.l = 0x3f81111111130215},
//	{.l = 0x3f56c170ff5e6b4b},
//	{.l = 0x3f2a01980c56f623},
//};
static const DL
interpolate[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff02c9a3e778061},
	{.l = 0x3ff059b0d3158574},
	{.l = 0x3ff0874518759bc8},
	{.l = 0x3ff0b5586cf9890f},
	{.l = 0x3ff0e3ec32d3d1a2},
	{.l = 0x3ff11301d0125b51},
	{.l = 0x3ff1429aaea92de0},
	{.l = 0x3ff172b83c7d517b},
	{.l = 0x3ff1a35beb6fcb75},
	{.l = 0x3ff1d4873168b9aa},
	{.l = 0x3ff2063b88628cd6},
	{.l = 0x3ff2387a6e756238},
	{.l = 0x3ff26b4565e27cdd},
	{.l = 0x3ff29e9df51fdee1},
	{.l = 0x3ff2d285a6e4030b},
	{.l = 0x3ff306fe0a31b715},
	{.l = 0x3ff33c08b26416ff},
	{.l = 0x3ff371a7373aa9cb},
	{.l = 0x3ff3a7db34e59ff7},
	{.l = 0x3ff3dea64c123422},
	{.l = 0x3ff4160a21f72e2a},
	{.l = 0x3ff44e086061892d},
	{.l = 0x3ff486a2b5c13cd0},
	{.l = 0x3ff4bfdad5362a27},
	{.l = 0x3ff4f9b2769d2ca7},
	{.l = 0x3ff5342b569d4f82},
	{.l = 0x3ff56f4736b527da},
	{.l = 0x3ff5ab07dd485429},
	{.l = 0x3ff5e76f15ad2148},
	{.l = 0x3ff6247eb03a5585},
	{.l = 0x3ff6623882552225},
	{.l = 0x3ff6a09e667f3bcd},
	{.l = 0x3ff6dfb23c651a2f},
	{.l = 0x3ff71f75e8ec5f74},
	{.l = 0x3ff75feb564267c9},
	{.l = 0x3ff7a11473eb0187},
	{.l = 0x3ff7e2f336cf4e62},
	{.l = 0x3ff82589994cce13},
	{.l = 0x3ff868d99b4492ed},
	{.l = 0x3ff8ace5422aa0db},
	{.l = 0x3ff8f1ae99157736},
	{.l = 0x3ff93737b0cdc5e5},
	{.l = 0x3ff97d829fde4e50},
	{.l = 0x3ff9c49182a3f090},
	{.l = 0x3ffa0c667b5de565},
	{.l = 0x3ffa5503b23e255d},
	{.l = 0x3ffa9e6b5579fdbf},
	{.l = 0x3ffae89f995ad3ad},
	{.l = 0x3ffb33a2b84f15fb},
	{.l = 0x3ffb7f76f2fb5e47},
	{.l = 0x3ffbcc1e904bc1d2},
	{.l = 0x3ffc199bdd85529c},
	{.l = 0x3ffc67f12e57d14b},
	{.l = 0x3ffcb720dcef9069},
	{.l = 0x3ffd072d4a07897c},
	{.l = 0x3ffd5818dcfba487},
	{.l = 0x3ffda9e603db3285},
	{.l = 0x3ffdfc97337b9b5f},
	{.l = 0x3ffe502ee78b3ff6},
	{.l = 0x3ffea4afa2a490da},
	{.l = 0x3ffefa1bee615a27},
	{.l = 0x3fff50765b6e4540},
	{.l = 0x3fffa7c1819e90d8}
};

double exp_gen(double x) {
	double temp;
	double T;
	long int T_int;
	double r, r1, r2, rr;
	double r_poly;
	unsigned long int hi, lo;
	double result;
	double r_hi, r_lo;

	T = x * invln2by64.d;
	T_int = T;
	// 0 <= r <= 1/k1
	r1 = x - T_int * ln2by64HI.d;
	r2 = T_int * ln2by64LO.d;
	r = r1 - r2;
	/*temp = (double)T_int * ln2HI;
	temp_ui = (*((unsigned long int *)(&temp)));
	temp_ui = temp_ui - SUBTRACTOR;
	temp = *((double *)(&temp_ui));
	temp1 = (double)T_int * ln2LO;
	temp_ui1 = (*((unsigned long int *)(&temp1)));
	temp_ui1 = temp_ui1 - SUBTRACTOR;
	temp1 = *((double *)(&temp_ui1));
	r = x - temp;
	r = r - temp1;*/

	r_poly = (r * r) * (coefficient[2].d + r * (coefficient[3].d + r * (coefficient[4].d + r * (coefficient[5].d + r * (coefficient[6].d))))) - r2 + r1;
	// r_poly = coefficient[0].d + r * (coefficient[1].d + r * (coefficient[2].d + r * (coefficient[3].d + r * (coefficient[4].d + r * (coefficient[5].d + r * (coefficient[6].d + r * (coefficient[7].d)))))));
	lo = T_int & BITNUM_1;
	hi = T_int >> BIT;
	hi = (hi + 0x3ff) << 52;
	r_hi = *((double *)&hi);
	//r_lo = pow(2, (((double)lo) / ((double)k)));
	r_lo = interpolate[lo].d;
	result = r_hi * (r_lo + r_lo * r_poly);

	return result;
}
