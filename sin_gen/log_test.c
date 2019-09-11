#include <stdio.h>
#include <math.h>

double log_gen(double);

int main(int argc, char *argv[]) {
    double x, result;

    x = 1.5;
    result = log_gen(x);

    printf("%e\n", result);

    return 0;
}
