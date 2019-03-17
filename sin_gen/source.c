#include <stdio.h>

#define NUM 100

static const double
pi = 3.1415926535897932384626433832795,
pi_2 = 1.5707963267948966192313216916398,
two_pi = 6.283185307179586476925286766559,
invpio2 = 6.36619772367581382433e-01, 
// table[i] restores the correct rounding x values where the step is PI/2
table[NUM] = { //pi/2 * T, T = 0, 1, 2, ......
	0,
	1.5707963267948966192313216916398,
	3.1415926535897932384626433832795,
	4.7123889803846898576939650749193,
	6.283185307179586476925286766559,
	7.8539816339744830961566084581988,
	9.4247779607693797153879301498385,
	10.995574287564276334619251841478,
	12.566370614359172953850573533118,
	14.137166941154069573081895224758,
	15.707963267948966192313216916398,
	17.278759594743862811544538608037,
	18.849555921538759430775860299677,
	20.420352248333656050007181991317,
	21.991148575128552669238503682957,
	23.561944901923449288469825374596,
	25.132741228718345907701147066236,
	26.703537555513242526932468757876,
	28.274333882308139146163790449516,
	29.845130209103035765395112141155,
	31.415926535897932384626433832795
};
extern double sin_gen(double);

int main(int argc, char *argv[]) {
	double a, b, midpoint, length;
	int p;
	double a1, b1, a2, b2;
	double X;
	long int Xtemp, atemp, btemp, i;
	FILE *func;

	if ((func = fopen("sin_gen.c", "w")) == (FILE *)0) {
		printf("open file error!\n");
		return 0;
	}
	printf("please input [a, b]: ");
	scanf("%lf %lf", &a, &b);
	printf("please input target precision: ");
	scanf("%d", &p);
	printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, p);

	midpoint = (a + b) / 2;
	length = (b - a) / 2;
 	Xtemp = (long int)(midpoint / two_pi);
	X = ((double)(Xtemp)) * two_pi;
	a1 = (0 - length) + X - midpoint;
	b1 = (0 + length) + X - midpoint;
	atemp = (long int)(a1 / pi_2) - 1;
	btemp = (long int)(b1 / pi_2) + 1;
	a2 = ((double)atemp) * pi_2;
	b2 = ((double)btemp) * pi_2;

	// generate code for sin_gen
	{
		// comments
		fprintf(func, "/** target func:\tsin\n");
		fprintf(func, "*** target domain:\t[%lf, %lf]\n", a, b);
		fprintf(func, "*** target precision:\t%d\n", p);
		fprintf(func, "**/\n");

		// init
		fprintf(func, "#define NUM %d\n", NUM);
		fprintf(func, "#define TableNum %d\n", (btemp + 1));
		fprintf(func, "static const double\n");
		fprintf(func, "X = %.17lf,\n", X);
		fprintf(func, "table[TableNum] = {\t// pi_2 * T, T = 0, 1, 2, ..., %d\n", btemp);
		for (i = 0; i < btemp; i++)
		{
			fprintf(func, "\t%.17lf,\n", table[i]);
		}
		fprintf(func, "\t%.17lf\n ", table[i]);
		fprintf(func, "};\n");
		fprintf(func, "extern double k_sin(double);\n");
		fprintf(func, "extern double k_cos(double);\n");
		fprintf(func, "\n");

		// func
		fprintf(func, "double sin_gen(double x) {\n");

		fprintf(func, "\tdouble ix, y;\n");
		fprintf(func, "\tint i, flag, pio2_times;\n");
		fprintf(func, "\n");

		fprintf(func, "\tpio2_times = 0;\n");
		fprintf(func, "\tflag = 1;\n");
		fprintf(func, "\tix = x - X;\n");
		fprintf(func, "\tif (ix < 0) {\n");
		fprintf(func, "\t\tix = -ix;\n");
		fprintf(func, "\t\tflag = -1;\n");
		fprintf(func, "\t}\n");
		fprintf(func, "\n");

		fprintf(func, "\tfor (i = 0; ix > table[i]; i++) {\n");
		fprintf(func, "\t\tpio2_times++;\n");
		fprintf(func, "\t}\n");
		fprintf(func, "\tix = ix - table[i - 1];\n");
		fprintf(func, "\n");

		fprintf(func, "\tswitch (pio2_times&3) {\n");
		fprintf(func, "\t\tcase 1:\n");
		fprintf(func, "\t\t\ty = flag * k_sin(ix);\n");
		fprintf(func, "\t\t\tbreak;\n");
		fprintf(func, "\t\tcase 2: \n");
		fprintf(func, "\t\t\ty = flag * k_cos(ix);\n");
		fprintf(func, "\t\t\tbreak;\n");
		fprintf(func, "\t\tcase 3: \n");
		fprintf(func, "\t\t\ty = -1 * flag * k_sin(ix);\n");
		fprintf(func, "\t\t\tbreak;\n");
		fprintf(func, "\t\tcase 0:\n");
		fprintf(func, "\t\t\ty = -1 * flag * k_cos(ix);\n");
		fprintf(func, "\t\t\tbreak;\n");
		fprintf(func, "\t\tdefault:\n");
		fprintf(func, "\t\t\tbreak;\n");
		fprintf(func, "\t}\n");
		fprintf(func, "\treturn y;\n");

		fprintf(func, "}\n");
	}
	fclose(func);

	return 0;
}