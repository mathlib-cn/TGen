#include <stdio.h>
#include <stdlib.h>
#include "myhead.h"

#define NUM 100
#define DEGREE 7
#define COEFFICIENTS 7
#define COEFFICIENTS_NUM 7
#define COEFFICIENTS_VERSION 8
#define BIT 7
#define BITNUM 128
#define	FORMAT 64

struct constraint {
	double start;
	double end;
	int precision;
	int bit;
	int fnum;
	int degree;
};


// sin & cos
// coefficient[0] is for sin; coefficient[1] is for cos;

static const DL
coefficient_1_1[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,1/1]); printexpansion(P);
		// 0x3cafffffffffffff
		{
			{ .l = 0x3cafffffffffffff },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,1/1]); printexpansion(P);
		// 0x3fe673208375d704
		{
			{ .l = 0x3fe673208375d704 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,1/1]); printexpansion(P);
		// x * 0x3fed3ec3da82b28a
		{
			{ .l = 0 },
			{ .l = 0x3fed3ec3da82b28a },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,1/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfddc0dcd202eace
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfddc0dcd202eace },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,1/1]); printexpansion(P);
		// x * (0x3feff7091e401904 + x^2 * 0xbfc444fe8b2c3dae)
		{
			{ .l = 0 },
			{ .l = 0x3feff7091e401904 },
			{ .l = 0xbfc444fe8b2c3dae },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,1/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdff547de42bdca + x^2 * 0x3fa451bdedaf4311)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdff547de42bdca },
			{ .l = 0x3fa451bdedaf4311 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,1/1]); printexpansion(P);
		// x * (0x3feffff23a645acc + x^2 * (0xbfc5519f57473383 + x^2 * 0x3f8075704a389113))
		{
			{ .l = 0 },
			{ .l = 0x3feffff23a645acc },
			{ .l = 0xbfc5519f57473383 },
			{ .l = 0x3f8075704a389113 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,1/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffea2d27eab3 + x^2 * (0x3fa55179865ea973 + x^2 * 0xbf55f5910046091b))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffea2d27eab3 },
			{ .l = 0x3fa55179865ea973 },
			{ .l = 0xbf55f5910046091b },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,1/1]); printexpansion(P);
		// x * (0x3feffffff3b1e9e3 + x^2 * (0xbfc5554f64a1e2f3 + x^2 * (0x3f810f3f1799f274 + x^2 * 0xbf294923980d125c)))
		{
			{ .l = 0 },
			{ .l = 0x3feffffff3b1e9e3 },
			{ .l = 0xbfc5554f64a1e2f3 },
			{ .l = 0x3f810f3f1799f274 },
			{ .l = 0xbf294923980d125c },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,1/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffe7b26a37 + x^2 * (0x3fa5554e6db68b03 + x^2 * (0xbf56beef552e0577 + x^2 * 0x3ef94a4c7e54b5d9)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffe7b26a37 },
			{ .l = 0x3fa5554e6db68b03 },
			{ .l = 0xbf56beef552e0577 },
			{ .l = 0x3ef94a4c7e54b5d9 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,1/1]); printexpansion(P);
		// x * (0x3feffffffff8d0b0 + x^2 * (0xbfc555554fe2164e + x^2 * (0x3f81110e64991b5b + x^2 * (0xbf29ffc6f9d065aa + x^2 * 0x3ec697b018937d5e))))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffff8d0b0 },
			{ .l = 0xbfc555554fe2164e },
			{ .l = 0x3f81110e64991b5b },
			{ .l = 0xbf29ffc6f9d065aa },
			{ .l = 0x3ec697b018937d5e },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,1/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffef005b + x^2 * (0x3fa555554e405168 + x^2 * (0xbf56c1683271dcc9 + x^2 * (0x3ef9ffbd1378c813 + x^2 * 0xbe921311166ed6bc))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffef005b },
			{ .l = 0x3fa555554e405168 },
			{ .l = 0xbf56c1683271dcc9 },
			{ .l = 0x3ef9ffbd1378c813 },
			{ .l = 0xbe921311166ed6bc },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,1/1]); printexpansion(P);
		// x * (0x3feffffffffffd0b + x^2 * (0xbfc5555555521752 + x^2 * (0x3f8111110ebe8e92 + x^2 * (0xbf2a019da802050a + x^2 * (0x3ec71cb89b443c82 + x^2 * 0xbe5a621b603097cb)))))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffffd0b },
			{ .l = 0xbfc5555555521752 },
			{ .l = 0x3f8111110ebe8e92 },
			{ .l = 0xbf2a019da802050a },
			{ .l = 0x3ec71cb89b443c82 },
			{ .l = 0xbe5a621b603097cb },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,1/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffff7da + x^2 * (0x3fa555555550a72b + x^2 * (0xbf56c16c13218487 + x^2 * (0x3efa019d7f693c2b + x^2 * (0xbe927d5bf3e6b670 + x^2 * 0x3e219699eb654ee7)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffffff7da },
			{ .l = 0x3fa555555550a72b },
			{ .l = 0xbf56c16c13218487 },
			{ .l = 0x3efa019d7f693c2b },
			{ .l = 0xbe927d5bf3e6b670 },
			{ .l = 0x3e219699eb654ee7 },
		},
	},
};

