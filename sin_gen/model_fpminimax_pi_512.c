// 自动化的话，从BIT、BITNUM入手，控制表项的多少，以及生成对应级数的多项式系数
#include <stdio.h>
#include "myhead.h"
#define DEGREE 6

// coefficient[0] is for sin; coefficient[1] is for cos;
// sin:
// 0xb984d64a29b505c5 + x * (0x3ff0000000000000 + x * (0x3cb845aef92c1d42 + x * (0xbfc55555555599c3 + x * (0x3decb7924858362b + x * 0x3f81110d09aab89d))))

static const DL
coefficient[2][DEGREE] = {
	{
		{.l = 0xb984d64a29b505c5},
		{.l = 0x3ff0000000000000},
		{.l = 0x3cb845aef92c1d42},
		{.l = 0xbfc55555555599c3},
		{.l = 0x3decb7924858362b},
		{.l = 0x3f81110d09aab89d}
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

double sin_gen(double x) {
	double y, xx, appro_s, appro_c;

	//xx = x * x;
	appro_s = coefficient[0][0].d + x * (coefficient[0][1].d + x * (coefficient[0][2].d + x * (coefficient[0][3].d + x * (coefficient[0][4].d + x * coefficient[0][5].d))));

	y = appro_s;

	return y;
}
