#include <stdio.h>
#include <math.h>
#include "myhead.h"

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
	e = e - (1 << 10) + 1;

	if (sign == 0) {
		printf("+1.");
	}
	else {
		printf("-1.");
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
