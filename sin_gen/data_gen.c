#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "myhead.h"

_UL ftoid(double);
void getbinary(_UL , int *);
void printbinary(int *);
void fprintbinary(FILE *, int *);

void data_gen(double start, double end) {
	FILE *data;
	double inputData;
	double temp;
	_UL temp_ul;
	int i, j, binary[64];

	data = fopen("data.txt", "w");
	srand(time(NULL));
	for (i = 0; i < RUN_COUNT; i++) {
		inputData=(double)rand() / ((double)RAND_MAX + 1) * (end - start) + (start);
		temp_ul = ftoid(inputData);
		getbinary(temp_ul, binary);
		//printbinary(binary);
		fprintbinary(data, binary);
	}

	fclose(data);
}


int main() {
	double start, end;
	DL s, e;

	s.l = 0x3e40000000000000;
	start = s.d;
	end = 6.1359231515425649188723503579677790706975964831545e-3; // pi/512
	end = 1.2271846303085129837744700715935558141395192966309e-2; // pi/256
	end = 2.4543692606170259675489401431871116282790385932618e-2; // pi/128
	end = 4.9087385212340519350978802863742232565580771865236e-2; // pi/64
	end = 9.817477042468103870195760572748446513116154373047e-2; // pi/32
	end = 0.19634954084936207740391521145496893026232308746094; // pi/16
	start = -40;
	end = 40;
	data_gen(start, end);
}
