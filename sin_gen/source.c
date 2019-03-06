#include <stdio.h>

#define PI 3.1415926
#define NUM 100

extern double sin_gen(double);

int main(int argc, char *argv[]) {
	double a, b, midpoint, length;
	int p;
	double a1, b1, a2, b2;
	double X;
	long int Xtemp, atemp, btemp, i;
	double table[NUM] = {0};
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
 	Xtemp = (long int)(midpoint / (2 * PI));
	X = ((double)(Xtemp)) * (2 * PI);
	a1 = (0 - length) + X - midpoint;
	b1 = (0 + length) + X - midpoint;
	atemp = (long int)(a1 / (2 * PI) - 1);
	btemp = (long int)(b1 / (2 * PI) + 1);
	a2 = ((double)atemp) * (2 * PI);
	b2 = ((double)btemp) * (2 * PI);
	// table[i] restores the correct rounding x values where the step is PI/2, from 0 to b2
	for (i = 0; i < btemp * 4 + 1; i++) {
		table[i] = PI / 2 * i;
	}

	// generate code for sin_gen
	{
		// comments
		fprintf(func, "/** target func:\tsin\n");
		fprintf(func, "*** target domain:\t[%lf, %lf]\n", a, b);
		fprintf(func, "*** target precision:\t%d\n", p);
		fprintf(func, "**/\n");

		// init
		fprintf(func, "#define NUM %d\n", NUM);
		fprintf(func, "#define TableNum %d\n", (btemp * 4 + 1));
		fprintf(func, "static const double\n");
		fprintf(func, "X = %.17lf,\n", X);
		fprintf(func, "table[TableNum] = {\t// pi/2 * T, T = 0, 1, 2, ..., %d\n", (btemp * 4));
		for (i = 0; i < (btemp * 4); i++)
		{
			fprintf(func, "\t%.17lf,\n", table[i]);
		}
		fprintf(func, "\t%.17lf\n", table[i]);
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