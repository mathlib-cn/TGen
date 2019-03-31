// 自动化的话，从BIT、BITNUM入手，控制表项的多少，以及生成对应级数的多项式系数
#include <stdio.h>
#define DEGREE 6

// coefficient[0] is for sin; coefficient[1] is for cos;
static const double
coefficient[2][DEGREE] = {
	{0, 1, 0, -1.6666666666666666666666666666667e-1, 0, -0.05},
	{1, 0, -0.5, 0, -8.333333333333333333333333333333e-2, 0}
};

double sin_gen(double x) {
	double y, xx, appro_s, appro_c;

	xx = x * x;
	appro_s = coefficient[0][1] * x * (1 + coefficient[0][3] * xx * (1 + coefficient[0][5] * xx));
	appro_c = coefficient[1][0] + coefficient[1][2] * xx * (1 + coefficient[1][4] * xx);

	y = appro_s;

	return y;
}
