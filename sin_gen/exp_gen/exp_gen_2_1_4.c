#include "myhead.h"

#define BIT 2
#define BITNUM 4
#define BITNUM_1 3
#define DEGREE 5

static const DL
ln2by4HI = { .l = 0x3fc62e42fee00000 },
ln2by4LO = { .l = 0x3dca39ef35793c76 },
invln2by4 = { .l = 0x40171547652b82fe };

// fpminmax: [-1/4*ln2, 1/4*ln2]
static const DL
C[DEGREE] = {
	{.l = 0x3ff0000000aeb856},
	{.l = 0x3feffffe6c0a4aab},
	{.l = 0x3fe00049e7dab9da},
	{.l = 0x3fc54290a5d78482},
	{.l = 0x3fa741c37b948b28},
};
static const DL
interpolate[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff306fe0a31b715},
	{.l = 0x3ff6a09e667f3bcd},
	{.l = 0x3ffae89f995ad3ad}
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

	T = x * invln2by4.d;
	T_int = T;
	// 0 <= r <= 1/k1
	r1 = x - T_int * ln2by4HI.d;
	r2 = T_int * ln2by4LO.d;
	r = r1 - r2;

	r_poly = (r * r) * (C[2].d + r * (C[3].d + r * (C[4].d))) - r2 * C[1].d + r1 * C[1].d + C[0].d;

	lo = T_int & BITNUM_1;
	hi = T_int >> BIT;
	hi = (hi + 0x3ff) << 52;
	r_hi = *((double *)&hi);
	//r_lo = pow(2, (((double)lo) / ((double)k)));
	r_lo = interpolate[lo].d;
	result = r_hi * (r_lo * r_poly);

	return result;
}
