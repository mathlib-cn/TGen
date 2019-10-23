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

#define _EXP_BITS 8
typedef unsigned int _TYPE;  //4 byte
typedef float DATATYPE;

typedef struct
{
	_TYPE m_nFraction : sizeof(_TYPE)*8-_EXP_BITS-1;
	_TYPE m_nExp	  : _EXP_BITS;
	_TYPE m_nSign     : 1;
} _DATA;

DATATYPE anyf(int nSign,int nExp,long nFraction)
{
	DATATYPE rv=0.0;
	_DATA *p=(_DATA *)&rv;
	p->m_nSign	     = nSign;
	p->m_nExp      = nExp;
	p->m_nFraction = nFraction;

	return rv;
	
}
int main(int argc,char *argv[]) 
{
	int sign=0;  //we aussume it positive as it makes no difference
	int exponent;  //(-127~128)according to the bais,the actual range is 0~255
	unsigned long fraction;  //travers first n bits,and the rest bits can be either 1 or 0 
	long restMaxVal=pow(2,23- FRA_TRAVERSED_COUNT)-1;
	long restFraction=0;
	long firstFraction=0;
	int sum=0;
	float r;
	FILE *stream,*stream_f;
	stream=fopen("../float_samples.txt","w");
	stream_f=fopen("../float_num_samples.txt","w");
	
	/* generate float numbers */
	for(exponent=1;exponent<=254;exponent++)
	{
		/* produce fisrt fraction bits */
		long i=0;
		long j=0;
		long ii=0;
		for(i=0;i<FRA_TRAVERSED_POW;i++)
		{
			ii=i;
			for(j=0;j<FRA_TRAVERSED_COUNT;j++)
			{
				sum=ii & 1;
                		if(sum==1)
				{
		 	 		firstFraction+=pow(2,23-(FRA_TRAVERSED_COUNT-j));
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
			r=anyf(0,exponent,fraction);
			fprintf(stream_f,"%e\n",r);

		}
	}
	return 0;
}
