#include "myhead.h"

#define BIT 5
#define BITNUM 32
#define DEGREE 2

static const long long	twop5 =
{ 0x4040000000000000ll }; // 32

static const double twopm5 = 3.1250000000e-02;

static const DL	log2_lead =
{ .l = 0x3fe62e42fefa4000 };

static const DL	log2_trail =
{ .l = 0xbd48432a1b0e2634 };

static const DL
C[DEGREE - 1] = {
	{.l = 0x3fefc0d1bd3af815},
};
static const DL
logtabhi[BITNUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3f9f829b0e780000},
	{.l = 0x3faf0a30c0100000},
	{.l = 0x3fb6f0d28ae60000},
	{.l = 0x3fbe27076e2a0000},
	{.l = 0x3fc29552f8200000},
	{.l = 0x3fc5ff3070a80000},
	{.l = 0x3fc9525a9cf40000},
	{.l = 0x3fcc8ff7c79b0000},
	{.l = 0x3fcfb9186d5e0000},
	{.l = 0x3fd1675cabab8000},
	{.l = 0x3fd2e8e2bae10000},
	{.l = 0x3fd4618bc21c8000},
	{.l = 0x3fd5d1bdbf580000},
	{.l = 0x3fd739d7f6bc0000},
	{.l = 0x3fd89a3386c18000},
	{.l = 0x3fd9f323ecbf8000},
	{.l = 0xbfd1178e82280000},
	{.l = 0xbfcf991c6cb40000},
	{.l = 0xbfcd1037f2650000},
	{.l = 0xbfca93ed3c8b0000},
	{.l = 0xbfc823c165520000},
	{.l = 0xbfc5bf406b540000},
	{.l = 0xbfc365fcb0160000},
	{.l = 0xbfc1178e82280000},
	{.l = 0xbfbda72763840000},
	{.l = 0xbfb9335e5d5a0000},
	{.l = 0xbfb4d3115d200000},
	{.l = 0xbfb08598b59e0000},
	{.l = 0xbfa894aa14a00000},
	{.l = 0xbfa0415d89e80000},
	{.l = 0xbf90205658900000}
};

static const DL
logtablo[BITNUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3d2980267c7e09e4},
	{.l = 0x3d662a6617cc9717},
	{.l = 0xbd62968c836cc8c2},
	{.l = 0x3d6e5cbd3d50fffc},
	{.l = 0xbd35b967f4471dfc},
	{.l = 0xbd6b0b0de3077d7e},
	{.l = 0x3d65ad1d904c1d4e},
	{.l = 0xbd697794f689f843},
	{.l = 0x3d5f1546aaa3361c},
	{.l = 0x3d630701ce63eab9},
	{.l = 0x3d5d309c2cc91a85},
	{.l = 0xbd609ec17a426426},
	{.l = 0x3d4394a11b1c1ee4},
	{.l = 0xbd67fcb18ed9d603},
	{.l = 0xbd6ed2a52c73bf78},
	{.l = 0x3d584bf2b68d766f},
	{.l = 0x3d5b8421cc74be04},
	{.l = 0x3d6321a099af9906},
	{.l = 0xbd679ed606292424},
	{.l = 0x3d530e486a0ac42d},
	{.l = 0x3d670f91232ce70c},
	{.l = 0xbd5ed8fdc149767e},
	{.l = 0x3d6bfa7415f72d23},
	{.l = 0x3d4b8421cc74be04},
	{.l = 0xbd51a89401fa7173},
	{.l = 0x3d66ceea3c542b82},
	{.l = 0xbd5fab1769f42c78},
	{.l = 0xbd4d034451fecdfb},
	{.l = 0x3d432f32ba0b44d0},
	{.l = 0x3d577771fd187145},
	{.l = 0xbd6ac23a4f91d083}
};

static const DL
log_ru[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3fef07c1f07c1f08},
	{.l = 0x3fee1e1e1e1e1e1e},
	{.l = 0x3fed41d41d41d41d},
	{.l = 0x3fec71c71c71c71c},
	{.l = 0x3febacf914c1bad0},
	{.l = 0x3feaf286bca1af28},
	{.l = 0x3fea41a41a41a41a},
	{.l = 0x3fe999999999999a},
	{.l = 0x3fe8f9c18f9c18fa},
	{.l = 0x3fe8618618618618},
	{.l = 0x3fe7d05f417d05f4},
	{.l = 0x3fe745d1745d1746},
	{.l = 0x3fe6c16c16c16c17},
	{.l = 0x3fe642c8590b2164},
	{.l = 0x3fe5c9882b931057},
	{.l = 0x3fe5555555555555},
	{.l = 0x3fe4e5e0a72f0539},
	{.l = 0x3fe47ae147ae147b},
	{.l = 0x3fe4141414141414},
	{.l = 0x3fe3b13b13b13b14},
	{.l = 0x3fe3521cfb2b78c1},
	{.l = 0x3fe2f684bda12f68},
	{.l = 0x3fe29e4129e4129e},
	{.l = 0x3fe2492492492492},
	{.l = 0x3fe1f7047dc11f70},
	{.l = 0x3fe1a7b9611a7b96},
	{.l = 0x3fe15b1e5f75270d},
	{.l = 0x3fe1111111111111},
	{.l = 0x3fe0c9714fbcda3b},
	{.l = 0x3fe0842108421084},
	{.l = 0x3fe0410410410410}
};

double log_gen(double x) {
	_UL ix;
	int	m, k;
	double	w, u, t, xmu, q, l_lead, l_trail, result;

	/* extract exponent and sign of x for some quick screening */

	ix = *(unsigned long long *)(&x);	/* copy arg to a long long */

	m = (ix >> DMANTWIDTH);		/* shift off mantissa	*/

	m -= DEXPBIAS;

	/* normalize x and compute the nearest 1/32th to x */

	ix &= DMANTISSA;	/* get the mantissa of x */
	ix |= twop5;	/* set exponent of x to 5 */

	w = *(double *)(&ix);
	k = w;
	u = k;

	k -= BITNUM; // the number 32 here is for zhengshu bit "1"

	xmu = twopm5*(w - u);

	t = log_ru[k].d*xmu;

	/* avoid loss of significance for values of x near two
		by adjusting index; effectively u is divided by two.
		The logtable has been adjusted for this.
	*/

	if (k > 16)
		m++;

	l_lead = logtabhi[k].d;
	l_trail = logtablo[k].d;
	l_lead += m * log2_lead.d;
	l_trail += m * log2_trail.d;

	result = l_lead + (C[0].d * t + l_trail);

	return result;
}
