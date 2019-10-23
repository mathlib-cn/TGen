#include <stdio.h>
#include "myhead.h"
#define NUM 5


static const DL
C[NUM] = {
	{.l = 0x3bf9629503fda89a},//log 
	{.l = 0x3fefffffffff3355},//log 
	{.l = 0xbfdfffffe6a834fd},// log 
	{.l = 0x3fd5551603cf9be3},//log 
 	{.l = 0xbfcf9a79f6f3bb40},//log 

};

double log_gen(double x) {
	double result;
	x = x - 1.0;	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d))));//最高次数为4
	

	return result;
}

