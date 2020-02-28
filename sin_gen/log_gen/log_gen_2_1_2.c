#include "myhead.h"

#define BIT 2
#define BITNUM 4
#define DEGREE 3

static const long long	twop2 =
{ 0x4010000000000000ll }; // 4

static const double twopm2 = 2.5000000000e-01;

static const DL	log2_lead =
{ .l = 0x3fe62e42fefa4000 };

static const DL	log2_trail =
{ .l = 0xbd48432a1b0e2634 };

static const DL
C[DEGREE - 1] = {
	{.l = 0x3fefee8c032150e5},
	{.l = 0xbfdb7141f1fdc47c},
};
static const DL
logtabhi[BITNUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3fcc8ff7c79b0000},
	{.l = 0x3fd9f323ecbf8000},
	{.l = 0xbfc1178e82280000}
};

static const DL
logtablo[BITNUM] = {
	{.l = 0x0000000000000000},
	{.l = 0xbd697794f689f843},
	{.l = 0x3d584bf2b68d766f},
	{.l = 0x3d4b8421cc74be04}
};

static const DL
log_ru[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3fe999999999999a},
	{.l = 0x3fe5555555555555},
	{.l = 0x3fe2492492492492}
};

double log_gen(double x) {
	_UL ix;
	int	m, k;
	double	w, u, t, xmu, q, l_lead, l_trail, result;

	/* extract exponent and sign of x for some quick screening */

	ix = *(unsigned long long *)(&x);	/* copy arg to a long long */

	m = (ix >> DMANTWIDTH);		/* shift off mantissa	*/

	m -= DEXPBIAS;

	/* normalize x and compute the nearest 1/4th to x */

	ix &= DMANTISSA;	/* get the mantissa of x */
	ix |= twop2;	/* set exponent of x to 2 */

	w = *(double *)(&ix);
	k = w;
	u = k;

	k -= BITNUM; // the number 4 here is for zhengshu bit "1"

	xmu = twopm2*(w - u);

	t = log_ru[k].d*xmu;

	/* avoid loss of significance for values of x near two
		by adjusting index; effectively u is divided by two.
		The logtable has been adjusted for this.
	*/

	if (k > 2)
		m++;

	q = (t * t) * (C[1].d);

	l_lead = logtabhi[k].d;
	l_trail = logtablo[k].d;
	l_lead += m * log2_lead.d;
	l_trail += m * log2_trail.d;

	result = l_lead + (C[0].d * t + (q + l_trail));

	return result;
}
