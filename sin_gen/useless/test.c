#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sched.h>
//typedef unsigned long int _UL;
typedef unsigned int _UL;

_UL ftoif(float x) {
	return *((_UL *)(&x));
}

int main(int argc,char *argv[]) {
	// init
//	unsigned int l1 = 0x3DCCCCCD;
//	unsigned int l2 = 0x3DE147AE;
//	printf("l1=%u\n",l1);		
//	printf("l2=%u\n",l2);		
//	float x1 = *((float *)(&l1));
//	float x2 = *((float *)(&l2));

	float x1 = 1.0;
	float x2 = 10.0;
	printf("x1=%f\n",x1);		
	printf("x2=%f\n",x2);		
	//double x1 = *((double *)(&l1));
	//double x2 = *((double *)(&l2));
	
	// compute step
	unsigned int temp1, temp2, step, temp3, temp_input;
	int number = 50;
	float input;
	temp1 = *((unsigned int *)(&x1));
	temp2 = *((unsigned int *)(&x2));
	temp3 = temp2 - temp1;
	step  = temp3/number;
	
	printf("temp1=%u\n",temp1);		
	printf("temp2=%u\n",temp2);		
	printf("temp3=%u\n",temp3);		
	printf("step=%u\n",step);
	
	for (int i = 0;i < 50; i++){
		temp_input = temp1 + i * step;
		input =*((float *)(&temp_input));
		printf("input=%f\n",input);
	}
	
	/*printf("long int 存储大小 : %lu \n", sizeof(long int));
	printf("long 存储大小 : %lu \n", sizeof(long));
	printf("int 存储大小 : %lu \n", sizeof(int));
	printf("float 存储大小 : %lu \n", sizeof(float));
	*/
}
