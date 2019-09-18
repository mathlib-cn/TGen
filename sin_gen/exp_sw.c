#include "libm.h"

extern	const du	__exptabhi[];
extern	const du	__exptablo[];

static const du	Ulimit =
{ D(0x40862e42, 0xfefa3940) };

static const du	Llimit =
{ D(0xc0874910, 0xd52d3052) };

static const du	rln2by32 =
{ D(0x40471547, 0x652b82fe) };

static const du	ln2by32hi =
{ D(0x3f962e42, 0xfef00000) };

static const du	ln2by32lo =
{ D(0x3d8473de, 0x6af278ed) };

static const du	Twopm54 =
{ D(0x3c900000, 0x00000000) };

static const	du	Qnan =
{ D(QNANHI, QNANLO) };

/* coefficients for polynomial approximation of exp on +/- log(2)/64     */
static const du	P[] =
{
{D(0x3ff00000, 0x00000000)},
{D(0x3ff00000, 0x00000000)},
{D(0x3fe00000, 0x00000000)},
{D(0x3fc55555, 0x55548f7c)},
{D(0x3fa55555, 0x55545d4e)},
{D(0x3f811115, 0xb7aa905e)},
{D(0x3f56c172, 0x8d739765)},
// genetate by Sollya
//{D(0x3ff00000, 0x00000000)},
//{D(0x3ff00000, 0x00000000)},
//{D(0x3fdfffff, 0xfffffffe)},
//{D(0x3fc55555, 0x55548ba1)},
//{D(0x3fa55555, 0x555b9e25)},
//{D(0x3f811115, 0xc090cf10)},
//{D(0x3f56c15c, 0xe3289cc3)},
};

double exp_gen(double x) {
	long long l;
	int	m, m1, m2;
	int	j, n;
	double	w;
	double	y1, y2, z;
	double	p, q;
	double	nd;
	double	s, s_lead, s_trail;
	double	twopm1, twopm2;
	double	result;

	w = x;

	if (x != x)
		x = 0.0;

	if (x > Ulimit.d)
		x = Ulimit.d;

	if (x < Llimit.d)
		x = Llimit.d;

	//if (fabs(x) < Twopm54.d)
	if ((x < Twopm54.d) && (x > -(Twopm54.d))) {
		x = 0.0;
	}	

	/* reduce x to +/- log(2)/64     */

	nd = x * rln2by32.d;
	//n = ROUND(nd);
	n = nd + 0.5;
	nd = n;

	y1 = x - nd * ln2by32hi.d;
	y2 = nd * ln2by32lo.d;
	z = y1 - y2;

	j = n & 0x1f;
	m = n >> 5;

	s_lead = __exptabhi[j].d;
	s_trail = __exptablo[j].d;
	s = s_lead + s_trail;

	q = ((((P[6].d*z + P[5].d)*z + P[4].d)*z + P[3].d)*z +
		P[2].d)*(z*z);

	p = (q - y2) + y1;

	result = s_lead + (s_trail + s * p);

	m1 = (m >> 1);
	m2 = m - m1;

	l = m1 + DEXPBIAS;
	l <<= DMANTWIDTH;

	//LL2DBL(l, twopm1);
	twopm1 = *((double *)(&l));

	l = m2 + DEXPBIAS;
	l <<= DMANTWIDTH;

	//LL2DBL(l, twopm2);
	twopm2 = *((double *)(&l));

	result *= twopm1;
	result *= twopm2;

	if (w != w)
		result = Qnan.d;

	return result;
}
