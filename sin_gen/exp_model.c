#include <stdio.h>
#include <math.h>

double exp_gen(double x) {
    double temp;
    int k;
    double k1;
    double T;
    long int T_int;
    double r;
    double coefficient[10] = {0};
    double r_poly;
    int hi, lo;
    double r_coefficient;
    double result;


    k = 1 << 6;
    k1 = k / log(2);
    T = x * k1;
    T_int = T;
    // 0 <= r1 <= 1/k1
    r = x - ((double)T_int)/k1;

    coefficient[0] = 1;
    coefficient[1] = 1;
    coefficient[2] = 1.0/2.0;
    coefficient[3] = 1.0/6.0;
    coefficient[4] = 1.0/24.0;
    coefficient[5] = 1.0/120.0;
    r_poly = coefficient[0] + r * (coefficient[1] + r * (coefficient[2] + r * (coefficient[3] + r * (coefficient[4] + r * (coefficient[5] + 0)))));

    lo = T_int % k;
    hi = T_int / k;
    r_coefficient = pow(2, (double)hi) * pow(2, ((double)lo / k));
    result = r_coefficient * r_poly;

    printf("lo = %d, hi = %d, r = %f, r_coefficient = %f, r_poly = %f\n", lo, hi, r, r_coefficient, r_poly);
    return result;
}

int main(int argc, char *argv[]) {
    double x, result;

    x = -11;
    result = exp_gen(x);

    printf("%f\n", result);

    return 0;
}
