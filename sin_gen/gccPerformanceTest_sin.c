#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//#include <mathimf.h>
#include <stdint.h>
//#include "myhead.h"
#include <math.h>

double sin_gen(double);
/* copy: copy 'from' into 'to'; assume to is big enough */
void copy(char to[], char from[]);

#define barrier() __asm__ __volatile__("mfence":::"memory")

unsigned long rpcc()
{
unsigned long a, d, c;
 __asm__ __volatile__ ("rdtsc" : "=a" (a), "=d" (d));
  c = a | (d << 32);
return c;
}
#define RUNTIME 1000
int main(int argc, char *argv[]){
	int i, j;
	unsigned long times[RUNTIME];
	unsigned long sum;
	uint64_t start, end;
	unsigned  cycles_low, cycles_high, cycles_low1, cycles_high1;
	double inputdata[RUNTIME];
	double result[RUNTIME], test = 1;
	double data_start = 0, data_end  = 1;
	FILE *stream_time;
	char outputFile[64] = "gcc_sin_time.txt";

	if (argc == 2) {
		copy(outputFile, argv[1]);
		printf("outputFile is %s.\ntest from %f, end at %f\n", outputFile, data_start, data_end);
	}
	if (argc == 4) {
		copy(outputFile, argv[1]);
		data_start = atof(argv[2]);
		data_end = atof(argv[3]);
		printf("outputFile is %s.\ntest from %f, end at %f\n", outputFile, data_start, data_end);
	}	
	stream_time = fopen(outputFile, "w");
	if(stream_time == (FILE *)0) {
                 printf("open file error!\n");
                 exit(1);
	}

	for(i = 0; i < RUNTIME; i++) {
        	inputdata[i] = (double)rand() / ((double)RAND_MAX + 1)  * ((data_end) - (data_start)) + (data_start);
	}
	for (i = 0; i < 100; i++) {
//		sin_gen(inputdata[i]);
		sin(inputdata[i]);
	}
	for(i = 0; i < RUNTIME; i++) {
		barrier();
		__asm__ __volatile__(
        		"RDTSC": "=a" (cycles_low), "=d" (cycles_high));
//		result[i] = sin_gen(inputdata[i]);
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
