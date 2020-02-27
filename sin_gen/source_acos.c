#include <stdio.h>
#include <stdlib.h>
#include "myhead.h"

#define BIT 7
#define TABLE_NUM 128
#define TABLE_NUM_1 127
#define DEGREE 15
#define COEFFICIENTS_NUM 8
#define COEFFICIENTS_VERSION 8
#define	FORMAT 64

struct constraint {
	double start;
	double end;
	int precision;
	int bit;
	int fnum;
	int degree;
};

// static const DL Twopm28 = { .l = 0x3e30000000000000 };

// P = fpminimax(asin(x), [| 1, 3, 5, 7, 9, 11, 13, 15| ], [| D... | ], [0, sqrt(1 - (127 / 128)*(127 / 128))]); printexpansion(P);
// x * (0x3ff0000000000000 + x ^ 2 * (0x3fc5555555555556 + x ^ 2 * (0x3fb3333333332cb8 + x ^ 2 * (0x3fa6db6db6f2a213 + x ^ 2 * (0x3f9f1c719dc96a96 + x ^ 2 * (0x3f96e8ce755ff3f7 + x ^ 2 * (0x3f91bf59a4e08f79 + x ^ 2 * 0x3f8e340c0c874f30)))))))
static const DL
C[COEFFICIENTS_NUM] = {
	{.l = 0x3ff0000000000000},	// 1
	{.l = 0x3fc5555555555556},	// 3
	{.l = 0x3fb3333333332cb8},	// 5
	{.l = 0x3fa6db6db6f2a213},	// 7
	{.l = 0x3f9f1c719dc96a96},	// 9
	{.l = 0x3f96e8ce755ff3f7},	// 11
	{.l = 0x3f91bf59a4e08f79},	// 13
	{.l = 0x3f8e340c0c874f30},	// 15
};

/* asin value: 0~1, step = 1/128 */
static const DL
asin_tab[TABLE_NUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3f80000aaabdde0c},
	{.l = 0x3f90002aabdde94c},
	{.l = 0x3f980090091d9024},
	{.l = 0x3fa000aabde0b9c8},
	{.l = 0x3fa4014d8ffaf8af},
	{.l = 0x3fa8024091fdb0a9},
	{.l = 0x3fac0393e65c2c93},
	{.l = 0x3fb002abde953619},
	{.l = 0x3fb203ce2b380cd3},
	{.l = 0x3fb405390240e6fd},
	{.l = 0x3fb606f49730ccc5},
	{.l = 0x3fb809092913e52e},
	{.l = 0x3fba0b7f03ba78ac},
	{.l = 0x3fbc0e5e80f7172d},
	{.l = 0x3fbe11b009e269b5},
	{.l = 0x3fc00abe0c129e1e},
	{.l = 0x3fc10ce59ba4a8c4},
	{.l = 0x3fc20f530308cc20},
	{.l = 0x3fc3120a9bed2f46},
	{.l = 0x3fc41510cb011423},
	{.l = 0x3fc5186a00ade974},
	{.l = 0x3fc61c1ab9d55d30},
	{.l = 0x3fc720278094cd3c},
	{.l = 0x3fc82494ed0e78fc},
	{.l = 0x3fc92967a638db38},
	{.l = 0x3fca2ea462b4998e},
	{.l = 0x3fcb344fe9a97c4d},
	{.l = 0x3fcc3a6f13aae84b},
	{.l = 0x3fcd4106cba45b08},
	{.l = 0x3fce481c0fce7134},
	{.l = 0x3fcf4fb3f2ad079b},
	{.l = 0x3fd02be9ce0b87cd},
	{.l = 0x3fd0b04025245ccc},
	{.l = 0x3fd134dfa9805147},
	{.l = 0x3fd1b9cb12545e62},
	{.l = 0x3fd23f0523c5dc2b},
	{.l = 0x3fd2c490af8bde81},
	{.l = 0x3fd34a709597aab1},
	{.l = 0x3fd3d0a7c4c4bd9c},
	{.l = 0x3fd457393b90e2aa},
	{.l = 0x3fd4de2808dce513},
	{.l = 0x3fd565774cb66f02},
	{.l = 0x3fd5ed2a392bb50f},
	{.l = 0x3fd675441329986e},
	{.l = 0x3fd6fdc83364f719},
	{.l = 0x3fd786ba074fef93},
	{.l = 0x3fd8101d121bed2d},
	{.l = 0x3fd899f4edc962d3},
	{.l = 0x3fd924454c462cc4},
	{.l = 0x3fd9af11f89ba61c},
	{.l = 0x3fda3a5ed82d9537},
	{.l = 0x3fdac62fec0b2a92},
	{.l = 0x3fdb5289525368ab},
	{.l = 0x3fdbdf6f47ae6904},
	{.l = 0x3fdc6ce628dd132c},
	{.l = 0x3fdcfaf27460fe9f},
	{.l = 0x3fdd8998cc3e6049},
	{.l = 0x3fde18ddf7da106b},
	{.l = 0x3fdea8c6e5f5e67f},
	{.l = 0x3fdf3958aecddef4},
	{.l = 0x3fdfca989658baaf},
	{.l = 0x3fe02e46075785a1},
	{.l = 0x3fe0779c5d4df4b8},
	{.l = 0x3fe0c152382d7366},
	{.l = 0x3fe10b6a9e43942f},
	{.l = 0x3fe155e8b2a00052},
	{.l = 0x3fe1a0cfb6c3e9eb},
	{.l = 0x3fe1ec230c714a96},
	{.l = 0x3fe237e6379cdfc7},
	{.l = 0x3fe2841ce0862975},
	{.l = 0x3fe2d0cad5f90e20},
	{.l = 0x3fe31df40fbd31cd},
	{.l = 0x3fe36b9cb13786e1},
	{.l = 0x3fe3b9c90c43296d},
	{.l = 0x3fe4087da4473296},
	{.l = 0x3fe457bf318fe517},
	{.l = 0x3fe4a792a4f26152},
	{.l = 0x3fe4f7fd2bc2fb34},
	{.l = 0x3fe5490434275b92},
	{.l = 0x3fe59aad71ced00f},
	{.l = 0x3fe5ecfee31c96e7},
	{.l = 0x3fe63ffed6d198f6},
	{.l = 0x3fe693b3f244ee17},
	{.l = 0x3fe6e825383cc40b},
	{.l = 0x3fe73d5a107bde74},
	{.l = 0x3fe7935a501afa78},
	{.l = 0x3fe7ea2e42c9027a},
	{.l = 0x3fe841deb5114bb4},
	{.l = 0x3fe89a74ffcc34a4},
	{.l = 0x3fe8f3fb14e496b4},
	{.l = 0x3fe94e7b8da3cf7a},
	{.l = 0x3fe9aa01babef75e},
	{.l = 0x3fea0699b66a8718},
	{.l = 0x3fea645078c6a78c},
	{.l = 0x3feac333ef06451a},
	{.l = 0x3feb235315c680dc},
	{.l = 0x3feb84be172438ef},
	{.l = 0x3febe7866d3b6481},
	{.l = 0x3fec4bbf09e19830},
	{.l = 0x3fecb17c849c7288},
	{.l = 0x3fed18d55010f295},
	{.l = 0x3fed81e1f875ea8d},
	{.l = 0x3fedecbd6cf77786},
	{.l = 0x3fee5985567b665d},
	{.l = 0x3feec85a7ee191da},
	{.l = 0x3fef39614cbef7d4},
	{.l = 0x3fefacc258c4aaf9},
	{.l = 0x3ff0115591d29d12},
	{.l = 0x3ff04da77ac5c9e5},
	{.l = 0x3ff08b73f9af1058},
	{.l = 0x3ff0cadc3d4378b1},
	{.l = 0x3ff10c066d3e6932},
	{.l = 0x3ff14f1ec67484ed},
	{.l = 0x3ff194590de7e7f6},
	{.l = 0x3ff1dbf27dd2221a},
	{.l = 0x3ff2263461820ad8},
	{.l = 0x3ff27377b2570a1e},
	{.l = 0x3ff2c42a3a3c7a87},
	{.l = 0x3ff318d619008ed9},
	{.l = 0x3ff3722d2feb24c8},
	{.l = 0x3ff3d11b3fc3b697},
	{.l = 0x3ff436e4418e69c0},
	{.l = 0x3ff4a55ae332c7a5},
	{.l = 0x3ff51f4bd13f8591},
	{.l = 0x3ff5a96e34bc532b},
	{.l = 0x3ff64cf55148366f},
	{.l = 0x3ff721a5d8718655}
};

