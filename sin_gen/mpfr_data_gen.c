#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>

#define PRECISION 128
#define INTERVAL 4
#define BIT 6
#define BITNUM 64

int main() {
	//printf ("MPFR library: %-12s\nMPFR header: %s (based on %d.%d.%d)\nMPFR_PREC_MIN is %d, MPFR_PREC_MAX is %ld.\n", mpfr_get_version (), MPFR_VERSION_STRING, MPFR_VERSION_MAJOR, MPFR_VERSION_MINOR, MPFR_VERSION_PATCHLEVEL, MPFR_PREC_MIN, MPFR_PREC_MAX);
	mpfr_t mpfr_r1, mpfr_r2, mpfr_r3, mpfr_r4, mpfr_result[BITNUM], mpfr_pi, mpfr_temp, mpfr_pi_4, mpfr_invpio4;
	double result[BITNUM];
	int i;
	double temp_d = 1, temp_d2[4] = {2, 6, 24, 120};
	unsigned long int op1;

	op1 = 2;

	//mpfr_init2(mpfr_pi, PRECISION);
	//mpfr_const_pi(mpfr_pi, MPFR_RNDN);
	//mpfr_init2(mpfr_temp, PRECISION);
	/*
	for (i = 0; i < INTERVAL; i++) {
		mpfr_init2(mpfr_result[i], PRECISION);
		temp_d = (double)i / (4 * INTERVAL);
		mpfr_mul_d(mpfr_result[i], mpfr_pi, temp_d, MPFR_RNDN);
		mpfr_set(mpfr_temp, mpfr_result[i], MPFR_RNDN);
		mpfr_cos(mpfr_result[i], mpfr_temp, MPFR_RNDN);
		//mpfr_printf("mpfr_result[%Pd] = %.17RNe\n", (mpfr_prec_t)i, mpfr_result[i]);
		mpfr_printf("%.17RNe,\n", mpfr_result[i]);
	}
	*/
	/*
	for (i = 0; i < BITNUM; i++) {
		mpfr_init2(mpfr_result[i], PRECISION);
		temp_d = (double)i / (double)BITNUM;
		mpfr_set_d(mpfr_temp, temp_d, MPFR_RNDN);
		mpfr_ui_pow(mpfr_result[i], 2, mpfr_temp, MPFR_RNDN);
		//mpfr_printf("%.17RNe,\n", mpfr_result[i]);
		result[i] = mpfr_get_d(mpfr_result[i], MPFR_RNDN);
		printf("i = %2d, %2d/%2d = 0x%lx\n", i, i, BITNUM, *((unsigned long int *)&(result[i])));
		//printf("%le\n", result[i]);
	}
	*/
	
	mpfr_init2(mpfr_r1, PRECISION);
	mpfr_init2(mpfr_r2, PRECISION);
	mpfr_init2(mpfr_r3, PRECISION);
	mpfr_set_d(mpfr_r1, temp_d, MPFR_RNDN);
	for (i = 0; i < 4; i++) {
		mpfr_set_d(mpfr_r2, temp_d2[i], MPFR_RNDN);
		mpfr_div(mpfr_r3, mpfr_r1, mpfr_r2, MPFR_RNDN);
		result[i] = mpfr_get_d(mpfr_r3, MPFR_RNDN);
		printf("i = %2d, %3.0f/%3.0f = 0x%lx\n", i, temp_d, temp_d2[i], *((unsigned long int *)&(result[i])));
		printf("%le\n", result[i]);
	}

	/*
	mpfr_init2(mpfr_pi_4, PRECISION);
	mpfr_mul_d(mpfr_pi_4, mpfr_pi, temp_d, MPFR_RNDN);
	mpfr_printf("pi_4 = %.37RNe\n", mpfr_pi_4);
	
	temp_d = 4;
	mpfr_init2(mpfr_invpio4, PRECISION);
	mpfr_d_div(mpfr_invpio4, temp_d, mpfr_pi, MPFR_RNDN);
	mpfr_printf("invpio4 = %.37RNe\n", mpfr_invpio4);
	*/
	
	return 0;
}
