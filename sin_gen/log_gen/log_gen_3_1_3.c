#include "myhead.h"

#define BIT 3
#define BITNUM 8
#define DEGREE 4

static const long long	twop3 =
{ 0x4020000000000000ll }; // 8

static const double twopm3 = 1.2500000000e-01;

static const DL	log2_lead =
{ .l = 0x3fe62e42fefa4000 };

static const DL	log2_trail =
{ .l = 0xbd48432a1b0e2634 };

static const DL
C[DEGREE - 1] = {
	{.l = 0x3fefffe4c49b62b9},
	{.l = 0xbfdfe0e3ce17349c},
	{.l = 0x3fd2a547ab062403},
};
static const DL
logtabhi[BITNUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3fbe27076e2a0000},
	{.l = 0x3fcc8ff7c79b0000},
	{.l = 0x3fd4618bc21c8000},
	{.l = 0x3fd9f323ecbf8000},
	{.l = 0xbfca93ed3c8b0000},
	{.l = 0xbfc1178e82280000},
	{.l = 0xbfb08598b59e0000}
};

static const DL
logtablo[BITNUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3d6e5cbd3d50fffc},
	{.l = 0xbd697794f689f843},
	{.l = 0xbd609ec17a426426},
	{.l = 0x3d584bf2b68d766f},
	{.l = 0x3d530e486a0ac42d},
	{.l = 0x3d4b8421cc74be04},
	{.l = 0xbd4d034451fecdfb}
};

static const DL
log_ru[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3fec71c71c71c71c},
	{.l = 0x3fe999999999999a},
	{.l = 0x3fe745d1745d1746},
	{.l = 0x3fe5555555555555},
	{.l = 0x3fe3b13b13b13b14},
	{.l = 0x3fe2492492492492},
	{.l = 0x3fe1111111111111}
};

double log_gen(double x) {
	_UL ix;
	int	m, k;
	double	w, u, t, xmu, q, l_lead, l_trail, result;

	/* extract exponent and sign of x for some quick screening */

	ix = *(unsigned long long *)(&x);	/* copy arg to a long long */

	m = (ix >> DMANTWIDTH);		/* shift off mantissa	*/

	m -= DEXPBIAS;

	/* normalize x and compute the nearest 1/8th to x */

	ix &= DMANTISSA;	/* get the mantissa of x */
	ix |= twop3;	/* set exponent of x to 3 */

	w = *(double *)(&ix);
	k = w;
	u = k;

	k -= BITNUM; // the number 8 here is for zhengshu bit "1"

	xmu = twopm3*(w - u);

	t = log_ru[k].d*xmu;

	/* avoid loss of significance for values of x near two
		by adjusting index; effectively u is divided by two.
		The logtable has been adjusted for this.
	*/

	if (k > 4)
		m++;

	q = (t * t) * (C[1].d + t * (C[2].d));

	l_lead = logtabhi[k].d;
	l_trail = logtablo[k].d;
	l_lead += m * log2_lead.d;
	l_trail += m * log2_trail.d;

	result = l_lead + (C[0].d * t + (q + l_trail));

	return result;
}