/* cos(y) = sqrt(1-[x]^2) value: 0~1, step = 1/128 */
static const DL
sk[TABLE_NUM] = {
	{.l = 0x0000000000000000},
	{.l = 0x3f80000000000000},
	{.l = 0x3f90000000000000},
	{.l = 0x3f98000000000000},
	{.l = 0x3fa0000000000000},
	{.l = 0x3fa4000000000000},
	{.l = 0x3fa8000000000000},
	{.l = 0x3fac000000000000},
	{.l = 0x3fb0000000000000},
	{.l = 0x3fb2000000000000},
	{.l = 0x3fb4000000000000},
	{.l = 0x3fb6000000000000},
	{.l = 0x3fb8000000000000},
	{.l = 0x3fba000000000000},
	{.l = 0x3fbc000000000000},
	{.l = 0x3fbe000000000000},
	{.l = 0x3fc0000000000000},
	{.l = 0x3fc1000000000000},
	{.l = 0x3fc2000000000000},
	{.l = 0x3fc3000000000000},
	{.l = 0x3fc4000000000000},
	{.l = 0x3fc5000000000000},
	{.l = 0x3fc6000000000000},
	{.l = 0x3fc7000000000000},
	{.l = 0x3fc8000000000000},
	{.l = 0x3fc9000000000000},
	{.l = 0x3fca000000000000},
	{.l = 0x3fcb000000000000},
	{.l = 0x3fcc000000000000},
	{.l = 0x3fcd000000000000},
	{.l = 0x3fce000000000000},
	{.l = 0x3fcf000000000000},
	{.l = 0x3fd0000000000000},
	{.l = 0x3fd0800000000000},
	{.l = 0x3fd1000000000000},
	{.l = 0x3fd1800000000000},
	{.l = 0x3fd2000000000000},
	{.l = 0x3fd2800000000000},
	{.l = 0x3fd3000000000000},
	{.l = 0x3fd3800000000000},
	{.l = 0x3fd4000000000000},
	{.l = 0x3fd4800000000000},
	{.l = 0x3fd5000000000000},
	{.l = 0x3fd5800000000000},
	{.l = 0x3fd6000000000000},
	{.l = 0x3fd6800000000000},
	{.l = 0x3fd7000000000000},
	{.l = 0x3fd7800000000000},
	{.l = 0x3fd8000000000000},
	{.l = 0x3fd8800000000000},
	{.l = 0x3fd9000000000000},
	{.l = 0x3fd9800000000000},
	{.l = 0x3fda000000000000},
	{.l = 0x3fda800000000000},
	{.l = 0x3fdb000000000000},
	{.l = 0x3fdb800000000000},
	{.l = 0x3fdc000000000000},
	{.l = 0x3fdc800000000000},
	{.l = 0x3fdd000000000000},
	{.l = 0x3fdd800000000000},
	{.l = 0x3fde000000000000},
	{.l = 0x3fde800000000000},
	{.l = 0x3fdf000000000000},
	{.l = 0x3fdf800000000000},
	{.l = 0x3fe0000000000000},
	{.l = 0x3fe0400000000000},
	{.l = 0x3fe0800000000000},
	{.l = 0x3fe0c00000000000},
	{.l = 0x3fe1000000000000},
	{.l = 0x3fe1400000000000},
	{.l = 0x3fe1800000000000},
	{.l = 0x3fe1c00000000000},
	{.l = 0x3fe2000000000000},
	{.l = 0x3fe2400000000000},
	{.l = 0x3fe2800000000000},
	{.l = 0x3fe2c00000000000},
	{.l = 0x3fe3000000000000},
	{.l = 0x3fe3400000000000},
	{.l = 0x3fe3800000000000},
	{.l = 0x3fe3c00000000000},
	{.l = 0x3fe4000000000000},
	{.l = 0x3fe4400000000000},
	{.l = 0x3fe4800000000000},
	{.l = 0x3fe4c00000000000},
	{.l = 0x3fe5000000000000},
	{.l = 0x3fe5400000000000},
	{.l = 0x3fe5800000000000},
	{.l = 0x3fe5c00000000000},
	{.l = 0x3fe6000000000000},
	{.l = 0x3fe6400000000000},
	{.l = 0x3fe6800000000000},
	{.l = 0x3fe6c00000000000},
	{.l = 0x3fe7000000000000},
	{.l = 0x3fe7400000000000},
	{.l = 0x3fe7800000000000},
	{.l = 0x3fe7c00000000000},
	{.l = 0x3fe8000000000000},
	{.l = 0x3fe8400000000000},
	{.l = 0x3fe8800000000000},
	{.l = 0x3fe8c00000000000},
	{.l = 0x3fe9000000000000},
	{.l = 0x3fe9400000000000},
	{.l = 0x3fe9800000000000},
	{.l = 0x3fe9c00000000000},
	{.l = 0x3fea000000000000},
	{.l = 0x3fea400000000000},
	{.l = 0x3fea800000000000},
	{.l = 0x3feac00000000000},
	{.l = 0x3feb000000000000},
	{.l = 0x3feb400000000000},
	{.l = 0x3feb800000000000},
	{.l = 0x3febc00000000000},
	{.l = 0x3fec000000000000},
	{.l = 0x3fec400000000000},
	{.l = 0x3fec800000000000},
	{.l = 0x3fecc00000000000},
	{.l = 0x3fed000000000000},
	{.l = 0x3fed400000000000},
	{.l = 0x3fed800000000000},
	{.l = 0x3fedc00000000000},
	{.l = 0x3fee000000000000},
	{.l = 0x3fee400000000000},
	{.l = 0x3fee800000000000},
	{.l = 0x3feec00000000000},
	{.l = 0x3fef000000000000},
	{.l = 0x3fef400000000000},
	{.l = 0x3fef800000000000},
	{.l = 0x3fefc00000000000}
};

