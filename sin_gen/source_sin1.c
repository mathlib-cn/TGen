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
coefficient_pi_2_1[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,pi/(2*1)]); printexpansion(P);
		// 0x3cafffffffffffff
		{
			{.l = 0x3cafffffffffffff },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,pi/(2*1)-(1b-53)]); printexpansion(P);
		// 0x3cafffffffffffff
		{
			{.l = 0x3cafffffffffffff },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,pi/(2*1)]); printexpansion(P);
		// x * 0x3fe8e51f5e5f5530
		{
			{.l = 0 },
			{.l = 0x3fe8e51f5e5f5530 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,pi/(2*1)-(1b-53)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfd9f02f6222c720
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfd9f02f6222c720 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,pi/(2*1)]); printexpansion(P);
		// x * (0x3fefc4eac57b4a27 + x^2 * 0xbfc2b704cf682899)
		{
			{.l = 0 },
			{.l = 0x3fefc4eac57b4a27 },
			{.l = 0xbfc2b704cf682899 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,pi/(2*1)-(1b-53)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdf947ad6e09cd9 + x^2 * 0x3fa24ace0eefebce)
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdf947ad6e09cd9 },
			{.l = 0x3fa24ace0eefebce },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,pi/(2*1)]); printexpansion(P);
		// x * (0x3fefff1d21fa9ded + x^2 * (0xbfc53e2e5c7dd831 + x^2 * 0x3f7f2438d36d9dbb))
		{
			{.l = 0 },
			{.l = 0x3fefff1d21fa9ded },
			{.l = 0xbfc53e2e5c7dd831 },
			{.l = 0x3f7f2438d36d9dbb },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,pi/(2*1)-(1b-53)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffd6eb82a11b7 + x^2 * (0x3fa532935163caa2 + x^2 * 0xbf5470d71b78edae))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffd6eb82a11b7 },
			{.l = 0x3fa532935163caa2 },
			{.l = 0xbf5470d71b78edae },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,pi/(2*1)]); printexpansion(P);
		// x * (0x3feffffe07d31fe8 + x^2 * (0xbfc554f800fc5ea1 + x^2 * (0x3f8105d44e6222a0 + x^2 * 0xbf283b9725dff6e8)))
		{
			{.l = 0 },
			{.l = 0x3feffffe07d31fe8 },
			{.l = 0xbfc554f800fc5ea1 },
			{.l = 0x3f8105d44e6222a0 },
			{.l = 0xbf283b9725dff6e8 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,pi/(2*1)-(1b-53)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffff81b53925b + x^2 * (0x3fa554a4321d8ac3 + x^2 * (0xbf56aca372ace16b + x^2 * 0x3ef7fa834896fd9b)))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffff81b53925b },
			{.l = 0x3fa554a4321d8ac3 },
			{.l = 0xbf56aca372ace16b },
			{.l = 0x3ef7fa834896fd9b },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,pi/(2*1)]); printexpansion(P);
		// x * (0x3feffffffd25a681 + x^2 * (0xbfc555547ef5150b + x^2 * (0x3f8110e7b396c557 + x^2 * (0xbf29f6445023f795 + x^2 * 0x3ec5d38b56aee7f1))))
		{
			{.l = 0 },
			{.l = 0x3feffffffd25a681 },
			{.l = 0xbfc555547ef5150b },
			{.l = 0x3f8110e7b396c557 },
			{.l = 0xbf29f6445023f795 },
			{.l = 0x3ec5d38b56aee7f1 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,pi/(2*1)-(1b-53)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffff15a6509 + x^2 * (0x3fa5555363daa6cc + x^2 * (0xbf56c1118453742d + x^2 * (0x3ef9f2a4982141fb + x^2 * 0xbe91548125ee8007))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffff15a6509 },
			{.l = 0x3fa5555363daa6cc },
			{.l = 0xbf56c1118453742d },
			{.l = 0x3ef9f2a4982141fb },
			{.l = 0xbe91548125ee8007 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,pi/(2*1)]); printexpansion(P);
		// x * (0x3feffffffffd17d1 + x^2 * (0xbfc55555541759fa + x^2 * (0x3f811110b74adb14 + x^2 * (0xbf2a017a8fe15033 + x^2 * (0x3ec716ba4fe56f6e + x^2 * 0xbe59a0e192a4e2cb)))))
		{
			{.l = 0 },
			{.l = 0x3feffffffffd17d1 },
			{.l = 0xbfc55555541759fa },
			{.l = 0x3f811110b74adb14 },
			{.l = 0xbf2a017a8fe15033 },
			{.l = 0x3ec716ba4fe56f6e },
			{.l = 0xbe59a0e192a4e2cb },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,pi/(2*1)-(1b-53)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffedc722 + x^2 * (0x3fa5555551e8a1dd + x^2 * (0xbf56c16b31e01e7e + x^2 * (0x3efa0167633e54cf + x^2 * (0xbe927705d4832421 + x^2 * 0x3e20fd66974dfc51)))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffffffedc722 },
			{.l = 0x3fa5555551e8a1dd },
			{.l = 0xbf56c16b31e01e7e },
			{.l = 0x3efa0167633e54cf },
			{.l = 0xbe927705d4832421 },
			{.l = 0x3e20fd66974dfc51 },
		},
	},
};

