
#include <stdio.h>
#include "myhead.h"
#define NUM 8

static const DL
C[NUM] = {
	{.l = 0x390ba592791f1fe1},
	{.l = 0x3ff0000000000000},
	{.l = 0xbc403a8c718f44c8},
	{.l = 0x3fc55555555555eb},
	{.l = 0xbd8c9a67cb07d38c},
	{.l = 0x3fb333333821648c},
	{.l = 0xbe8a24e180a9d835},
	{.l = 0x3fa6dd3adb79cec0},
};


double asin_gen(double x) {
	double result;
	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d + x * (C[6].d + x * (C[7].d))))))); 
	


	return result;
}