/* sin(y) = [x] value: 0~1, step = 1/128 */
static const DL
ck[TABLE_NUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3fefffbfffbfff80},
	{.l = 0x3feffefffbffdfff},
	{.l = 0x3feffdbfebbe9360},
	{.l = 0x3feffbffbff7fec0},
	{.l = 0x3feff9bf63a1740b},
	{.l = 0x3feff6febba4bfea},
	{.l = 0x3feff3bda6d9c950},
	{.l = 0x3fefeffbfdfebf1f},
	{.l = 0x3fefebb993aecf99},
	{.l = 0x3fefe6f634576477},
	{.l = 0x3fefe1b1a62bddad},
	{.l = 0x3fefdbeba917c3f5},
	{.l = 0x3fefd5a3f6af6b74},
	{.l = 0x3fefceda421efdb5},
	{.l = 0x3fefc78e3817e16e},
	{.l = 0x3fefbfbf7ebc755f},
	{.l = 0x3fefb76db58a1299},
	{.l = 0x3fefae987541497f},
	{.l = 0x3fefa53f4fcc4b79},
	{.l = 0x3fef9b61d0237250},
	{.l = 0x3fef90ff7a2fd4d2},
	{.l = 0x3fef8617caabd6f6},
	{.l = 0x3fef7aaa3701a270},
	{.l = 0x3fef6eb62d27730d},
	{.l = 0x3fef623b1379a09b},
	{.l = 0x3fef553848924e81},
	{.l = 0x3fef47ad231ea746},
	{.l = 0x3fef3998f1b1886c},
	{.l = 0x3fef2afafa9380f9},
	{.l = 0x3fef1bd27b9002c4},
	{.l = 0x3fef0c1ea9bfa45f},
	{.l = 0x3feefbdeb14f4eda},
	{.l = 0x3feeeb11b5442ff1},
	{.l = 0x3feed9b6cf3c4663},
	{.l = 0x3feec7cd0f2b5ae0},
	{.l = 0x3feeb5537b1434da},
	{.l = 0x3feea2490ebdd6b8},
	{.l = 0x3fee8eacbb648910},
	{.l = 0x3fee7a7d6766784b},
	{.l = 0x3fee65b9edeba38e},
	{.l = 0x3fee50611e88d6b5},
	{.l = 0x3fee3a71bcdd63de},
	{.l = 0x3fee23ea802b4b1a},
	{.l = 0x3fee0cca12e97895},
	{.l = 0x3fedf50f124fba75},
	{.l = 0x3feddcb80ddc085b},
	{.l = 0x3fedc3c386d0ae09},
	{.l = 0x3fedaa2fefaae1d8},
	{.l = 0x3fed8ffbab9145d4},
	{.l = 0x3fed75250db9c792},
	{.l = 0x3fed59aa58c6471c},
	{.l = 0x3fed3d89be176072},
	{.l = 0x3fed20c15d14a4e5},
	{.l = 0x3fed034f42698214},
	{.l = 0x3fece5316736032e},
	{.l = 0x3fecc665b0328622},
	{.l = 0x3feca6e9ecc569b9},
	{.l = 0x3fec86bbd609a260},
	{.l = 0x3fec65d90dc509f4},
	{.l = 0x3fec443f1d4d22af},
	{.l = 0x3fec21eb7458e5cc},
	{.l = 0x3febfedb67be13b3},
	{.l = 0x3febdb0c30185485},
	{.l = 0x3febb67ae8584caa},
	{.l = 0x3feb91248c38986b},
	{.l = 0x3feb6b05f6966b9b},
	{.l = 0x3feb441bdfab5580},
	{.l = 0x3feb1c62db2564fe},
	{.l = 0x3feaf3d7561a9c43},
	{.l = 0x3feaca7594d44cbd},
	{.l = 0x3feaa039b06e926d},
	{.l = 0x3fea751f9447b724},
	{.l = 0x3fea4922fb3ac8c2},
	{.l = 0x3fea1c3f6ca01f29},
	{.l = 0x3fe9ee70390dec3d},
	{.l = 0x3fe9bfb076d236eb},
	{.l = 0x3fe98ffafe1ece2f},
	{.l = 0x3fe95f4a64decda8},
	{.l = 0x3fe92d98fa2c355e},
	{.l = 0x3fe8fae0c15ad38a},
	{.l = 0x3fe8c71b6c8c49b4},
	{.l = 0x3fe8924256bf4545},
	{.l = 0x3fe85c4e7d4a0bb1},
	{.l = 0x3fe8253878ae2e09},
	{.l = 0x3fe7ecf874b086df},
	{.l = 0x3fe7b386279d7bf3},
	{.l = 0x3fe778d8c89dc27c},
	{.l = 0x3fe73ce704fb7b23},
	{.l = 0x3fe6ffa6f4323c0d},
	{.l = 0x3fe6c10e0a9e5d65},
	{.l = 0x3fe681110a985d4d},
	{.l = 0x3fe63fa3f3c02962},
	{.l = 0x3fe5fcb9f031317b},
	{.l = 0x3fe5b8453f4ae294},
	{.l = 0x3fe572371da8f26e},
	{.l = 0x3fe52a7fa9d2f8ea},
	{.l = 0x3fe4e10dc51235a7},
	{.l = 0x3fe495ceefbdc28a},
	{.l = 0x3fe448af2027201c},
	{.l = 0x3fe3f9989320b7f7},
	{.l = 0x3fe3a87394da947b},
	{.l = 0x3fe355264082fea0},
	{.l = 0x3fe2ff9434b34639},
	{.l = 0x3fe2a79e3a2cd2e6},
	{.l = 0x3fe24d21d9bcbd2d},
	{.l = 0x3fe1eff8dd34fde3},
	{.l = 0x3fe18ff8b63353dc},
	{.l = 0x3fe12cf1c3c6a213},
	{.l = 0x3fe0c6ae6dbb479d},
	{.l = 0x3fe05cf20924c254},
	{.l = 0x3fdfdeeeea5d2084},
	{.l = 0x3fdefbdeb14f4eda},
	{.l = 0x3fde0ffbbe00c34f},
	{.l = 0x3fdd1a6f89821641},
	{.l = 0x3fdc1a3cd9862dfb},
	{.l = 0x3fdb0e35269b38f5},
	{.l = 0x3fd9f4e9f1b5859d},
	{.l = 0x3fd8cc97f2912db6},
	{.l = 0x3fd79308a1dd964a},
	{.l = 0x3fd645640568c1c3},
	{.l = 0x3fd4dfe7790ba696},
	{.l = 0x3fd35d6b2ed19148},
	{.l = 0x3fd1b6867c415166},
	{.l = 0x3fcfbfbf7ebc755f},
	{.l = 0x3fcb8cc9d3952a45},
	{.l = 0x3fc689f26c6b01d0},
	{.l = 0x3fbfeffbfdfebf1f}
};

// fpminmax: [0, sqrt(1 - (127/128)^2)]
static const DL
coefficient_128[COEFFICIENTS_NUM][COEFFICIENTS_NUM] = {

	// P = fpminimax(asin(x), [|1|], [|D...|], [0,sqrt(1 - (127/128)^2)]);
	// x * 0x3ff00557d3e5cfe7
	{
		{.l = 0x3ff00557d3e5cfe7 },
	},

	// P = fpminimax(asin(x), [|1,3|], [|D...|], [0,sqrt(1 - (127/128)^2)]);
	// x * (0x3feffffb2d3ba60e + x^2 * 0x3fc57bedc863328f)
	{
		{.l = 0x3feffffb2d3ba60e },
		{.l = 0x3fc57bedc863328f },
	},

	// P = fpminimax(asin(x), [|1,3,5|], [|D...|], [0,sqrt(1 - (127/128)^2)]);
	// x * (0x3ff000000170bf8a + x^2 * (0x3fc555216bf0fee3 + x^2 * 0x3fb3785fbd768012))
	{
		{.l = 0x3ff000000170bf8a },
		{.l = 0x3fc555216bf0fee3 },
		{.l = 0x3fb3785fbd768012 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7|], [|D...|], [0,sqrt(1 - (127/128)^2)]);
	// x * (0x3feffffffffe0849 + x^2 * (0x3fc5555594661fe2 + x^2 * (0x3fb3329568cfcdd5 + x^2 * 0x3fa7597098d28969)))
	{
		{.l = 0x3feffffffffe0849 },
		{.l = 0x3fc5555594661fe2 },
		{.l = 0x3fb3329568cfcdd5 },
		{.l = 0x3fa7597098d28969 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9|], [|D...|], [0,sqrt(1 - (127/128)^2)]);
	// x * (0x3ff00000000000ba + x^2 * (0x3fc55555550c7e34 + x^2 * (0x3fb33334570823a5 + x^2 * (0x3fa6d9d5033584ee + x^2 * 0x3fa002a6bcedbf65))))
	{
		{.l = 0x3ff00000000000ba },
		{.l = 0x3fc55555550c7e34 },
		{.l = 0x3fb33334570823a5 },
		{.l = 0x3fa6d9d5033584ee },
		{.l = 0x3fa002a6bcedbf65 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11|], [|D...|], [0,sqrt(1 - (127/128)^2)]);
	// x * (0x3fefffffffffffff + x^2 * (0x3fc555555555a461 + x^2 * (0x3fb33333315d28c4 + x^2 * (0x3fa6db71aa3983e1 + x^2 * (0x3f9f189eda601e33 + x^2 * 0x3f97c1e97df2475e)))))
	{
		{.l = 0x3fefffffffffffff },
		{.l = 0x3fc555555555a461 },
		{.l = 0x3fb33333315d28c4 },
		{.l = 0x3fa6db71aa3983e1 },
		{.l = 0x3f9f189eda601e33 },
		{.l = 0x3f97c1e97df2475e },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13|], [|D...|], [0,sqrt(1 - (127/128)^2)]);
	// x * (0x3ff0000000000000 + x^2 * (0x3fc5555555555515 + x^2 * (0x3fb3333333359d73 + x^2 * (0x3fa6db6daf15e830 + x^2 * (0x3f9f1c7d4c24b7e3 + x^2 * (0x3f96e469868b261e + x^2 * 0x3f928fb9dc1417d2))))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fc5555555555515 },
		{.l = 0x3fb3333333359d73 },
		{.l = 0x3fa6db6daf15e830 },
		{.l = 0x3f9f1c7d4c24b7e3 },
		{.l = 0x3f96e469868b261e },
		{.l = 0x3f928fb9dc1417d2 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13,15|], [|D...|], [0,sqrt(1 - (127/128)^2)]);
	// x * (0x3ff0000000000000 + x^2 * (0x3fc5555555555556 + x^2 * (0x3fb3333333332cb8 + x^2 * (0x3fa6db6db6f2a213 + x^2 * (0x3f9f1c719dc96a96 + x^2 * (0x3f96e8ce755ff3f7 + x^2 * (0x3f91bf59a4e08f79 + x^2 * 0x3f8e340c0c874f30)))))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fc5555555555556 },
		{.l = 0x3fb3333333332cb8 },
		{.l = 0x3fa6db6db6f2a213 },
		{.l = 0x3f9f1c719dc96a96 },
		{.l = 0x3f96e8ce755ff3f7 },
		{.l = 0x3f91bf59a4e08f79 },
		{.l = 0x3f8e340c0c874f30 },
	},
};