static const DL
coefficient_pi_2_2[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,pi/(2*2)]); printexpansion(P);
		// 0x3cafffffffffffff
		{
			{.l = 0x3cafffffffffffff },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,pi/(2*2)]); printexpansion(P);
		// 0x3fea827999fcef32
		{
			{.l = 0x3fea827999fcef32 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,pi/(2*2)]); printexpansion(P);
		// x * 0x3fee5247082cdbdb
		{
			{.l = 0 },
			{.l = 0x3fee5247082cdbdb },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,pi/(2*2)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfde9ff2d35b1f8f
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfde9ff2d35b1f8f },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,pi/(2*2)]); printexpansion(P);
		// x * (0x3feffca77813835f + x^2 * 0xbfc4ad2ea064b3fc)
		{
			{.l = 0 },
			{.l = 0x3feffca77813835f },
			{.l = 0xbfc4ad2ea064b3fc },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,pi/(2*2)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffc13b421689c + x^2 * 0x3fa4b6f862a96803)
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffc13b421689c },
			{.l = 0x3fa4b6f862a96803 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,pi/(2*2)]); printexpansion(P);
		// x * (0x3feffffcd6e00185 + x^2 * (0xbfc553edef7432ce + x^2 * 0x3f80b0fa12fa067a))
		{
			{.l = 0 },
			{.l = 0x3feffffcd6e00185 },
			{.l = 0xbfc553edef7432ce },
			{.l = 0x3f80b0fa12fa067a },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,pi/(2*2)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffb2a77e2f5 + x^2 * (0x3fa553e7f02aec72 + x^2 * 0xbf5644d629940d0e))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffb2a77e2f5 },
			{.l = 0x3fa553e7f02aec72 },
			{.l = 0xbf5644d629940d0e },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,pi/(2*2)]); printexpansion(P);
		// x * (0x3feffffffe42f1f6 + x^2 * (0xbfc55553f43c204f + x^2 * (0x3f8110605166e038 + x^2 * 0xbf298fb9c115f731)))
		{
			{.l = 0 },
			{.l = 0x3feffffffe42f1f6 },
			{.l = 0xbfc55553f43c204f },
			{.l = 0x3f8110605166e038 },
			{.l = 0xbf298fb9c115f731 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,pi/(2*2)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffcb82e97 + x^2 * (0x3fa55553c7899625 + x^2 * (0xbf56c07f169439bc + x^2 * 0x3ef99169fc8e4c77)))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffffcb82e97 },
			{.l = 0x3fa55553c7899625 },
			{.l = 0xbf56c07f169439bc },
			{.l = 0x3ef99169fc8e4c77 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,pi/(2*2)]); printexpansion(P);
		// x * (0x3fefffffffff5fec + x^2 * (0xbfc55555548e3564 + x^2 * (0x3f8111107187297e + x^2 * (0xbf2a00ec71bab5e5 + x^2 * 0x3ec6cb0a8ccc2f2e))))
		{
			{.l = 0 },
			{.l = 0x3fefffffffff5fec },
			{.l = 0xbfc55555548e3564 },
			{.l = 0x3f8111107187297e },
			{.l = 0xbf2a00ec71bab5e5 },
			{.l = 0x3ec6cb0a8ccc2f2e },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,pi/(2*2)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffe98af + x^2 * (0x3fa55555545c5146 + x^2 * (0xbf56c16b348bbf59 + x^2 * (0x3efa00eb9af29a14 + x^2 * 0xbe923c97e5fa4c61))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffffffe98af },
			{.l = 0x3fa55555545c5146 },
			{.l = 0xbf56c16b348bbf59 },
			{.l = 0x3efa00eb9af29a14 },
			{.l = 0xbe923c97e5fa4c61 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,pi/(2*2)]); printexpansion(P);
		// x * (0x3fefffffffffffd7 + x^2 * (0xbfc5555555550c42 + x^2 * (0x3f81111110bbbf03 + x^2 * (0xbf2a019f87b803db + x^2 * (0x3ec71d71fc9791be + x^2 * 0xbe5a94acae81e084)))))
		{
			{.l = 0 },
			{.l = 0x3fefffffffffffd7 },
			{.l = 0xbfc5555555550c42 },
			{.l = 0x3f81111110bbbf03 },
			{.l = 0xbf2a019f87b803db },
			{.l = 0x3ec71d71fc9791be },
			{.l = 0xbe5a94acae81e084 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,pi/(2*2)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffff96 + x^2 * (0x3fa555555554f070 + x^2 * (0xbf56c16c16407c8a + x^2 * (0x3efa019f81aa19a1 + x^2 * (0xbe927df45565800c + x^2 * 0x3e21b8b6b70f96c1)))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffffffffff96 },
			{.l = 0x3fa555555554f070 },
			{.l = 0xbf56c16c16407c8a },
			{.l = 0x3efa019f81aa19a1 },
			{.l = 0xbe927df45565800c },
			{.l = 0x3e21b8b6b70f96c1 },
		},
	},
};

static const DL
coefficient_pi_2_4[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,pi/(2*4)]); printexpansion(P);
		// 0x3caffffffffffffd
		{
			{.l = 0x3caffffffffffffd },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,pi/(2*4)]); printexpansion(P);
		// 0x3feebbdfcfc213ab
		{
			{.l = 0x3feebbdfcfc213ab },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,pi/(2*4)]); printexpansion(P);
		// x * 0x3fef962eff4dd79d
		{
			{.l = 0 },
			{.l = 0x3fef962eff4dd79d },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,pi/(2*4)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdfa89cd07d6136
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfa89cd07d6136 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,pi/(2*4)]); printexpansion(P);
		// x * (0x3fefffcba9e28f03 + x^2 * 0xbfc52b3df7cee2e9)
		{
			{.l = 0 },
			{.l = 0x3fefffcba9e28f03 },
			{.l = 0xbfc52b3df7cee2e9 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,pi/(2*4)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffc3a0e801bd + x^2 * 0x3fa52e237614898e)
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffc3a0e801bd },
			{.l = 0x3fa52e237614898e },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,pi/(2*4)]); printexpansion(P);
		// x * (0x3feffffff3afb28c + x^2 * (0xbfc5553f0532cbe2 + x^2 * 0x3f80f903a4410e80))
		{
			{.l = 0 },
			{.l = 0x3feffffff3afb28c },
			{.l = 0xbfc5553f0532cbe2 },
			{.l = 0x3f80f903a4410e80 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,pi/(2*4)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffedc39945 + x^2 * (0x3fa5553f1e536cf9 + x^2 * 0xbf56a2895e6ec219))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffffedc39945 },
			{.l = 0x3fa5553f1e536cf9 },
			{.l = 0xbf56a2895e6ec219 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,pi/(2*4)]); printexpansion(P);
		// x * (0x3feffffffffe4f8d + x^2 * (0xbfc555554fe29da4 + x^2 * (0x3f8111060f24ce8a + x^2 * 0xbf29e51e25262471)))
		{
			{.l = 0 },
			{.l = 0x3feffffffffe4f8d },
			{.l = 0xbfc555554fe29da4 },
			{.l = 0x3f8111060f24ce8a },
			{.l = 0xbf29e51e25262471 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,pi/(2*4)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffcf20e + x^2 * (0x3fa555554f5f9472 + x^2 * (0xbf56c15d96f7e0d6 + x^2 * 0x3ef9e5c4b50d32c1)))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffffffcf20e },
			{.l = 0x3fa555554f5f9472 },
			{.l = 0xbf56c15d96f7e0d6 },
			{.l = 0x3ef9e5c4b50d32c1 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,pi/(2*4)]); printexpansion(P);
		// x * (0x3fefffffffffffd9 + x^2 * (0xbfc555555554914b + x^2 * (0x3f8111110e97b2aa + x^2 * (0xbf2a0194e54c797b + x^2 * 0x3ec709274ddfbf3a))))
		{
			{.l = 0 },
			{.l = 0x3fefffffffffffd9 },
			{.l = 0xbfc555555554914b },
			{.l = 0x3f8111110e97b2aa },
			{.l = 0xbf2a0194e54c797b },
			{.l = 0x3ec709274ddfbf3a },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,pi/(2*4)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffffad + x^2 * (0x3fa555555554690b + x^2 * (0xbf56c16c13558194 + x^2 * (0x3efa019503600099 + x^2 * 0xbe926dfa5c536df0))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffffffffffad },
			{.l = 0x3fa555555554690b },
			{.l = 0xbf56c16c13558194 },
			{.l = 0x3efa019503600099 },
			{.l = 0xbe926dfa5c536df0 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,pi/(2*4)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555548 + x^2 * (0x3f8111111110c6ad + x^2 * (0xbf2a01a017dfbdf7 + x^2 * (0x3ec71ddcc6da1bf9 + x^2 * 0xbe5ad21cd0c87c8b)))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555548 },
			{.l = 0x3f8111111110c6ad },
			{.l = 0xbf2a01a017dfbdf7 },
			{.l = 0x3ec71ddcc6da1bf9 },
			{.l = 0xbe5ad21cd0c87c8b },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,pi/(2*4)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa555555555554c + x^2 * (0xbf56c16c16c11fb6 + x^2 * (0x3efa01a0183cb6df + x^2 * (0xbe927e4ad7fd6e0d + x^2 * 0x3e21e22b7f1b9ecf)))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa555555555554c },
			{.l = 0xbf56c16c16c11fb6 },
			{.l = 0x3efa01a0183cb6df },
			{.l = 0xbe927e4ad7fd6e0d },
			{.l = 0x3e21e22b7f1b9ecf },
		},
	},
};

