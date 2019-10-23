#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sched.h>
#include <math.h>

#include "typh_const_num.h"

int main(int argc, char *argv[]) {

/*
long m=FLOAT_DATA_LENGTH;
printf("test of header:");
printf("%ld\n",m);



double a,b,c,d;
long e;
b=2;
c=52;
a=pow(b,c-3)-1;
e=pow(-1,0);
printf("pow:");
printf("%ld\n",e);



printf("unsigned int:\t");
printf("%d\n",sizeof(unsigned int));

printf("int:\t");
printf("%d\n",sizeof(int));

printf("float:\t");
printf("%d\n",sizeof(float));

printf("double:\t");
printf("%d\n",sizeof(double));

printf("unsigned long:\t");
printf("%d\n",sizeof(unsigned long));

printf("long:\t");
printf("%d\n",sizeof(long));

*/
int i;
for(i=0;i<10;i++)
{
              float k=(float)rand() / ((float)RAND_MAX + 1)* ((1)-(0)) + (0);
              printf("%f\n",k);
}

	return 0;
}

