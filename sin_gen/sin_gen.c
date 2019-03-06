/** target func:	sin
*** target domain:	[-13.000000, 13.000000]
*** target precision:	12
**/
#define NUM 100
#define TableNum 13
static const doublep
X = 0.00000000000000000,
table[TableNum] = {	// pi/2 * T, T = 0, 1, 2, ..., 12
	0.00000000000000000,
	1.57079630000000003,
	3.14159260000000007,
	4.71238890000000055,
	6.28318520000000014,
	7.85398149999999973,
	9.42477780000000109,
	10.99557410000000068,
	12.56637040000000027,
	14.13716669999999986,
	15.70796299999999945,
	17.27875930000000082,
	18.84955560000000219
};
extern double k_sin(double);
extern double k_cos(double);

double sin_gen(double x) {
	double ix, y;
	int i, flag, pio2_times;

	pio2_times = 0;
	flag = 1;
	ix = x - X;
	if (ix < 0) {
		ix = -ix;
		flag = -1;
	}

	for (i = 0; ix > table[i]; i++) {
		pio2_times++;
	}
	switch (pio2_times&3) {
		case 1:
			y = flag * k_sin(ix);
			break;
		case 2: 
			y = flag * k_cos(ix);
			break;
		case 3: 
			y = -1 * flag * k_sin(ix);
			break;
		case 0:
			y = -1 * flag * k_cos(ix);
			break;
		default:
			break;
	}
	return y;
}
