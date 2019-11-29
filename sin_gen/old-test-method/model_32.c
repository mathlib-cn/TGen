// 自动化的话，从BIT、BITNUM入手，控制表项的多少，以及生成对应级数的多项式系数
#include <stdio.h>

#define NUM 100
#define BIT 3
#define BITNUM 8
#define DEGREE 6

// 经验证，该精度为double下极限
static const double
invpio4 = 1.2732395447351626861510701069801,
pi_4 = 0.78539816339744830961566084581988,
pi_2 = 1.5707963267948966192313216916398,
X = 0;

// 区间小的话则直接提供，区间大的话则交由程序计算
//static const double
//pio2_table[NUM] = { //pi/2 * T, T = 0, 1, 2, ..., 20
//	0,
//	1.5707963267948966192313216916398,
//	3.1415926535897932384626433832795,
//	4.7123889803846898576939650749193,
//	6.283185307179586476925286766559,
//	7.8539816339744830961566084581988,
//	9.4247779607693797153879301498385,
//	10.995574287564276334619251841478,
//	12.566370614359172953850573533118,
//	14.137166941154069573081895224758,
//	15.707963267948966192313216916398,
//	17.278759594743862811544538608037,
//	18.849555921538759430775860299677,
//	20.420352248333656050007181991317,
//	21.991148575128552669238503682957,
//	23.561944901923449288469825374596,
//	25.132741228718345907701147066236,
//	26.703537555513242526932468757876,
//	28.274333882308139146163790449516,
//	29.845130209103035765395112141155,
//	31.415926535897932384626433832795
//};

// coefficient[0] is for sin; coefficient[1] is for cos;
static const double
coefficient[2][DEGREE] = {
	{0, 1, 0, -1.6666666666666666666666666666667e-1, 0, -0.05},
	{1, 0, -0.5, 0, -8.333333333333333333333333333333e-2, 0}
};

// interpolate[0] is for sin(x) where x is in [0, pi/4] as interpolate[1] is for cos(x)
// sin(x) OR cos(x), x = pi_4 / 64 * i, i is 0, 1, 2, ..., 127
static const double
interpolate[2][BITNUM] = {
	{
		0.00000000000000000e+00,
		9.80171403295606020e-02,
		1.95090322016128268e-01,
		2.90284677254462368e-01,
		3.82683432365089772e-01,
		4.71396736825997649e-01,
		5.55570233019602225e-01,
		6.34393284163645498e-01
	},
	{
		1.00000000000000000e+00,
		9.95184726672196886e-01,
		9.80785280403230449e-01,
		9.56940335732208865e-01,
		9.23879532511286756e-01,
		8.81921264348355030e-01,
		8.31469612302545237e-01,
		7.73010453362736961e-01
	}
};

double sin_gen(double x) {
	double ix, iix, iiix, y, z, appro_s, appro_c;
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
	z = iiix * iiix;
	appro_s = coefficient[0][1] * iiix * (1 + coefficient[0][3] * z * (1 + coefficient[0][5] * z));
	appro_c = coefficient[1][0] + coefficient[1][2] * z * (1 + coefficient[1][4] * z);

	y = interpolate[sin_or_cos][table_order] * appro_c + (1 - sin_or_cos * 2) * interpolate[1 - sin_or_cos][table_order] * appro_s;
	y = (1 - sign * 2) * y;
	
	return y;
}
