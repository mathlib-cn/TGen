#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#include "myhead.h"

struct test_data {
	unsigned long sum;
	unsigned long max;
	unsigned long max_times;
};

double sin_gen(double);
void getbinary(_UL, int *);
_UL getUL(int *);
void printbinary(int *);
int computeAccurateBit(double, double);
_UL computeULPDiff(double, double);
void binaryshow(double);
double itofd(_UL);
double computeULP(double);

struct test_data test(void)
{
	struct test_data correctness_result;
	//double a, b, x;
	double y1, y2;
	int i, j, bit_num;
	unsigned long max_times;
	int result[64];
	_UL ulpdiff, ulpdiff_max, temp_ul, ulpdiff_sum;
	double input;
	double ulp, ulpd;
	FILE *inputData;
	char temp_c;
	mpfr_t mpfr_temp, mpfr_result;

	correctness_result.sum = 0;
	correctness_result.max = 0;
	correctness_result.max_times = 0;
	ulpdiff_sum = 0;
	ulpdiff_max = 0;
	max_times = 0;
	mpfr_init2(mpfr_temp, 100);
	mpfr_init2(mpfr_result, 100);

	inputData = fopen("data_1000000_-pi_to_pi.txt", "r");

	if (inputData == (FILE *)0) {
		printf("open error!!!\n");
		exit(1);
	}

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

		//printf("for x = %.17e:\n", input);
		//printf("\tsin_gen = %.17e\n\tsin     = %.17e\n", y1, y2);
		//printf("binary of sin_gen is ");
		//binaryshow(y1);
		//printf("binary of sin     is ");
		//binaryshow(y2);
		//bit_num = computeAccurateBit(y1, y2);
		//printf("the accurate binary bit is %d\n", bit_num);
		ulpdiff = computeULPDiff(y1, y2);
		//printf("%d\n", ulpdiff);
		if (ulpdiff < 0)
		{
			//printf("shit!!!\n");
		}
		if (ulpdiff_max < ulpdiff) {
			ulpdiff_max = ulpdiff;
			max_times = 1;
		}
		else if (ulpdiff_max == ulpdiff) {
			max_times++;
		}
		ulpdiff_sum += ulpdiff;
		//printf("The ULPDiff is %ld\n", ulpdiff);
		//printf("The ULPDiff is 0x%lx\n\n", ulpdiff);
		//ulp = computeULP(y2);
		//ulpd = (y1 - y2) / ulp;
		//printf("The ULPD is %f\n\n", ulpd);
	}
	//printf("sum = %ld\n", ulpdiff_sum);
	//printf("%ld\n", ulpdiff_sum);
	//printf("ulpdiff_max = %ld\n", ulpdiff_max);
	//printf("%ld\n", ulpdiff_max);
	//printf("max_times = %d\n", max_times);
	correctness_result.sum = ulpdiff_sum;
	correctness_result.max = ulpdiff_max;
	correctness_result.max_times = max_times;
	fclose(inputData);

	return correctness_result;
}

int main(int argc, char *argv[]) {
	struct test_data correctness_result;
	correctness_result = test();
	//printf("%d %d %d\n", p->max, p->max_times, p->sum);
	printf("%lu\n", correctness_result.max);
	printf("%lu\n", correctness_result.max_times);
	printf("%lu\n", correctness_result.sum);
	return 0;
}
