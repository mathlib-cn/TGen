#include <stdio.h>
#include "myhead.h"
#define NUM 8


static const DL
C[NUM] = {
	{.l = 0},
	{.l = 0x3ff0000000000000},//sin 1!
	{.l = 0},
	{.l = 0xbfc5555555555555},//sin -1/3!
 	{.l = 0},
	{.l = 0x3f81111111111111},//sin 1/5!
	{.l = 0},
	{.l = 0xbf2a01a01a01a01a},//sin -1/7!
//	{.l = 0},
//	{.l = 0x3ec71de3a556c734},//sin 1/9!
//	{.l = 0},
//	{.l = 0xbe5ae64567f544e4},//sin -1/11!
//	{.l = 0},
//	{.l = 0x3de6124613a86d09},//sin 1/13!
//	{.l = 0},
};

double sin_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d +x * (C[6].d + x * (C[7].d)))))));//最高次数为7

	
	return result;
}
