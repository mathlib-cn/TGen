#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//#include <mathimf.h>
#include <stdint.h>
#include <math.h>
#define barrier() __asm__ __volatile__("mfence":::"memory")

unsigned long rpcc()
{
unsigned long a, d,c;
 __asm__ __volatile__ ("rdtsc" : "=a" (a), "=d" (d));
  c = a | (d << 32);
return c;
}

//extern double sin_gen(double);

int main(){
	int i,j;
	double r1,r2;
	double aaa=1,a1[200];
	unsigned long times[200];
	unsigned long sum;
	uint64_t start,end;
	unsigned  cycles_low,cycles_high,cycles_low1,cycles_high1;
	double bc[200],bcu[200],test,ff=1;
	FILE *stream_time1;

	stream_time1=fopen("intel_sin_gen_time.txt","w");
	if(stream_time1==(FILE *)0)
        {
                 printf("open file error!\n");
                 exit(1);
        }

	for(i=0;i<200;i++)
	{
        	a1[i]=(double)rand() / ((double)RAND_MAX + 1)  * ((1)-(0)) + (0);
	}
	for(i = 0; i < 200; i++) {
		sin_gen(a1[i]);
	}
	for(i=0;i<200;i++){
	
		barrier();
		__asm__ __volatile__(
        		"RDTSC": "=a" (cycles_low),"=d" (cycles_high));
		bcu[i] = sin_gen(a1[i]);
		__asm__ __volatile__(
        		"RDTSC": "=a" (cycles_low1),"=d" (cycles_high1));
		barrier();
		bcu[i]=bcu[i]+test;
//		printf("%f\n",bcu[i]);
		start=(((uint64_t)cycles_high << 32)| cycles_low);
		end=(((uint64_t)cycles_high1 << 32)| cycles_low1);
		times[i]=end-start;
		//printf("time is %llu clock cycles\n", times[i]);
	}
	sum = 0;
	for(i=0;i<200;i++)
	{
		fprintf(stream_time1,"%lu\n", times[i]);
		sum = sum+times[i];
	}
	sum = sum/199;
	printf("%lu\n",sum);
	if(fclose(stream_time1)==EOF)
        {
        	printf("close file error!\n");
                exit(1);
        }
}