static const DL
coefficient_1_2[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,1/2]); printexpansion(P);
		// 0x3caffffffffffffe
		{
			{ .l = 0x3caffffffffffffe },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,1/2]); printexpansion(P);
		// 0x3fede9e2beba22d3
		{
			{ .l = 0x3fede9e2beba22d3 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,1/2]); printexpansion(P);
		// x * 0x3fef53e9d6216d2d
		{
			{ .l = 0 },
			{ .l = 0x3fef53e9d6216d2d },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,1/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdf72251f12b534
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdf72251f12b534 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,1/2]); printexpansion(P);
		// x * (0x3fefff75d15eebbd + x^2 * 0xbfc5111dc143ce68)
		{
			{ .l = 0 },
			{ .l = 0x3fefff75d15eebbd },
			{ .l = 0xbfc5111dc143ce68 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,1/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfff60252d49ca + x^2 * 0x3fa515acf89f1565)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfff60252d49ca },
			{ .l = 0x3fa515acf89f1565 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,1/2]); printexpansion(P);
		// x * (0x3fefffffcb410166 + x^2 * (0xbfc5551a9cf1bd2a + x^2 * 0x3f80ea15b1f2f7a0))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffcb410166 },
			{ .l = 0xbfc5551a9cf1bd2a },
			{ .l = 0x3f80ea15b1f2f7a0 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,1/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffb16bbaf0 + x^2 * (0x3fa5551aa48348a0 + x^2 * 0xbf568f466703f674))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffb16bbaf0 },
			{ .l = 0x3fa5551aa48348a0 },
			{ .l = 0xbf568f466703f674 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,1/2]); printexpansion(P);
		// x * (0x3feffffffff44386 + x^2 * (0xbfc555553e0fd8d4 + x^2 * (0x3f8110f41dfdfbb8 + x^2 * 0xbf29d36c017f6ab5)))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffff44386 },
			{ .l = 0xbfc555553e0fd8d4 },
			{ .l = 0x3f8110f41dfdfbb8 },
			{ .l = 0xbf29d36c017f6ab5 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,1/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffea9bf2 + x^2 * (0x3fa555553bbc31e7 + x^2 * (0xbf56c145d432b90b + x^2 * 0x3ef9d46842e0c8ab)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffea9bf2 },
			{ .l = 0x3fa555553bbc31e7 },
			{ .l = 0xbf56c145d432b90b },
			{ .l = 0x3ef9d46842e0c8ab },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,1/2]); printexpansion(P);
		// x * (0x3feffffffffffe4b + x^2 * (0xbfc55555555008a2 + x^2 * (0x3f8111110683f0c1 + x^2 * (0xbf2a0182a5b4124c + x^2 * 0x3ec6fc490d38e113))))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffffe4b },
			{ .l = 0xbfc55555555008a2 },
			{ .l = 0x3f8111110683f0c1 },
			{ .l = 0xbf2a0182a5b4124c },
			{ .l = 0x3ec6fc490d38e113 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,1/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffffc51 + x^2 * (0x3fa55555554ee56d + x^2 * (0xbf56c16c08173fe1 + x^2 * (0x3efa0182de85bab7 + x^2 * 0xbe9263ce4623f237))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffffffc51 },
			{ .l = 0x3fa55555554ee56d },
			{ .l = 0xbf56c16c08173fe1 },
			{ .l = 0x3efa0182de85bab7 },
			{ .l = 0xbe9263ce4623f237 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,1/2]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc55555555554c4 + x^2 * (0x3f811111110f171e + x^2 * (0xbf2a01a0110147ea + x^2 * (0x3ec71dd1b0a94220 + x^2 * 0xbe5ac5ad322a2455)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc55555555554c4 },
			{ .l = 0x3f811111110f171e },
			{ .l = 0xbf2a01a0110147ea },
			{ .l = 0x3ec71dd1b0a94220 },
			{ .l = 0xbe5ac5ad322a2455 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,1/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa55555555554e6 + x^2 * (0xbf56c16c16bf462e + x^2 * (0x3efa01a01244a8f5 + x^2 * (0xbe927e42b7a86f59 + x^2 * 0x3e21da2a5be70e80)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa55555555554e6 },
			{ .l = 0xbf56c16c16bf462e },
			{ .l = 0x3efa01a01244a8f5 },
			{ .l = 0xbe927e42b7a86f59 },
			{ .l = 0x3e21da2a5be70e80 },
		},
	},
};

static const DL
coefficient_1_4[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,1/4]); printexpansion(P);
		// 0x3caffffffffffffc
		{
			{ .l = 0x3caffffffffffffc },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,1/4]); printexpansion(P);
		// 0x3fef7ea79ea1580d
		{
			{ .l = 0x3fef7ea79ea1580d },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,1/4]); printexpansion(P);
		// x * 0x3fefd53e9620cb96
		{
			{ .l = 0 },
			{ .l = 0x3fefd53e9620cb96 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,1/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdfdca039a6d3b6
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfdca039a6d3b6 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,1/4]); printexpansion(P);
		// x * (0x3feffff770f1247f + x^2 * 0xbfc544451314e3b4)
		{
			{ .l = 0 },
			{ .l = 0x3feffff770f1247f },
			{ .l = 0xbfc544451314e3b4 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,1/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffff626f3eb5a + x^2 * 0x3fa545798afc8306)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffff626f3eb5a },
			{ .l = 0x3fa545798afc8306 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,1/4]); printexpansion(P);
		// x * (0x3fefffffff2f395f + x^2 * (0xbfc55551ac4a0f75 + x^2 * 0x3f810750e7fddd16))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffff2f395f },
			{ .l = 0xbfc55551ac4a0f75 },
			{ .l = 0x3f810750e7fddd16 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,1/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffecc7862 + x^2 * (0x3fa55551b3b99360 + x^2 * 0xbf56b4ec152898be))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffecc7862 },
			{ .l = 0x3fa55551b3b99360 },
			{ .l = 0xbf56b4ec152898be },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,1/4]); printexpansion(P);
		// x * (0x3feffffffffff466 + x^2 * (0xbfc5555554f8b6f3 + x^2 * (0x3f81110f4285fe1a + x^2 * 0xbf29f611a6d2eafd)))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffff466 },
			{ .l = 0xbfc5555554f8b6f3 },
			{ .l = 0x3f81110f4285fe1a },
			{ .l = 0xbf29f611a6d2eafd },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,1/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffeb2c + x^2 * (0x3fa5555554f0844d + x^2 * (0xbf56c169b755f9c7 + x^2 * 0x3ef9f659329ab61c)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffffeb2c },
			{ .l = 0x3fa5555554f0844d },
			{ .l = 0xbf56c169b755f9c7 },
			{ .l = 0x3ef9f659329ab61c },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,1/4]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555179 + x^2 * (0x3f81111110ec0b49 + x^2 * (0xbf2a019e5e0e139c + x^2 * 0x3ec715abd91e50c1))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555179 },
			{ .l = 0x3f81111110ec0b49 },
			{ .l = 0xbf2a019e5e0e139c },
			{ .l = 0x3ec715abd91e50c1 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,1/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffffff + x^2 * (0x3fa5555555554e9a + x^2 * (0xbf56c16c16853e78 + x^2 * (0x3efa019e3f950602 + x^2 * 0xbe9277a41514d9f3))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffffffff },
			{ .l = 0x3fa5555555554e9a },
			{ .l = 0xbf56c16c16853e78 },
			{ .l = 0x3efa019e3f950602 },
			{ .l = 0xbe9277a41514d9f3 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,1/4]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f81111111110d90 + x^2 * (0xbf2a01a019cd54ff + x^2 * (0x3ec71de23edd59d4 + x^2 * 0xbe5add43261d4d80)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f81111111110d90 },
			{ .l = 0xbf2a01a019cd54ff },
			{ .l = 0x3ec71de23edd59d4 },
			{ .l = 0xbe5add43261d4d80 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,1/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c16703 + x^2 * (0x3efa01a019c7b456 + x^2 * (0xbe927e4e7e24e98c + x^2 * 0x3e21e883acc1ad1a)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c16703 },
			{ .l = 0x3efa01a019c7b456 },
			{ .l = 0xbe927e4e7e24e98c },
			{ .l = 0x3e21e883acc1ad1a },
		},
	},
};

