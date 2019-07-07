#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN 255
#define RUN_COUNT 1000
#define PERCENT 0.1
int main(int argc,char *argv[])
{
	int flag=0,count=0,row=0,base=0,i=0;
	long cyle1=0;
	double perf_point=0.0;
	FILE * data1;
        long time_base[RUN_COUNT];
        long time_base_sum=0;
	int start,end;
	
	data1=fopen(argv[1],"r");
	
	if(!data1)
	{
		printf("Error in opening file.\r\n");
		exit(1);
	}
	
	row = 0;    
	while(fscanf(data1,"%ld\n",&time_base[row])!=EOF)    row ++;		
        
	start=row * PERCENT;	
	end=row - start;

	for(i=start;i< end;i++) {   time_base_sum += time_base[i];}	

	cyle1=time_base_sum/(end - start);
	
	printf("%ld\n",cyle1);
	
	fclose(data1);	
}
