#include <stdio.h>
#include <math.h>

double exp_gen(double);

int main(int argc, char *argv[]) {
    double x, result;

    x = -11;
    result = exp_gen(x);

    printf("%e\n", result);

    return 0;
}
