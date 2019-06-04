#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//#include <mathimf.h>
#include <stdint.h>
#include "myhead.h"
#include <math.h>

#define barrier() __asm__ __volatile__("mfence":::"memory")

unsigned long rpcc()
{
unsigned long a, d, c;
 __asm__ __volatile__ ("rdtsc" : "=a" (a), "=d" (d));
  c = a | (d << 32);
return c;
}

int main(){
	int i, j;
	unsigned long times[RUNTIME];
	unsigned long sum;
	uint64_t start, end;
	unsigned  cycles_low, cycles_high, cycles_low1, cycles_high1;
	double inputdata[RUNTIME];
	double result[RUNTIME], test = 1;
	FILE *stream_time;

	stream_time = fopen("gcc_sin_time.txt", "w");
	if(stream_time == (FILE *)0) {
                 printf("open file error!\n");
                 exit(1);
	}

	for(i = 0; i < RUNTIME; i++) {
        	inputdata[i] = (double)rand() / ((double)RAND_MAX + 1)  * ((1) - (0)) + (0);
	}
	for (i = 0; i < 100; i++) {
		sin(inputdata[i]);
	}
	for(i = 0; i < RUNTIME; i++) {
		barrier();
		__asm__ __volatile__(
        		"RDTSC": "=a" (cycles_low), "=d" (cycles_high));
		result[i] = sin(inputdata[i]);
		__asm__ __volatile__(
        		"RDTSC": "=a" (cycles_low1), "=d" (cycles_high1));
		barrier();
		result[i] = result[i] + test;
		start = (((uint64_t)cycles_high << 32) | cycles_low);
		end = (((uint64_t)cycles_high1 << 32) | cycles_low1);
		times[i] = end - start;
		//printf("time is %llu clock cycles\n", times[i]);
	}
	sum = 0;
	for(i = 0; i < RUNTIME; i++) {
		fprintf(stream_time, "%lu\n", times[i]);
		sum = sum + times[i];
	}
	sum = sum / RUNTIME;
	//printf("%lu\n",sum);
	if(fclose(stream_time) == EOF) {
		printf("close file error!\n");
		exit(1);
    }
}
