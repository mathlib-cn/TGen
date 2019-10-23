#include <stdio.h>
#include "myhead.h"
#define NUM 5


static const DL
C[NUM] = {
	{.l = 0},//log 0
	{.l = 0x3ff0000000000000},//log 1
	{.l = 0xbfe0000000000000},// log -1/2
	{.l = 0x3fd5555555555555},//log 1/3
 	{.l = 0xbfd0000000000000},//log -1/4
	//{.l = 0x3fc999999999999a},//log 1/5
//	{.l = 0xbfc5555555555555},//log -1/6
	//{.l = 0x3fc2492492492492},//log 1/7
};

double log_gen(double x) {
	double result;
	x = x - 1.0;	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d))));//最高次数为4
	
	return result;
}