static const DL
coefficient_pi_2_8[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,pi/(2*8)]); printexpansion(P);
		// 0x3caffffffffffffb
		{
			{.l = 0x3caffffffffffffb },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,pi/(2*8)]); printexpansion(P);
		// 0x3fefb088755db289
		{
			{.l = 0x3fefb088755db289 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,pi/(2*8)]); printexpansion(P);
		// x * 0x3fefe5a5b188a7e9
		{
			{.l = 0 },
			{.l = 0x3fefe5a5b188a7e9 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,pi/(2*8)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdfea2fa5e5e86a
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfea2fa5e5e86a },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,pi/(2*8)]); printexpansion(P);
		// x * (0x3feffffcbf3f141b + x^2 * 0xbfc54ace94dac38a)
		{
			{.l = 0 },
			{.l = 0x3feffffcbf3f141b },
			{.l = 0xbfc54ace94dac38a },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,pi/(2*8)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffc426e513c + x^2 * 0x3fa54b8e1ad265e8)
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffc426e513c },
			{.l = 0x3fa54b8e1ad265e8 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,pi/(2*8)]); printexpansion(P);
		// x * (0x3fefffffffcf100d + x^2 * (0xbfc55553f0e73933 + x^2 * 0x3f810b0d3420664d))
		{
			{.l = 0 },
			{.l = 0x3fefffffffcf100d },
			{.l = 0xbfc55553f0e73933 },
			{.l = 0x3f810b0d3420664d },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,pi/(2*8)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffb8032b + x^2 * (0x3fa55553f40cdaa6 + x^2 * 0xbf56b9b6dbae52cb))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffffffb8032b },
			{.l = 0x3fa55553f40cdaa6 },
			{.l = 0xbf56b9b6dbae52cb },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,pi/(2*8)]); printexpansion(P);
		// x * (0x3feffffffffffe53 + x^2 * (0xbfc55555553f9cf0 + x^2 * (0x3f811110611d0c28 + x^2 * 0xbf29fa7f1fafc725)))
		{
			{.l = 0 },
			{.l = 0x3feffffffffffe53 },
			{.l = 0xbfc55555553f9cf0 },
			{.l = 0x3f811110611d0c28 },
			{.l = 0xbf29fa7f1fafc725 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,pi/(2*8)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffffcff + x^2 * (0x3fa55555553db8fc + x^2 * (0xbf56c16b2fe2a30c + x^2 * 0x3ef9faabf2c18131)))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffffffffcff },
			{.l = 0x3fa55555553db8fc },
			{.l = 0xbf56c16b2fe2a30c },
			{.l = 0x3ef9faabf2c18131 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,pi/(2*8)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc55555555554c6 + x^2 * (0x3f81111111085e65 + x^2 * (0xbf2a019f70fb4d4f + x^2 * 0x3ec718d179815e74))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc55555555554c6 },
			{.l = 0x3f81111111085e65 },
			{.l = 0xbf2a019f70fb4d4f },
			{.l = 0x3ec718d179815e74 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,pi/(2*8)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa55555555554ef + x^2 * (0xbf56c16c16b83e45 + x^2 * (0x3efa019f88556bcf + x^2 * 0xbe927a88f2313633))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa55555555554ef },
			{.l = 0xbf56c16c16b83e45 },
			{.l = 0x3efa019f88556bcf },
			{.l = 0xbe927a88f2313633 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,pi/(2*8)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f81111111110ca0 + x^2 * (0xbf2a01a019b15a05 + x^2 * (0x3ec71de13c13e5ea + x^2 * 0xbe5ad758d23e4997)))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555555 },
			{.l = 0x3f81111111110ca0 },
			{.l = 0xbf2a01a019b15a05 },
			{.l = 0x3ec71de13c13e5ea },
			{.l = 0xbe5ad758d23e4997 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,pi/(2*8)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c164ed + x^2 * (0x3efa01a0199400c9 + x^2 * (0xbe927e4ce0362afb + x^2 * 0x3e21e014b077ef29)))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555555 },
			{.l = 0xbf56c16c16c164ed },
			{.l = 0x3efa01a0199400c9 },
			{.l = 0xbe927e4ce0362afb },
			{.l = 0x3e21e014b077ef29 },
		},
	},
};

