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
//typedef unsigned long int _UL;
/*typedef unsigned int _UL;

_UL ftoif(float x) {
	return *((_UL *)(&x));
}

*/
#define _EXP_BITS 8
#define SIGN_BIT 0x80000000
#define EXP_BIT  0x7f800000
#define TAIL_BIT 0x007fffff
typedef unsigned int _TYPE;  //4 byte
typedef float DATATYPE;

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
	
	unsigned int temp1, temp2, temp3, step, temp_input, exp, fra;
	int n, sign;  
	float a, b;
	FILE *stream;
	stream=fopen("../float_any_interval.txt","w");
	
	unsigned int r_temp;
	float input, r;
	

	a = atof(argv[1]);
	b = atof(argv[2]);
	n = atoi(argv[3]);
	if (a < 0 || b < 0 || b < a || n == 0){
		printf("check your input ：b>a>0, n !=0\n");
		return 1;
	}	
	printf("a=%f\n",a);		
	printf("b=%f\n",b);		
	printf("n=%d\n",n);		

	temp1 = *((unsigned int *)(&a));
	temp2 = *((unsigned int *)(&b));
	temp3 = temp2 - temp1;
	step  = temp3/n;
	
	printf("temp1=%u\n",temp1);		
	printf("temp2=%u\n",temp2);		
	printf("temp3=%u\n",temp3);		
	printf("step=%u\n",step);
	
	for (int i = 0;i < n; i++){
		temp_input = temp1 + i * step;
		input = *((float *)(&temp_input));                  //浮点数
        	exp = ((temp_input & EXP_BIT) >> 23);              //指数值
        	fra = temp_input & TAIL_BIT;                       //尾数各位 
        	sign = temp_input & SIGN_BIT;                      //符号位
		printf("--------------------------|\n");
		printf("temp_input_DEC=%u\n",temp_input);
		printf("temp_input_HEX=0x%x\n",temp_input);
        	printf("exp=%u\n",exp);
        	printf("fra=%u\n",fra);
		printf("input=%e\n",input);
		printf("--------------------------|\n\n");
		r = anyf(sign,exp,fra);
		r_temp =  *((unsigned int *)(&r));
		printf("r=%e\n",r);
		printf("r=0x%x\n",r_temp);               

		fprintf(stream,"%d\t%d\t%d\n",sign,exp,fra);
	}
	return 0;
}	
