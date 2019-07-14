#include <stdio.h>
#include <math.h>
#include "myhead.h"

#define BIT 6
#define BITNUM 64
#define DEGREE 6

static const double
ln2HI = 6.93147180369123816490e-01,  /* 0x3fe62e42, 0xfee00000 */
ln2LO = 1.90821492927058770002e-10, /* 0x3dea39ef, 0x35793c76 */
invln2 = 1.44269504088896338700e+00; /* 0x3ff71547, 0x652b82fe */

//
static const double
interpolate[BITNUM] = {
	0
};
static const DL
coefficient[DEGREE] = {
	{.d = 1},
	{.d = 1},
	{.d = 0.5},
	{.d = 1.0 / 6.0},
	{.d = 1.0 / 24.0},
	{.d = 1.0 / 120.0}
};

double exp_gen(double x) {
	double temp;
	int k;
	double k1;
	double T;
	long int T_int;
	double r;
	double r_poly;
	int hi, lo;
	double r_coefficient;
	double result;
	double r_hi, r_lo;

	k = BITNUM;
	k1 = k * invln2;
	T = x * k1;
	T_int = T;
	// 0 <= r1 <= 1/k1
	//r = x - ((double)T_int) / k1;
	r = x - T_int * (ln2HI / k);
	r = r - T_int * (ln2LO / k);

	r_poly = coefficient[0].d + r * (coefficient[1].d + r * (coefficient[2].d + r * (coefficient[3].d + r * (coefficient[4].d + r * (coefficient[5].d)))));

	lo = T_int % k;
	hi = T_int / k;
	r_hi = pow(2, hi);
	r_lo = pow(2, (((double)lo) / ((double)k)));
	r_coefficient = r_hi * r_lo;
	result = r_coefficient * r_poly;

	return result;
}

