#include "myhead.h"

#define BIT 6
#define BITNUM 64
#define DEGREE 5

static const long long	twop6 =
{ 0x4050000000000000ll }; // 64

static const double twopm6 = 1.5625000000e-02;

static const DL	log2_lead =
{ .l = 0x3fe62e42fefa4000 };

static const DL	log2_trail =
{ .l = 0xbd48432a1b0e2634 };

static const DL
C[DEGREE - 1] = {
	{.l = 0x3feffffffff38f34},
	{.l = 0xbfdfffff38a4cf76},
	{.l = 0x3fd5545b6bf73938},
	{.l = 0xbfcf36df996eaaee},
};
static const DL
logtabhi[BITNUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3f8fc0a8b1000000},
	{.l = 0x3f9f829b0e780000},
	{.l = 0x3fa77458f6340000},
	{.l = 0x3faf0a30c0100000},
	{.l = 0x3fb341d7961c0000},
	{.l = 0x3fb6f0d28ae60000},
	{.l = 0x3fba926d3a4a0000},
	{.l = 0x3fbe27076e2a0000},
	{.l = 0x3fc0d77e7cd10000},
	{.l = 0x3fc29552f8200000},
	{.l = 0x3fc44d2b6ccb0000},
	{.l = 0x3fc5ff3070a80000},
	{.l = 0x3fc7ab8902110000},
	{.l = 0x3fc9525a9cf40000},
	{.l = 0x3fcaf3c94e810000},
	{.l = 0x3fcc8ff7c79b0000},
	{.l = 0x3fce27076e2b0000},
	{.l = 0x3fcfb9186d5e0000},
	{.l = 0x3fd0a324e2738000},
	{.l = 0x3fd1675cabab8000},
	{.l = 0x3fd22941fbcf8000},
	{.l = 0x3fd2e8e2bae10000},
	{.l = 0x3fd3a64c55698000},
	{.l = 0x3fd4618bc21c8000},
	{.l = 0x3fd51aad872e0000},
	{.l = 0x3fd5d1bdbf580000},
	{.l = 0x3fd686c81e9b0000},
	{.l = 0x3fd739d7f6bc0000},
	{.l = 0x3fd7eaf83b828000},
	{.l = 0x3fd89a3386c18000},
	{.l = 0x3fd947941c210000},
	{.l = 0x3fd9f323ecbf8000},
	{.l = 0xbfd1bf99635a8000},
	{.l = 0xbfd1178e82280000},
	{.l = 0xbfd07138604d8000},
	{.l = 0xbfcf991c6cb40000},
	{.l = 0xbfce530effe70000},
	{.l = 0xbfcd1037f2650000},
	{.l = 0xbfcbd087383c0000},
	{.l = 0xbfca93ed3c8b0000},
	{.l = 0xbfc95a5adcf70000},
	{.l = 0xbfc823c165520000},
	{.l = 0xbfc6f0128b750000},
	{.l = 0xbfc5bf406b540000},
	{.l = 0xbfc4913d83340000},
	{.l = 0xbfc365fcb0160000},
	{.l = 0xbfc23d712a4a0000},
	{.l = 0xbfc1178e82280000},
	{.l = 0xbfbfe89139dc0000},
	{.l = 0xbfbda72763840000},
	{.l = 0xbfbb6ac88dae0000},
	{.l = 0xbfb9335e5d5a0000},
	{.l = 0xbfb700d30aea0000},
	{.l = 0xbfb4d3115d200000},
	{.l = 0xbfb2aa04a4480000},
	{.l = 0xbfb08598b59e0000},
	{.l = 0xbfaccb73cddc0000},
	{.l = 0xbfa894aa14a00000},
	{.l = 0xbfa466aed42c0000},
	{.l = 0xbfa0415d89e80000},
	{.l = 0xbf98492528c80000},
	{.l = 0xbf90205658900000},
	{.l = 0xbf80101575900000}
};

static const DL
logtablo[BITNUM] = {
	{.l = 0x0000000000000000},
	{.l = 0xbd5fe0e183092c59},
	{.l = 0x3d2980267c7e09e4},
	{.l = 0xbd62303b9cb0d5e1},
	{.l = 0x3d662a6617cc9717},
	{.l = 0xbd4717b6b33e44f8},
	{.l = 0xbd62968c836cc8c2},
	{.l = 0x3d6aac6ca17a4554},
	{.l = 0x3d6e5cbd3d50fffc},
	{.l = 0xbd6c69a65a23a170},
	{.l = 0xbd35b967f4471dfc},
	{.l = 0x3d6f4799f4f6543e},
	{.l = 0xbd6b0b0de3077d7e},
	{.l = 0xbd537b720e4a694b},
	{.l = 0x3d65ad1d904c1d4e},
	{.l = 0xbd600349cc67f9b2},
	{.l = 0xbd697794f689f843},
	{.l = 0xbd3a342c2af0003c},
	{.l = 0x3d5f1546aaa3361c},
	{.l = 0x3d50e35f73f7a018},
	{.l = 0x3d630701ce63eab9},
	{.l = 0xbd3a6976f5eb0963},
	{.l = 0x3d5d309c2cc91a85},
	{.l = 0xbd6d0b1c68651946},
	{.l = 0xbd609ec17a426426},
	{.l = 0xbd3f4bd8db0a7cc1},
	{.l = 0x3d4394a11b1c1ee4},
	{.l = 0x3d54aec442be1015},
	{.l = 0xbd67fcb18ed9d603},
	{.l = 0x3d67e1b259d2f3da},
	{.l = 0xbd6ed2a52c73bf78},
	{.l = 0x3d56faba4cdd147d},
	{.l = 0x3d584bf2b68d766f},
	{.l = 0x3d546b2255d70849},
	{.l = 0x3d5b8421cc74be04},
	{.l = 0x3d63cec649d22563},
	{.l = 0x3d6321a099af9906},
	{.l = 0xbd401212276041f4},
	{.l = 0xbd679ed606292424},
	{.l = 0x3d53a9788b2a8257},
	{.l = 0x3d530e486a0ac42d},
	{.l = 0xbd07f22858a0ff6f},
	{.l = 0x3d670f91232ce70c},
	{.l = 0xbd6aaee721a63de2},
	{.l = 0xbd5ed8fdc149767e},
	{.l = 0x3d62a7c86ab02498},
	{.l = 0x3d6bfa7415f72d23},
	{.l = 0x3d5eff2dc702c20a},
	{.l = 0x3d4b8421cc74be04},
	{.l = 0x3d454d3593e842bf},
	{.l = 0xbd51a89401fa7173},
	{.l = 0x3d649c84015fbb47},
	{.l = 0x3d66ceea3c542b82},
	{.l = 0xbd681c1e8da99ded},
	{.l = 0xbd5fab1769f42c78},
	{.l = 0x3d6d0b6e8ae9c698},
	{.l = 0xbd4d034451fecdfb},
	{.l = 0xbd6b2cb86dc13ec0},
	{.l = 0x3d432f32ba0b44d0},
	{.l = 0xbd6e3e98c8a4202d},
	{.l = 0x3d577771fd187145},
	{.l = 0xbd4957d173697cf3},
	{.l = 0xbd6ac23a4f91d083},
	{.l = 0x3d6c863b5ccce95f}
};