static const DL
coefficient_pi_2_16[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,pi/(2*16)]); printexpansion(P);
		// 0x3caffffffffffff6
		{
			{.l = 0x3caffffffffffff6 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,pi/(2*16)]); printexpansion(P);
		// 0x3fefec3aa24f71ea
		{
			{.l = 0x3fefec3aa24f71ea },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,pi/(2*16)]); printexpansion(P);
		// x * 0x3feff96b0bdfe4a0
		{
			{.l = 0 },
			{.l = 0x3feff96b0bdfe4a0 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,pi/(2*16)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdffa8c6b67964f
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffa8c6b67964f },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,pi/(2*16)]); printexpansion(P);
		// x * (0x3fefffffcc064e02 + x^2 * 0xbfc552b396702d9a)
		{
			{.l = 0 },
			{.l = 0x3fefffffcc064e02 },
			{.l = 0xbfc552b396702d9a },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,pi/(2*16)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffc447610a + x^2 * 0x3fa552e3d6de7535)
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffffc447610a },
			{.l = 0x3fa552e3d6de7535 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,pi/(2*16)]); printexpansion(P);
		// x * (0x3fefffffffff3c91 + x^2 * (0xbfc555553f10c0a7 + x^2 * 0x3f810f9011a6a3bd))
		{
			{.l = 0 },
			{.l = 0x3fefffffffff3c91 },
			{.l = 0xbfc555553f10c0a7 },
			{.l = 0x3f810f9011a6a3bd },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,pi/(2*16)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffee0fa + x^2 * (0x3fa555553f4926fa + x^2 * 0xbf56bf7efcd3a48b))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffffffee0fa },
			{.l = 0x3fa555553f4926fa },
			{.l = 0xbf56bf7efcd3a48b },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,pi/(2*16)]); printexpansion(P);
		// x * (0x3feffffffffffffe + x^2 * (0xbfc555555554fa47 + x^2 * (0x3f81111105db6a90 + x^2 * 0xbf29ffd4a3860db6)))
		{
			{.l = 0 },
			{.l = 0x3feffffffffffffe },
			{.l = 0xbfc555555554fa47 },
			{.l = 0x3f81111105db6a90 },
			{.l = 0xbf29ffd4a3860db6 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,pi/(2*16)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffffffd + x^2 * (0x3fa555555554f6f9 + x^2 * (0xbf56c16c0854fc84 + x^2 * 0x3ef9ffe315c03b4a)))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffffffffffd },
			{.l = 0x3fa555555554f6f9 },
			{.l = 0xbf56c16c0854fc84 },
			{.l = 0x3ef9ffe315c03b4a },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,pi/(2*16)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110f5d4 + x^2 * (0xbf2a01a010a1a86c + x^2 * 0x3ec71cae04a4c7b6))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555555 },
			{.l = 0x3f8111111110f5d4 },
			{.l = 0xbf2a01a010a1a86c },
			{.l = 0x3ec71cae04a4c7b6 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,pi/(2*16)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c14b20 + x^2 * (0x3efa01a0116d4883 + x^2 * 0xbe927d640c59a0f3))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555555 },
			{.l = 0xbf56c16c16c14b20 },
			{.l = 0x3efa01a0116d4883 },
			{.l = 0xbe927d640c59a0f3 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,pi/(2*16)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110ffc7 + x^2 * (0xbf2a01a01555dbf2 + x^2 * (0x3ec71d5f7a275634 + x^2 * 0xbe5098acddc8ff0c)))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555555 },
			{.l = 0x3f8111111110ffc7 },
			{.l = 0xbf2a01a01555dbf2 },
			{.l = 0x3ec71d5f7a275634 },
			{.l = 0xbe5098acddc8ff0c },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,pi/(2*16)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c14fd6 + x^2 * (0x3efa01a01368636f + x^2 * (0xbe927da881b9b0e4 + x^2 * 0x3e07ec64d4710ba9)))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555555 },
			{.l = 0xbf56c16c16c14fd6 },
			{.l = 0x3efa01a01368636f },
			{.l = 0xbe927da881b9b0e4 },
			{.l = 0x3e07ec64d4710ba9 },
		},
	},
};

static const DL
coefficient_pi_2_32[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,pi/(2*32)]); printexpansion(P);
		// 0x3cafffffffffffec
		{
			{.l = 0x3cafffffffffffec },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,pi/(2*32)]); printexpansion(P);
		// 0x3feffb102ee13dbc
		{
			{.l = 0x3feffb102ee13dbc },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,pi/(2*32)]); printexpansion(P);
		// x * 0x3feffe5adcf149df
		{
			{.l = 0 },
			{.l = 0x3feffe5adcf149df },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,pi/(2*32)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdffea322e4f1a4
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffea322e4f1a4 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,pi/(2*32)]); printexpansion(P);
		// x * (0x3feffffffcc0ae2d + x^2 * 0xbfc554ace4aed601)
		{
			{.l = 0 },
			{.l = 0x3feffffffcc0ae2d },
			{.l = 0xbfc554ace4aed601 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,pi/(2*32)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffc44f708 + x^2 * 0x3fa554b8faac29f4)
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffffc44f708 },
			{.l = 0x3fa554b8faac29f4 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,pi/(2*32)]); printexpansion(P);
		// x * (0x3feffffffffffcf3 + x^2 * (0xbfc5555553f1201b + x^2 * 0x3f8110b0d194e132))
		{
			{.l = 0 },
			{.l = 0x3feffffffffffcf3 },
			{.l = 0xbfc5555553f1201b },
			{.l = 0x3f8110b0d194e132 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,pi/(2*32)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffffb85 + x^2 * (0x3fa5555553f4b9fb + x^2 * 0xbf56c0f0d46772f3))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffffffffb85 },
			{.l = 0x3fa5555553f4b9fb },
			{.l = 0xbf56c0f0d46772f3 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,pi/(2*32)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555451 + x^2 * (0x3f8111111072c563 + x^2 * 0xbf2a01321c030841)))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555451 },
			{.l = 0x3f8111111072c563 },
			{.l = 0xbf2a01321c030841 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,pi/(2*32)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa55555555554a3 + x^2 * (0xbf56c16c16193435 + x^2 * 0x3efa013d1c0f4559)))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa55555555554a3 },
			{.l = 0xbf56c16c16193435 },
			{.l = 0x3efa013d1c0f4559 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,pi/(2*32)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110e45a + x^2 * (0xbf2a019ffd7fda4a + x^2 * 0x3ec71819b98c9c33))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555555 },
			{.l = 0x3f8111111110e45a },
			{.l = 0xbf2a019ffd7fda4a },
			{.l = 0x3ec71819b98c9c33 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,pi/(2*32)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c12080 + x^2 * (0x3efa019fef60b8db + x^2 * 0xbe9276811baa0b4a))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555555 },
			{.l = 0xbf56c16c16c12080 },
			{.l = 0x3efa019fef60b8db },
			{.l = 0xbe9276811baa0b4a },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,pi/(2*32)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110cbe7 + x^2 * (0xbf2a019fcf427779 + x^2 * (0x3ec6fcd7b7e16af7 + x^2 * 0x3ec464f571f818b8)))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555555 },
			{.l = 0x3f8111111110cbe7 },
			{.l = 0xbf2a019fcf427779 },
			{.l = 0x3ec6fcd7b7e16af7 },
			{.l = 0x3ec464f571f818b8 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,pi/(2*32)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c0fb0e + x^2 * (0x3efa019fb0647ead + x^2 * (0xbe92547d96ef3391 + x^2 * 0xbe97c626212df7c1)))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555555 },
			{.l = 0xbf56c16c16c0fb0e },
			{.l = 0x3efa019fb0647ead },
			{.l = 0xbe92547d96ef3391 },
			{.l = 0xbe97c626212df7c1 },
		},
	},
};