// fpminmax: [0, ln2/64]
static const DL
coefficient_64[COEFFICIENTS_NUM][COEFFICIENTS_NUM] = {

	// P = fpminimax(asin(x), [|1|], [|D...|], [0,sqrt(1 - (63/64)^2)]);
	// x * 0x3ff00ab4aa9676dd
	{
		{.l = 0x3ff00ab4aa9676dd },
	},

	// P = fpminimax(asin(x), [|1,3|], [|D...|], [0,sqrt(1 - (63/64)^2)]);
	// x * (0x3fefffec9cde92ba + x^2 * 0x3fc5a2eb38c39477)
	{
		{.l = 0x3fefffec9cde92ba },
		{.l = 0x3fc5a2eb38c39477 },
	},

	// P = fpminimax(asin(x), [|1,3,5|], [|D...|], [0,sqrt(1 - (63/64)^2)]);
	// x * (0x3ff000000b9e8c80 + x^2 * (0x3fc55483ba69ef45 + x^2 * 0x3fb3bec52976836d))
	{
		{.l = 0x3ff000000b9e8c80 },
		{.l = 0x3fc55483ba69ef45 },
		{.l = 0x3fb3bec52976836d },
	},

	// P = fpminimax(asin(x), [|1,3,5,7|], [|D...|], [0,sqrt(1 - (63/64)^2)]);
	// x * (0x3fefffffffe02404 + x^2 * (0x3fc5555754b99a8a + x^2 * (0x3fb330b2ed20828b + x^2 * 0x3fa7daa543a146e8)))
	{
		{.l = 0x3fefffffffe02404 },
		{.l = 0x3fc5555754b99a8a },
		{.l = 0x3fb330b2ed20828b },
		{.l = 0x3fa7daa543a146e8 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9|], [|D...|], [0,sqrt(1 - (63/64)^2)]);
	// x * (0x3ff00000000017a2 + x^2 * (0x3fc5555550b2fcee + x^2 * (0x3fb3333c7f1099dc + x^2 * (0x3fa6d4ead0cf51fe + x^2 * 0x3fa07af2d74014e7))))
	{
		{.l = 0x3ff00000000017a2 },
		{.l = 0x3fc5555550b2fcee },
		{.l = 0x3fb3333c7f1099dc },
		{.l = 0x3fa6d4ead0cf51fe },
		{.l = 0x3fa07af2d74014e7 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11|], [|D...|], [0,sqrt(1 - (63/64)^2)]);
	// x * (0x3fefffffffffffb6 + x^2 * (0x3fc55555555fc616 + x^2 * (0x3fb3333314a35eed + x^2 * (0x3fa6db8e6422adec + x^2 * (0x3f9f0cb2f4256fb4 + x^2 * 0x3f98a4d8dc01b991)))))
	{
		{.l = 0x3fefffffffffffb6 },
		{.l = 0x3fc55555555fc616 },
		{.l = 0x3fb3333314a35eed },
		{.l = 0x3fa6db8e6422adec },
		{.l = 0x3f9f0cb2f4256fb4 },
		{.l = 0x3f98a4d8dc01b991 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13|], [|D...|], [0,sqrt(1 - (63/64)^2)]);
	// x * (0x3ff0000000000000 + x^2 * (0x3fc55555555544e5 + x^2 * (0x3fb333333382785a + x^2 * (0x3fa6db6d36fc73e0 + x^2 * (0x3f9f1cd0af5d8a17 + x^2 * (0x3f96d6f5d47b511b + x^2 * 0x3f936453b215347a))))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fc55555555544e5 },
		{.l = 0x3fb333333382785a },
		{.l = 0x3fa6db6d36fc73e0 },
		{.l = 0x3f9f1cd0af5d8a17 },
		{.l = 0x3f96d6f5d47b511b },
		{.l = 0x3f936453b215347a },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13,15|], [|D...|], [0,sqrt(1 - (63/64)^2)]);
	// x * (0x3ff0000000000000 + x^2 * (0x3fc5555555555579 + x^2 * (0x3fb33333333251c1 + x^2 * (0x3fa6db6db8c030c4 + x^2 * (0x3f9f1c6fd80c3571 + x^2 * (0x3f96e9415097964f + x^2 * (0x3f91b0b76412c1ab + x^2 * 0x3f8fb38ec5e9e3a5)))))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fc5555555555579 },
		{.l = 0x3fb33333333251c1 },
		{.l = 0x3fa6db6db8c030c4 },
		{.l = 0x3f9f1c6fd80c3571 },
		{.l = 0x3f96e9415097964f },
		{.l = 0x3f91b0b76412c1ab },
		{.l = 0x3f8fb38ec5e9e3a5 },
	},
};

// fpminmax: [0, ln2/32]
static const DL
coefficient_32[COEFFICIENTS_NUM][COEFFICIENTS_NUM] = {

	// P = fpminimax(asin(x), [|1|], [|D...|], [0,sqrt(1 - (31/32)^2)]);
	// x * 0x3ff0157d82ab9e9d
	{
		{.l = 0x3ff0157d82ab9e9d },
	},

	// P = fpminimax(asin(x), [|1,3|], [|D...|], [0,sqrt(1 - (31/32)^2)]);
	// x * (0x3fefffb1b09b2826 + x^2 * 0x3fc5f21a802b2aa1)
	{
		{.l = 0x3fefffb1b09b2826 },
		{.l = 0x3fc5f21a802b2aa1 },
	},

	// P = fpminimax(asin(x), [|1,3,5|], [|D...|], [0,sqrt(1 - (31/32)^2)]);
	// x * (0x3ff000005e848e02 + x^2 * (0x3fc551feed49b88d + x^2 * 0x3fb44f558c7eb33d))
	{
		{.l = 0x3ff000005e848e02 },
		{.l = 0x3fc551feed49b88d },
		{.l = 0x3fb44f558c7eb33d },
	},

	// P = fpminimax(asin(x), [|1,3,5,7|], [|D...|], [0,sqrt(1 - (31/32)^2)]);
	// x * (0x3feffffffdf5e52f + x^2 * (0x3fc55565c1480f09 + x^2 * (0x3fb328e7191926af + x^2 * 0x3fa8e7046c27ed3e)))
	{
		{.l = 0x3feffffffdf5e52f },
		{.l = 0x3fc55565c1480f09 },
		{.l = 0x3fb328e7191926af },
		{.l = 0x3fa8e7046c27ed3e },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9|], [|D...|], [0,sqrt(1 - (31/32)^2)]);
	// x * (0x3ff0000000030c71 + x^2 * (0x3fc555550883cd00 + x^2 * (0x3fb333807c7040dc + x^2 * (0x3fa6c058291a8cc7 + x^2 * 0x3fa177b6fc1b265b))))
	{
		{.l = 0x3ff0000000030c71 },
		{.l = 0x3fc555550883cd00 },
		{.l = 0x3fb333807c7040dc },
		{.l = 0x3fa6c058291a8cc7 },
		{.l = 0x3fa177b6fc1b265b },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11|], [|D...|], [0,sqrt(1 - (31/32)^2)]);
	// x * (0x3fefffffffffeccc + x^2 * (0x3fc5555556b21ca1 + x^2 * (0x3fb333313297cf2c + x^2 * (0x3fa6dc807bb37177 + x^2 * (0x3f9eda4548b35712 + x^2 * 0x3f9a866666c1cdee)))))
	{
		{.l = 0x3fefffffffffeccc },
		{.l = 0x3fc5555556b21ca1 },
		{.l = 0x3fb333313297cf2c },
		{.l = 0x3fa6dc807bb37177 },
		{.l = 0x3f9eda4548b35712 },
		{.l = 0x3f9a866666c1cdee },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13|], [|D...|], [0,sqrt(1 - (31/32)^2)]);
	// x * (0x3ff000000000001f + x^2 * (0x3fc55555554f49e4 + x^2 * (0x3fb333333f6b5213 + x^2 * (0x3fa6db647a148619 + x^2 * (0x3f9f1fc1e5478411 + x^2 * (0x3f969b384124d866 + x^2 * 0x3f9537d58830aca4))))))
	{
		{.l = 0x3ff000000000001f },
		{.l = 0x3fc55555554f49e4 },
		{.l = 0x3fb333333f6b5213 },
		{.l = 0x3fa6db647a148619 },
		{.l = 0x3f9f1fc1e5478411 },
		{.l = 0x3f969b384124d866 },
		{.l = 0x3f9537d58830aca4 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13,15|], [|D...|], [0,sqrt(1 - (31/32)^2)]);
	// x * (0x3fefffffffffffff + x^2 * (0x3fc55555555571a5 + x^2 * (0x3fb3333332ea4a79 + x^2 * (0x3fa6db6dff77c14d + x^2 * (0x3f9f1c4e256e63a1 + x^2 * (0x3f96ed77cf5b5047 + x^2 * (0x3f916c12ed9db0f5 + x^2 * 0x3f919ecbcc5a2a51)))))))
	{
		{.l = 0x3fefffffffffffff },
		{.l = 0x3fc55555555571a5 },
		{.l = 0x3fb3333332ea4a79 },
		{.l = 0x3fa6db6dff77c14d },
		{.l = 0x3f9f1c4e256e63a1 },
		{.l = 0x3f96ed77cf5b5047 },
		{.l = 0x3f916c12ed9db0f5 },
		{.l = 0x3f919ecbcc5a2a51 },
	},
};