static const DL
coefficient_1_8[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,1/8]); printexpansion(P);
		// 0x3caffffffffffff8
		{
			{ .l = 0x3caffffffffffff8 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,1/8]); printexpansion(P);
		// 0x3fefdfea9e8d99ba
		{
			{ .l = 0x3fefdfea9e8d99ba },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,1/8]); printexpansion(P);
		// x * 0x3feff553e9474bd8
		{
			{ .l = 0 },
			{ .l = 0x3feff553e9474bd8 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,1/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdff729669f5772
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdff729669f5772 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,1/8]); printexpansion(P);
		// x * (0x3fefffff775d6ff1 + x^2 * 0xbfc551111e0cf544)
		{
			{ .l = 0 },
			{ .l = 0x3fefffff775d6ff1 },
			{ .l = 0xbfc551111e0cf544 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,1/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffff62fa83a4 + x^2 * 0x3fa5515f375e3a0a)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffff62fa83a4 },
			{ .l = 0x3fa5515f375e3a0a },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,1/8]); printexpansion(P);
		// x * (0x3feffffffffcbf13 + x^2 * (0xbfc555551ace6cac + x^2 * 0x3f810ea0f14703f3))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffcbf13 },
			{ .l = 0xbfc555551ace6cac },
			{ .l = 0x3f810ea0f14703f3 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,1/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffb3852 + x^2 * (0x3fa555551b5f6086 + x^2 * 0xbf56be4ca2572101))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffffb3852 },
			{ .l = 0x3fa555551b5f6086 },
			{ .l = 0xbf56be4ca2572101 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,1/8]); printexpansion(P);
		// x * (0x3feffffffffffff4 + x^2 * (0xbfc555555553dfde + x^2 * (0x3f811110f40f4de0 + x^2 * 0xbf29febb697032f6)))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffffff4 },
			{ .l = 0xbfc555555553dfde },
			{ .l = 0x3f811110f40f4de0 },
			{ .l = 0xbf29febb697032f6 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,1/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffffeb + x^2 * (0x3fa555555553c071 + x^2 * (0xbf56c16bf0b5ee82 + x^2 * 0x3ef9fecd9d6c2d7f)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffffffeb },
			{ .l = 0x3fa555555553c071 },
			{ .l = 0xbf56c16bf0b5ee82 },
			{ .l = 0x3ef9fecd9d6c2d7f },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,1/8]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555551 + x^2 * (0x3f8111111110733e + x^2 * (0xbf2a019ffd4ed5c7 + x^2 * 0x3ec71bce368f08dd))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555551 },
			{ .l = 0x3f8111111110733e },
			{ .l = 0xbf2a019ffd4ed5c7 },
			{ .l = 0x3ec71bce368f08dd },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,1/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555553 + x^2 * (0xbf56c16c16c0de90 + x^2 * (0x3efa01a0035d8556 + x^2 * 0xbe927cd1024246d2))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555553 },
			{ .l = 0xbf56c16c16c0de90 },
			{ .l = 0x3efa01a0035d8556 },
			{ .l = 0xbe927cd1024246d2 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,1/8]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f81111111110666 + x^2 * (0xbf2a01a0183a462b + x^2 * (0x3ec71dc49217dfbf + x^2 * 0xbe5966aa6e4b5daa)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f81111111110666 },
			{ .l = 0xbf2a01a0183a462b },
			{ .l = 0x3ec71dc49217dfbf },
			{ .l = 0xbe5966aa6e4b5daa },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,1/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c15aa9 + x^2 * (0x3efa01a0177eb9d5 + x^2 * (0xbe927e2871f90c81 + x^2 * 0x3e2032efc1dac7ab)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c15aa9 },
			{ .l = 0x3efa01a0177eb9d5 },
			{ .l = 0xbe927e2871f90c81 },
			{ .l = 0x3e2032efc1dac7ab },
		},
	},
};

