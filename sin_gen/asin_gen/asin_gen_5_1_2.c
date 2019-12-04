#include <math.h>
#include "myhead.h"

#define BIT 5
#define TABLE_NUM 32
#define DEGREE 5
#define COEFFICIENTS_NUM 3

// static const DL Twopm28 = { .l = 0x3e30000000000000 };

// P = fpminimax(asin(x), [| 1, 3, 5| ], [| D... | ], [0, sqrt(1 - (31 / 32)^2)]); printexpansion(P);
static const DL
C[COEFFICIENTS_NUM] = {
	{.l = 0x3ff000005e848e02},
	{.l = 0x3fc551feed49b88d},
	{.l = 0x3fb44f558c7eb33d},
};

/* asin value: 0~1, step = 1/128 */
static const DL
asin_tab[TABLE_NUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3fa000aabde0b9c8},
	{.l = 0x3fb002abde953619},
	{.l = 0x3fb809092913e52e},
	{.l = 0x3fc00abe0c129e1e},
	{.l = 0x3fc41510cb011423},
	{.l = 0x3fc82494ed0e78fc},
	{.l = 0x3fcc3a6f13aae84b},
	{.l = 0x3fd02be9ce0b87cd},
	{.l = 0x3fd23f0523c5dc2b},
	{.l = 0x3fd457393b90e2aa},
	{.l = 0x3fd675441329986e},
	{.l = 0x3fd899f4edc962d3},
	{.l = 0x3fdac62fec0b2a92},
	{.l = 0x3fdcfaf27460fe9f},
	{.l = 0x3fdf3958aecddef4},
	{.l = 0x3fe0c152382d7366},
	{.l = 0x3fe1ec230c714a96},
	{.l = 0x3fe31df40fbd31cd},
	{.l = 0x3fe457bf318fe517},
	{.l = 0x3fe59aad71ced00f},
	{.l = 0x3fe6e825383cc40b},
	{.l = 0x3fe841deb5114bb4},
	{.l = 0x3fe9aa01babef75e},
	{.l = 0x3feb235315c680dc},
	{.l = 0x3fecb17c849c7288},
	{.l = 0x3fee5985567b665d},
	{.l = 0x3ff0115591d29d12},
	{.l = 0x3ff10c066d3e6932},
	{.l = 0x3ff2263461820ad8},
	{.l = 0x3ff3722d2feb24c8},
	{.l = 0x3ff51f4bd13f8591},
};

/* cos(y) = sqrt(1-[x]^2) value: 0~1, step = 1/128 */
static const DL
sk[TABLE_NUM] = 
{
	{.l = 0x0000000000000000},
	{.l = 0x3fa0000000000000},
	{.l = 0x3fb0000000000000},
	{.l = 0x3fb8000000000000},
	{.l = 0x3fc0000000000000},
	{.l = 0x3fc4000000000000},
	{.l = 0x3fc8000000000000},
	{.l = 0x3fcc000000000000},
	{.l = 0x3fd0000000000000},
	{.l = 0x3fd2000000000000},
	{.l = 0x3fd4000000000000},
	{.l = 0x3fd6000000000000},
	{.l = 0x3fd8000000000000},
	{.l = 0x3fda000000000000},
	{.l = 0x3fdc000000000000},
	{.l = 0x3fde000000000000},
	{.l = 0x3fe0000000000000},
	{.l = 0x3fe1000000000000},
	{.l = 0x3fe2000000000000},
	{.l = 0x3fe3000000000000},
	{.l = 0x3fe4000000000000},
	{.l = 0x3fe5000000000000},
	{.l = 0x3fe6000000000000},
	{.l = 0x3fe7000000000000},
	{.l = 0x3fe8000000000000},
	{.l = 0x3fe9000000000000},
	{.l = 0x3fea000000000000},
	{.l = 0x3feb000000000000},
	{.l = 0x3fec000000000000},
	{.l = 0x3fed000000000000},
	{.l = 0x3fee000000000000},
	{.l = 0x3fef000000000000},
};

/* sin(y) = [x] value: 0~1, step = 1/128 */
static const DL
ck[TABLE_NUM] = 
{
	{.l = 0x3ff0000000000000},
	{.l = 0x3feffbffbff7fec0},
	{.l = 0x3fefeffbfdfebf1f},
	{.l = 0x3fefdbeba917c3f5},
	{.l = 0x3fefbfbf7ebc755f},
	{.l = 0x3fef9b61d0237250},
	{.l = 0x3fef6eb62d27730d},
	{.l = 0x3fef3998f1b1886c},
	{.l = 0x3feefbdeb14f4eda},
	{.l = 0x3feeb5537b1434da},
	{.l = 0x3fee65b9edeba38e},
	{.l = 0x3fee0cca12e97895},
	{.l = 0x3fedaa2fefaae1d8},
	{.l = 0x3fed3d89be176072},
	{.l = 0x3fecc665b0328622},
	{.l = 0x3fec443f1d4d22af},
	{.l = 0x3febb67ae8584caa},
	{.l = 0x3feb1c62db2564fe},
	{.l = 0x3fea751f9447b724},
	{.l = 0x3fe9bfb076d236eb},
	{.l = 0x3fe8fae0c15ad38a},
	{.l = 0x3fe8253878ae2e09},
	{.l = 0x3fe73ce704fb7b23},
	{.l = 0x3fe63fa3f3c02962},
	{.l = 0x3fe52a7fa9d2f8ea},
	{.l = 0x3fe3f9989320b7f7},
	{.l = 0x3fe2a79e3a2cd2e6},
	{.l = 0x3fe12cf1c3c6a213},
	{.l = 0x3fdefbdeb14f4eda},
	{.l = 0x3fdb0e35269b38f5},
	{.l = 0x3fd645640568c1c3},
	{.l = 0x3fcfbfbf7ebc755f},
};

double asin_gen(double x) {
	double result, temp, delta, zsq;
	int order;

	temp = x * (double)TABLE_NUM;
	order = temp;
	
	// delta = x * sqrt(1 - temp * temp) - temp * sqrt(1 - x * x);
	delta = x * ck[order].d - sk[order].d * sqrt(1.0 - x + x * (1.0 - x));

	x = delta;
	// if (delta < Twopm28.d)
	//	x = 0.0;
	zsq = x * x;

	result = C[0].d * delta + (delta * zsq) * (C[1].d + zsq * (C[2].d));

	result = asin_tab[order].d + result;

	return result;
}
