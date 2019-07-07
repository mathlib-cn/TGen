#include <stdio.h>
#include <stdlib.h>
#include "myhead.h"

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
	//bit = 7;
	bitnum = 1 << bit;
	bitnum_1 = bitnum - 1;
	fnum = input_parameter.fnum;
	degree = input_parameter.degree;
	//degree = 6;
	format = 64;
	/*printf("please input [a, b]: ");
	scanf("%lf %lf", &a, &b);
	printf("please input target precision: ");
	scanf("%d", &precision);
	printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);*/

	// generate code for exp_gen
	{
		// comments
		fprintf(func, "/** target func:\texp\n");
		fprintf(func, "*** target domain:\t[%lf, %lf]\n", a, b);
		fprintf(func, "*** target precision:\t%d\n", precision);
		fprintf(func, "**/\n");

		fprintf(func, "/** target func:	exp\n");
		fprintf(func, "*** target domain:	[-3.000000, 3.000000]\n");
		fprintf(func, "*** target precision:	90\n");
		fprintf(func, "**/\n");
		fprintf(func, "#include <stdio.h>\n");
		fprintf(func, "#include <math.h>\n");
		fprintf(func, "#include %cmyhead.h%c\n", '"', '"');
		fprintf(func, "\n");
		fprintf(func, "#define BIT 6\n");
		fprintf(func, "#define BITNUM 64\n");
		fprintf(func, "#define DEGREE 6\n");
		fprintf(func, "\n");
		fprintf(func, "static const DL\n");
		fprintf(func, "coefficient[DEGREE] = {\n");
		fprintf(func, "	{.d = 1},\n");
		fprintf(func, "	{.d = 1},\n");
		fprintf(func, "	{.d = 0.5},\n");
		fprintf(func, "	{.d = 1.0 / 6.0},\n");
		fprintf(func, "	{.d = 1.0 / 24.0},\n");
		fprintf(func, "	{.d = 1.0 / 120.0},\n");
		fprintf(func, "};\n");
		fprintf(func, "\n");
		fprintf(func, "double exp_gen(double x) {\n");
		fprintf(func, "	double temp;\n");
		fprintf(func, "	int k;\n");
		fprintf(func, "	double k1;\n");
		fprintf(func, "	double T;\n");
		fprintf(func, "	long int T_int;\n");
		fprintf(func, "	double r;\n");
		fprintf(func, "	double r_poly;\n");
		fprintf(func, "	int hi, lo;\n");
		fprintf(func, "	double r_coefficient;\n");
		fprintf(func, "	double result;\n");
		fprintf(func, "\n");
		fprintf(func, "	k = BITNUM;\n");
		fprintf(func, "	k1 = k / log(2);\n");
		fprintf(func, "	T = x * k1;\n");
		fprintf(func, "	T_int = T;\n");
		fprintf(func, "	// 0 <= r <= 1/k1\n");
		fprintf(func, "	r = x - ((double)T_int)/k1;\n");
		fprintf(func, "\tappro_s = coefficient[0][0].d");
		for (i = 1; i <= degree; i++) {
			fprintf(func, " + iiix * (coefficient[%d].d", i);
		}
		for (i = 1; i <= degree; i++) {
			fprintf(func, ")");
		}
		fprintf(func, ";\n");
		
		fprintf(func, "	lo = T_int % k;\n");
		fprintf(func, "	hi = T_int / k;\n");
		fprintf(func, "	r_coefficient = pow(2, (double)hi) * pow(2, ((double)lo / k));\n");
		fprintf(func, "	result = r_coefficient * r_poly;\n");
		fprintf(func, "\n");
		fprintf(func, "	return result;\n");
		fprintf(func, "}\n");
		fprintf(func, "\n");

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