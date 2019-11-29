#include "myhead.h"

#define BIT 0
#define BITNUM 1
#define BITNUM_1 0
#define DEGREE 2

static const DL
ln2by1HI = { .l = 0x0 },
ln2by1LO = { .l = 0x0 },
invln2by1 = { .l = 0x0 };

// fpminmax: [-1/1*ln2, 1/1*ln2]
static const DL
C[DEGREE] = {
	{.l = 0x3fef0bb4660a65a0},
	{.l = 0x3ff6650e9bd9da77},
};
static const DL
interpolate[BITNUM] = {
	{.l = 0x3ff0000000000000}
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

	T = x * invln2by1.d;
	T_int = T;
	// 0 <= r <= 1/k1
	r1 = x - T_int * ln2by1HI.d;
	r2 = T_int * ln2by1LO.d;
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