static const DL
coefficient_pi_2_64[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,pi/(2*64)]); printexpansion(P);
		// 0x3cafffffffffffd7
		{
			{.l = 0x3cafffffffffffd7 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,pi/(2*64)]); printexpansion(P);
		// 0x3feffec424152731
		{
			{.l = 0x3feffec424152731 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,pi/(2*64)]); printexpansion(P);
		// x * 0x3fefff96b8dbed43
		{
			{.l = 0 },
			{.l = 0x3fefff96b8dbed43 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,pi/(2*64)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdfffa8c9399ead
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffa8c9399ead },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,pi/(2*64)]); printexpansion(P);
		// x * (0x3fefffffffcc0c08 + x^2 * 0xbfc5552b391ce3a3)
		{
			{.l = 0 },
			{.l = 0x3fefffffffcc0c08 },
			{.l = 0xbfc5552b391ce3a3 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,pi/(2*64)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffc45173 + x^2 * 0x3fa5552e3efa03a3)
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffffffc45173 },
			{.l = 0x3fa5552e3efa03a3 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,pi/(2*64)]); printexpansion(P);
		// x * (0x3feffffffffffff4 + x^2 * (0xbfc55555553f2719 + x^2 * 0x3f8110f907f48441))
		{
			{.l = 0 },
			{.l = 0x3feffffffffffff4 },
			{.l = 0xbfc55555553f2719 },
			{.l = 0x3f8110f907f48441 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,pi/(2*64)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffffee + x^2 * (0x3fa55555553f4014 + x^2 * 0xbf56c14d3fdeb189))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffffffffffee },
			{.l = 0x3fa55555553f4014 },
			{.l = 0xbf56c14d3fdeb189 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,pi/(2*64)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555551 + x^2 * (0x3f8111111106dd93 + x^2 * 0xbf2a0184455164d7)))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555551 },
			{.l = 0x3f8111111106dd93 },
			{.l = 0xbf2a0184455164d7 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,pi/(2*64)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555553 + x^2 * (0xbf56c16c16b7d7b7 + x^2 * 0x3efa018848df7dd6)))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555553 },
			{.l = 0xbf56c16c16b7d7b7 },
			{.l = 0x3efa018848df7dd6 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,pi/(2*64)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f81111111105f34 + x^2 * (0xbf2a019e5975a950 + x^2 * 0x3ec5bd48c06d0f4f))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555555 },
			{.l = 0x3f81111111105f34 },
			{.l = 0xbf2a019e5975a950 },
			{.l = 0x3ec5bd48c06d0f4f },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,pi/(2*64)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c03e98 + x^2 * (0x3efa019d75ca67f9 + x^2 * 0xbe90975d673dce7c))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555555 },
			{.l = 0xbf56c16c16c03e98 },
			{.l = 0x3efa019d75ca67f9 },
			{.l = 0xbe90975d673dce7c },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,pi/(2*64)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110ffc65 + x^2 * (0xbf2a019b6df32f0f + x^2 * (0x3ebdb54fc85d8eb0 + x^2 * 0x3f449b49938d0ac4)))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555555 },
			{.l = 0x3f811111110ffc65 },
			{.l = 0xbf2a019b6df32f0f },
			{.l = 0x3ebdb54fc85d8eb0 },
			{.l = 0x3f449b49938d0ac4 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,pi/(2*64)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16bfa7ec + x^2 * (0x3efa01997ffbc281 + x^2 * (0xbe8012b059f4acf3 + x^2 * 0xbf17eb16538902db)))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555555 },
			{.l = 0xbf56c16c16bfa7ec },
			{.l = 0x3efa01997ffbc281 },
			{.l = 0xbe8012b059f4acf3 },
			{.l = 0xbf17eb16538902db },
		},
	},
};

