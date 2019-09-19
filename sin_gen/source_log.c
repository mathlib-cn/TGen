#include <stdio.h>
#include <stdlib.h>
#include "myhead.h"
#include "libm.h"

#define BIT 7
#define BITNUM 128
#define BITNUM_1 127
#define DEGREE 7

struct constraint {
	double start;
	double end;
	int precision;
	int bit;
	int fnum;
	int degree;
};

int gen(struct constraint input_parameter) {
	double a, b;
	int precision;
	double X;
	FILE *func;
	// init
	int i, num, bit, bitnum, bitnum_1, degree, fnum, format;
	num = 100;

	if ((func = fopen("log_gen.c", "w")) == (FILE *)0) {
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
	degree = 7;
	format = 64;
	/*printf("please input [a, b]: ");
	scanf("%lf %lf", &a, &b);
	printf("please input target precision: ");
	scanf("%d", &precision);
	printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);*/

	// generate code for exp_gen
	{
		fprintf(func, "#include %clibm.h%c\n", '"', '"');
		fprintf(func, "\n");
		fprintf(func, "extern const du _logtabhi[];\n");
		fprintf(func, "extern const du _logtablo[];\n");
		fprintf(func, "extern const du _log_ru[];\n");
		fprintf(func, "\n");
		fprintf(func, "static const long long twop7 =\n");
		fprintf(func, "{ 0x4060000000000000ll }; // 128\n");
		fprintf(func, "\n");
		fprintf(func, "static const du twopm7 =\n");
		fprintf(func, "{ D(0x3f800000, 0x00000000) }; // 1/128\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	log2_lead =\n");
		fprintf(func, "{ D(0x3fe62e42, 0xfefa4000) };\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	log2_trail =\n");
		fprintf(func, "{ D(0xbd48432a, 0x1b0e2634) };\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	Scaleup =\n");
		fprintf(func, "{ D(0x43300000, 0x00000000) };\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	Qnan =\n");
		fprintf(func, "{ D(QNANHI, QNANLO) };\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	Inf =\n");
		fprintf(func, "{ D(0x7ff00000, 0x00000000) };\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	Neginf =\n");
		fprintf(func, "{ D(0xfff00000, 0x00000000) };\n");
		fprintf(func, "\n");
		fprintf(func, "/* coefficients for polynomial approximation of log(1 + t) on +/- 1/256   */\n");
		fprintf(func, "\n");
		fprintf(func, "// x * (0x3ff0000000000000 + x * (0xbfdffffffffffffc + x * (0x3fd5555555552dde + x * (0xbfcffffffefe562c + x * (0x3fc9999817d3a30f + x * (0xbfc554317b3e1c57 + x * 0x3fc1dc5c459adbd8))))))\n");
		fprintf(func, "static const du	P[] =\n");
		fprintf(func, "{\n");
		fprintf(func, "{D(0x3ff00000, 0x00000000)},\n");
		fprintf(func, "{D(0xbfdfffff, 0xfffffffc)},\n");
		fprintf(func, "{D(0x3fd55555, 0x55552dde)},\n");
		fprintf(func, "{D(0xbfcfffff, 0xfefe562c)},\n");
		fprintf(func, "{D(0x3fc99998, 0x17d3a30f)},\n");
		fprintf(func, "{D(0xbfc55431, 0x7b3e1c57)},\n");
		fprintf(func, "{D(0x3fc1dc5c, 0x459adbd8)}\n");
		fprintf(func, "};\n");
		fprintf(func, "\n");
		fprintf(func, "#define MAXEXP	0x7ffu\n");
		fprintf(func, "\n");
		fprintf(func, "#define	MINEXP	0x001u\n");
		fprintf(func, "\n");	
	}
	{
		fprintf(func, "double log_gen(double x) {\n");
		fprintf(func, "	unsigned long long ix;\n");
		fprintf(func, "	int	j;\n");
		fprintf(func, "	int	m;\n");
		fprintf(func, "	int	k;\n");
		fprintf(func, "	double	u;\n");
		fprintf(func, "	double	t;\n");
		fprintf(func, "	double	xmu;\n");
		fprintf(func, "	double	q;\n");
		fprintf(func, "	double	l_lead, l_trail;\n");
		fprintf(func, "	double	w;\n");
		fprintf(func, "	double	result;\n");
		fprintf(func, "\n");
		fprintf(func, "	/* extract exponent and sign of x for some quick screening */\n");
		fprintf(func, "\n");
		fprintf(func, "	ix = *(unsigned long long *)(&x);	/* copy arg to a long long */\n");
		fprintf(func, "\n");
		fprintf(func, "	m = (ix >> DMANTWIDTH);		/* shift off mantissa	*/\n");
		fprintf(func, "	j = m - MINEXP;\n");
		fprintf(func, "\n");
		fprintf(func, "	m -= DEXPBIAS;\n");
		fprintf(func, "\n");
		fprintf(func, "	/* normalize x and compute the nearest 1/128th to x */\n");
		fprintf(func, "\n");
		fprintf(func, "	ix &= (DSIGNMASK & DEXPMASK);	/* mask off sign and exponent\n");
		fprintf(func, "						* bits of x\n");
		fprintf(func, "						*/\n");
		fprintf(func, "	ix |= twop7;	/* set exponent of x to 0x406 */\n");
		fprintf(func, "\n");
		fprintf(func, "	/* adjust scaled arg	*/\n");
		fprintf(func, "\n");
		fprintf(func, "	//LL2DBL(ix, w);\n");
		fprintf(func, "	w = *(double *)(&ix);\n");
		fprintf(func, "\n");
		fprintf(func, "	//k = ROUND(w);\n");
		fprintf(func, "	k = w + 0.5; // !!! +0.5 !!!\n");
		fprintf(func, "\n");
		fprintf(func, "	u = k;\n");
		fprintf(func, "\n");
		fprintf(func, "	k -= 128;\n");
		fprintf(func, "\n");
		fprintf(func, "	xmu = twopm7.d*(w - u);\n");
		fprintf(func, "\n");
		fprintf(func, "	t = _log_ru[k].d*xmu;\n");
		fprintf(func, "\n");
		fprintf(func, "	/* avoid loss of significance for values of x near two\n");
		fprintf(func, "		by adjusting index; effectively u is divided by two.\n");
		fprintf(func, "		The logtable has been adjusted for this.\n");
		fprintf(func, "	*/\n");
		fprintf(func, "\n");
		fprintf(func, "	if (k > 64) {\n");
		fprintf(func, "		m++;\n");
		fprintf(func, "	}\n");
		fprintf(func, "\n");
		fprintf(func, "	q = (((((P[6].d*t + P[5].d)*t + P[4].d)*t + P[3].d)*t + P[2].d)*t + P[1].d)*(t*t);\n");
		fprintf(func, "	\n");
		fprintf(func, "	l_lead = _logtabhi[k].d;\n");
		fprintf(func, "	l_trail = _logtablo[k].d;\n");
		fprintf(func, "\n");
		fprintf(func, "	l_lead += m * log2_lead.d;\n");
		fprintf(func, "	l_trail += m * log2_trail.d;\n");
		fprintf(func, "\n");
		fprintf(func, "	result = l_lead + (t + (q + l_trail));\n");
		fprintf(func, "	\n");
		fprintf(func, "	/* take care of negative args, NaNs, and Infinities     */\n");
		fprintf(func, "\n");
		fprintf(func, "	if (j >= (MAXEXP - MINEXP)) {\n");
		fprintf(func, "		result = Qnan.d;\n");
		fprintf(func, "	}\n");
		fprintf(func, "\n");
		fprintf(func, "	if (x == 0.0){\n");
		fprintf(func, "		result = Neginf.d;\n");
		fprintf(func, "	}\n");
		fprintf(func, "\n");
		fprintf(func, "	if (x == Inf.d) {\n");
		fprintf(func, "		result = Inf.d;\n");
		fprintf(func, "	}\n");
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