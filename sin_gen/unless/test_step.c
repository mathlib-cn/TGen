#include <stdio.h>

int main(){

	// init
	unsigned long l1 = 0x0110000000000001;
	unsigned long l2 = 0x0110000000000002;
	double x1 = *((double *)(&l1));
	double x2 = *((double *)(&l2));
	
	// compute step
	long temp1, temp2, step;
	temp1 = *((unsigned long *)(&x1));
	temp2 = *((unsigned long *)(&x2));
	step = temp2 - temp1;

	// print info
	printf("x1 = %lf, temp1 = %lx\n", x1, temp1);
	printf("x2 = %lf, temp2 = %lx\n", x2, temp2);
	printf("step = %lu\n", step);

	return 0;
}
