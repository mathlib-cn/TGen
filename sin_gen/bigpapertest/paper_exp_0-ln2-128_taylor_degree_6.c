#include <stdio.h>
#include "myhead.h"
#define NUM 7


static const DL
C[NUM] = {
	{.l = 0x3ff0000000000000},//exp 1
	{.l = 0x3ff0000000000000},// exp 1/1!
	{.l = 0x3fe0000000000000},//exp 1/2!
 	{.l = 0x3fc5555555555555},//exp 1/3!
	{.l = 0x3fa5555555555555},//exp 1/4!
	{.l = 0x3f81111111111111},//exp 1/5!
	{.l = 0x3f56c16c16c16c17},//exp 1/6！
	//{.l = 0x3f2a01a01a01a01a},//exp 1/7！

};

double exp_gen(double x) {
	double result;
	
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d +x * (C[6].d))))));
	return result;
}
