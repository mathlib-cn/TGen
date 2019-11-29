// k_cos
static const double
one = 1.00000000000000000000e+00, /* 0x3FF00000, 0x00000000 */
C1 = 4.16666666666666019037e-02, /* 0x3FA55555, 0x5555554C */
C2 = -1.38888888888741095749e-03, /* 0xBF56C16C, 0x16C15177 */
C3 = 2.48015872894767294178e-05, /* 0x3EFA01A0, 0x19CB1590 */
C4 = -2.75573143513906633035e-07, /* 0xBE927E4F, 0x809C52AD */
C5 = 2.08757232129817482790e-09, /* 0x3E21EE9E, 0xBDB4B1C4 */
C6 = -1.13596475577881948265e-11; /* 0xBDA8FAE9, 0xBE8838D4 */

double k_cos(double x) {
	double a, hz, z, r, qx;
	int ix;
	long int temp;
	temp = *((long int *)(&x));
	ix = (int)(temp >> 32);
	ix &= 0x7fffffff;			/* ix = |x|'s high word*/
	if (ix<0x3e400000) {			/* if x < 2**-27 */
		if (((int)x) == 0) return 1;		/* generate inexact */
	}
	z = x * x;
	r = z * (C1 + z * (C2 + z * (C3 + z * (C4 + z * (C5 + z * C6)))));

	return 1 - (0.5*z - z * r);
}