static const DL
log_ru[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3fef81f81f81f820},
	{.l = 0x3fef07c1f07c1f08},
	{.l = 0x3fee9131abf0b767},
	{.l = 0x3fee1e1e1e1e1e1e},
	{.l = 0x3fedae6076b981db},
	{.l = 0x3fed41d41d41d41d},
	{.l = 0x3fecd85689039b0b},
	{.l = 0x3fec71c71c71c71c},
	{.l = 0x3fec0e070381c0e0},
	{.l = 0x3febacf914c1bad0},
	{.l = 0x3feb4e81b4e81b4f},
	{.l = 0x3feaf286bca1af28},
	{.l = 0x3fea98ef606a63be},
	{.l = 0x3fea41a41a41a41a},
	{.l = 0x3fe9ec8e951033d9},
	{.l = 0x3fe999999999999a},
	{.l = 0x3fe948b0fcd6e9e0},
	{.l = 0x3fe8f9c18f9c18fa},
	{.l = 0x3fe8acb90f6bf3aa},
	{.l = 0x3fe8618618618618},
	{.l = 0x3fe8181818181818},
	{.l = 0x3fe7d05f417d05f4},
	{.l = 0x3fe78a4c8178a4c8},
	{.l = 0x3fe745d1745d1746},
	{.l = 0x3fe702e05c0b8170},
	{.l = 0x3fe6c16c16c16c17},
	{.l = 0x3fe6816816816817},
	{.l = 0x3fe642c8590b2164},
	{.l = 0x3fe6058160581606},
	{.l = 0x3fe5c9882b931057},
	{.l = 0x3fe58ed2308158ed},
	{.l = 0x3fe5555555555555},
	{.l = 0x3fe51d07eae2f815},
	{.l = 0x3fe4e5e0a72f0539},
	{.l = 0x3fe4afd6a052bf5b},
	{.l = 0x3fe47ae147ae147b},
	{.l = 0x3fe446f86562d9fb},
	{.l = 0x3fe4141414141414},
	{.l = 0x3fe3e22cbce4a902},
	{.l = 0x3fe3b13b13b13b14},
	{.l = 0x3fe3813813813814},
	{.l = 0x3fe3521cfb2b78c1},
	{.l = 0x3fe323e34a2b10bf},
	{.l = 0x3fe2f684bda12f68},
	{.l = 0x3fe2c9fb4d812ca0},
	{.l = 0x3fe29e4129e4129e},
	{.l = 0x3fe27350b8812735},
	{.l = 0x3fe2492492492492},
	{.l = 0x3fe21fb78121fb78},
	{.l = 0x3fe1f7047dc11f70},
	{.l = 0x3fe1cf06ada2811d},
	{.l = 0x3fe1a7b9611a7b96},
	{.l = 0x3fe1811811811812},
	{.l = 0x3fe15b1e5f75270d},
	{.l = 0x3fe135c81135c811},
	{.l = 0x3fe1111111111111},
	{.l = 0x3fe0ecf56be69c90},
	{.l = 0x3fe0c9714fbcda3b},
	{.l = 0x3fe0a6810a6810a7},
	{.l = 0x3fe0842108421084},
	{.l = 0x3fe0624dd2f1a9fc},
	{.l = 0x3fe0410410410410},
	{.l = 0x3fe0204081020408}
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

	/* normalize x and compute the nearest 1/64th to x */

	ix &= DMANTISSA;	/* get the mantissa of x */
	ix |= twop6;	/* set exponent of x to 6 */

	w = *(double *)(&ix);
	k = w;
	u = k;

	k -= BITNUM; // the number 64 here is for zhengshu bit "1"

	xmu = twopm6*(w - u);

	t = log_ru[k].d*xmu;

	/* avoid loss of significance for values of x near two
		by adjusting index; effectively u is divided by two.
		The logtable has been adjusted for this.
	*/

	if (k > 32)
		m++;

	q = (t * t) * (C[1].d + t * (C[2].d + t * (C[3].d)));

	l_lead = logtabhi[k].d;
	l_trail = logtablo[k].d;
	l_lead += m * log2_lead.d;
	l_trail += m * log2_trail.d;

	result = l_lead + (C[0].d * t + (q + l_trail));

	return result;
}
