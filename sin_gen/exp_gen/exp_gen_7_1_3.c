#include "myhead.h"

#define BIT 7
#define BITNUM 128
#define BITNUM_1 127
#define DEGREE 4

static const DL
ln2by128HI = { .l = 0x3f762e42fee00000 },
ln2by128LO = { .l = 0x3d7a39ef35793c76 },
invln2by128 = { .l = 0x40671547652b82fe };

// fpminmax: [-1/128*ln2, 1/128*ln2]
static const DL
C[DEGREE] = {
	{.l = 0x3feffffffffff627},
	{.l = 0x3ff000000071c5c4},
	{.l = 0x3fdffff995f16a5d},
	{.l = 0x3fc5642370510f12},
};
static const DL
interpolate[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff0163da9fb3335},
	{.l = 0x3ff02c9a3e778061},
	{.l = 0x3ff04315e86e7f85},
	{.l = 0x3ff059b0d3158574},
	{.l = 0x3ff0706b29ddf6de},
	{.l = 0x3ff0874518759bc8},
	{.l = 0x3ff09e3ecac6f383},
	{.l = 0x3ff0b5586cf9890f},
	{.l = 0x3ff0cc922b7247f7},
	{.l = 0x3ff0e3ec32d3d1a2},
	{.l = 0x3ff0fb66affed31b},
	{.l = 0x3ff11301d0125b51},
	{.l = 0x3ff12abdc06c31cc},
	{.l = 0x3ff1429aaea92de0},
	{.l = 0x3ff15a98c8a58e51},
	{.l = 0x3ff172b83c7d517b},
	{.l = 0x3ff18af9388c8dea},
	{.l = 0x3ff1a35beb6fcb75},
	{.l = 0x3ff1bbe084045cd4},
	{.l = 0x3ff1d4873168b9aa},
	{.l = 0x3ff1ed5022fcd91d},
	{.l = 0x3ff2063b88628cd6},
	{.l = 0x3ff21f49917ddc96},
	{.l = 0x3ff2387a6e756238},
	{.l = 0x3ff251ce4fb2a63f},
	{.l = 0x3ff26b4565e27cdd},
	{.l = 0x3ff284dfe1f56381},
	{.l = 0x3ff29e9df51fdee1},
	{.l = 0x3ff2b87fd0dad990},
	{.l = 0x3ff2d285a6e4030b},
	{.l = 0x3ff2ecafa93e2f56},
	{.l = 0x3ff306fe0a31b715},
	{.l = 0x3ff32170fc4cd831},
	{.l = 0x3ff33c08b26416ff},
	{.l = 0x3ff356c55f929ff1},
	{.l = 0x3ff371a7373aa9cb},
	{.l = 0x3ff38cae6d05d866},
	{.l = 0x3ff3a7db34e59ff7},
	{.l = 0x3ff3c32dc313a8e5},
	{.l = 0x3ff3dea64c123422},
	{.l = 0x3ff3fa4504ac801c},
	{.l = 0x3ff4160a21f72e2a},
	{.l = 0x3ff431f5d950a897},
	{.l = 0x3ff44e086061892d},
	{.l = 0x3ff46a41ed1d0057},
	{.l = 0x3ff486a2b5c13cd0},
	{.l = 0x3ff4a32af0d7d3de},
	{.l = 0x3ff4bfdad5362a27},
	{.l = 0x3ff4dcb299fddd0d},
	{.l = 0x3ff4f9b2769d2ca7},
	{.l = 0x3ff516daa2cf6642},
	{.l = 0x3ff5342b569d4f82},
	{.l = 0x3ff551a4ca5d920f},
	{.l = 0x3ff56f4736b527da},
	{.l = 0x3ff58d12d497c7fd},
	{.l = 0x3ff5ab07dd485429},
	{.l = 0x3ff5c9268a5946b7},
	{.l = 0x3ff5e76f15ad2148},
	{.l = 0x3ff605e1b976dc09},
	{.l = 0x3ff6247eb03a5585},
	{.l = 0x3ff6434634ccc320},
	{.l = 0x3ff6623882552225},
	{.l = 0x3ff68155d44ca973},
	{.l = 0x3ff6a09e667f3bcd},
	{.l = 0x3ff6c012750bdabf},
	{.l = 0x3ff6dfb23c651a2f},
	{.l = 0x3ff6ff7df9519484},
	{.l = 0x3ff71f75e8ec5f74},
	{.l = 0x3ff73f9a48a58174},
	{.l = 0x3ff75feb564267c9},
	{.l = 0x3ff780694fde5d3f},
	{.l = 0x3ff7a11473eb0187},
	{.l = 0x3ff7c1ed0130c132},
	{.l = 0x3ff7e2f336cf4e62},
	{.l = 0x3ff80427543e1a12},
	{.l = 0x3ff82589994cce13},
	{.l = 0x3ff8471a4623c7ad},
	{.l = 0x3ff868d99b4492ed},
	{.l = 0x3ff88ac7d98a6699},
	{.l = 0x3ff8ace5422aa0db},
	{.l = 0x3ff8cf3216b5448c},
	{.l = 0x3ff8f1ae99157736},
	{.l = 0x3ff9145b0b91ffc6},
	{.l = 0x3ff93737b0cdc5e5},
	{.l = 0x3ff95a44cbc8520f},
	{.l = 0x3ff97d829fde4e50},
	{.l = 0x3ff9a0f170ca07ba},
	{.l = 0x3ff9c49182a3f090},
	{.l = 0x3ff9e86319e32323},
	{.l = 0x3ffa0c667b5de565},
	{.l = 0x3ffa309bec4a2d33},
	{.l = 0x3ffa5503b23e255d},
	{.l = 0x3ffa799e1330b358},
	{.l = 0x3ffa9e6b5579fdbf},
	{.l = 0x3ffac36bbfd3f37a},
	{.l = 0x3ffae89f995ad3ad},
	{.l = 0x3ffb0e07298db666},
	{.l = 0x3ffb33a2b84f15fb},
	{.l = 0x3ffb59728de5593a},
	{.l = 0x3ffb7f76f2fb5e47},
	{.l = 0x3ffba5b030a1064a},
	{.l = 0x3ffbcc1e904bc1d2},
	{.l = 0x3ffbf2c25bd71e09},
	{.l = 0x3ffc199bdd85529c},
	{.l = 0x3ffc40ab5fffd07a},
	{.l = 0x3ffc67f12e57d14b},
	{.l = 0x3ffc8f6d9406e7b5},
	{.l = 0x3ffcb720dcef9069},
	{.l = 0x3ffcdf0b555dc3fa},
	{.l = 0x3ffd072d4a07897c},
	{.l = 0x3ffd2f87080d89f2},
	{.l = 0x3ffd5818dcfba487},
	{.l = 0x3ffd80e316c98398},
	{.l = 0x3ffda9e603db3285},
	{.l = 0x3ffdd321f301b460},
	{.l = 0x3ffdfc97337b9b5f},
	{.l = 0x3ffe264614f5a129},
	{.l = 0x3ffe502ee78b3ff6},
	{.l = 0x3ffe7a51fbc74c83},
	{.l = 0x3ffea4afa2a490da},
	{.l = 0x3ffecf482d8e67f1},
	{.l = 0x3ffefa1bee615a27},
	{.l = 0x3fff252b376bba97},
	{.l = 0x3fff50765b6e4540},
	{.l = 0x3fff7bfdad9cbe14},
	{.l = 0x3fffa7c1819e90d8},
	{.l = 0x3fffd3c22b8f71f1}
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

	T = x * invln2by128.d;
	T_int = T;
	// 0 <= r <= 1/k1
	r1 = x - T_int * ln2by128HI.d;
	r2 = T_int * ln2by128LO.d;
	r = r1 - r2;

	r_poly = (r * r) * (C[2].d + r * (C[3].d)) - r2 * C[1].d + r1 * C[1].d + C[0].d;

	lo = T_int & BITNUM_1;
	hi = T_int >> BIT;
	hi = (hi + 0x3ff) << 52;
	r_hi = *((double *)&hi);
	//r_lo = pow(2, (((double)lo) / ((double)k)));
	r_lo = interpolate[lo].d;
	result = r_hi * (r_lo * r_poly);

	return result;
}