// fpminmax: [0, 1/16]
static const DL
coefficient_16[COEFFICIENTS_NUM][COEFFICIENTS_NUM] = {

	// P = fpminimax(asin(x), [|1|], [|D...|], [0,sqrt(1 - (15/16)^2)]);
	// x * 0x3ff02b4cd2d8b87c
	{
		{.l = 0x3ff02b4cd2d8b87c },
	},

	// P = fpminimax(asin(x), [|1,3|], [|D...|], [0,sqrt(1 - (15/16)^2)]);
	// x * (0x3feffec0847baccd + x^2 * 0x3fc69571f6c8513a)
	{
		{.l = 0x3feffec0847baccd },
		{.l = 0x3fc69571f6c8513a },
	},

	// P = fpminimax(asin(x), [|1,3,5|], [|D...|], [0,sqrt(1 - (15/16)^2)]);
	// x * (0x3ff000030e14b4d5 + x^2 * (0x3fc5477682bf604e + x^2 * 0x3fb5805275b9032b))
	{
		{.l = 0x3ff000030e14b4d5 },
		{.l = 0x3fc5477682bf604e },
		{.l = 0x3fb5805275b9032b },
	},

	// P = fpminimax(asin(x), [|1,3,5,7|], [|D...|], [0,sqrt(1 - (15/16)^2)]);
	// x * (0x3fefffffddbf7e98 + x^2 * (0x3fc555e025479ab8 + x^2 * (0x3fb30786ee744bfa + x^2 * 0x3fab2a6bc40c1e93)))
	{
		{.l = 0x3fefffffddbf7e98 },
		{.l = 0x3fc555e025479ab8 },
		{.l = 0x3fb30786ee744bfa },
		{.l = 0x3fab2a6bc40c1e93 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9|], [|D...|], [0,sqrt(1 - (15/16)^2)]);
	// x * (0x3ff000000067f5f6 + x^2 * (0x3fc555502cf09bb4 + x^2 * (0x3fb335cf98a37a73 + x^2 * (0x3fa66620f0d4f766 + x^2 * 0x3fa3a65cdcabdc1a))))
	{
		{.l = 0x3ff000000067f5f6 },
		{.l = 0x3fc555502cf09bb4 },
		{.l = 0x3fb335cf98a37a73 },
		{.l = 0x3fa66620f0d4f766 },
		{.l = 0x3fa3a65cdcabdc1a },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11|], [|D...|], [0,sqrt(1 - (15/16)^2)]);
	// x * (0x3feffffffffacde6 + x^2 * (0x3fc5555584f21616 + x^2 * (0x3fb3330fee8d8dda + x^2 * (0x3fa6e4ee5c2a087a + x^2 * (0x3f9df7abfd5cc29b + x^2 * 0x3f9ec838c76084de)))))
	{
		{.l = 0x3feffffffffacde6 },
		{.l = 0x3fc5555584f21616 },
		{.l = 0x3fb3330fee8d8dda },
		{.l = 0x3fa6e4ee5c2a087a },
		{.l = 0x3f9df7abfd5cc29b },
		{.l = 0x3f9ec838c76084de },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13|], [|D...|], [0,sqrt(1 - (15/16)^2)]);
	// x * (0x3ff000000000113f + x^2 * (0x3fc5555553a664ab + x^2 * (0x3fb33334ea00041a + x^2 * (0x3fa6dac6bd24171e + x^2 * (0x3f9f3a7fcf971c8a + x^2 * (0x3f958a32ffffe17a + x^2 * 0x3f9968982b44b7d1))))))
	{
		{.l = 0x3ff000000000113f },
		{.l = 0x3fc5555553a664ab },
		{.l = 0x3fb33334ea00041a },
		{.l = 0x3fa6dac6bd24171e },
		{.l = 0x3f9f3a7fcf971c8a },
		{.l = 0x3f958a32ffffe17a },
		{.l = 0x3f9968982b44b7d1 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13,15|], [|D...|], [0,sqrt(1 - (15/16)^2)]);
	// x * (0x3fefffffffffff15 + x^2 * (0x3fc5555555645a17 + x^2 * (0x3fb333331f16d542 + x^2 * (0x3fa6db77f29089b1 + x^2 * (0x3f9f19e5f7ff868a + x^2 * (0x3f97146b094199f4 + x^2 * (0x3f902c2b170f92d2 + x^2 * 0x3f95c4578c81b9d1)))))))
	{
		{.l = 0x3fefffffffffff15 },
		{.l = 0x3fc5555555645a17 },
		{.l = 0x3fb333331f16d542 },
		{.l = 0x3fa6db77f29089b1 },
		{.l = 0x3f9f19e5f7ff868a },
		{.l = 0x3f97146b094199f4 },
		{.l = 0x3f902c2b170f92d2 },
		{.l = 0x3f95c4578c81b9d1 },
	},
};

// fpminmax: [0, 1/8]
static const DL
coefficient_8[COEFFICIENTS_NUM][COEFFICIENTS_NUM] = {

	// P = fpminimax(asin(x), [|1|], [|D...|], [0,sqrt(1 - (7/8)^2)]);
	// x * 0x3ff057e9ea11c326
	{
		{.l = 0x3ff057e9ea11c326 },
	},

	// P = fpminimax(asin(x), [|1,3|], [|D...|], [0,sqrt(1 - (7/8)^2)]);
	// x * (0x3feffacd8ff6eada + x^2 * 0x3fc7f1534b51ade0)
	{
		{.l = 0x3feffacd8ff6eada },
		{.l = 0x3fc7f1534b51ade0 },
	},

	// P = fpminimax(asin(x), [|1,3,5|], [|D...|], [0,sqrt(1 - (7/8)^2)]);
	// x * (0x3ff0001a2fba188c + x^2 * (0x3fc5195417864b3e + x^2 * 0x3fb8288c03c6490a))
	{
		{.l = 0x3ff0001a2fba188c },
		{.l = 0x3fc5195417864b3e },
		{.l = 0x3fb8288c03c6490a },
	},

	// P = fpminimax(asin(x), [|1,3,5,7|], [|D...|], [0,sqrt(1 - (7/8)^2)]);
	// x * (0x3feffffda2c39e55 + x^2 * (0x3fc55a30a50e7684 + x^2 * (0x3fb26e2192534aaa + x^2 * 0x3fb03abdad4ded3d)))
	{
		{.l = 0x3feffffda2c39e55 },
		{.l = 0x3fc55a30a50e7684 },
		{.l = 0x3fb26e2192534aaa },
		{.l = 0x3fb03abdad4ded3d },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9|], [|D...|], [0,sqrt(1 - (7/8)^2)]);
	// x * (0x3ff000000ecdec4a + x^2 * (0x3fc554f5bdd0f236 + x^2 * (0x3fb34bb7523886d4 + x^2 * (0x3fa4b277070e388a + x^2 * 0x3fa901872093d383))))
	{
		{.l = 0x3ff000000ecdec4a },
		{.l = 0x3fc554f5bdd0f236 },
		{.l = 0x3fb34bb7523886d4 },
		{.l = 0x3fa4b277070e388a },
		{.l = 0x3fa901872093d383 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11|], [|D...|], [0,sqrt(1 - (7/8)^2)]);
	// x * (0x3feffffffe790a0e + x^2 * (0x3fc5555c76a99854 + x^2 * (0x3fb33083d3e143d4 + x^2 * (0x3fa73919fd992d6e + x^2 * (0x3f9978fbc2b148b5 + x^2 * 0x3fa4e09a4fff4417)))))
	{
		{.l = 0x3feffffffe790a0e },
		{.l = 0x3fc5555c76a99854 },
		{.l = 0x3fb33083d3e143d4 },
		{.l = 0x3fa73919fd992d6e },
		{.l = 0x3f9978fbc2b148b5 },
		{.l = 0x3fa4e09a4fff4417 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13|], [|D...|], [0,sqrt(1 - (7/8)^2)]);
	// x * (0x3ff00000000a77b1 + x^2 * (0x3fc55554cfdcccde + x^2 * (0x3fb3337873b67e2d + x^2 * (0x3fa6ce0b26da1552 + x^2 * (0x3fa029e25ad5f130 + x^2 * (0x3f8faf2cb8e3a357 + x^2 * 0x3fa260a861023846))))))
	{
		{.l = 0x3ff00000000a77b1 },
		{.l = 0x3fc55554cfdcccde },
		{.l = 0x3fb3337873b67e2d },
		{.l = 0x3fa6ce0b26da1552 },
		{.l = 0x3fa029e25ad5f130 },
		{.l = 0x3f8faf2cb8e3a357 },
		{.l = 0x3fa260a861023846 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13,15|], [|D...|], [0,sqrt(1 - (7/8)^2)]);
	// x * (0x3feffffffffed8e4 + x^2 * (0x3fc555555ef50c69 + x^2 * (0x3fb3332c9fafa20e + x^2 * (0x3fa6dd21f8f14195 + x^2 * (0x3f9ee56124fd2f28 + x^2 * (0x3f98c4aaeaf801f6 + x^2 * (0x3f824537d53b15f4 + x^2 * 0x3fa0cb713e43fa82)))))))
	{
		{.l = 0x3feffffffffed8e4 },
		{.l = 0x3fc555555ef50c69 },
		{.l = 0x3fb3332c9fafa20e },
		{.l = 0x3fa6dd21f8f14195 },
		{.l = 0x3f9ee56124fd2f28 },
		{.l = 0x3f98c4aaeaf801f6 },
		{.l = 0x3f824537d53b15f4 },
		{.l = 0x3fa0cb713e43fa82 },
	},
};