static const DL
coefficient_pi_2_128[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,pi/(2*128)]); printexpansion(P);
		// 0x3cafffffffffffaf
		{
			{.l = 0x3cafffffffffffaf },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,pi/(2*128)]); printexpansion(P);
		// 0x3fefffb10a8af759
		{
			{.l = 0x3fefffb10a8af759 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,pi/(2*128)]); printexpansion(P);
		// x * 0x3fefffe5ae50f514
		{
			{.l = 0 },
			{.l = 0x3fefffe5ae50f514 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,pi/(2*128)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdfffea32566c97
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdfffea32566c97 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,pi/(2*128)]); printexpansion(P);
		// x * (0x3feffffffffcc0c5 + x^2 * 0xbfc5554ace46409c)
		{
			{.l = 0 },
			{.l = 0x3feffffffffcc0c5 },
			{.l = 0xbfc5554ace46409c },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,pi/(2*128)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffc451f + x^2 * 0x3fa5554b8fc33adf)
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfdffffffffc451f },
			{.l = 0x3fa5554b8fc33adf },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,pi/(2*128)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555544042 + x^2 * 0x3f81110b73b39923))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555544042 },
			{.l = 0x3f81110b73b39923 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,pi/(2*128)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555549a1a + x^2 * 0xbf56c165c7285304))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555549a1a },
			{.l = 0xbf56c165c7285304 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,pi/(2*128)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110f32e2 + x^2 * 0xbf2a0193d3382167)))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555555 },
			{.l = 0x3f811111110f32e2 },
			{.l = 0xbf2a0193d3382167 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,pi/(2*128)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16be601b + x^2 * 0x3efa0190625e6e4d)))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555555 },
			{.l = 0xbf56c16c16be601b },
			{.l = 0x3efa0190625e6e4d },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,pi/(2*128)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110e499f + x^2 * (0xbf2a0184119ca53c + x^2 * 0xbee04136b42cdac1))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555555 },
			{.l = 0x3f811111110e499f },
			{.l = 0xbf2a0184119ca53c },
			{.l = 0xbee04136b42cdac1 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,pi/(2*128)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16bcb619 + x^2 * (0x3efa0175d689d458 + x^2 * 0x3eb9cf2440323462))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555555 },
			{.l = 0xbf56c16c16bcb619 },
			{.l = 0x3efa0175d689d458 },
			{.l = 0x3eb9cf2440323462 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,pi/(2*128)]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110cbe5f + x^2 * (0xbf2a015558e381fc + x^2 * (0xbf0f9b317b3d728c + x^2 * 0x3fc49b97c1644815)))))
		{
			{.l = 0 },
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfc5555555555555 },
			{.l = 0x3f811111110cbe5f },
			{.l = 0xbf2a015558e381fc },
			{.l = 0xbf0f9b317b3d728c },
			{.l = 0x3fc49b97c1644815 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,pi/(2*128)], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16ba5b65 + x^2 * (0x3efa013678ee9267 + x^2 * (0x3ee4562d84268b8d + x^2 * 0xbf97eb7e7168358f)))))
		{
			{.l = 0x3ff0000000000000 },
			{.l = 0xbfe0000000000000 },
			{.l = 0x3fa5555555555555 },
			{.l = 0xbf56c16c16ba5b65 },
			{.l = 0x3efa013678ee9267 },
			{.l = 0x3ee4562d84268b8d },
			{.l = 0xbf97eb7e7168358f },
		},
	},
};


// link for all coefficients array
static const DL *link[COEFFICIENTS_VERSION];

