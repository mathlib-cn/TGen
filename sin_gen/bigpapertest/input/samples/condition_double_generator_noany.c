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

_UL ftoif(double x) {
	return *((_UL *)(&x));
}

*/
#define _EXP_BITS 11
#define SIGN_BIT 0x8000000000000000
#define EXP_BIT  0x7FF0000000000000
#define TAIL_BIT 0x0007ffffffffffff
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
	
	unsigned long temp1, temp2, temp3, step, temp_input, fra;
	int n, sign, exp;  
	double a, b;
	FILE *stream;
	stream=fopen("../double_any_interval.txt","w");
	
	unsigned long r_temp;
	double input, r;
	

	a = atof(argv[1]);
	b = atof(argv[2]);
	n = atoi(argv[3]);
	
	printf("a=%f\n",a);		
	printf("b=%f\n",b);		
	printf("n=%d\n",n);
	
	if (a < 0 || b < 0 || b < a || n == 0){
		printf("check your input ：b>a>0, n !=0\n");
		return 1;
	};		

	temp1 = *((unsigned long *)(&a));
	temp2 = *((unsigned long *)(&b));
	temp3 = temp2 - temp1;
	step  = temp3/n;
	
	printf("temp1=%lu\n",temp1);		
	printf("temp2=%lu\n",temp2);		
	printf("temp3=%lu\n",temp3);		
	printf("step=%lu\n",step);
	
	for (int i = 0;i < n; i++){
		temp_input = temp1 + i * step;
		input = *((double *)(&temp_input));                  //浮点数
		printf("%e\n",input);
     
	}
	return 0;
}	
