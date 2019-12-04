#include <math.h>
#include "myhead.h"

#define BIT 6
#define TABLE_NUM 64
#define DEGREE 5
#define COEFFICIENTS_NUM 3

// static const DL Twopm28 = { .l = 0x3e30000000000000 };

// P = fpminimax(asin(x), [| 1, 3, 5| ], [| D... | ], [0, sqrt(1 - (63 / 64)^2)]); printexpansion(P);
static const DL
C[COEFFICIENTS_NUM] = {
	{.l = 0x3ff000000b9e8c80},
	{.l = 0x3fc55483ba69ef45},
	{.l = 0x3fb3bec52976836d},
};

/* asin value: 0~1, step = 1/128 */
static const DL
asin_tab[TABLE_NUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3f90002aabdde94c},
	{.l = 0x3fa000aabde0b9c8},
	{.l = 0x3fa8024091fdb0a9},
	{.l = 0x3fb002abde953619},
	{.l = 0x3fb405390240e6fd},
	{.l = 0x3fb809092913e52e},
	{.l = 0x3fbc0e5e80f7172d},
	{.l = 0x3fc00abe0c129e1e},
	{.l = 0x3fc20f530308cc20},
	{.l = 0x3fc41510cb011423},
	{.l = 0x3fc61c1ab9d55d30},
	{.l = 0x3fc82494ed0e78fc},
	{.l = 0x3fca2ea462b4998e},
	{.l = 0x3fcc3a6f13aae84b},
	{.l = 0x3fce481c0fce7134},
	{.l = 0x3fd02be9ce0b87cd},
	{.l = 0x3fd134dfa9805147},
	{.l = 0x3fd23f0523c5dc2b},
	{.l = 0x3fd34a709597aab1},
	{.l = 0x3fd457393b90e2aa},
	{.l = 0x3fd565774cb66f02},
	{.l = 0x3fd675441329986e},
	{.l = 0x3fd786ba074fef93},
	{.l = 0x3fd899f4edc962d3},
	{.l = 0x3fd9af11f89ba61c},
	{.l = 0x3fdac62fec0b2a92},
	{.l = 0x3fdbdf6f47ae6904},
	{.l = 0x3fdcfaf27460fe9f},
	{.l = 0x3fde18ddf7da106b},
	{.l = 0x3fdf3958aecddef4},
	{.l = 0x3fe02e46075785a1},
	{.l = 0x3fe0c152382d7366},
	{.l = 0x3fe155e8b2a00052},
	{.l = 0x3fe1ec230c714a96},
	{.l = 0x3fe2841ce0862975},
	{.l = 0x3fe31df40fbd31cd},
	{.l = 0x3fe3b9c90c43296d},
	{.l = 0x3fe457bf318fe517},
	{.l = 0x3fe4f7fd2bc2fb34},
	{.l = 0x3fe59aad71ced00f},
	{.l = 0x3fe63ffed6d198f6},
	{.l = 0x3fe6e825383cc40b},
	{.l = 0x3fe7935a501afa78},
	{.l = 0x3fe841deb5114bb4},
	{.l = 0x3fe8f3fb14e496b4},
	{.l = 0x3fe9aa01babef75e},
	{.l = 0x3fea645078c6a78c},
	{.l = 0x3feb235315c680dc},
	{.l = 0x3febe7866d3b6481},
	{.l = 0x3fecb17c849c7288},
	{.l = 0x3fed81e1f875ea8d},
	{.l = 0x3fee5985567b665d},
	{.l = 0x3fef39614cbef7d4},
	{.l = 0x3ff0115591d29d12},
	{.l = 0x3ff08b73f9af1058},
	{.l = 0x3ff10c066d3e6932},
	{.l = 0x3ff194590de7e7f6},
	{.l = 0x3ff2263461820ad8},
	{.l = 0x3ff2c42a3a3c7a87},
	{.l = 0x3ff3722d2feb24c8},
	{.l = 0x3ff436e4418e69c0},
	{.l = 0x3ff51f4bd13f8591},
	{.l = 0x3ff64cf55148366f},
};

