#include <stdio.h>
#include <math.h>
#include "myhead.h"

/* copy: copy 'from' into 'to'; assume to is big enough */
void copy(char to[], char from[])
{
	int i;
	i = 0;
	while ((to[i] = from[i]) != '\0')
		++i;
}

_UL ftoid(double x) {
	return *((_UL *)(&x));
}

double itofd(_UL x) {
	return *((double *)(&x));
}

void getbinary(_UL x, int *result) {
	int i;
	_UL temp;

	temp = x;
	for (i = 0; i <= 63; i++) {
		result[i] = temp & 0x0000000000000001;
		temp = temp >> 1;
	}
}

_UL getUL(int *result) {
	int i;
	_UL temp;

	temp = 0;
	for (i = 63; i >= 0; i--) {
		//result[i] = temp & 0x0000000000000001;
		//temp = temp >> 1;
		temp = temp << 1;
		temp = temp + result[i];
	}

	return temp;
}

void printbinary(int *result) {
	int i;
	
	for (i = 63; i >= 0; i--) {
		printf("%d", result[i]);
	}
	printf("\n");
}

void fprintbinary(FILE * file, int *result) {
	int i;

	for (i = 63; i >= 0; i--) {
		fprintf(file, "%d", result[i]);
	}
	fprintf(file, "\n");
}

void binaryshow(double x) {
	_UL temp;
	int sign, e, i, result[64];

	temp = ftoid(x);
	getbinary(temp, result);	
	sign = result[63];
	e = 0;
	for (i = 10; i >= 0; i--) {
		e = (e << 1) + result[52 + i];
	}
	
	if (e == 0) {
		printf(" 0.");
	}
	else {
		e = e - (1 << 10) + 1;
		if (sign == 0) {
			printf("+1.");
		}
		else {
			printf("-1.");
		}
	}
	for (i = 51; i >= 0; i--) {
		printf("%d", result[i]);
	}
	printf("b%d\n", e);
}

_UL computeULPDiff(double x1, double x2) {
	_UL temp1, temp2;
	_UL t;

	temp1 = ftoid(x1);
	temp2 = ftoid(x2);

	temp1 = temp1 & 0x000fffffffffffff;
	temp2 = temp2 & 0x000fffffffffffff;

	if (temp1 > temp2)
	{
		t = temp1 - temp2;
	}
	else {
		t = temp2 - temp1;
	}

	return t;
}

int computeAccurateBit(double x1, double x2) {
	int y1[64], y2[64], i, count;
	_UL temp1, temp2;

	temp1 = ftoid(x1);
	temp2 = ftoid(x2);
	getbinary(temp1, y1);
	getbinary(temp2, y2);
	
	count = 0;
	for (i = 51; i >= 0; i--) {
		if (y1[i] != y2[i]) {
			break;
		}
		count++;
	}

	return count;
}

int jiou(int x) {
	
	if(x % 2 == 0)
		return 1;
	else
		return 0;
}

double  computeULP(double y) {
	double x  =  0, res  =  0, powermin  =  0, powermax  =  0, powermiddle  =  0;
	int expmin  =  0, expmax = 0, expmiddle = 0, jioupanduan = 0;
	x =  fabs(y);

//  printf("res=%.8f\n", pow(2, logb(x)+1-53));

	if(x<pow(2, -1021))
		res = pow(2, -1074);
	else if(x>(1-pow(2, -53))*pow(2, 1024))
		res = pow(2, 971);
	else {
		powermin = pow(2, -1021);
		expmin=-1021;
		powermax = pow(2, 1024);
		expmax =  1024;
	}
  
	while(expmax - expmin >1) {
		
		jioupanduan = jiou(expmin+expmax);
		
		if(jioupanduan ==1)
			expmiddle=(expmax+expmin)/2;
		else
			expmiddle=(expmax+expmin+1)/2;
		
		powermiddle  =  pow(2, expmiddle);

		if(x>=powermiddle) {
			powermin  =  powermiddle;
			expmin  =  expmiddle;
		}
		else {
			powermax  =  powermiddle;
			expmax = expmiddle;
		}

		if(x==powermin)  
			res = pow(2, expmin-53);
		else
			res = pow(2, expmin-52);
	}
	return res;
}
// 没法使用，如果都转化成double类型，再对比差值，还有什么意义呢
/*
_UL computeULPDiff1(double mine,  double biaozhun) {
	double reUlp,  ulpdiff;
	mpfr_t mpfr_biaozhun,  mpfr_mine,  mpfr_ulpdiff,   mpfr_temp;
	
	mpfr_init2(mpfr_biaozhun,  128);
	mpfr_init2(mpfr_mine,  128);
	mpfr_init2(mpfr_ulpdiff,  128);
	mpfr_init2(mpfr_temp,  128);
	
	mpfr_set_d(mpfr_mine,  mine,  MPFR_RNDN);
	mpfr_set_d(mpfr_biaozhun,  biaozhun,  MPFR_RNDN);
	mpfr_sub(mpfr_temp,  mpfr_biaozhun,  mpfr_mine,  MPFR_RNDN);

	if(reUlp != 0) {
		reUlp  =  computeULP(biaozhun);
		mpfr_div_d(mpfr_ulpdiff,  mpfr_temp,  reUlp,  GMP_RNDN);
		ulpdiff  =  mpfr_get_d(mpfr_ulpdiff,  GMP_RNDN);
		ulpdiff  =  fabs(ulpdiff);
	} else {
		printf("reUlp = 0\n");
	}
}
*/