// fpminmax: [0, 1/4]
static const DL
coefficient_4[COEFFICIENTS_NUM][COEFFICIENTS_NUM] = {

	// P = fpminimax(asin(x), [|1|], [|D...|], [0,sqrt(1 - (3/4)^2)]);
	// x * 0x3ff0b562f20ee850
	{
		{.l = 0x3ff0b562f20ee850 },
	},

	// P = fpminimax(asin(x), [|1,3|], [|D...|], [0,sqrt(1 - (3/4)^2)]);
	// x * (0x3fefe9642348fb68 + x^2 * 0x3fcb09c755f37b25)
	{
		{.l = 0x3fefe9642348fb68 },
		{.l = 0x3fcb09c755f37b25 },
	},

	// P = fpminimax(asin(x), [|1,3,5|], [|D...|], [0,sqrt(1 - (3/4)^2)]);
	// x * (0x3ff000f222247d47 + x^2 * (0x3fc43a413abf8c6e + x^2 * 0x3fbed482abdcc3c7))
	{
		{.l = 0x3ff000f222247d47 },
		{.l = 0x3fc43a413abf8c6e },
		{.l = 0x3fbed482abdcc3c7 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7|], [|D...|], [0,sqrt(1 - (3/4)^2)]);
	// x * (0x3fefffd16345cc9c + x^2 * (0x3fc586a2b6936ce0 + x^2 * (0x3fae720c2a4672d0 + x^2 * 0x3fb794124984d25b)))
	{
		{.l = 0x3fefffd16345cc9c },
		{.l = 0x3fc586a2b6936ce0 },
		{.l = 0x3fae720c2a4672d0 },
		{.l = 0x3fb794124984d25b },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9|], [|D...|], [0,sqrt(1 - (3/4)^2)]);
	// x * (0x3ff000026f616c53 + x^2 * (0x3fc54d2fde90fce9 + x^2 * (0x3fb44689a0037e5c + x^2 * (0x3f95281ce214111c + x^2 * 0x3fb4beebede70f2d))))
	{
		{.l = 0x3ff000026f616c53 },
		{.l = 0x3fc54d2fde90fce9 },
		{.l = 0x3fb44689a0037e5c },
		{.l = 0x3f95281ce214111c },
		{.l = 0x3fb4beebede70f2d },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11|], [|D...|], [0,sqrt(1 - (3/4)^2)]);
	// x * (0x3fefffff76871513 + x^2 * (0x3fc556a3629f2e3f + x^2 * (0x3fb2f1fd7e08af5c + x^2 * (0x3fab6bc8ae45e283 + x^2 * (0xbf711f3d7524d0ad + x^2 * 0x3fb3d0acc9e9d679)))))
	{
		{.l = 0x3fefffff76871513 },
		{.l = 0x3fc556a3629f2e3f },
		{.l = 0x3fb2f1fd7e08af5c },
		{.l = 0x3fab6bc8ae45e283 },
		{.l = 0xbf711f3d7524d0ad },
		{.l = 0x3fb3d0acc9e9d679 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13|], [|D...|], [0,sqrt(1 - (3/4)^2)]);
	// x * (0x3ff0000007df8313 + x^2 * (0x3fc55520e74a410a + x^2 * (0x3fb3415e6e47a254 + x^2 * (0x3fa57064400dec22 + x^2 * (0x3fa800550425ca75 + x^2 * (0xbf9a37f594618207 + x^2 * 0x3fb3f965a6d03299))))))
	{
		{.l = 0x3ff0000007df8313 },
		{.l = 0x3fc55520e74a410a },
		{.l = 0x3fb3415e6e47a254 },
		{.l = 0x3fa57064400dec22 },
		{.l = 0x3fa800550425ca75 },
		{.l = 0xbf9a37f594618207 },
		{.l = 0x3fb3f965a6d03299 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13,15|], [|D...|], [0,sqrt(1 - (3/4)^2)]);
	// x * (0x3feffffffe2505b8 + x^2 * (0x3fc5555d71f176a4 + x^2 * (0x3fb3304d89878ee9 + x^2 * (0x3fa73f7cf0a83fb0 + x^2 * (0x3f9894c4555240f2 + x^2 * (0x3fa9dd7bacd02159 + x^2 * (0xbfa871aeaa0c0640 + x^2 * 0x3fb4ea86b8587754)))))))
	{
		{.l = 0x3feffffffe2505b8 },
		{.l = 0x3fc5555d71f176a4 },
		{.l = 0x3fb3304d89878ee9 },
		{.l = 0x3fa73f7cf0a83fb0 },
		{.l = 0x3f9894c4555240f2 },
		{.l = 0x3fa9dd7bacd02159 },
		{.l = 0xbfa871aeaa0c0640 },
		{.l = 0x3fb4ea86b8587754 },
	},
};