/* cos(y) = sqrt(1-[x]^2) value: 0~1, step = 1/128 */
static const DL
sk[TABLE_NUM] = 
{
	{.l = 0x0000000000000000},
	{.l = 0x3f90000000000000},
	{.l = 0x3fa0000000000000},
	{.l = 0x3fa8000000000000},
	{.l = 0x3fb0000000000000},
	{.l = 0x3fb4000000000000},
	{.l = 0x3fb8000000000000},
	{.l = 0x3fbc000000000000},
	{.l = 0x3fc0000000000000},
	{.l = 0x3fc2000000000000},
	{.l = 0x3fc4000000000000},
	{.l = 0x3fc6000000000000},
	{.l = 0x3fc8000000000000},
	{.l = 0x3fca000000000000},
	{.l = 0x3fcc000000000000},
	{.l = 0x3fce000000000000},
	{.l = 0x3fd0000000000000},
	{.l = 0x3fd1000000000000},
	{.l = 0x3fd2000000000000},
	{.l = 0x3fd3000000000000},
	{.l = 0x3fd4000000000000},
	{.l = 0x3fd5000000000000},
	{.l = 0x3fd6000000000000},
	{.l = 0x3fd7000000000000},
	{.l = 0x3fd8000000000000},
	{.l = 0x3fd9000000000000},
	{.l = 0x3fda000000000000},
	{.l = 0x3fdb000000000000},
	{.l = 0x3fdc000000000000},
	{.l = 0x3fdd000000000000},
	{.l = 0x3fde000000000000},
	{.l = 0x3fdf000000000000},
	{.l = 0x3fe0000000000000},
	{.l = 0x3fe0800000000000},
	{.l = 0x3fe1000000000000},
	{.l = 0x3fe1800000000000},
	{.l = 0x3fe2000000000000},
	{.l = 0x3fe2800000000000},
	{.l = 0x3fe3000000000000},
	{.l = 0x3fe3800000000000},
	{.l = 0x3fe4000000000000},
	{.l = 0x3fe4800000000000},
	{.l = 0x3fe5000000000000},
	{.l = 0x3fe5800000000000},
	{.l = 0x3fe6000000000000},
	{.l = 0x3fe6800000000000},
	{.l = 0x3fe7000000000000},
	{.l = 0x3fe7800000000000},
	{.l = 0x3fe8000000000000},
	{.l = 0x3fe8800000000000},
	{.l = 0x3fe9000000000000},
	{.l = 0x3fe9800000000000},
	{.l = 0x3fea000000000000},
	{.l = 0x3fea800000000000},
	{.l = 0x3feb000000000000},
	{.l = 0x3feb800000000000},
	{.l = 0x3fec000000000000},
	{.l = 0x3fec800000000000},
	{.l = 0x3fed000000000000},
	{.l = 0x3fed800000000000},
	{.l = 0x3fee000000000000},
	{.l = 0x3fee800000000000},
	{.l = 0x3fef000000000000},
	{.l = 0x3fef800000000000},
};

/* sin(y) = [x] value: 0~1, step = 1/128 */
static const DL
ck[TABLE_NUM] = 
{
	{.l = 0x3ff0000000000000},
	{.l = 0x3feffefffbffdfff},
	{.l = 0x3feffbffbff7fec0},
	{.l = 0x3feff6febba4bfea},
	{.l = 0x3fefeffbfdfebf1f},
	{.l = 0x3fefe6f634576477},
	{.l = 0x3fefdbeba917c3f5},
	{.l = 0x3fefceda421efdb5},
	{.l = 0x3fefbfbf7ebc755f},
	{.l = 0x3fefae987541497f},
	{.l = 0x3fef9b61d0237250},
	{.l = 0x3fef8617caabd6f6},
	{.l = 0x3fef6eb62d27730d},
	{.l = 0x3fef553848924e81},
	{.l = 0x3fef3998f1b1886c},
	{.l = 0x3fef1bd27b9002c4},
	{.l = 0x3feefbdeb14f4eda},
	{.l = 0x3feed9b6cf3c4663},
	{.l = 0x3feeb5537b1434da},
	{.l = 0x3fee8eacbb648910},
	{.l = 0x3fee65b9edeba38e},
	{.l = 0x3fee3a71bcdd63de},
	{.l = 0x3fee0cca12e97895},
	{.l = 0x3feddcb80ddc085b},
	{.l = 0x3fedaa2fefaae1d8},
	{.l = 0x3fed75250db9c792},
	{.l = 0x3fed3d89be176072},
	{.l = 0x3fed034f42698214},
	{.l = 0x3fecc665b0328622},
	{.l = 0x3fec86bbd609a260},
	{.l = 0x3fec443f1d4d22af},
	{.l = 0x3febfedb67be13b3},
	{.l = 0x3febb67ae8584caa},
	{.l = 0x3feb6b05f6966b9b},
	{.l = 0x3feb1c62db2564fe},
	{.l = 0x3feaca7594d44cbd},
	{.l = 0x3fea751f9447b724},
	{.l = 0x3fea1c3f6ca01f29},
	{.l = 0x3fe9bfb076d236eb},
	{.l = 0x3fe95f4a64decda8},
	{.l = 0x3fe8fae0c15ad38a},
	{.l = 0x3fe8924256bf4545},
	{.l = 0x3fe8253878ae2e09},
	{.l = 0x3fe7b386279d7bf3},
	{.l = 0x3fe73ce704fb7b23},
	{.l = 0x3fe6c10e0a9e5d65},
	{.l = 0x3fe63fa3f3c02962},
	{.l = 0x3fe5b8453f4ae294},
	{.l = 0x3fe52a7fa9d2f8ea},
	{.l = 0x3fe495ceefbdc28a},
	{.l = 0x3fe3f9989320b7f7},
	{.l = 0x3fe355264082fea0},
	{.l = 0x3fe2a79e3a2cd2e6},
	{.l = 0x3fe1eff8dd34fde3},
	{.l = 0x3fe12cf1c3c6a213},
	{.l = 0x3fe05cf20924c254},
	{.l = 0x3fdefbdeb14f4eda},
	{.l = 0x3fdd1a6f89821641},
	{.l = 0x3fdb0e35269b38f5},
	{.l = 0x3fd8cc97f2912db6},
	{.l = 0x3fd645640568c1c3},
	{.l = 0x3fd35d6b2ed19148},
	{.l = 0x3fcfbfbf7ebc755f},
	{.l = 0x3fc689f26c6b01d0},
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
