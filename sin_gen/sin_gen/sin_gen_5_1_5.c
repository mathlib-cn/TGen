/** target func:	sin
*** target domain:	[0.000000, 1.000000]
*** target precision:	23
**/

#include <stdio.h>
#include "myhead.h"

#define NUM 100
#define BIT 5
#define BITNUM 32
#define BITNUM_1 31
#define DEGREE 6
static const double
invpio4 = 1.2732395447351626861510701069801,
invpio512 = 162.97466172610082382733697369345,
pi_4 = 0.78539816339744830961566084581988,
pi_2 = 1.5707963267948966192313216916398,
pi_512 = 0.00613592315154256491887235035797;

static const double
pio2_1 = 1.57079632673412561417e+00, /* 0x3FF921FB, 0x54400000 */
pio2_1t = 6.07710050650619224932e-11; /* 0x3DD0B461, 0x1A626331 */

static const DL
pi_4_1 = { .l = 0x3FE921FB54400000 },
pi_4_1t = { .l = 0x3DC0B4611A626331 };
static const DL
coefficient[2][DEGREE] = {
	{
		{.l = 0x3b0fd4acbb1215cd},
		{.l = 0x3feffffffffffffe},
		{.l = 0x3d6064617071654d},
		{.l = 0xbfc5555555a38704},
		{.l = 0x3e4eca6b4cb8c93e},
		{.l = 0x3f8110ce41aee4f3}
	},
	{
		{.l = 0x3ff0000000000001},
		{.l = 0xbd616abbb1cc8ecc},
		{.l = 0xbfdfffffffc3682c},
		{.l = 0xbe63fa7ba182ef14},
		{.l = 0x3fa555b5d381c878},
		{.l = 0xbf1b02895e60699f}
	}
};
static const double
interpolate[2][BITNUM] = {
	{
		0.00000000000000000e+00,
		2.45412285229122881e-02,
		4.90676743274180149e-02,
		7.35645635996674263e-02,
		9.80171403295606036e-02,
		1.22410675199216196e-01,
		1.46730474455361748e-01,
		1.70961888760301217e-01,
		1.95090322016128276e-01,
		2.19101240156869798e-01,
		2.42980179903263899e-01,
		2.66712757474898365e-01,
		2.90284677254462387e-01,
		3.13681740398891462e-01,
		3.36889853392220051e-01,
		3.59895036534988166e-01,
		3.82683432365089782e-01,
		4.05241314004989861e-01,
		4.27555093430282085e-01,
		4.49611329654606595e-01,
		4.71396736825997642e-01,
		4.92898192229784038e-01,
		5.14102744193221772e-01,
		5.34997619887097264e-01,
		5.55570233019602178e-01,
		5.75808191417845339e-01,
		5.95699304492433357e-01,
		6.15231590580626819e-01,
		6.34393284163645488e-01,
		6.53172842953776756e-01,
		6.71558954847018441e-01,
		6.89540544737066941e-01
	},
	{
		1.00000000000000000e+00,
		9.99698818696204250e-01,
		9.98795456205172405e-01,
		9.97290456678690207e-01,
		9.95184726672196929e-01,
		9.92479534598709967e-01,
		9.89176509964781014e-01,
		9.85277642388941222e-01,
		9.80785280403230431e-01,
		9.75702130038528570e-01,
		9.70031253194543974e-01,
		9.63776065795439840e-01,
		9.56940335732208824e-01,
		9.49528180593036675e-01,
		9.41544065183020806e-01,
		9.32992798834738846e-01,
		9.23879532511286738e-01,
		9.14209755703530691e-01,
		9.03989293123443338e-01,
		8.93224301195515324e-01,
		8.81921264348355050e-01,
		8.70086991108711461e-01,
		8.57728610000272118e-01,
		8.44853565249707117e-01,
		8.31469612302545236e-01,
		8.17584813151583711e-01,
		8.03207531480644943e-01,
		7.88346427626606228e-01,
		7.73010453362736993e-01,
		7.57208846506484567e-01,
		7.40951125354959106e-01,
		7.24247082951466892e-01
	}
};

double sin_gen(double ix) {
	double iix, iiix, y, appro_s, appro_c;
	long int temp, table_order, status_pi_4, status_pi_2, status_pi_1, flag, sign, sin_or_cos;
	int i;

	temp = *((long int *)(&ix));
	flag = temp >> 63; // - is 1, + is 0
	temp = temp & 0x7fffffffffffffff;
	ix = *((double *)(&temp)); // at this time, ix is absolute value
	
	// if ix is too little, then return ix;
	if (temp < 0x3e40000000000000) {
		return (1 - flag * 2) * ix;
	}

	// new
	// iix = ix - [ix * invpio4] * pi_4
	// iiix = iix - i / 32 * pi_4
	temp = (long int)(ix * invpio4 * BITNUM); // why not ix * invpio_512;
	status_pi_4 = (temp >> BIT) & 0x1; 
	status_pi_2 = (temp >> (BIT + 1)) & 0x1;
	status_pi_1 = (temp >> (BIT + 2)) & 0x1;
	iix = ix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_1;
	iix = iix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_1t;
	//iix = iix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_2;
	//iix = iix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_2t;
	//iix = iix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_3;
	//iix = iix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_3t;
	table_order = temp & (BITNUM_1); // BITNUM - 1 = (2^BIT -1)
	table_order = table_order - status_pi_4 * (BITNUM_1);
	table_order = (1 - status_pi_4 * 2) * table_order;
	temp = *((long int *)(&iix));
	temp = temp & 0x7fffffffffffffff;
	iix = *((double *)(&temp)); // at this time, iix is absolute value
	iiix = iix - ((double)table_order) / BITNUM * pi_4_1.d;
	iiix = iiix - ((double)table_order) / BITNUM * pi_4_1t.d;
	//iiix = iiix - ((double)table_order) / BITNUM * pi_4_2.d;
	//iiix = iiix - ((double)table_order) / BITNUM * pi_4_2t.d;
	//iiix = iiix - ((double)table_order) / BITNUM * pi_4_3.d;
	//iiix = iiix - ((double)table_order) / BITNUM * pi_4_3t.d;
	sign = flag ^ status_pi_1; // ornot��0 is +, 1 is -
	sin_or_cos = status_pi_2 ^ status_pi_4; // ornot, 0 is sin, 1 is cos

	// new, sin(x+x') = sin(x)cos(x') + sin(x')cos(x), x' = iiix
	// OR, cos(x+x') = cos(x)cos(x') - sin(x)sin(x'), x' = iiix
	appro_s = 0.0 + iiix * (coefficient[0][1].d + iiix * (coefficient[0][2].d + iiix * (coefficient[0][3].d + iiix * (coefficient[0][4].d + iiix * (coefficient[0][5].d)))));
	appro_c = 0.0 + iiix * (coefficient[1][1].d + iiix * (coefficient[1][2].d + iiix * (coefficient[1][3].d + iiix * (coefficient[1][4].d + iiix * (coefficient[1][5].d)))));

	y = interpolate[sin_or_cos][table_order] * appro_c + (1 - sin_or_cos * 2) * interpolate[1 - sin_or_cos][table_order] * appro_s;
	y += (1 - sin_or_cos * 2) * interpolate[1 - sin_or_cos][table_order] * coefficient[0][0].d + interpolate[sin_or_cos][table_order] * coefficient[1][0].d;
	y = (1 - sign * 2) * y;

	return y;
}

