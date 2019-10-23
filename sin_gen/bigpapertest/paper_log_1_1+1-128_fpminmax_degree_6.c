#include <stdio.h>
#include "myhead.h"
#define NUM 7


static const DL
C[NUM] = {
	{.l = 0xba08dc0372e7d729},//log 
	{.l = 0x3ff0000000000000},//log 
	{.l = 0xbfdffffffffff8bc},// log 
	{.l = 0x3fd55555552268cd},//log 
 	{.l = 0xbfcfffff16db8966},//log 
	{.l = 0x3fc998afee4a1511},//log 
	{.l = 0xbfc4eaa84c10d4fc},//log 


};

double log_gen(double x) {
	double result;
	
	x = x - 1.0;	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d +x * (C[6].d))))));
	return result;
}