static const DL
coefficient_1_16[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,1/16]); printexpansion(P);
		// 0x3caffffffffffff0
		{
			{ .l = 0x3caffffffffffff0 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,1/16]); printexpansion(P);
		// 0x3feff7feaa7a4958
		{
			{ .l = 0x3feff7feaa7a4958 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,1/16]); printexpansion(P);
		// x * 0x3feffd553e940bf8
		{
			{ .l = 0 },
			{ .l = 0x3feffd553e940bf8 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,1/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdffdca6ed5eb2a
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffdca6ed5eb2a },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,1/16]); printexpansion(P);
		// x * (0x3feffffff7770f6b + x^2 * 0xbfc5544445143d3c)
		{
			{ .l = 0 },
			{ .l = 0x3feffffff7770f6b },
			{ .l = 0xbfc5544445143d3c },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,1/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffff631ce89 + x^2 * 0x3fa55457dae4fceb)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffff631ce89 },
			{ .l = 0x3fa55457dae4fceb },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,1/16]); printexpansion(P);
		// x * (0x3feffffffffff2fe + x^2 * (0xbfc5555551ad0661 + x^2 * 0x3f8110750764da95))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffff2fe },
			{ .l = 0xbfc5555551ad0661 },
			{ .l = 0x3f8110750764da95 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,1/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffece8 + x^2 * (0x3fa5555551b68b7e + x^2 * 0xbf56c0a442ed54f7))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffffece8 },
			{ .l = 0x3fa5555551b68b7e },
			{ .l = 0xbf56c0a442ed54f7 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,1/16]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555100 + x^2 * (0x3f8111110f710599 + x^2 * 0xbf2a00edc91872bf)))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555100 },
			{ .l = 0x3f8111110f710599 },
			{ .l = 0xbf2a00edc91872bf },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,1/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa555555555525d + x^2 * (0xbf56c16c1507289d + x^2 * 0x3efa00ff98bd0bea)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa555555555525d },
			{ .l = 0xbf56c16c1507289d },
			{ .l = 0x3efa00ff98bd0bea },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,1/16]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110f491 + x^2 * (0xbf2a01a00e16af3e + x^2 * 0x3ec71c24233f1baf))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f8111111110f491 },
			{ .l = 0xbf2a01a00e16af3e },
			{ .l = 0x3ec71c24233f1baf },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,1/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c13ca9 + x^2 * (0x3efa01a008f40544 + x^2 * 0xbe927c3387462dde))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c13ca9 },
			{ .l = 0x3efa01a008f40544 },
			{ .l = 0xbe927c3387462dde },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,1/16]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110e667 + x^2 * (0xbf2a019ffd908d87 + x^2 * (0x3ec71621f6989956 + x^2 * 0x3e962f40fbe7c12a)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f8111111110e667 },
			{ .l = 0xbf2a019ffd908d87 },
			{ .l = 0x3ec71621f6989956 },
			{ .l = 0x3e962f40fbe7c12a },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,1/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c1265e + x^2 * (0x3efa019ff1d2cd8b + x^2 * (0xbe92747f2484ab45 + x^2 * 0xbe6a92f625100025)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c1265e },
			{ .l = 0x3efa019ff1d2cd8b },
			{ .l = 0xbe92747f2484ab45 },
			{ .l = 0xbe6a92f625100025 },
		},
	},
};

static const DL
coefficient_1_32[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,1/32]); printexpansion(P);
		// 0x3cafffffffffffe0
		{
			{ .l = 0x3cafffffffffffe0 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,1/32]); printexpansion(P);
		// 0x3feffdffeaa9e938
		{
			{ .l = 0x3feffdffeaa9e938 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,1/32]); printexpansion(P);
		// x * 0x3fefff5553e93f1f
		{
			{ .l = 0 },
			{ .l = 0x3fefff5553e93f1f },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,1/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdfff729d070cfb
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfff729d070cfb },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,1/32]); printexpansion(P);
		// x * (0x3fefffffff7775d7 + x^2 * 0xbfc55511111e0a46)
		{
			{ .l = 0 },
			{ .l = 0x3fefffffff7775d7 },
			{ .l = 0xbfc55511111e0a46 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,1/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffff63257b + x^2 * 0x3fa55515f7892315)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffff63257b },
			{ .l = 0x3fa55515f7892315 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,1/32]); printexpansion(P);
		// x * (0x3fefffffffffffcc + x^2 * (0xbfc55555551ad277 + x^2 * 0x3f8110ea0edae32e))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffffffcc },
			{ .l = 0xbfc55555551ad277 },
			{ .l = 0x3f8110ea0edae32e },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,1/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffffb4 + x^2 * (0x3fa55555551b8ad0 + x^2 * 0xbf56c13a2ceffbea))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffffffb4 },
			{ .l = 0x3fa55555551b8ad0 },
			{ .l = 0xbf56c13a2ceffbea },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,1/32]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555544 + x^2 * (0x3f81111110f71040 + x^2 * 0xbf2a01738588943a)))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555544 },
			{ .l = 0x3f81111110f71040 },
			{ .l = 0xbf2a01738588943a },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,1/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555549 + x^2 * (0xbf56c16c16a53449 + x^2 * 0x3efa01779ecbafb4)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555549 },
			{ .l = 0xbf56c16c16a53449 },
			{ .l = 0x3efa01779ecbafb4 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,1/32]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110a356 + x^2 * (0xbf2a019f6f43317c + x^2 * 0x3ec6cb0757c5b38c))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f8111111110a356 },
			{ .l = 0xbf2a019f6f43317c },
			{ .l = 0x3ec6cb0757c5b38c },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,1/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c0b21a + x^2 * (0x3efa019f18a85975 + x^2 * 0xbe920bf3d573e5aa))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c0b21a },
			{ .l = 0x3efa019f18a85975 },
			{ .l = 0xbe920bf3d573e5aa },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,1/32]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f81111111106667 + x^2 * (0xbf2a019e52e802c7 + x^2 * (0x3ec52d6babc0476c + x^2 * 0x3f17dca93b96e220)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f81111111106667 },
			{ .l = 0xbf2a019e52e802c7 },
			{ .l = 0x3ec52d6babc0476c },
			{ .l = 0x3f17dca93b96e220 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,1/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c0552c + x^2 * (0x3efa019d96f48c51 + x^2 * (0xbe900a00ab3bc693 + x^2 * 0xbeebb30848f08dad)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c0552c },
			{ .l = 0x3efa019d96f48c51 },
			{ .l = 0xbe900a00ab3bc693 },
			{ .l = 0xbeebb30848f08dad },
		},
	},
};

