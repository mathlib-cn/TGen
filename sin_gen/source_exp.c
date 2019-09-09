#include <stdio.h>
#include <stdlib.h>
#include "myhead.h"

#define BIT 6
#define BITNUM 64

struct constraint {
	double start;
	double end;
	int precision;
	int bit;
	int fnum;
	int degree;
};

static double
ln2HI[2] = {6.93147180369123816490e-01,  /* 0x3fe62e42, 0xfee00000 */
			-6.93147180369123816490e-01, },/* 0xbfe62e42, 0xfee00000 */
ln2LO[2] = {1.90821492927058770002e-10,  /* 0x3dea39ef, 0x35793c76 */
			-1.90821492927058770002e-10, },/* 0xbdea39ef, 0x35793c76 */
invln2 = 1.44269504088896338700e+00; /* 0x3ff71547, 0x652b82fe */

static const DL
interpolate[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff02c9a3e778061},
	{.l = 0x3ff059b0d3158574},
	{.l = 0x3ff0874518759bc8},
	{.l = 0x3ff0b5586cf9890f},
	{.l = 0x3ff0e3ec32d3d1a2},
	{.l = 0x3ff11301d0125b51},
	{.l = 0x3ff1429aaea92de0},
	{.l = 0x3ff172b83c7d517b},
	{.l = 0x3ff1a35beb6fcb75},
	{.l = 0x3ff1d4873168b9aa},
	{.l = 0x3ff2063b88628cd6},
	{.l = 0x3ff2387a6e756238},
	{.l = 0x3ff26b4565e27cdd},
	{.l = 0x3ff29e9df51fdee1},
	{.l = 0x3ff2d285a6e4030b},
	{.l = 0x3ff306fe0a31b715},
	{.l = 0x3ff33c08b26416ff},
	{.l = 0x3ff371a7373aa9cb},
	{.l = 0x3ff3a7db34e59ff7},
	{.l = 0x3ff3dea64c123422},
	{.l = 0x3ff4160a21f72e2a},
	{.l = 0x3ff44e086061892d},
	{.l = 0x3ff486a2b5c13cd0},
	{.l = 0x3ff4bfdad5362a27},
	{.l = 0x3ff4f9b2769d2ca7},
	{.l = 0x3ff5342b569d4f82},
	{.l = 0x3ff56f4736b527da},
	{.l = 0x3ff5ab07dd485429},
	{.l = 0x3ff5e76f15ad2148},
	{.l = 0x3ff6247eb03a5585},
	{.l = 0x3ff6623882552225},
	{.l = 0x3ff6a09e667f3bcd},
	{.l = 0x3ff6dfb23c651a2f},
	{.l = 0x3ff71f75e8ec5f74},
	{.l = 0x3ff75feb564267c9},
	{.l = 0x3ff7a11473eb0187},
	{.l = 0x3ff7e2f336cf4e62},
	{.l = 0x3ff82589994cce13},
	{.l = 0x3ff868d99b4492ed},
	{.l = 0x3ff8ace5422aa0db},
	{.l = 0x3ff8f1ae99157736},
	{.l = 0x3ff93737b0cdc5e5},
	{.l = 0x3ff97d829fde4e50},
	{.l = 0x3ff9c49182a3f090},
	{.l = 0x3ffa0c667b5de565},
	{.l = 0x3ffa5503b23e255d},
	{.l = 0x3ffa9e6b5579fdbf},
	{.l = 0x3ffae89f995ad3ad},
	{.l = 0x3ffb33a2b84f15fb},
	{.l = 0x3ffb7f76f2fb5e47},
	{.l = 0x3ffbcc1e904bc1d2},
	{.l = 0x3ffc199bdd85529c},
	{.l = 0x3ffc67f12e57d14b},
	{.l = 0x3ffcb720dcef9069},
	{.l = 0x3ffd072d4a07897c},
	{.l = 0x3ffd5818dcfba487},
	{.l = 0x3ffda9e603db3285},
	{.l = 0x3ffdfc97337b9b5f},
	{.l = 0x3ffe502ee78b3ff6},
	{.l = 0x3ffea4afa2a490da},
	{.l = 0x3ffefa1bee615a27},
	{.l = 0x3fff50765b6e4540},
	{.l = 0x3fffa7c1819e90d8}
};

