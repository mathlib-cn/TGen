// 自动化的话，从BIT、BITNUM入手，控制表项的多少，以及生成对应级数的多项式系数
#include <stdio.h>
#include "myhead.h"

#define NUM 100
#define BIT 2
#define BITNUM 4
#define DEGREE 6

// 经验证，该精度为double下极限
static const double
invpio4 = 1.2732395447351626861510701069801,
pi_4 = 0.78539816339744830961566084581988,
pi_2 = 1.5707963267948966192313216916398,
X = 0;

// coefficient[0] is for sin; coefficient[1] is for cos;
// pi/4/4
// sin:
// 0x3c327cafa27860fe + x * (0x3feffffffff6bd37 + x * (0x3e51111eba33a411 + x * (0xbfc5555a5102bb09 + x * (0x3edf1bd2dcdde00c + x * 0x3f81004755cee660))))
static const DL
coefficient[2][DEGREE] = {
	{
		{.l = 0x3c327cafa27860fe},
		{.l = 0x3feffffffff6bd37},
		{.l = 0x3e51111eba33a411},
		{.l = 0xbfc5555a5102bb09},
		{.l = 0x3edf1bd2dcdde00c},
		{.l = 0x3f81004755cee660}
	},
	{
		{.l = 0},
		{.l = 0},
		{.l = 0},
		{.l = 0},
		{.l = 0},
		{.l = 0}
	}
};

// interpolate[0] is for sin(x) where x is in [0, pi/4] as interpolate[1] is for cos(x)
// sin(x) OR cos(x), x = pi_4 / 4 * i, i is 0, 1, 2, 3
static const double
interpolate[2][BITNUM] = {
	{
		0.00000000000000000e+00,
		1.95090322016128268e-01,
		3.82683432365089772e-01,
		5.55570233019602225e-01,
	},
	{
		1.00000000000000000e+00,
		9.80785280403230449e-01,
		9.23879532511286756e-01,
		8.31469612302545237e-01,
	}
};

double sin_gen(double x) {
	double ix, iix, iiix, y, appro_s, appro_c;
	long int temp, table_order, status_pi_4, status_pi_2, status_pi_1, flag, sign, sin_or_cos;
	int i;

	ix = x - X;
	temp = *((long int *)(&ix));
	flag = temp >> 63; // 负为1，正为0
	flag = flag & 0x0000000000000001;
	temp = temp & 0x7fffffffffffffff;
	ix = *((double *)(&temp)); // 此时 ix 为绝对值

	// ix过小，则return ix;
	if (temp < 0x3e40000000000000) {
		return (1 - flag * 2) * ix;
	}

	// new
	// iix = ix - [ix * invpio4] * pi_4
	// iiix = iix - i / 128* pi_4
	temp = (long int)(ix * invpio4 * BITNUM); // why not ix * invpio_512; ??
	status_pi_4 = (temp >> BIT) & 0x1;
	status_pi_2 = (temp >> (BIT + 1)) & 0x1;
	status_pi_1 = (temp >> (BIT + 2)) & 0x1;
	iix = ix - (temp >> (BIT + 1)) * pi_2 - status_pi_4 * pi_2;
	table_order = temp & (BITNUM - 1); // 对应 (2^BIT -1) ，即BIT位的1
	table_order = table_order - status_pi_4 * (BITNUM - 1);
	table_order = (1 - status_pi_4 * 2) * table_order;
	temp = *((long int *)(&iix));
	temp = temp & 0x7fffffffffffffff;
	iix = *((double *)(&temp)); // 此时 iix 为绝对值
	iiix = iix - ((double)table_order) / BITNUM * pi_4;
	sign = flag ^ status_pi_1; // 异或，0为正，1为负
	sin_or_cos = status_pi_2 ^ status_pi_4; // 异或，0为sin，1为cos

	// new, sin(x+x') = sin(x)cos(x') + sin(x')cos(x), x' = iiix
	// OR, cos(x+x') = cos(x)cos(x') - sin(x)sin(x'), x' = iiix
	appro_s = coefficient[0][0].d + iiix * (coefficient[0][1].d + iiix * (coefficient[0][2].d + iiix * (coefficient[0][3].d + iiix * (coefficient[0][4].d + iiix * coefficient[0][5].d))));
	appro_c = coefficient[1][0].d + iiix * (coefficient[1][1].d + iiix * (coefficient[1][2].d + iiix * (coefficient[1][3].d + iiix * (coefficient[1][4].d + iiix * coefficient[1][5].d))));

	y = interpolate[sin_or_cos][table_order] * appro_c + (1 - sin_or_cos * 2) * interpolate[1 - sin_or_cos][table_order] * appro_s;
	y = (1 - sign * 2) * y;

	return y;
}

//double sin_gen(double x) {
//	double y, xx, appro_s, appro_c;
//
//	//xx = x * x;
//	appro_s = coefficient[0][0].d + x * (coefficient[0][1].d + x * (coefficient[0][2].d + x * (coefficient[0][3].d + x * (coefficient[0][4].d + x * coefficient[0][5].d))));
//
//	y = appro_s;
//
//	return y;
//}
