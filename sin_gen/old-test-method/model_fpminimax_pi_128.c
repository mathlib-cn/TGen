// 自动化的话，从BIT、BITNUM入手，控制表项的多少，以及生成对应级数的多项式系数
#include <stdio.h>
#include "myhead.h"

#define NUM 100
#define BIT 5
#define BITNUM 32
#define DEGREE 6

// 经验证，该精度为double下极限
static const double
invpio4 = 1.2732395447351626861510701069801,
pi_4 = 0.78539816339744830961566084581988,
pi_2 = 1.5707963267948966192313216916398,
X = 0;

// coefficient[0] is for sin; coefficient[1] is for cos;
// pi/4/32
// sin:
// 0x3b0fd4acbb1215cd + x * (0x3feffffffffffffe + x * (0x3d6064617071654d + x * (0xbfc5555555a38704 + x * (0x3e4eca6b4cb8c93e + x * 0x3f8110ce41aee4f3))))
// cos:
// 0x3ff0000000000001 + x * (0xbd616abbb1cc8ecc + x * (0xbfdfffffffc3682c + x * (0xbe63fa7ba182ef14 + x * (0x3fa555b5d381c878 + x * 0xbf1b02895e60699f))))

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

// interpolate[0] is for sin(x) where x is in [0, pi/4] as interpolate[1] is for cos(x)
// sin(x) OR cos(x), x = pi_4 / 32 * i, i is 0, 1, 2, ..., 32
static const double
interpolate[2][BITNUM] = {
	{
		0.00000000000000000e+00,
		2.45412285229122880e-02,
		4.90676743274180143e-02,
		7.35645635996674235e-02,
		9.80171403295606020e-02,
		1.22410675199216198e-01,
		1.46730474455361752e-01,
		1.70961888760301226e-01,
		1.95090322016128268e-01,
		2.19101240156869797e-01,
		2.42980179903263890e-01,
		2.66712757474898386e-01,
		2.90284677254462368e-01,
		3.13681740398891477e-01,
		3.36889853392220051e-01,
		3.59895036534988149e-01,
		3.82683432365089772e-01,
		4.05241314004989871e-01,
		4.27555093430282094e-01,
		4.49611329654606600e-01,
		4.71396736825997649e-01,
		4.92898192229784037e-01,
		5.14102744193221727e-01,
		5.34997619887097211e-01,
		5.55570233019602225e-01,
		5.75808191417845301e-01,
		5.95699304492433343e-01,
		6.15231590580626845e-01,
		6.34393284163645498e-01,
		6.53172842953776764e-01,
		6.71558954847018401e-01,
		6.89540544737066925e-01
	},
	{
		1.00000000000000000e+00,
		9.99698818696204220e-01,
		9.98795456205172393e-01,
		9.97290456678690216e-01,
		9.95184726672196886e-01,
		9.92479534598709998e-01,
		9.89176509964780973e-01,
		9.85277642388941245e-01,
		9.80785280403230449e-01,
		9.75702130038528544e-01,
		9.70031253194543993e-01,
		9.63776065795439867e-01,
		9.56940335732208865e-01,
		9.49528180593036667e-01,
		9.41544065183020778e-01,
		9.32992798834738888e-01,
		9.23879532511286756e-01,
		9.14209755703530655e-01,
		9.03989293123443332e-01,
		8.93224301195515320e-01,
		8.81921264348355030e-01,
		8.70086991108711419e-01,
		8.57728610000272070e-01,
		8.44853565249707073e-01,
		8.31469612302545237e-01,
		8.17584813151583697e-01,
		8.03207531480644910e-01,
		7.88346427626606262e-01,
		7.73010453362736961e-01,
		7.57208846506484548e-01,
		7.40951125354959091e-01,
		7.24247082951466921e-01
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
