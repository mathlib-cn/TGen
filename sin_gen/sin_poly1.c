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



static const DL
coefficient[2][DEGREE] = {
	// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,1]); printexpansion(P);
	// x * (0x3feffffffff8d0b0 + x^2 * (0xbfc555554fe2164e + x^2 * (0x3f81110e64991b5b + x^2 * (0xbf29ffc6f9d065aa + x^2 * 0x3ec697b018937d5e))))
	{
		{.l = 0x0},
		{.l = 0x3feffffffff8d0b0},
		{.l = 0xbfc555554fe2164e},
		{.l = 0x3f81110e64991b5b},
		{.l = 0xbf29ffc6f9d065aa},
		{.l = 0x3ec697b018937d5e}
	},
	// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,1],1); printexpansion(P);
	// 0x3ff0000000000000 + x^2 * (0xbfdfffffffef005b + x^2 * (0x3fa555554e405168 + x^2 * (0xbf56c1683271dcc9 + x^2 * (0x3ef9ffbd1378c813 + x^2 * 0xbe921311166ed6bc))))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0xbfdfffffffef005b},
		{.l = 0x3fa555554e405168},
		{.l = 0xbf56c1683271dcc9},
		{.l = 0x3ef9ffbd1378c813},
		{.l = 0xbe921311166ed6bc}
	}
};

double sin_gen(double x) {
	double iiix, y, appro_s, appro_c, z;
	double start = 1;
	double interpolate_sin = 0.8414709848078965066525023216303;
	double interpolate_cos = 0.54030230586813971740093660744298;
	iiix = x - start;
	
	// if ix is too little, then return ix;
	//if (temp < 0x3e40000000000000) {
	//	return (1 - flag * 2) * ix;
	//}

	// new, sin(x+x') = sin(x)cos(x') + sin(x')cos(x), x' = iiix
	// OR, cos(x+x') = cos(x)cos(x') - sin(x)sin(x'), x' = iiix
	z = iiix * iiix;
	appro_s = 0.0 + iiix * (coefficient[0][1].d + z * (coefficient[0][2].d + z * (coefficient[0][3].d + z * (coefficient[0][4].d + z * (coefficient[0][5].d)))));
	appro_c = 1.0 + z * (coefficient[1][1].d + z * (coefficient[1][2].d + z * (coefficient[1][3].d + z * (coefficient[1][4].d + z * (coefficient[1][5].d)))));

	y = interpolate_sin * appro_c + interpolate_cos * appro_s;
	//y += interpolate[0][table_order] * coefficient[1][0].d;

	return y;
}