// fpminmax: [0, 1/2]
static const DL
coefficient_2[COEFFICIENTS_NUM][COEFFICIENTS_NUM] = {

	// P = fpminimax(asin(x), [|1|], [|D...|], [0,sqrt(1 - (1/2)^2)]);
	// x * 0x3ff183dea14f4d3b
	{
		{.l = 0x3ff183dea14f4d3b },
	},

	// P = fpminimax(asin(x), [|1,3|], [|D...|], [0,sqrt(1 - (1/2)^2)]);
	// x * (0x3fef935729bc4436 + x^2 * 0x3fd19d6ac54fc530)
	{
		{.l = 0x3fef935729bc4436 },
		{.l = 0x3fd19d6ac54fc530 },
	},

	// P = fpminimax(asin(x), [|1,3,5|], [|D...|], [0,sqrt(1 - (1/2)^2)]);
	// x * (0x3ff00a706d46e21c + x^2 * (0x3fbddec0c0c8ce50 + x^2 * 0x3fca68ed7146e2af))
	{
		{.l = 0x3ff00a706d46e21c },
		{.l = 0x3fbddec0c0c8ce50 },
		{.l = 0x3fca68ed7146e2af },
	},

	// P = fpminimax(asin(x), [|1,3,5,7|], [|D...|], [0,sqrt(1 - (1/2)^2)]);
	// x * (0x3feffb59f5b1b2aa + x^2 * (0x3fc7f6f8dc68015f + x^2 * (0xbfa299b22027d107 + x^2 * 0x3fcb0867dd34e8f0)))
	{
		{.l = 0x3feffb59f5b1b2aa },
		{.l = 0x3fc7f6f8dc68015f },
		{.l = 0xbfa299b22027d107 },
		{.l = 0x3fcb0867dd34e8f0 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9|], [|D...|], [0,sqrt(1 - (1/2)^2)]);
	// x * (0x3ff000904a0053b2 + x^2 * (0x3fc44f59a8a20711 + x^2 * (0x3fc2e2b7ff93691e + x^2 * (0xbfc5f50bdb53d77d + x^2 * 0x3fd00893946bae86))))
	{
		{.l = 0x3ff000904a0053b2 },
		{.l = 0x3fc44f59a8a20711 },
		{.l = 0x3fc2e2b7ff93691e },
		{.l = 0xbfc5f50bdb53d77d },
		{.l = 0x3fd00893946bae86 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11|], [|D...|], [0,sqrt(1 - (1/2)^2)]);
	// x * (0x3fefffb604980a6f + x^2 * (0x3fc5b7e65494f4cd + x^2 * (0x3fa1707c43bb7661 + x^2 * (0x3fcee817050548a1 + x^2 * (0xbfd77255dda0367b + x^2 * 0x3fd4b8605c02c72e)))))
	{
		{.l = 0x3fefffb604980a6f },
		{.l = 0x3fc5b7e65494f4cd },
		{.l = 0x3fa1707c43bb7661 },
		{.l = 0x3fcee817050548a1 },
		{.l = 0xbfd77255dda0367b },
		{.l = 0x3fd4b8605c02c72e },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13|], [|D...|], [0,sqrt(1 - (1/2)^2)]);
	// x * (0x3ff00009dc8cfddc + x^2 * (0x3fc5310c862679b9 + x^2 * (0x3fb8981448cc49e8 + x^2 * (0xbfba3e58e4fd9b75 + x^2 * (0x3fe001b124886772 + x^2 * (0xbfe5f3c137032839 + x^2 * 0x3fdc509f98d0098b))))))
	{
		{.l = 0x3ff00009dc8cfddc },
		{.l = 0x3fc5310c862679b9 },
		{.l = 0x3fb8981448cc49e8 },
		{.l = 0xbfba3e58e4fd9b75 },
		{.l = 0x3fe001b124886772 },
		{.l = 0xbfe5f3c137032839 },
		{.l = 0x3fdc509f98d0098b },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13,15|], [|D...|], [0,sqrt(1 - (1/2)^2)]);
	// x * (0x3feffffa9660218c + x^2 * (0x3fc5627ca1660be4 + x^2 * (0x3fb098cbc41e7ba3 + x^2 * (0x3fc2183f0b28f9c8 + x^2 * (0xbfda3f6dc6236959 + x^2 * (0x3ff1052c4010c01f + x^2 * (0xbff3abbd6e116775 + x^2 * 0x3fe41f050b6cfd70)))))))
	{
		{.l = 0x3feffffa9660218c },
		{.l = 0x3fc5627ca1660be4 },
		{.l = 0x3fb098cbc41e7ba3 },
		{.l = 0x3fc2183f0b28f9c8 },
		{.l = 0xbfda3f6dc6236959 },
		{.l = 0x3ff1052c4010c01f },
		{.l = 0xbff3abbd6e116775 },
		{.l = 0x3fe41f050b6cfd70 },
	},
};


// fpminmax: [0, 1/1]
static const DL
coefficient_1[COEFFICIENTS_NUM][COEFFICIENTS_NUM] = {

	// P = fpminimax(asin(x), [|1|], [|D...|], [0,sqrt(1 - (0/1)^2)]);
	// x * 0x3ff38d7050d05568
	{
		{.l = 0x3ff38d7050d05568 },
	},

	// P = fpminimax(asin(x), [|1,3|], [|D...|], [0,sqrt(1 - (0/1)^2)]);
	// x * (0x3fed55e4822cba3e + x^2 * 0x3fe0be9e7e764e84)
	{
		{.l = 0x3fed55e4822cba3e },
		{.l = 0x3fe0be9e7e764e84 },
	},

	// P = fpminimax(asin(x), [|1,3,5|], [|D...|], [0,sqrt(1 - (0/1)^2)]);
	// x * (0x3ff0c13f827ad40c + x^2 * (0xbfd72fd5aff0cbd8 + x^2 * 0x3fe9fa4726b4df31))
	{
		{.l = 0x3ff0c13f827ad40c },
		{.l = 0xbfd72fd5aff0cbd8 },
		{.l = 0x3fe9fa4726b4df31 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7|], [|D...|], [0,sqrt(1 - (0/1)^2)]);
	// x * (0x3feef8e573d75321 + x^2 * (0x3febd5f7cc5fadd5 + x^2 * (0xc00154bb76d78342 + x^2 * 0x3ffd955f7e8a5087)))
	{
		{.l = 0x3feef8e573d75321 },
		{.l = 0x3febd5f7cc5fadd5 },
		{.l = 0xc00154bb76d78342 },
		{.l = 0x3ffd955f7e8a5087 },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9|], [|D...|], [0,sqrt(1 - (0/1)^2)]);
	// x * (0x3ff062cbc74429e5 + x^2 * (0xbfe6d1a60f679022 + x^2 * (0x401417e826bd40d5 + x^2 * (0xc021a36c8b1ffae4 + x^2 * 0x4014122585cd020e))))
	{
		{.l = 0x3ff062cbc74429e5 },
		{.l = 0xbfe6d1a60f679022 },
		{.l = 0x401417e826bd40d5 },
		{.l = 0xc021a36c8b1ffae4 },
		{.l = 0x4014122585cd020e },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11|], [|D...|], [0,sqrt(1 - (0/1)^2)]);
	// x * (0x3fef62771ac63dc5 + x^2 * (0x3ff3974d71c4f4f8 + x^2 * (0xc0224ac83ceab6eb + x^2 * (0x403c063f6aafe68e + x^2 * (0xc04145466b6cb387 + x^2 * 0x402dff1a182ff70a)))))
	{
		{.l = 0x3fef62771ac63dc5 },
		{.l = 0x3ff3974d71c4f4f8 },
		{.l = 0xc0224ac83ceab6eb },
		{.l = 0x403c063f6aafe68e },
		{.l = 0xc04145466b6cb387 },
		{.l = 0x402dff1a182ff70a },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13|], [|D...|], [0,sqrt(1 - (0/1)^2)]);
	// x * (0x3ff04158c0029cea + x^2 * (0xbff11e91ca3acbbc + x^2 * (0x402efe9dad022e92 + x^2 * (0xc0517d8afbc0edce + x^2 * (0x4061e7583f50a679 + x^2 * (0xc060d87ddce4ae5d + x^2 * 0x4047ccc979d90e2e))))))
	{
		{.l = 0x3ff04158c0029cea },
		{.l = 0xbff11e91ca3acbbc },
		{.l = 0x402efe9dad022e92 },
		{.l = 0xc0517d8afbc0edce },
		{.l = 0x4061e7583f50a679 },
		{.l = 0xc060d87ddce4ae5d },
		{.l = 0x4047ccc979d90e2e },
	},

	// P = fpminimax(asin(x), [|1,3,5,7,9,11,13,15|], [|D...|], [0,sqrt(1 - (0/1)^2)]);
	// x * (0x3fef907983114288 + x^2 * (0x3ff9527725b94659 + x^2 * (0xc037d6babc188b15 + x^2 * (0x4062dcda22386e43 + x^2 * (0xc07c3d9eab7c554d + x^2 * (0x4085abb8cf0a2c23 + x^2 * (0xc08077db131a0f76 + x^2 * 0x4063a922fc5c6fb7)))))))
	{
		{.l = 0x3fef907983114288 },
		{.l = 0x3ff9527725b94659 },
		{.l = 0xc037d6babc188b15 },
		{.l = 0x4062dcda22386e43 },
		{.l = 0xc07c3d9eab7c554d },
		{.l = 0x4085abb8cf0a2c23 },
		{.l = 0xc08077db131a0f76 },
		{.l = 0x4063a922fc5c6fb7 },
	},
};


// link for all coefficients array
static const DL *link[COEFFICIENTS_VERSION];

