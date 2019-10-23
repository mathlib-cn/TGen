#include <stdio.h>
#include "myhead.h"
#define NUM 8


static const DL
C[NUM] = {
	/*
	{.l = 0xc004b6f0b46b1399},//log 
	{.l = 0x401be6bb33afcbe9},//log 
	{.l = 0xc024da2846df654b},// log
	{.l = 0x4027165cdeba3e5e},//log 
 	{.l = 0xc02141213b0eb8e0},//log 
	{.l = 0x40108179da28add4},//log 
	{.l = 0xbff2466506e41d22},//log 
	{.l = 0x3fc1d6ba8fbade40},//log 
	*/
	{.l = 0xb97af3fd1fcfef73},//log 
	{.l = 0x3ff0000000000000},//log 
	{.l = 0xbfdffffffffffffc},// log
	{.l = 0x3fd5555555552dde},//log 
 	{.l = 0xbfcffffffefe562c},//log 
	{.l = 0x3fc9999817d3a2ff},//log 
	{.l = 0xbfc554317b3e0984},//log 
	{.l = 0x3fc1dc5c459568d8},//log 
};

double log_gen(double x) {
	double result;
		x = x - 1.0;	
	result = C[0].d + x * (C[1].d + x * (C[2].d + x * (C[3].d + x * (C[4].d + x * (C[5].d +x * (C[6].d + x * (C[7].d)))))));
	return result;

}