static const DL
coefficient_1_64[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,1/64]); printexpansion(P);
		// 0x3cafffffffffffc0
		{
			{ .l = 0x3cafffffffffffc0 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,1/64]); printexpansion(P);
		// 0x3fefff7ffeaaa7a5
		{
			{ .l = 0x3fefff7ffeaaa7a5 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,1/64]); printexpansion(P);
		// x * 0x3fefffd5553e93eb
		{
			{ .l = 0 },
			{ .l = 0x3fefffd5553e93eb },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,1/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdfffdca756d72b
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffdca756d72b },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,1/64]); printexpansion(P);
		// x * (0x3feffffffff77771 + x^2 * 0xbfc555444445174d)
		{
			{ .l = 0 },
			{ .l = 0x3feffffffff77771 },
			{ .l = 0xbfc555444445174d },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,1/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffff6327a + x^2 * 0x3fa555457def4aa7)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffff6327a },
			{ .l = 0x3fa555457def4aa7 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,1/64]); printexpansion(P);
		// x * (0x3fefffffffffffff + x^2 * (0xbfc5555555517d28 + x^2 * 0x3f8111072a1a5f9d))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffffffff },
			{ .l = 0xbfc5555555517d28 },
			{ .l = 0x3f8111072a1a5f9d },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,1/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffffff + x^2 * (0x3fa555555551fcfb + x^2 * 0xbf56c15ff722cac4))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffffffff },
			{ .l = 0x3fa555555551fcfb },
			{ .l = 0xbf56c15ff722cac4 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,1/64]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110f434d + x^2 * 0xbf2a01947af8f235)))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f811111110f434d },
			{ .l = 0xbf2a01947af8f235 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,1/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16bef17b + x^2 * 0x3efa019438b6b71e)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16bef17b },
			{ .l = 0x3efa019438b6b71e },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,1/64]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110f5a36 + x^2 * (0xbf2a01956f5a4ca4 + x^2 * 0x3e93708ba3d4b322))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f811111110f5a36 },
			{ .l = 0xbf2a01956f5a4ca4 },
			{ .l = 0x3e93708ba3d4b322 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,1/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16be842d + x^2 * (0x3efa0190052e0964 + x^2 * 0x3e84280111786715))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16be842d },
			{ .l = 0x3efa0190052e0964 },
			{ .l = 0x3e84280111786715 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,1/64]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110e6667 + x^2 * (0xbf2a0183a845d588 + x^2 * (0xbee94038712ab510 + x^2 * 0x3f97de841c776248)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f811111110e6667 },
			{ .l = 0xbf2a0183a845d588 },
			{ .l = 0xbee94038712ab510 },
			{ .l = 0x3f97de841c776248 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,1/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16bd1066 + x^2 * (0x3efa0177e8d09b7f + x^2 * (0x3ec152ee8966ae35 + x^2 * 0xbf6bb495164a48dc)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16bd1066 },
			{ .l = 0x3efa0177e8d09b7f },
			{ .l = 0x3ec152ee8966ae35 },
			{ .l = 0xbf6bb495164a48dc },
		},
	},
};

static const DL
coefficient_1_128[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,1/128]); printexpansion(P);
		// 0x3cafffffffffff80
		{
			{ .l = 0x3cafffffffffff80 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,1/128]); printexpansion(P);
		// 0x3fefffdfffeaaa9f
		{
			{ .l = 0x3fefffdfffeaaa9f },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,1/128]); printexpansion(P);
		// x * 0x3feffff55553e93f
		{
			{ .l = 0 },
			{ .l = 0x3feffff55553e93f },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,1/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdffff729d706f5
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffff729d706f5 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,1/128]); printexpansion(P);
		// x * (0x3fefffffffff7777 + x^2 * 0xbfc555511110a175)
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffff7777 },
			{ .l = 0xbfc555511110a175 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,1/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffff6328 + x^2 * 0x3fa555515f7c44ac)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffff6328 },
			{ .l = 0x3fa555515f7c44ac },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,1/128]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc55555555527d2 + x^2 * 0x3f81110eca841784))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc55555555527d2 },
			{ .l = 0x3f81110eca841784 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,1/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555553694 + x^2 * 0xbf56c16987f76b85))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555553694 },
			{ .l = 0xbf56c16987f76b85 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,1/128]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110d3435 + x^2 * 0xbf2a01753242af9f)))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f811111110d3435 },
			{ .l = 0xbf2a01753242af9f },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,1/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16ba974d + x^2 * 0x3efa015b6fa1d53c)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16ba974d },
			{ .l = 0x3efa015b6fa1d53c },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,1/128]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110a35a4 + x^2 * (0xbf2a00f56f7232e2 + x^2 * 0xbf24535aca865184))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f811111110a35a4 },
			{ .l = 0xbf2a00f56f7232e2 },
			{ .l = 0xbf24535aca865184 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,1/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16b5cc6b + x^2 * (0x3efa009ecc14ceba + x^2 * 0x3efc486efc52a465))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16b5cc6b },
			{ .l = 0x3efa009ecc14ceba },
			{ .l = 0x3efc486efc52a465 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,1/128]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f81111111066667 + x^2 * (0xbf29ffd8fdbd20c2 + x^2 * (0xbf4ef09fe74c17f3 + x^2 * 0x4017de915454aa82)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f81111111066667 },
			{ .l = 0xbf29ffd8fdbd20c2 },
			{ .l = 0xbf4ef09fe74c17f3 },
			{ .l = 0x4017de915454aa82 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,1/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16affd4d + x^2 * (0x3ef9ff1d05323bde + x^2 * (0x3f23998bf649f3f3 + x^2 * 0xbfebb4b64ef972c0)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16affd4d },
			{ .l = 0x3ef9ff1d05323bde },
			{ .l = 0x3f23998bf649f3f3 },
			{ .l = 0xbfebb4b64ef972c0 },
		},
	},
};


// link for all coefficients array
static const DL *link[COEFFICIENTS_VERSION];

