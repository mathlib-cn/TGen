#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "myhead.h"

int main(int argc, char *argv[]) {
	int sum, max;
	if (argc != 2) {
		printf("please input 2 arguements!\n");
		return 0;
	}
	sum = itod(argv[1]);
	max = itod(argv[2]);

	printf("");

	return 0;
}