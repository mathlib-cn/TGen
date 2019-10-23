/*xyy
Generate Float Numbers with condition

Give the interval and the amount of data taken within the interval, and return the corresponding data set

like input [A,B] N
return N data between A and B
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sched.h>
#include "myhead.h"

void getbinary(_UL , int *);
void printbinary(int *);
void fprintbinary(FILE *, int *);

#define _EXP_BITS 11
#define SIGN_BIT 0x8000000000000000
#define EXP_BIT  0x7FF0000000000000
#define TAIL_BIT 0x000fffffffffffff
typedef unsigned long _TYPE;  //8 byte
typedef double DATATYPE;

typedef struct{
	_TYPE m_nFraction : sizeof(_TYPE)*8-_EXP_BITS-1;
	_TYPE m_nExp	  : _EXP_BITS;
	_TYPE m_nSign     : 1;
} _DATA;

DATATYPE anyf(int nSign,int nExp,long nFraction){
	DATATYPE rv=0.0;
	_DATA *p=(_DATA *)&rv;
	p->m_nSign	     = nSign;
	p->m_nExp      = nExp;
	p->m_nFraction = nFraction;

	return rv;
}


/*-------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
	
	_UL a_int, b_int, temp, step, temp_input, fra;
	int n, sign, exp;  
	double a, b;
	int binary[64];
	FILE *stream;
	stream=fopen("../double_any_interval.txt","w");
	
	unsigned long r_temp;
	double input, r;
	
	if (argc != 4) {
		printf("input parameters number is not right\n");
		return 0;
	}
	
	a = atof(argv[1]);
	b = atof(argv[2]);
	n = atoi(argv[3]);

	if (a < 0 || b < 0 || b < a || n == 0){
		printf("check your input ：b>a>0, n !=0\n");
		return 1;
	}	
	printf("a\t= %e\n",a);		
	printf("b\t= %e\n",b);		
	printf("n\t= %d\n",n);		

	a_int = *((unsigned long *)(&a));
	b_int = *((unsigned long *)(&b));
	temp = b_int - a_int;
	step  = temp/n;
	
	printf("a_int\t= %lu\n",a_int);		
	printf("b_int\t= %lu\n",b_int);		
	printf("temp\t= %lu\n",temp);		
	printf("step\t= %lu\n",step);
	
	for (int i = 0;i < n; i++){
		temp_input = a_int + i * step;
		getbinary(temp_input, binary);
		fprintbinary(stream, binary);
		
		//input = *((double *)(&temp_input));                  //浮点数
        //exp = ((temp_input & EXP_BIT) >> 52);              //指数值
        //fra = temp_input & TAIL_BIT;                       //尾数各位 
        //sign = temp_input & SIGN_BIT;                      //符号位
		
		//printf("------------------------------------------------------|\n");
		//printf("temp_input_DEC=%lu\n",temp_input);
		//printf("*********************temp_input_HEX=0x%lx\n",temp_input);
        //printf("exp=%u\n",exp);
        //printf("fra=%lu\n",fra);
		//printf("*********************input=%e\n",input);
		//printf("--------------------|\n");
		//r = anyf(sign,exp,fra);
		//r_temp =  *((unsigned long *)(&r));
		//printf("*********************r=%e\n",r);
		//printf("*********************r=0x%lx\n",r_temp);               
		//printf("--------------------|\n");
		//if (r == input)
		//	printf("float-point all right!\n");
		//else
		//	printf("float-point show is wrong!\n");
		//if (r_temp == temp_input)
		//	printf("HEX all right!\n");
		//else
		//	printf("HEX show is wrong!\n");
		//printf("-----------------------------------------------------|\n\n\n\n\n");
		
		//fprintf(stream,"%d\t%d\t%ld\n",sign,exp,fra);
	}
	return 0;
}	