static const double
interpolate[2][BITNUM] = {
	{
		0.0000000000000000000e+00,
		7.8124205273828314575e-03,
		1.5624364224883372301e-02,
		2.3435354291722571701e-02,
		3.1244913985326080302e-02,
		3.9052566650422150529e-02,
		4.6857835748134242515e-02,
		5.4660244885066461284e-02,
		6.2459317842380200625e-02,
		7.0254578604860049018e-02,
		7.8045551389967313116e-02,
		8.5831760676879351180e-02,
		9.3612731235512891548e-02,
		1.0138798815552964305e-01,
		1.0915705687532235857e-01,
		1.1691946321097965167e-01,
		1.2467473338522769288e-01,
		1.3242239405634717575e-01,
		1.4016197234706370689e-01,
		1.4789299587340962172e-01,
		1.5561499277355603232e-01,
		1.6332749173661284536e-01,
		1.7103002203139502924e-01,
		1.7872211353515365939e-01,
		1.8640329676226988220e-01,
		1.9407310289290979899e-01,
		2.0173106380163879914e-01,
		2.0937671208599364925e-01,
		2.1700958109501014559e-01,
		2.2462920495770530294e-01,
		2.3223511861151147073e-01,
		2.3982685783066157170e-01,
		2.4740395925452293713e-01,
		2.5496596041587848980e-01,
		2.6251239976915330399e-01,
		2.7004281671858504366e-01,
		2.7755675164633630825e-01,
		2.8505374594054744275e-01,
		2.9253334202332753611e-01,
		2.9999508337868302510e-01,
		3.0743851458038085056e-01,
		3.1486318131974527779e-01,
		3.2226863043338660475e-01,
		3.2965440993086014831e-01,
		3.3702006902225306595e-01,
		3.4436515814569840233e-01,
		3.5168922899481408484e-01,
		3.5899183454606503618e-01,
		3.6627252908604757131e-01,
		3.7353086823869297017e-01,
		3.8076640899239017068e-01,
		3.8797870972702502845e-01,
		3.9516733024093425586e-01,
		4.0233183177777309680e-01,
		4.0947177705329507180e-01,
		4.1658673028204112887e-01,
		4.2367625720393803368e-01,
		4.3073992511080322343e-01,
		4.3777730287275512522e-01,
		4.4478796096452721809e-01,
		4.5177147149168378482e-01,
		4.5872740821673657630e-01,
		4.6565534658516016808e-01,
		4.7255486375130445476e-01,
		4.7942553860420300538e-01,
		4.8626695179327555918e-01,
		4.9307868575392305166e-01,
		4.9986032473301345025e-01,
		5.0661145481425740034e-01,
		5.1333166394347118366e-01,
		5.2002054195372704459e-01,
		5.2667768059038677642e-01,
		5.3330267353602012204e-01,
		5.3989511643520438078e-01,
		5.4645460691920355600e-01,
		5.5298074463052737659e-01,
		5.5947313124736686163e-01,
		5.6593137050790598419e-01,
		5.7235506823450721381e-01,
		5.7874383235777038248e-01,
		5.8509727294046220969e-01,
		5.9141500220131626442e-01,
		5.9769663453870147674e-01,
		6.0394178655415664547e-01,
		6.1015007707579138607e-01,
		6.1632112718155096509e-01,
		6.2245456022234368909e-01,
		6.2855000184502962668e-01,
		6.3460708001526933142e-01,
		6.4062542504023045620e-01,
		6.4660466959115237007e-01,
		6.5254444872576600201e-01,
		6.5844439991056757933e-01,
		6.6430416304294626073e-01,
		6.7012338047316288847e-01,
		6.7590169702617886038e-01,
		6.8163876002333412263e-01,
		6.8733421930387350596e-01,
		6.9298772724631796383e-01,
		6.9859893878968171155e-01,
		7.0416751145453371219e-01,
		7.0969310536389973443e-01,
		7.1517538326400764692e-01,
		7.2061401054487106421e-01,
		7.2600865526071256539e-01,
		7.3135898815022359898e-01,
		7.3666468265666140702e-01,
		7.4192541494777963784e-01,
		7.4714086393559420163e-01,
		7.5231071129598037217e-01,
		7.5743464148810146774e-01,
		7.6251234177366833400e-01,
		7.6754350223602707537e-01,
		7.7252781579907447984e-01,
		7.7746497824600080406e-01,
		7.8235468823785747627e-01,
		7.8719664733194893991e-01,
		7.9199056000004852685e-01,
		7.9673613364643569579e-01,
		8.0143307862575474676e-01,
		8.0608110826069301336e-01,
		8.1067993885947842170e-01,
		8.1522928973319441770e-01,
		8.1972888321291192959e-01,
		8.2417844466663670033e-01,
		8.2857770251607154588e-01,
		8.3292638825319198492e-01,
		8.3722423645663435199e-01,		
	},
	{
		1.0000000000000000000e+00,
		9.9996948257709505903e-01,
		9.9987793217100662257e-01,
		9.9972535436949949705e-01,
		9.9951175848513640343e-01,
		9.9923715755470898792e-01,
		9.9890156833844290230e-01,
		9.9850501131897406726e-01,
		9.9804751070009911640e-01,
		9.9752909440529780039e-01,
		9.9694979407602868360e-01,
		9.9630964506979802220e-01,
		9.9560868645800171262e-01,
		9.9484696102354064351e-01,
		9.9402451525820911815e-01,
		9.9314139935985767949e-01,
		9.9219766722932900560e-01,
		9.9119337646716820789e-01,
		9.9012858837010708779e-01,
		9.8900336792732290725e-01,
		9.8781778381647189491e-01,
		9.8657190839949759908e-01,
		9.8526581771821386546e-01,
		9.8389959148966399383e-01,
		9.8247331310125529669e-01,
		9.8098706960566917079e-01,
		9.7944095171554834689e-01,
		9.7783505379795976342e-01,
		9.7616947386863528457e-01,
		9.7444431358598893045e-01,
		9.7265967824491272875e-01,
		9.7081567677034941166e-01,
		9.6891242171064473343e-01,
		9.6695002923067785439e-01,
		9.6492861910477101262e-01,
		9.6284831470937970455e-01,
		9.6070924301556193114e-01,
		9.5851153458122861917e-01,
		9.5625532354317532846e-01,
		9.5394074760889469022e-01,
		9.5156794804817224076e-01,
		9.4913706968446298617e-01,
		9.4664826088605336096e-01,
		9.4410167355700436165e-01,
		9.4149746312788107350e-01,
		9.3883578854626548171e-01,
		9.3611681226705534264e-01,
		9.3334070024254844888e-01,
		9.3050762191231428666e-01,
		9.2761775019285186428e-01,
		9.2467126146703604306e-01,
		9.2166833557335192673e-01,
		9.1860915579491830840e-01,
		9.1549390884830117443e-01,
		9.1232278487211782014e-01,
		9.0909597741543102245e-01,
		9.0581368342593637788e-01,
		9.0247610323794147380e-01,
		8.9908344056013844714e-01,
		8.9563590246317070775e-01,
		8.9213369936699438156e-01,
		8.8857704502803558366e-01,
		8.8496615652614329939e-01,
		8.8130125425134064887e-01,
		8.7758256189037275874e-01,
		8.7381030641305446061e-01,
		8.6998471805841737226e-01,
		8.6610603032065669460e-01,
		8.6217447993488049995e-01,
		8.5819030686266040142e-01,
		8.5415375427738537972e-01,
		8.5006506854942021079e-01,
		8.4592449923106793896e-01,
		8.4173229904133839430e-01,
		8.3748872385052364198e-01,
		8.3319403266458136326e-01,
		8.2884848760932572365e-01,
		8.2445235391442917017e-01,
		8.2000589989723404738e-01,
		8.1550939694637547550e-01,
		8.1096311950521793310e-01,
		8.0636734505510387905e-01,
		8.0172235409841841047e-01,
		7.9702843014146829148e-01,
		7.9228585967717857219e-01,
		7.8749493216760613201e-01,
		7.8265594002627281167e-01,
		7.7776917860031791196e-01,
		7.7283494615247150250e-01,
		7.6785354384285031681e-01,
		7.6282527571057623383e-01,
		7.5775044865521934412e-01,
		7.5262937241806648903e-01,
		7.4746235956321616101e-01,
		7.4224972545850131933e-01,
		7.3699178825624078737e-01,
		7.3168886887382089679e-01,
		7.2634129097410859988e-01,
		7.2094938094569638309e-01,
		7.1551346788298153534e-01,
		7.1003388356607965992e-01,
		7.0451096244057465068e-01,
		6.9894504159710568736e-01,
		6.9333646075079335969e-01,
		6.8768556222050480908e-01,
		6.8199269090796055259e-01,
		6.7625819427668332207e-01,
		6.7048242233079080599e-01,
		6.6466572759363329315e-01,
		6.5880846508627688429e-01,
		6.5291099230583582447e-01,
		6.4697366920365195764e-01,
		6.4099685816332507837e-01,
		6.3498092397859584590e-01,
		6.2892623383107926216e-01,
		6.2283315726785515309e-01,
		6.1670206617891198952e-01,
		6.1053333477444904354e-01,
		6.0432733956203643633e-01,
		5.9808445932363496489e-01,
		5.9180507509247748388e-01,
		5.8548957012981228676e-01,
		5.7913832990151103974e-01,
		5.7275174205454160159e-01,
		5.6633019639330872685e-01,
		5.5987408485586143136e-01,
		5.5338380148997212693e-01,
		5.4685974242908530485e-01,
	}
};

