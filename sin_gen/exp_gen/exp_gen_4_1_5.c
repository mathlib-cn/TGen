#include "myhead.h"

#define BIT 4
#define BITNUM 16
#define BITNUM_1 15
#define DEGREE 6

static const DL
ln2by16HI = { .l = 0x3fa62e42fee00000 },
ln2by16LO = { .l = 0x3daa39ef35793c76 },
invln2by16 = { .l = 0x40371547652b82fe };

// fpminmax: [-1/16*ln2, 1/16*ln2]
static const DL
C[DEGREE] = {
	{.l = 0x3fefffffffffffd8},
	{.l = 0x3ff0000000008376},
	{.l = 0x3fdffffffdd412d3},
	{.l = 0x3fc55557023e90ac},
	{.l = 0x3fa5542ab580ecf6},
	{.l = 0x3f8170a5ec861e7b},
};
static const DL
interpolate[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff0b5586cf9890f},
	{.l = 0x3ff172b83c7d517b},
	{.l = 0x3ff2387a6e756238},
	{.l = 0x3ff306fe0a31b715},
	{.l = 0x3ff3dea64c123422},
	{.l = 0x3ff4bfdad5362a27},
	{.l = 0x3ff5ab07dd485429},
	{.l = 0x3ff6a09e667f3bcd},
	{.l = 0x3ff7a11473eb0187},
	{.l = 0x3ff8ace5422aa0db},
	{.l = 0x3ff9c49182a3f090},
	{.l = 0x3ffae89f995ad3ad},
	{.l = 0x3ffc199bdd85529c},
	{.l = 0x3ffd5818dcfba487},
	{.l = 0x3ffea4afa2a490da}
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

	T = x * invln2by16.d;
	T_int = T;
	// 0 <= r <= 1/k1
	r1 = x - T_int * ln2by16HI.d;
	r2 = T_int * ln2by16LO.d;
	r = r1 - r2;

	r_poly = (r * r) * (C[2].d + r * (C[3].d + r * (C[4].d + r * (C[5].d)))) - r2 * C[1].d + r1 * C[1].d + C[0].d;

	lo = T_int & BITNUM_1;
	hi = T_int >> BIT;
	hi = (hi + 0x3ff) << 52;
	r_hi = *((double *)&hi);
	//r_lo = pow(2, (((double)lo) / ((double)k)));
	r_lo = interpolate[lo].d;
	result = r_hi * (r_lo * r_poly);

	return result;
}