static const double
interpolate[2][BITNUM] = {
	{
		0.0000000000000000000e+00,
		1.2271538285719925387e-02,
		2.4541228522912288124e-02,
		3.6807222941358831714e-02,
		4.9067674327418014935e-02,
		6.1320736302208578294e-02,
		7.3564563599667426308e-02,
		8.5797312344439893850e-02,
		9.8017140329560603629e-02,
		1.1022220729388305938e-01,
		1.2241067519921619566e-01,
		1.3458070850712619548e-01,
		1.4673047445536174793e-01,
		1.5885814333386144570e-01,
		1.7096188876030121717e-01,
		1.8303988795514095078e-01,
		1.9509032201612827584e-01,
		2.0711137619221856032e-01,
		2.1910124015686979759e-01,
		2.3105810828067110951e-01,
		2.4298017990326389870e-01,
		2.5486565960451457169e-01,
		2.6671275747489836538e-01,
		2.7851968938505311524e-01,
		2.9028467725446238656e-01,
		3.0200594931922808417e-01,
		3.1368174039889146210e-01,
		3.2531029216226292622e-01,
		3.3688985339222005111e-01,
		3.4841868024943456472e-01,
		3.5989503653498816638e-01,
		3.7131719395183754306e-01,
		3.8268343236508978178e-01,
		3.9399204006104809883e-01,
		4.0524131400498986100e-01,
		4.1642956009763720804e-01,
		4.2755509343028208491e-01,
		4.3861623853852765853e-01,
		4.4961132965460659516e-01,
		4.6053871095824000514e-01,
		4.7139673682599764204e-01,
		4.8218377207912277438e-01,
		4.9289819222978403790e-01,
		5.0353838372571757542e-01,
		5.1410274419322177231e-01,
		5.2458968267846894928e-01,
		5.3499761988709726435e-01,
		5.4532498842204646383e-01,
		5.5557023301960217765e-01,
		5.6573181078361323149e-01,
		5.7580819141784533866e-01,
		5.8579785745643886408e-01,
		5.9569930449243335691e-01,
		6.0551104140432554512e-01,
		6.1523159058062681925e-01,
		6.2485948814238634341e-01,
		6.3439328416364548779e-01,
		6.4383154288979149715e-01,
		6.5317284295377675551e-01,
		6.6241577759017178373e-01,
		6.7155895484701844111e-01,
		6.8060099779545302212e-01,
		6.8954054473706694051e-01,
		6.9837624940897280457e-01,
		7.0710678118654757274e-01,
		7.1573082528381870571e-01,
		7.2424708295146689174e-01,
		7.3265427167241281570e-01,
		7.4095112535495910588e-01,
		7.4913639452345937020e-01,
		7.5720884650648456748e-01,
		7.6516726562245895860e-01,
		7.7301045336273699338e-01,
		7.8073722857209448822e-01,
		7.8834642762660622761e-01,
		7.9583690460888356633e-01,
		8.0320753148064494287e-01,
		8.1045719825259476821e-01,
		8.1758481315158371139e-01,
		8.2458930278502529099e-01,
		8.3146961230254523567e-01,
		8.3822470555483807875e-01,
		8.4485356524970711689e-01,
		8.5135519310526519554e-01,
		8.5772861000027211809e-01,
		8.6397285612158669643e-01,
		8.7008699110871146054e-01,
		8.7607009419540660122e-01,
		8.8192126434835504956e-01,
		8.8763962040285393496e-01,
		8.9322430119551532446e-01,
		8.9867446569395381673e-01,
		9.0398929312344333820e-01,
		9.0916798309052238025e-01,
		9.1420975570353069095e-01,
		9.1911385169005777040e-01,
		9.2387953251128673848e-01,
		9.2850608047321558924e-01,
		9.3299279883473884567e-01,
		9.3733901191257495977e-01,
		9.4154406518302080631e-01,
		9.4560732538052127971e-01,
		9.4952818059303667475e-01,
		9.5330604035419386211e-01,
		9.5694033573220882438e-01,
		9.6043051941556578655e-01,
		9.6377606579543984022e-01,
		9.6697647104485207059e-01,
		9.7003125319454397424e-01,
		9.7293995220556017678e-01,
		9.7570213003852857003e-01,
		9.7831737071962765473e-01,
		9.8078528040323043058e-01,
		9.8310548743121628501e-01,
		9.8527764238894122162e-01,
		9.8730141815785843473e-01,
		9.8917650996478101444e-01,
		9.9090263542778000971e-01,
		9.9247953459870996706e-01,
		9.9390697000235606051e-01,
		9.9518472667219692873e-01,
		9.9631261218277800129e-01,
		9.9729045667869020697e-01,
		9.9811811290014917919e-01,
		9.9879545620517240501e-01,
		9.9932238458834954375e-01,
		9.9969881869620424997e-01,
		9.9992470183914450299e-01,
	},
	{
		1.0000000000000000000e+00,
		9.9992470183914450299e-01,
		9.9969881869620424997e-01,
		9.9932238458834954375e-01,
		9.9879545620517240501e-01,
		9.9811811290014917919e-01,
		9.9729045667869020697e-01,
		9.9631261218277800129e-01,
		9.9518472667219692873e-01,
		9.9390697000235606051e-01,
		9.9247953459870996706e-01,
		9.9090263542778000971e-01,
		9.8917650996478101444e-01,
		9.8730141815785843473e-01,
		9.8527764238894122162e-01,
		9.8310548743121628501e-01,
		9.8078528040323043058e-01,
		9.7831737071962765473e-01,
		9.7570213003852857003e-01,
		9.7293995220556017678e-01,
		9.7003125319454397424e-01,
		9.6697647104485207059e-01,
		9.6377606579543984022e-01,
		9.6043051941556578655e-01,
		9.5694033573220882438e-01,
		9.5330604035419386211e-01,
		9.4952818059303667475e-01,
		9.4560732538052127971e-01,
		9.4154406518302080631e-01,
		9.3733901191257495977e-01,
		9.3299279883473884567e-01,
		9.2850608047321558924e-01,
		9.2387953251128673848e-01,
		9.1911385169005777040e-01,
		9.1420975570353069095e-01,
		9.0916798309052238025e-01,
		9.0398929312344333820e-01,
		8.9867446569395381673e-01,
		8.9322430119551532446e-01,
		8.8763962040285393496e-01,
		8.8192126434835504956e-01,
		8.7607009419540660122e-01,
		8.7008699110871146054e-01,
		8.6397285612158669643e-01,
		8.5772861000027211809e-01,
		8.5135519310526519554e-01,
		8.4485356524970711689e-01,
		8.3822470555483807875e-01,
		8.3146961230254523567e-01,
		8.2458930278502529099e-01,
		8.1758481315158371139e-01,
		8.1045719825259476821e-01,
		8.0320753148064494287e-01,
		7.9583690460888356633e-01,
		7.8834642762660622761e-01,
		7.8073722857209448822e-01,
		7.7301045336273699338e-01,
		7.6516726562245895860e-01,
		7.5720884650648456748e-01,
		7.4913639452345937020e-01,
		7.4095112535495910588e-01,
		7.3265427167241281570e-01,
		7.2424708295146689174e-01,
		7.1573082528381870571e-01,
		7.0710678118654757274e-01,
		6.9837624940897280457e-01,
		6.8954054473706694051e-01,
		6.8060099779545302212e-01,
		6.7155895484701844111e-01,
		6.6241577759017178373e-01,
		6.5317284295377675551e-01,
		6.4383154288979149715e-01,
		6.3439328416364548779e-01,
		6.2485948814238634341e-01,
		6.1523159058062681925e-01,
		6.0551104140432554512e-01,
		5.9569930449243335691e-01,
		5.8579785745643886408e-01,
		5.7580819141784533866e-01,
		5.6573181078361323149e-01,
		5.5557023301960217765e-01,
		5.4532498842204646383e-01,
		5.3499761988709726435e-01,
		5.2458968267846894928e-01,
		5.1410274419322177231e-01,
		5.0353838372571757542e-01,
		4.9289819222978403790e-01,
		4.8218377207912277438e-01,
		4.7139673682599764204e-01,
		4.6053871095824000514e-01,
		4.4961132965460659516e-01,
		4.3861623853852765853e-01,
		4.2755509343028208491e-01,
		4.1642956009763720804e-01,
		4.0524131400498986100e-01,
		3.9399204006104809883e-01,
		3.8268343236508978178e-01,
		3.7131719395183754306e-01,
		3.5989503653498816638e-01,
		3.4841868024943456472e-01,
		3.3688985339222005111e-01,
		3.2531029216226292622e-01,
		3.1368174039889146210e-01,
		3.0200594931922808417e-01,
		2.9028467725446238656e-01,
		2.7851968938505311524e-01,
		2.6671275747489836538e-01,
		2.5486565960451457169e-01,
		2.4298017990326389870e-01,
		2.3105810828067110951e-01,
		2.1910124015686979759e-01,
		2.0711137619221856032e-01,
		1.9509032201612827584e-01,
		1.8303988795514095078e-01,
		1.7096188876030121717e-01,
		1.5885814333386144570e-01,
		1.4673047445536174793e-01,
		1.3458070850712619548e-01,
		1.2241067519921619566e-01,
		1.1022220729388305938e-01,
		9.8017140329560603629e-02,
		8.5797312344439893850e-02,
		7.3564563599667426308e-02,
		6.1320736302208578294e-02,
		4.9067674327418014935e-02,
		3.6807222941358831714e-02,
		2.4541228522912288124e-02,
		1.2271538285719925387e-02,
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
invpio2 = 6.36619772367581382433e-01, 
// table[i] restores the correct rounding x values where the step is PI/2
table[NUM] = { //pi/2 * T, T = 0, 1, 2, ......
	0,
	1.5707963267948966192313216916398,
	3.1415926535897932384626433832795,
	4.7123889803846898576939650749193,
	6.283185307179586476925286766559,
	7.8539816339744830961566084581988,
	9.4247779607693797153879301498385,
	10.995574287564276334619251841478,
	12.566370614359172953850573533118,
	14.137166941154069573081895224758,
	15.707963267948966192313216916398,
	17.278759594743862811544538608037,
	18.849555921538759430775860299677,
	20.420352248333656050007181991317,
	21.991148575128552669238503682957,
	23.561944901923449288469825374596,
	25.132741228718345907701147066236,
	26.703537555513242526932468757876,
	28.274333882308139146163790449516,
	29.845130209103035765395112141155,
	31.415926535897932384626433832795
};
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
	
	if ((func = fopen("sin1_gen.c", "w")) == (FILE *)0) {
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

	// 日后需要优化，尽可能地精确
	// it is useless now
	/*
	midpoint = (a + b) / 2;
	// the half of the domain [a, b]
	length = (b - a) / 2;
 	Xtemp = (long int)(midpoint / two_pi);
	X = ((double)(Xtemp)) * two_pi;
	a1 = (0 - length) + X - midpoint;
	b1 = (0 + length) + X - midpoint;
	atemp = (long int)(a1 / pi_2) - 1;
	btemp = (long int)(b1 / pi_2) + 1;
	a2 = ((double)atemp) * pi_2;
	b2 = ((double)btemp) * pi_2;
	*/

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
		//fprintf(func, "#define TableNum %d\n", (btemp + 1));
		//fprintf(func, "static const double\n");
		
		//fprintf(func, "invpio4 = 1.2732395447351626861510701069801,\n");
		//fprintf(func, "invpio512 = 162.97466172610082382733697369345,\n");
		//fprintf(func, "pi_4 = 0.78539816339744830961566084581988,\n");

		//fprintf(func, "pi_512 = 0.00613592315154256491887235035797;\n");
		
		//fprintf(func, "X = 0;\n");
		//fprintf(func, "X = %.17lf;\n", X);
		/*
		fprintf(func, "table[TableNum] = {\t// pi_2 * T, T = 0, 1, 2, ..., %d\n", btemp);
		for (i = 0; i < btemp; i++)
		{
			fprintf(func, "\t%.17lf,\n", table[i]);
		}
		fprintf(func, "\t%.17lf\n ", table[i]);
		fprintf(func, "};\n");

		fprintf(func, "\n");
		*/
		fprintf(func, "static const double\n");
		fprintf(func, "invpio%d = %.19f,\n", bitnum * 2, invpio[bit].d);
		//fprintf(func, "invpio2 = 0.63661977236758134307553505349006,\n");
		fprintf(func, "pi_2 = 1.5707963267948966192313216916398,\n");
		fprintf(func, "pio2_1 = 1.57079632673412561417e+00, // 0x3FF921FB, 0x54400000 \n");
		fprintf(func, "pio2_1t = 6.07710050650619224932e-11; // 0x3DD0B461, 0x1A626331 \n");
		fprintf(func, "\n");

		//fprintf(func, "static const DL\n");
		//fprintf(func, "pi_4_1 = { .l = 0x3FE921FB54400000 },\n");
		//fprintf(func, "pi_4_1t = { .l = 0x3DC0B4611A626331 };\n");

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
		fprintf(func, "\tdouble ix, iix, iiix, y, appro_s, appro_c, z;\n");
		fprintf(func, "\tlong temp, flag, sign, temp1, temp2, status_pi_2, status_pi_1, table_order;\n");
		fprintf(func, "\tint i1;\n");
		fprintf(func, "\n");
		//fprintf(func, "\tix = x - X;\n");
		fprintf(func, "\ttemp = *((long *)(&x));\n");
		fprintf(func, "\tflag = temp >> %d; // - is 1, + is 0\n", format - 1);
		fprintf(func, "\ttemp = temp & 0x7fffffffffffffff;\n");
		fprintf(func, "\tix = *((double *)(&temp)); // at this time, ix is absolute value\n");
		fprintf(func, "\t\n");
		fprintf(func, "\t// if ix is too little, then return ix;\n");
		fprintf(func, "\tif (temp < 0x3e40000000000000) {\n");
		fprintf(func, "\t\treturn (1 - flag * 2) * ix;\n");
		fprintf(func, "\t}\n");
		fprintf(func, "\n");
		fprintf(func, "\t// new\n");
		fprintf(func, "\ttemp1 = (long)(ix * invpio%d);\n", bitnum * 2);
		fprintf(func, "\ttemp2 = temp1 >> BIT;\n");
		fprintf(func, "\tstatus_pi_2 = temp2 & 0x1;\n");
		fprintf(func, "\tstatus_pi_1 = (temp2 >> 1) & 0x1;\n");
		fprintf(func, "\ti1 = 1 - status_pi_2 * 2;\n");
		fprintf(func, "\tsign = 1 - 2 * (flag ^ status_pi_1); // ornot 0 is +, 1 is -\n");
		fprintf(func, "\t// iix = ix - temp2 * pi_2;\n");

		fprintf(func, "\tiix = ix - temp2 * pio2_1;\n");
		fprintf(func, "\tiix = iix - temp2 * pio2_1t;\n");
		
		fprintf(func, "\ttable_order = temp1 & BITNUM_1; // BITNUM - 1 = (2^BIT -1)\n");
		fprintf(func, "\tiiix = iix - ((double)table_order) / BITNUM * pio2_1;\n");
		fprintf(func, "\tiiix = iiix - ((double)table_order) / BITNUM * pio2_1t;\n");
		fprintf(func, "\tz = iiix * iiix;\n");
		fprintf(func, "\n");
		
		fprintf(func, "\t// new, sin(x+x') = sin(x)cos(x') + sin(x')cos(x), x' = iiix\n");
		fprintf(func, "\t// OR, cos(x+x') = cos(x)cos(x') - sin(x)sin(x'), x' = iiix\n");

		if (degree == 0) {
			fprintf(func, "\tappro_s = coefficient[0][0].d;\n");
			fprintf(func, "\tappro_c = coefficient[1][0].d;\n");
			fprintf(func, "\n");
			fprintf(func, "\ty = interpolate[status_pi_2][table_order] * appro_c + i1 * interpolate[1 - status_pi_2][table_order] * appro_s;\n");
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
			fprintf(func, "\ty = interpolate[status_pi_2][table_order] * appro_c + i1 * interpolate[1 - status_pi_2][table_order] * appro_s;\n");
			fprintf(func, "\ty += interpolate[status_pi_2][table_order] * coefficient[1][0].d;\n");
		}
		
		fprintf(func, "\ty = sign * y;\n");
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

	link[7] = &(coefficient_pi_2_128[0][0][0]);
	link[6] = &(coefficient_pi_2_64[0][0][0]);
	link[5] = &(coefficient_pi_2_32[0][0][0]);
	link[4] = &(coefficient_pi_2_16[0][0][0]);
	link[3] = &(coefficient_pi_2_8[0][0][0]);
	link[2] = &(coefficient_pi_2_4[0][0][0]);
	link[1] = &(coefficient_pi_2_2[0][0][0]);
	link[0] = &(coefficient_pi_2_1[0][0][0]);

	input_parameter.start = a;
	input_parameter.end = b;
	input_parameter.precision = precision;
	input_parameter.bit = bit;
	input_parameter.fnum = fnum;
	input_parameter.degree = degree;
	gen(input_parameter);

	return 0;
}