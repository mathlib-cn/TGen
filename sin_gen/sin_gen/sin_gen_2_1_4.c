/** target func:	sin
*** target domain:	[0.000000, 1.000000]
*** target precision:	23
**/

#include <stdio.h>
#include "myhead.h"

#define NUM 100
#define BIT 2
#define BITNUM 4
#define BITNUM_1 3
#define DEGREE 5
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
		{.l = 0x3cd9f861ed669817},
		{.l = 0x3fefffffcbfa83fb},
		{.l = 0x3ef085772a93af38},
		{.l = 0xbfc5627986bf9c6b},
		{.l = 0x3f6ac6eb28e67b4e}
	},
	{
		{.l = 0x3fefffffffbf1399},
		{.l = 0x3e7df2b0677cc0c2},
		{.l = 0xbfe00008bd14c7a8},
		{.l = 0x3f0b41f8a98b058e},
		{.l = 0x3fa537ee739e6cb8}
	}
};
static const double
interpolate[2][BITNUM] = {
	{
		0.00000000000000000e+00,
		1.95090322016128276e-01,
		3.82683432365089782e-01,
		5.55570233019602178e-01
	},
	{
		1.00000000000000000e+00,
		9.80785280403230431e-01,
		9.23879532511286738e-01,
		8.31469612302545236e-01
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
	// iiix = iix - i / 4 * pi_4
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
	sign = flag ^ status_pi_1; // ornot£¬0 is +, 1 is -
	sin_or_cos = status_pi_2 ^ status_pi_4; // ornot, 0 is sin, 1 is cos

	// new, sin(x+x') = sin(x)cos(x') + sin(x')cos(x), x' = iiix
	// OR, cos(x+x') = cos(x)cos(x') - sin(x)sin(x'), x' = iiix
	appro_s = 0.0 + iiix * (coefficient[0][1].d + iiix * (coefficient[0][2].d + iiix * (coefficient[0][3].d + iiix * (coefficient[0][4].d))));
	appro_c = 0.0 + iiix * (coefficient[1][1].d + iiix * (coefficient[1][2].d + iiix * (coefficient[1][3].d + iiix * (coefficient[1][4].d))));

	y = interpolate[sin_or_cos][table_order] * appro_c + (1 - sin_or_cos * 2) * interpolate[1 - sin_or_cos][table_order] * appro_s;
	y += (1 - sin_or_cos * 2) * interpolate[1 - sin_or_cos][table_order] * coefficient[0][0].d + interpolate[sin_or_cos][table_order] * coefficient[1][0].d;
	y = (1 - sign * 2) * y;

	return y;
}

