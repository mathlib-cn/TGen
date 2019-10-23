#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sched.h>
#include <math.h>

double randomExponential(double lambda)
{
	double pv = 0.2;
//	pv = (double)(rand()%100)/100;
//	while(pv == 0)
//	{
//		pv = (double)(rand() % 100)/100;
//	}
	pv = (-1  / lambda)*log(1-pv);
	return pv;
}
int main(int argc,char *argv[]) {
	double lambda,pv;
	pv = randomExponential(lambda);
	printf("----------end!!\npv=%f\n",pv);
}
