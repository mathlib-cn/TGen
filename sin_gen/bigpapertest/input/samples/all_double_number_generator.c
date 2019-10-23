/* Typhoon 20190109 */
/* Generate Float Numbers */
/* We aussume sign bit as 0 and travers exponent bits from 0 to 254.
   For fraction bits ,we travers first n bits("n" is defined in typh_const_num.h),
   and the rest bits can be either 1 or 0 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sched.h>
#include "../include/typh_const_num.h"

int main(int argc,char *argv[]) 
{
	//double input32;
	int sign=0;  //we aussume it positive as it makes no difference
	int exponent;  //(-1023~1024)according to the bais,the actual range is 0~2047
	unsigned long fraction;  //travers first n bits,and the rest bits can be either 1 or 0 
	long restMaxVal=pow(2,52- FRA_TRAVERSED_COUNT_DOUBLE)-1;//FRA_TRAVERSED_COUNT_DOUBLE=26
	long restFraction=0;
	long firstFraction=0;
	int sum=0;  
	FILE *stream;
	stream=fopen("../double_samples.txt","w");
	
	/* generate float numbers */
	for(exponent=1;exponent<=2046;exponent++)
	{
		/* produce fisrt fraction bits */
		long i=0;
		long j=0;
		long ii=0;
		for(i=0;i<FRA_TRAVERSED_POW_DOUBLE;i++)//FRA_TRAVERSED_POW_DOUBLE=pow(2,FRA_TRAVERSED_COUNT)=pow(2,26)
		{
			ii=i;
			for(j=0;j<FRA_TRAVERSED_COUNT_DOUBLE;j++)
			{
				sum=ii & 1;
                		if(sum==1)
				{
		 	 		firstFraction+=pow(2,52-(FRA_TRAVERSED_COUNT_DOUBLE-j));
				}
				ii=ii>>1;
			}
		
			/* produce rest fraction bits */	
			restFraction=rand()%(restMaxVal+1);
        		/* combine */
     	   		fraction=firstFraction+restFraction;
			/* reset */
			firstFraction=0;
			/* write to file */
			fprintf(stream,"%d\t%d\t%ld\n",0,exponent,fraction);
		}
	}
	return 0;
}
