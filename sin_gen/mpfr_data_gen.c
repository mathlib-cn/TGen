#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>

#define PRECISION 128
#define INTERVAL 4

int main() {
	//printf ("MPFR library: %-12s\nMPFR header: %s (based on %d.%d.%d)\nMPFR_PREC_MIN is %d, MPFR_PREC_MAX is %ld.\n", mpfr_get_version (), MPFR_VERSION_STRING, MPFR_VERSION_MAJOR, MPFR_VERSION_MINOR, MPFR_VERSION_PATCHLEVEL, MPFR_PREC_MIN, MPFR_PREC_MAX);
	mpfr_t mpfr_r1, mpfr_r2, mpfr_r3, mpfr_r4, mpfr_result[INTERVAL], mpfr_pi, mpfr_temp, mpfr_pi_4, mpfr_invpio4;
	double result[INTERVAL];
	int i;
	double temp_d;

	mpfr_init2(mpfr_pi, PRECISION);
	mpfr_const_pi(mpfr_pi, MPFR_RNDN);
	mpfr_init2(mpfr_temp, PRECISION);
	for (i = 0; i < INTERVAL; i++) {
		mpfr_init2(mpfr_result[i], PRECISION);
		temp_d = (double)i / (4 * INTERVAL);
		mpfr_mul_d(mpfr_result[i], mpfr_pi, temp_d, MPFR_RNDN);
		mpfr_set(mpfr_temp, mpfr_result[i], MPFR_RNDN);
		mpfr_cos(mpfr_result[i], mpfr_temp, MPFR_RNDN);
		//mpfr_printf("mpfr_result[%Pd] = %.17RNe\n", (mpfr_prec_t)i, mpfr_result[i]);
		mpfr_printf("%.17RNe,\n", mpfr_result[i]);
	}
	
	/*temp_d = (double)1 / 4;
	mpfr_init2(mpfr_pi_4, PRECISION);
	mpfr_mul_d(mpfr_pi_4, mpfr_pi, temp_d, MPFR_RNDN);
	mpfr_printf("pi_4 = %.37RNe\n", mpfr_pi_4);
	
	temp_d = 4;
	mpfr_init2(mpfr_invpio4, PRECISION);
	mpfr_d_div(mpfr_invpio4, temp_d, mpfr_pi, MPFR_RNDN);
	mpfr_printf("invpio4 = %.37RNe\n", mpfr_invpio4);*/
	
	return 0;
}
