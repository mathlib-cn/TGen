#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int a1, a2, a3, a4;
	a1 = atoi(argv[1]);
	a2 = atoi(argv[2]);
	a3 = atoi(argv[3]);
	a4 = a1 + a2 + a3;
	printf("%d\n", a1);
	printf("%d\n", a2);
	printf("%d\n", a3);
	//printf("%d %d %d %d\n", a1, a2, a3, a4);
}
