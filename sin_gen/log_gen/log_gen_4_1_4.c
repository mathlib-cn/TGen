#include "myhead.h"

#define BIT 4
#define BITNUM 16
#define DEGREE 5

static const long long	twop4 =
{ 0x4030000000000000ll }; // 16

static const double twopm4 = 6.2500000000e-02;

static const DL	log2_lead =
{ .l = 0x3fe62e42fefa4000 };

static const DL	log2_trail =
{ .l = 0xbd48432a1b0e2634 };

static const DL
C[DEGREE - 1] = {
	{.l = 0x3feffffff48e56d3},
	{.l = 0xbfdfffd1f21e6466},
	{.l = 0x3fd546c6e62248ab},
	{.l = 0xbfcd04de34c177c2},
};
static const DL
logtabhi[BITNUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3faf0a30c0100000},
	{.l = 0x3fbe27076e2a0000},
	{.l = 0x3fc5ff3070a80000},
	{.l = 0x3fcc8ff7c79b0000},
	{.l = 0x3fd1675cabab8000},
	{.l = 0x3fd4618bc21c8000},
	{.l = 0x3fd739d7f6bc0000},
	{.l = 0x3fd9f323ecbf8000},
	{.l = 0xbfcf991c6cb40000},
	{.l = 0xbfca93ed3c8b0000},
	{.l = 0xbfc5bf406b540000},
	{.l = 0xbfc1178e82280000},
	{.l = 0xbfb9335e5d5a0000},
	{.l = 0xbfb08598b59e0000},
	{.l = 0xbfa0415d89e80000}
};

static const DL
logtablo[BITNUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3d662a6617cc9717},
	{.l = 0x3d6e5cbd3d50fffc},
	{.l = 0xbd6b0b0de3077d7e},
	{.l = 0xbd697794f689f843},
	{.l = 0x3d630701ce63eab9},
	{.l = 0xbd609ec17a426426},
	{.l = 0xbd67fcb18ed9d603},
	{.l = 0x3d584bf2b68d766f},
	{.l = 0x3d6321a099af9906},
	{.l = 0x3d530e486a0ac42d},
	{.l = 0xbd5ed8fdc149767e},
	{.l = 0x3d4b8421cc74be04},
	{.l = 0x3d66ceea3c542b82},
	{.l = 0xbd4d034451fecdfb},
	{.l = 0x3d577771fd187145}
};

static const DL
log_ru[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3fee1e1e1e1e1e1e},
	{.l = 0x3fec71c71c71c71c},
	{.l = 0x3feaf286bca1af28},
	{.l = 0x3fe999999999999a},
	{.l = 0x3fe8618618618618},
	{.l = 0x3fe745d1745d1746},
	{.l = 0x3fe642c8590b2164},
	{.l = 0x3fe5555555555555},
	{.l = 0x3fe47ae147ae147b},
	{.l = 0x3fe3b13b13b13b14},
	{.l = 0x3fe2f684bda12f68},
	{.l = 0x3fe2492492492492},
	{.l = 0x3fe1a7b9611a7b96},
	{.l = 0x3fe1111111111111},
	{.l = 0x3fe0842108421084}
};

double log_gen(double x) {
	_UL ix;
	int	j, m, k;
	double	w, u, t, xmu, q, l_lead, l_trail, result;

	/* extract exponent and sign of x for some quick screening */

	ix = *(unsigned long long *)(&x);	/* copy arg to a long long */

	m = (ix >> DMANTWIDTH);		/* shift off mantissa	*/
	j = m - MINEXP;

	m -= DEXPBIAS;

	/* normalize x and compute the nearest 1/16th to x */

	ix &= DMANTISSA;	/* get the mantissa of x */
	ix |= twop4;	/* set exponent of x to 4 */

	w = *(double *)(&ix);
	k = w;
	u = k;

	k -= BITNUM; // the number 16 here is for zhengshu bit "1"

	xmu = twopm4*(w - u);

	t = log_ru[k].d*xmu;

	/* avoid loss of significance for values of x near two
		by adjusting index; effectively u is divided by two.
		The logtable has been adjusted for this.
	*/

	if (k > 8)
		m++;

	q = (t * t) * (C[1].d + t * (C[2].d + t * (C[3].d)));

	l_lead = logtabhi[k].d;
	l_trail = logtablo[k].d;
	l_lead += m * log2_lead.d;
	l_trail += m * log2_trail.d;

	result = l_lead + (C[0].d * t + (q + l_trail));

	return result;
}
