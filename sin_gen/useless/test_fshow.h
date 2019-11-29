#include <stdio.h>
typedef unsigned char *byte_pointer;
void show_bytes(FILE *f,byte_pointer start,int len)
{
	int i;
	for (i=len-1;i>=0;i--)
		fprintf(f,"%.2x",start[i]);
//	printf("\n");
}
void show_bytes2(byte_pointer start,int len)
{
	int i;
	for (i=len-1;i>=0;i--)
		printf("%.2x",start[i]);
//	printf("\n");
}

void show_double2(double x)
{
	show_bytes2((byte_pointer)&x,sizeof(double));
}
void show_float2(float x)
{
        show_bytes2((byte_pointer)&x,sizeof(float));
}
void show_int2(int x)
{
        show_bytes2((byte_pointer)&x,sizeof(int));
}
void show_int(FILE *f,int x)
{
	show_bytes(f,(byte_pointer)&x,sizeof(int));
}
void show_long(FILE *f,long x)
{
        show_bytes(f,(byte_pointer)&x,sizeof(long));
}
void show_long2(long x)
{
        show_bytes2((byte_pointer)&x,sizeof(long));
}

void show_float(FILE *f,float x)
{
	show_bytes(f,(byte_pointer)&x,sizeof(float));
}
void show_double(FILE *f,double x)
{
	show_bytes(f,(byte_pointer)&x,sizeof(double));
}
void show_pointer(FILE *f,void *x)
{
	show_bytes(f,(byte_pointer)&x,sizeof(void *));
}
