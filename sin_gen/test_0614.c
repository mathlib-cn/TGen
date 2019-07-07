#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#include "test_fshow.h"

int main() {
	mpfr_t mpfr_temp, mpfr_result;
	long x = 0x400004fa1e2a95c0;
	double input = *(double *)&x;
	double y;

	mpfr_init2(mpfr_temp, 100);
	mpfr_init2(mpfr_result, 100);

	mpfr_set_d(mpfr_temp, input, MPFR_RNDN);
	mpfr_sin(mpfr_result, mpfr_temp, MPFR_RNDN);
	y = mpfr_get_d(mpfr_result, MPFR_RNDN);

	show_double2(y);
	printf("\n");
	y = sin(input);
	show_double2(y);
	printf("\n");
}

