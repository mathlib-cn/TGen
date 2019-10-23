#include <stdio.h>
#include "myhead.h"
#define NUM 6


static const DL
C[NUM] = {
	
	{.l = 0x3b650b5df3289bd9},//log 
	{.l = 0x3fefffffffffffab},// log 
	{.l = 0xbfdfffffffef8f3d},//log
 	{.l = 0x3fd5555513a3e760},//log 
	{.l = 0xbfcfff47e4db9039},//log 
	{.l = 0x3fc930109e8f0eec},//log 
	

};

double log_gen(double x) {
	double result;
	x = x - 1.0;	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d)))));
	return result;
}

