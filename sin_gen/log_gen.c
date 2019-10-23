#include "libm.h"

extern const du _logtabhi[];
extern const du _logtablo[];
extern const du _log_ru[];

static const long long twop7 =
{ 0x4060000000000000ll }; // 128

static const du twopm7 =
{ D(0x3f800000, 0x00000000) }; // 1/128

static const du	log2_lead =
{ D(0x3fe62e42, 0xfefa4000) };

static const du	log2_trail =
{ D(0xbd48432a, 0x1b0e2634) };

static const du	Scaleup =
{ D(0x43300000, 0x00000000) };

static const du	Qnan =
{ D(QNANHI, QNANLO) };

static const du	Inf =
{ D(0x7ff00000, 0x00000000) };

static const du	Neginf =
{ D(0xfff00000, 0x00000000) };

/* coefficients for polynomial approximation of log(1 + t) on +/- 1/256   */

// x * (0x3ff0000000000000 + x * (0xbfdffffffffffffc + x * (0x3fd5555555552dde + x * (0xbfcffffffefe562c + x * (0x3fc9999817d3a30f + x * (0xbfc554317b3e1c57 + x * 0x3fc1dc5c459adbd8))))))
static const du	P[] =
{
{D(0x3ff00000, 0x00000000)},
{D(0xbfdfffff, 0xfffffffc)},
{D(0x3fd55555, 0x55552dde)},
{D(0xbfcfffff, 0xfefe562c)},
{D(0x3fc99998, 0x17d3a30f)},
{D(0xbfc55431, 0x7b3e1c57)},
{D(0x3fc1dc5c, 0x459adbd8)}
};

#define MAXEXP	0x7ffu

#define	MINEXP	0x001u

double log_gen(double x) {
	unsigned long long ix;
	int	j;
	int	m;
	int	k;
	double	u;
	double	t;
	double	xmu;
	double	q;
	double	l_lead, l_trail;
	double	w;
	double	result;

	/* extract exponent and sign of x for some quick screening */

	ix = *(unsigned long long *)(&x);	/* copy arg to a long long */

	m = (ix >> DMANTWIDTH);		/* shift off mantissa	*/
	j = m - MINEXP;

	m -= DEXPBIAS;

	/* normalize x and compute the nearest 1/128th to x */

	ix &= (DSIGNMASK & DEXPMASK);	/* mask off sign and exponent
						* bits of x
						*/
	ix |= twop7;	/* set exponent of x to 0x406 */

	/* adjust scaled arg	*/

	//LL2DBL(ix, w);
	w = *(double *)(&ix);

	//k = ROUND(w);
	k = w + 0.5; // !!! +0.5 !!!

	u = k;

	k -= 128;

	xmu = twopm7.d*(w - u);

	t = _log_ru[k].d*xmu;

	/* avoid loss of significance for values of x near two
		by adjusting index; effectively u is divided by two.
		The logtable has been adjusted for this.
	*/

	if (k > 64) {
		m++;
	}

	q = (((((P[6].d*t + P[5].d)*t + P[4].d)*t + P[3].d)*t + P[2].d)*t + P[1].d)*(t*t);
	
	l_lead = _logtabhi[k].d;
	l_trail = _logtablo[k].d;

	l_lead += m * log2_lead.d;
	l_trail += m * log2_trail.d;

	result = l_lead + (t + (q + l_trail));
	
	/* take care of negative args, NaNs, and Infinities     */

	if (j >= (MAXEXP - MINEXP)) {
		result = Qnan.d;
	}

	if (x == 0.0){
		result = Neginf.d;
	}

	if (x == Inf.d) {
		result = Inf.d;
	}

	return result;
}