static const DL invpio[BIT + 1] =
{
	{.l = 0x3fe45f306dc9c883}, // invpio(2*1)	2/pi
	{.l = 0x3ff45f306dc9c883}, // invpio(2*2)	4/pi
	{.l = 0x40045f306dc9c883}, // invpio(2*4)	8/pi
	{.l = 0x40145f306dc9c883}, // invpio(2*8)	16/pi
	{.l = 0x40245f306dc9c883}, // invpio(2*16)	32/pi
	{.l = 0x40345f306dc9c883}, // invpio(2*32)	64/pi
	{.l = 0x40445f306dc9c883}, // invpio(2*64)	128/pi
	{.l = 0x40545f306dc9c883}, // invpio(2*128)	256/pi
};

static const double
pi = 3.1415926535897932384626433832795,
pi_2 = 1.5707963267948966192313216916398,
two_pi = 6.283185307179586476925286766559,
invpio2 = 6.36619772367581382433e-01;

//extern double sin_gen(double);

int gen(struct constraint input_parameter) {
	double a, b;
	int precision;
	double midpoint, length;
	double a1, b1, a2, b2;
	double X;
	long int Xtemp, atemp, btemp, i;
	FILE *func;
	// init
	int num, bit, bitnum, bitnum_1, degree, fnum, format;
	num = 100;
	
	if ((func = fopen("sin0-1_gen.c", "w")) == (FILE *)0) {
		printf("open file error!\n");
		return 1;
	}
	a = input_parameter.start;
	b = input_parameter.end;
	precision = input_parameter.precision;
	bit = input_parameter.bit;
	//bit = 7;
	bitnum = 1 << bit;
	bitnum_1 = bitnum - 1;
	fnum = input_parameter.fnum;
	degree = input_parameter.degree;
	// format --> float number format: double is 64, float is 32
	format = 64;

	// test
	printf("After initial:\n");
	printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);
	printf("bit = %d\n", bit);
	printf("degree = %d\n", degree);
	printf("fnum = %d\n", fnum);

	// generate code for sin_gen
	{
		// comments
		fprintf(func, "/** target func:\tsin\n");
		fprintf(func, "*** target domain:\t[%lf, %lf]\n", a, b);
		fprintf(func, "*** target precision:\t%d\n", precision);
		fprintf(func, "**/\n\n");

		fprintf(func, "#include <stdio.h>\n");
		fprintf(func, "#include %cmyhead.h%c\n", '"', '"');
		fprintf(func, "\n");
		fprintf(func, "#define NUM %d\n", num);
		fprintf(func, "#define BIT %d\n", bit);
		fprintf(func, "#define BITNUM %d\n", bitnum);
		fprintf(func, "#define BITNUM_1 %d\n", bitnum_1);
		fprintf(func, "#define DEGREE %d\n", degree + 1);

		// degree
		fprintf(func, "static const DL\n");
		fprintf(func, "coefficient[2][DEGREE] = {\n");
		fprintf(func, "\t{\n");
		for (i = 0; i < degree; i++) {
			fprintf(func, "\t\t{.l = 0x%lx},\n", link[bit][degree * 2 * COEFFICIENTS + 0 * COEFFICIENTS + i].l);
			//fprintf(func, "\t\t{.l = 0x%lx},\n", coefficient[degree][0][i].l);
		}
		fprintf(func, "\t\t{.l = 0x%lx}\n", link[bit][degree * 2 * COEFFICIENTS + 0 * COEFFICIENTS + i].l);
		//fprintf(func, "\t\t{.l = 0x%lx}\n", coefficient[degree][0][i].l);
		fprintf(func, "\t},\n");
		fprintf(func, "\t{\n");
		for (i = 0; i < degree; i++) {
			fprintf(func, "\t\t{.l = 0x%lx},\n", link[bit][degree * 2 * COEFFICIENTS + 1 * COEFFICIENTS + i].l);
			//fprintf(func, "\t\t{.l = 0x%lx},\n", coefficient[degree][1][i].l);
		}
		fprintf(func, "\t\t{.l = 0x%lx}\n", link[bit][degree * 2 * COEFFICIENTS + 1 * COEFFICIENTS + i].l);
		//fprintf(func, "\t\t{.l = 0x%lx}\n", coefficient[degree][1][i].l);
		fprintf(func, "\t}\n");
		fprintf(func, "};\n");

		// interpolate
		fprintf(func, "static const double\n");
		fprintf(func, "interpolate[2][BITNUM] = {\n");
		fprintf(func, "\t{\n");
		for (i = 0; i < bitnum - 1; i++) {
			fprintf(func, "\t\t%.17e,\n", interpolate[0][i * (1 << (7 - bit))]);
		}
		fprintf(func, "\t\t%.17e\n", interpolate[0][i * (1 << (BIT - bit))]);
		fprintf(func, "\t},\n");
		fprintf(func, "\t{\n");
		for (i = 0; i < bitnum - 1; i++) {
			fprintf(func, "\t\t%.17e,\n", interpolate[1][i * (1 << (7 - bit))]);
		}
		fprintf(func, "\t\t%.17e\n", interpolate[1][i * (1 << (BIT - bit))]);
		fprintf(func, "\t}\n");
		fprintf(func, "};\n\n");

		// func
		//fprintf(func, "double sin_gen(double x) {\n");
		//fprintf(func, "\tdouble ix, iix, iiix, y, appro_s, appro_c;\n");
		fprintf(func, "double sin_gen(double x) {\n");
		fprintf(func, "\tdouble iiix, y, appro_s, appro_c, z;\n");
		fprintf(func, "\tlong status_pi_2 = 0, table_order;\n");
		fprintf(func, "\n");
		//fprintf(func, "\tix = x - X;\n");
		fprintf(func, "\t// if x is too little, then return x;\n");
		fprintf(func, "\tif (x < 1e-10) {\n");
		fprintf(func, "\t\treturn x;\n");
		fprintf(func, "\t}\n");
		fprintf(func, "\n");
		
		fprintf(func, "\ttable_order = (long)(x * BITNUM);\n");
		fprintf(func, "\tiiix = x - ((double)table_order) / BITNUM;\n");
		fprintf(func, "\tz = iiix * iiix;\n");
		fprintf(func, "\n");
		
		fprintf(func, "\t// new, sin(x+x') = sin(x)cos(x') + sin(x')cos(x), x' = iiix\n");
		fprintf(func, "\t// OR, cos(x+x') = cos(x)cos(x') - sin(x)sin(x'), x' = iiix\n");

		if (degree == 0) {
			fprintf(func, "\tappro_s = coefficient[0][0].d;\n");
			fprintf(func, "\tappro_c = coefficient[1][0].d;\n");
			fprintf(func, "\n");
			fprintf(func, "\ty = interpolate[status_pi_2][table_order] * appro_c + interpolate[1 - status_pi_2][table_order] * appro_s;\n");
		}
		else {
			//fprintf(func, "\tappro_s = coefficient[0][0].d");
			fprintf(func, "\tappro_s = 0.0");
			for (i = 1; i <= degree; i++) {
				if (i == 1) {
					fprintf(func, " + iiix * (coefficient[0][%ld].d", i);
				}
				else {
					fprintf(func, " + z * (coefficient[0][%ld].d", i);
				}
			}
			for (i = 1; i <= degree; i++) {
				fprintf(func, ")");
			}
			fprintf(func, ";\n");
			//fprintf(func, "\tappro_c = coefficient[1][0].d");
			fprintf(func, "\tappro_c = 0.0");
			for (i = 1; i <= degree; i++) {
				fprintf(func, " + z * (coefficient[1][%ld].d", i);
			}
			for (i = 1; i <= degree; i++) {
				fprintf(func, ")");
			}
			fprintf(func, ";\n");

			//fprintf(func, "\tappro_s = coefficient[0][0].d + iiix * (coefficient[0][1].d + iiix * (coefficient[0][2].d + iiix * (coefficient[0][3].d + iiix * (coefficient[0][4].d + iiix * coefficient[0][5].d))));\n");
			//fprintf(func, "\tappro_c = coefficient[1][0].d + iiix * (coefficient[1][1].d + iiix * (coefficient[1][2].d + iiix * (coefficient[1][3].d + iiix * (coefficient[1][4].d + iiix * coefficient[1][5].d))));\n");
			fprintf(func, "\n");
			fprintf(func, "\ty = interpolate[status_pi_2][table_order] * appro_c + interpolate[1 - status_pi_2][table_order] * appro_s;\n");
			fprintf(func, "\ty += interpolate[status_pi_2][table_order] * coefficient[1][0].d;\n");
		}
		
		fprintf(func, "\n");
		fprintf(func, "\treturn y;\n");
		fprintf(func, "}\n");
		fprintf(func, "\n");
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
		printf("please input degree: ");
		scanf("%d", &degree);
		fnum = 1;
		
		// test
		// printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);
		// printf("bit = %d\n", bit);
		// printf("degree = %d\n", degree);
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

	link[7] = &(coefficient_1_128[0][0][0]);
	link[6] = &(coefficient_1_64[0][0][0]);
	link[5] = &(coefficient_1_32[0][0][0]);
	link[4] = &(coefficient_1_16[0][0][0]);
	link[3] = &(coefficient_1_8[0][0][0]);
	link[2] = &(coefficient_1_4[0][0][0]);
	link[1] = &(coefficient_1_2[0][0][0]);
	link[0] = &(coefficient_1_1[0][0][0]);

	input_parameter.start = a;
	input_parameter.end = b;
	input_parameter.precision = precision;
	input_parameter.bit = bit;
	input_parameter.fnum = fnum;
	input_parameter.degree = degree;
	gen(input_parameter);

	return 0;
}