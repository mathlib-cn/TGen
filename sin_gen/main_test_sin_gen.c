#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#include "myhead.h"

double sin_gen(double);
void getbinary(_UL, int *);
_UL getUL(int *);
void printbinary(int *);
int computeAccurateBit(double, double);
_UL computeULPDiff(double, double);
void binaryshow(double);
double itofd(_UL);
double computeULP(double);

int main(int argc, char *argv[]) {
	double a, b, x, y1, y2;
	int i, j, bit_num;
	int result[64];
	_UL ulpdiff, ulpdiff_max, temp_ul, ulpdiff_sum;
	double input;
	double ulp, ulpd;
	FILE *inputData;
	char temp_c;
	mpfr_t mpfr_temp, mpfr_result;

	ulpdiff_sum = 0;
	mpfr_init2(mpfr_temp, 100);
	mpfr_init2(mpfr_result, 100);

	inputData = fopen("data.txt", "r");
	
	for (i = 0; i < RUN_COUNT; i++) {
		for (j = 63; j >= 0; j--) {
			temp_c = getc(inputData);
			result[j] = temp_c - '0';
		}
		getc(inputData); // pass the "\n"
		temp_ul = getUL(result);
		input = itofd(temp_ul);

		y1 = sin_gen(input);

		mpfr_set_d(mpfr_temp, input, MPFR_RNDN);
		mpfr_sin(mpfr_result, mpfr_temp, MPFR_RNDN);
		y2 = mpfr_get_d(mpfr_result, MPFR_RNDN);

		//printf("for x = %.17e:\n", input[i]);
		//printf("\tsin_gen = %.17e\n\tsin     = %.17e\n", y1, y2);
		//printf("binary of sin_gen is ");
		//binaryshow(y1);
		//printf("binary of sin     is ");
		//binaryshow(y2);
		//bit_num = computeAccurateBit(y1, y2);
		//printf("the accurate binary bit is %d\n", bit_num);
		ulpdiff = computeULPDiff(y1, y2);
		ulpdiff_sum += ulpdiff;
		printf("%ld\n", ulpdiff);
		//printf("The ULPDiff is 0x%lx\n\n", ulpdiff);
		//ulp = computeULP(y2);
		//ulpd = (y1 - y2) / ulp;
		//printf("The ULPD is %f\n\n", ulpd);
	}
	printf("sum = %ld\n", ulpdiff_sum);

	fclose(inputData);
	return 0;
}