int gen(struct constraint input_parameter) {
	double a, b;
	int precision;
	double X;
	FILE *func;
	// init
	int i, num, bit, table_num, table_num_1, coefficients_num, fnum, format;
	num = 100;
	char degree_string[][100] = {
		"[| 1| ]",
		"[| 1, 3| ]",
		"[| 1, 3, 5| ]",
		"[| 1, 3, 5, 7| ]",
		"[| 1, 3, 5, 7, 9| ]",
		"[| 1, 3, 5, 7, 9, 11| ]",
		"[| 1, 3, 5, 7, 9, 11, 13| ]",
		"[| 1, 3, 5, 7, 9, 11, 13, 15| ]",
	};

	if ((func = fopen("acos_gen.c", "w")) == (FILE *)0) {
		printf("open file error!\n");
		return 1;
	}
	a = input_parameter.start;
	b = input_parameter.end;
	precision = input_parameter.precision;
	bit = input_parameter.bit;
	//bit = 7;
	table_num = 1 << bit;
	table_num_1 = table_num - 1;
	fnum = input_parameter.fnum;
	coefficients_num = input_parameter.degree + 1;
	//coefficients_num = 8;
	// format --> float number format: double is 64, float is 32
	format = 64;

	/*
	printf("please input [a, b]: ");
	scanf("%lf %lf", &a, &b);
	printf("please input target precision: ");
	scanf("%d", &precision);
	printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);
	*/

	// generate code for asin_gen
	{
		fprintf(func, "#include <math.h>\n");
		fprintf(func, "#include %cmyhead.h%c\n", '"', '"');
		fprintf(func, "\n");
		fprintf(func, "#define BIT %d\n", bit);
		fprintf(func, "#define TABLE_NUM %d\n", 1 << bit);
		fprintf(func, "#define DEGREE %d\n", coefficients_num * 2 - 1);
		fprintf(func, "#define COEFFICIENTS_NUM %d\n", coefficients_num);
		fprintf(func, "\n");
		fprintf(func, "// static const DL Twopm28 = { .l = 0x3e30000000000000 };\n");
		fprintf(func, "\n");
		
		// COEFFICIENTS
		//fprintf(func, "// P = fpminimax(asin(x), [| 1, 3, 5, 7, 9, 11, 13, 15| ], [| D... | ], [0, sqrt(1 - (127 / 128)*(127 / 128))]); printexpansion(P);\n");
		fprintf(func, "// P = fpminimax(asin(x), %s, [| D... | ], [0, sqrt(1 - (%d / %d)^2)]); printexpansion(P);\n", degree_string[coefficients_num - 1], table_num_1, table_num);
		//fprintf(func, "// x * (0x3ff0000000000000 + x ^ 2 * (0x3fc5555555555556 + x ^ 2 * (0x3fb3333333332cb8 + x ^ 2 * (0x3fa6db6db6f2a213 + x ^ 2 * (0x3f9f1c719dc96a96 + x ^ 2 * (0x3f96e8ce755ff3f7 + x ^ 2 * (0x3f91bf59a4e08f79 + x ^ 2 * 0x3f8e340c0c874f30)))))))\n");
		fprintf(func, "static const DL\n");
		fprintf(func, "C[COEFFICIENTS_NUM] = {\n");
		for (i = 0; i < coefficients_num; i++) {
			fprintf(func, "\t{.l = 0x%016lx},\n", link[bit][(coefficients_num - 1) * COEFFICIENTS_NUM + i].l);
			//fprintf(func, "\t{.l = 0x%lx},\n", coefficient[coefficients_num][i].l);
		}
		fprintf(func, "};\n\n");
		
		// asin_tab
		fprintf(func, "/* asin value: 0~1, step = 1/128 */\n");
		fprintf(func, "static const DL\n");
		fprintf(func, "asin_tab[TABLE_NUM] = {\n");
		for (i = 0; i < table_num; i++) {
			fprintf(func, "\t{.l = 0x%016lx},\n", asin_tab[i * (1 << (BIT - bit))].l);
		}
		fprintf(func, "};\n\n");
		
		// sk
		fprintf(func, "/* cos(y) = sqrt(1-[x]^2) value: 0~1, step = 1/128 */\n");
		fprintf(func, "static const DL\n");
		fprintf(func, "sk[TABLE_NUM] = \n");
		fprintf(func, "{\n");
		for (i = 0; i < table_num; i++) {
			fprintf(func, "\t{ .l = 0x%016lx },\n", sk[i * (1 << (BIT - bit))].l);
		}
		fprintf(func, "};\n\n");
		
		// ck
		fprintf(func, "/* sin(y) = [x] value: 0~1, step = 1/128 */\n");
		fprintf(func, "static const DL\n");
		fprintf(func, "ck[TABLE_NUM] = \n");
		fprintf(func, "{\n");
		for (i = 0; i < table_num; i++) {
			fprintf(func, "\t{ .l = 0x%016lx },\n", ck[i * (1 << (BIT - bit))].l);
		}
		fprintf(func, "};\n\n");
		fprintf(func, "static const double\n");
		fprintf(func, "pio2_1 = 1.57079632673412561417e+00, /* 0x3FF921FB, 0x54400000 */\n");
		fprintf(func, "pio2_1t = 6.07710050650619224932e-11; /* 0x3DD0B461, 0x1A626331 */\n");
	}
	{
		fprintf(func, "double acos_gen(double x) {\n");
		fprintf(func, "	double result, temp, delta, zsq;\n");
		fprintf(func, "	int order;\n");
		fprintf(func, "\n");
		fprintf(func, "	temp = x * (double)TABLE_NUM;\n");
		fprintf(func, "	order = temp;\n");
		fprintf(func, "	\n");
		fprintf(func, "	// delta = x * sqrt(1 - temp * temp) - temp * sqrt(1 - x * x);\n");
		fprintf(func, "	delta = x * ck[order].d - sk[order].d * sqrt(1.0 - x + x * (1.0 - x));\n");
		fprintf(func, "\n");
		fprintf(func, "	x = delta;\n");
		fprintf(func, "	// if (delta < Twopm28.d)\n");
		fprintf(func, "	//	x = 0.0;\n");
		fprintf(func, "	zsq = x * x;\n");
		fprintf(func, "\n");
		
		// polynomials
		// fprintf(func, "	result = C[0].d * delta + (delta * zsq) * (C[1].d + zsq * (C[2].d + zsq * (C[3].d + zsq * (C[4].d + zsq * (C[5].d + zsq * (C[6].d + zsq * (C[7].d)))))));\n");
		if (coefficients_num > 1) {
			fprintf(func, "\tresult = C[0].d * delta + (delta * zsq) * (C[1].d");
			for (i = 2; i < coefficients_num; i++) {
				fprintf(func, " + zsq * (C[%d].d", i);
			}
			for (i = 1; i < coefficients_num; i++) {
				fprintf(func, ")");
			}
			fprintf(func, ";\n");
			fprintf(func, "\n");
		}
		else {
			fprintf(func, "\tresult = C[0].d * delta;\n\n");
		}
		
		fprintf(func, "\tresult = pio2_1 - (asin_tab[order].d + result) + pio2_1t;\n");
		fprintf(func, "\n");
		fprintf(func, "\treturn result;\n");
		fprintf(func, "}\n");
	}
	
	fclose(func);

	return 0;
}

int main(int argc, char *argv[]) {
	double a, b;
	int precision;
	int bit, fnum, degree;
	struct constraint input_parameter;

	if (argc == 1) {
		printf("please input [a, b]: ");
		scanf("%lf %lf", &a, &b);
		printf("please input target precision: ");
		scanf("%d", &precision);
		printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);
		printf("please input interval paramenter -- bit: ");
		scanf("%d", &bit);
		printf("please input coefficients_num: ");
		scanf("%d", &degree);
		fnum = 1;

		// test
		// printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);
		// printf("bit = %d\n", bit);
		// printf("coefficients_num = %d\n", coefficients_num);
		// printf("fnum = %d\n", fnum);
	}
	else if (argc == 7) {
		a = atof(argv[1]);
		b = atof(argv[2]);
		precision = atoi(argv[3]);
		bit = atoi(argv[4]);
		fnum = atoi(argv[5]);
		degree = atoi(argv[6]);
	}
	else {
		printf("please input 6 parameters!!\n");
		return 1;
	}

	link[7] = &(coefficient_128[0][0]);
	link[6] = &(coefficient_64[0][0]);
	link[5] = &(coefficient_32[0][0]);
	link[4] = &(coefficient_16[0][0]);
	link[3] = &(coefficient_8[0][0]);
	link[2] = &(coefficient_4[0][0]);
	link[1] = &(coefficient_2[0][0]);
	link[0] = &(coefficient_1[0][0]);

	input_parameter.start = a;
	input_parameter.end = b;
	input_parameter.precision = precision;
	input_parameter.bit = bit;
	input_parameter.fnum = fnum;
	input_parameter.degree = degree;
	gen(input_parameter);

	return 0;
}