#include "myhead.h"

#define BIT 1
#define BITNUM 2
#define BITNUM_1 1
#define DEGREE 2

static const DL
ln2by2HI = { .l = 0x3fd62e42fee00000 },
ln2by2LO = { .l = 0x3dda39ef35793c76 },
invln2by2 = { .l = 0x40071547652b82fe };

// fpminmax: [-1/2*ln2, 1/2*ln2]
static const DL
C[DEGREE] = {
	{.l = 0x3fefc29b010051c2},
	{.l = 0x3ff2fab7f2ad6eeb},
};
static const DL
interpolate[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff6a09e667f3bcd}
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

	T = x * invln2by2.d;
	T_int = T;
	// 0 <= r <= 1/k1
	r1 = x - T_int * ln2by2HI.d;
	r2 = T_int * ln2by2LO.d;
	r = r1 - r2;

	r_poly = - r2 * C[1].d + r1 * C[1].d + C[0].d;

	lo = T_int & BITNUM_1;
	hi = T_int >> BIT;
	hi = (hi + 0x3ff) << 52;
	r_hi = *((double *)&hi);
	//r_lo = pow(2, (((double)lo) / ((double)k)));
	r_lo = interpolate[lo].d;
	result = r_hi * (r_lo * r_poly);

	return result;
}