int gen(struct constraint input_parameter) {
	double a, b;
	int precision;
	double X;
	FILE *func;
	// init
	int i, num, bit, bitnum, bitnum_1, degree, fnum, format;
	num = 100;

	if ((func = fopen("exp_gen.c", "w")) == (FILE *)0) {
		printf("open file error!\n");
		return 1;
	}
	a = input_parameter.start;
	b = input_parameter.end;
	precision = input_parameter.precision;
	bit = input_parameter.bit;
	bit = 6;
	bitnum = 1 << bit;
	bitnum_1 = bitnum - 1;
	fnum = input_parameter.fnum;
	degree = input_parameter.degree;
	degree = 6;
	format = 64;
	/*printf("please input [a, b]: ");
	scanf("%lf %lf", &a, &b);
	printf("please input target precision: ");
	scanf("%d", &precision);
	printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);*/

	// generate code for exp_gen
	{
		fprintf(func, "#include <stdio.h>\n");
		fprintf(func, "#include <math.h>\n");
		fprintf(func, "#include %cmyhead.h%c\n", '"', '"');
		fprintf(func, "\n");
		fprintf(func, "#define BIT %d\n", bit);
		fprintf(func, "#define BITNUM %d\n", bitnum);
		fprintf(func, "#define DEGREE %d\n", degree);
		fprintf(func, "\n");
		fprintf(func, "static const double\n");
		fprintf(func, "ln2HI = 6.93147180369123816490e-01,  /* 0x3fe62e42, 0xfee00000 */\n");
		fprintf(func, "ln2LO = 1.90821492927058770002e-10, /* 0x3dea39ef, 0x35793c76 */\n");
		fprintf(func, "invln2 = 1.44269504088896338700e+00; /* 0x3ff71547, 0x652b82fe */\n");
		fprintf(func, "\n");

		fprintf(func, "static const DL\n");
		fprintf(func, "coefficient[DEGREE] = {\n");
		fprintf(func, "	{.l = 0x3ff0000000000000}, // 1\n");
		fprintf(func, "	{.l = 0x3ff0000000000000}, // 1\n");
		fprintf(func, "	{.l = 0x3fe0000000000000}, // 1/2\n");
		fprintf(func, "	{.l = 0x3fc5555555555555}, // 1/6\n");
		fprintf(func, "	{.l = 0x3fa5555555555555}, // 1/24\n");
		fprintf(func, "	{.l = 0x3f81111111111111}  // 1/120\n");
		fprintf(func, "};\n");

		// interpolate
		fprintf(func, "static const DL\n");
		fprintf(func, "interpolate[BITNUM] = {\n");
		for (i = 0; i < bitnum - 1; i++) {
			fprintf(func, "\t{.l = 0x%lx},\n", interpolate[i * (1 << (BIT - bit))].l);
		}
		fprintf(func, "\t{.l = 0x%lx}\n", interpolate[i * (1 << (BIT - bit))].l);
		fprintf(func, "};\n\n");
	}
	{
		fprintf(func, "double exp_gen(double x) {\n");
		fprintf(func, "	double temp;\n");
		fprintf(func, "	int k;\n");
		fprintf(func, "	double k1;\n");
		fprintf(func, "	double T;\n");
		fprintf(func, "	long int T_int;\n");
		fprintf(func, "	double r;\n");
		fprintf(func, "	double r_poly;\n");
		fprintf(func, "	unsigned long int hi, lo;\n");
		fprintf(func, "	double r_coefficient;\n");
		fprintf(func, "	double result;\n");
		fprintf(func, "	double r_hi, r_lo;\n");
		fprintf(func, "\n");
		fprintf(func, "	k = BITNUM;\n");
		fprintf(func, "	k1 = k * invln2;\n");
		fprintf(func, "	T = x * k1;\n");
		fprintf(func, "	T_int = T;\n");
		fprintf(func, "	// 0 <= r <= 1/k1\n");
		fprintf(func, "	r = x - T_int * ln2HI / k;\n");
		fprintf(func, "	r = r - T_int * ln2LO / k;\n\n");
		fprintf(func, "\tr_poly = coefficient[0].d");
		for (i = 1; i < degree; i++) {
			fprintf(func, " + r * (coefficient[%d].d", i);
		}
		for (i = 1; i < degree; i++) {
			fprintf(func, ")");
		}
		fprintf(func, ";\n\n");
		fprintf(func, "	lo = T_int % k;\n");
		fprintf(func, "	hi = T_int / k;\n");
		fprintf(func, "	hi = (hi + 0x3ff) << 52;\n");
		fprintf(func, "	r_hi = *((double *)&hi);\n");
		fprintf(func, "	//r_lo = pow(2, (((double)lo) / ((double)k)));\n");
		fprintf(func, "	r_lo = interpolate[lo].d;\n");
		fprintf(func, "	r_coefficient = r_hi * r_lo;\n");
		fprintf(func, "	result = r_coefficient * r_poly;\n");
		fprintf(func, "\n");
		fprintf(func, "	return result;\n");
		fprintf(func, "}\n");
	}
	fclose(func);

	return 0;
}

int main(int argc, char *argv[]) {
	double a, b;
	int precision;
	int bit, fnum, degree;
	struct constraint input_parameter;

	if (argc == 1) {
		printf("please input [a, b]: ");
		scanf("%lf %lf", &a, &b);
		printf("please input target precision: ");
		scanf("%d", &precision);
		printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);
		printf("please input interval paramenter -- bit: ");
		scanf("%d", &bit);
		printf("please input degree: ");
		scanf("%d", &degree);
		fnum = 1;

		// test
		// printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);
		// printf("bit = %d\n", bit);
		// printf("degree = %d\n", degree);
		// printf("fnum = %d\n", fnum);
	}
	else if (argc == 7) {
		a = atof(argv[1]);
		b = atof(argv[2]);
		precision = atoi(argv[3]);
		bit = atoi(argv[4]);
		fnum = atoi(argv[5]);
		degree = atoi(argv[6]);
	}
	else {
		printf("please input 6 parameters!!\n");
		return 1;
	}
	input_parameter.start = a;
	input_parameter.end = b;
	input_parameter.precision = precision;
	input_parameter.bit = bit;
	input_parameter.fnum = fnum;
	input_parameter.degree = degree;
	gen(input_parameter);

	return 0;
}