#include "myhead.h"

#define BIT 5
#define BITNUM 32
#define BITNUM_1 31
#define DEGREE 4

static const DL
ln2by32HI = { .l = 0x3f962e42fee00000 },
ln2by32LO = { .l = 0x3d9a39ef35793c76 },
invln2by32 = { .l = 0x40471547652b82fe };

// fpminmax: [-1/32*ln2, 1/32*ln2]
static const DL
C[DEGREE] = {
	{.l = 0x3feffffffff626ad},
	{.l = 0x3ff000001c85a218},
	{.l = 0x3fdfff98f8298e5b},
	{.l = 0x3fc590c55eb4a9d3},
};
static const DL
interpolate[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff059b0d3158574},
	{.l = 0x3ff0b5586cf9890f},
	{.l = 0x3ff11301d0125b51},
	{.l = 0x3ff172b83c7d517b},
	{.l = 0x3ff1d4873168b9aa},
	{.l = 0x3ff2387a6e756238},
	{.l = 0x3ff29e9df51fdee1},
	{.l = 0x3ff306fe0a31b715},
	{.l = 0x3ff371a7373aa9cb},
	{.l = 0x3ff3dea64c123422},
	{.l = 0x3ff44e086061892d},
	{.l = 0x3ff4bfdad5362a27},
	{.l = 0x3ff5342b569d4f82},
	{.l = 0x3ff5ab07dd485429},
	{.l = 0x3ff6247eb03a5585},
	{.l = 0x3ff6a09e667f3bcd},
	{.l = 0x3ff71f75e8ec5f74},
	{.l = 0x3ff7a11473eb0187},
	{.l = 0x3ff82589994cce13},
	{.l = 0x3ff8ace5422aa0db},
	{.l = 0x3ff93737b0cdc5e5},
	{.l = 0x3ff9c49182a3f090},
	{.l = 0x3ffa5503b23e255d},
	{.l = 0x3ffae89f995ad3ad},
	{.l = 0x3ffb7f76f2fb5e47},
	{.l = 0x3ffc199bdd85529c},
	{.l = 0x3ffcb720dcef9069},
	{.l = 0x3ffd5818dcfba487},
	{.l = 0x3ffdfc97337b9b5f},
	{.l = 0x3ffea4afa2a490da},
	{.l = 0x3fff50765b6e4540}
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

	T = x * invln2by32.d;
	T_int = T;
	// 0 <= r <= 1/k1
	r1 = x - T_int * ln2by32HI.d;
	r2 = T_int * ln2by32LO.d;
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
