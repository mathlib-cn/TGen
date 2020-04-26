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
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/1]); printexpansion(P);
		// 0x3cafffffffffffff
		{
			{ .l = 0x3cafffffffffffff },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/1]); printexpansion(P);
		// 0x3fe0796150f5aa0e
		{
			{ .l = 0x3fe0796150f5aa0e },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,(7.5 - 2 * pi)/1]); printexpansion(P);
		// x * 0x3febdc0657f86e90
		{
			{ .l = 0 },
			{ .l = 0x3febdc0657f86e90 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,(7.5 - 2 * pi)/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdca0ee4855ae1c
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdca0ee4855ae1c },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,(7.5 - 2 * pi)/1]); printexpansion(P);
		// x * (0x3fefebd9b38f4725 + x^2 * 0xbfc3c282fc692a49)
		{
			{ .l = 0 },
			{ .l = 0x3fefebd9b38f4725 },
			{ .l = 0xbfc3c282fc692a49 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,(7.5 - 2 * pi)/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfe6fa948f1e6d + x^2 * 0x3fa3cd4e374940ff)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfe6fa948f1e6d },
			{ .l = 0x3fa3cd4e374940ff },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,(7.5 - 2 * pi)/1]); printexpansion(P);
		// x * (0x3fefffd1fa5cc00d + x^2 * (0xbfc54d22d84ce576 + x^2 * 0x3f802ae19b935f8a))
		{
			{ .l = 0 },
			{ .l = 0x3fefffd1fa5cc00d },
			{ .l = 0xbfc54d22d84ce576 },
			{ .l = 0x3f802ae19b935f8a },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,(7.5 - 2 * pi)/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffb25561b253 + x^2 * (0x3fa54c7858081388 + x^2 * 0xbf558e8b2741521d))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffb25561b253 },
			{ .l = 0x3fa54c7858081388 },
			{ .l = 0xbf558e8b2741521d },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,(7.5 - 2 * pi)/1]); printexpansion(P);
		// x * (0x3fefffffc2f5b11d + x^2 * (0xbfc55541c7573f6b + x^2 * (0x3f810d0f333e120b + x^2 * 0xbf28f0bc9e54a9d2)))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffc2f5b11d },
			{ .l = 0xbfc55541c7573f6b },
			{ .l = 0x3f810d0f333e120b },
			{ .l = 0xbf28f0bc9e54a9d2 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,(7.5 - 2 * pi)/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffff7d9111d3 + x^2 * (0x3fa5553d58b4a7f1 + x^2 * (0xbf56bbc3d0f6271b + x^2 * 0x3ef8ee6618dfb91f)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffff7d9111d3 },
			{ .l = 0x3fa5553d58b4a7f1 },
			{ .l = 0xbf56bbc3d0f6271b },
			{ .l = 0x3ef8ee6618dfb91f },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,(7.5 - 2 * pi)/1]); printexpansion(P);
		// x * (0x3fefffffffcb22c4 + x^2 * (0xbfc555553aa54ac2 + x^2 * (0x3f8111084ee41c54 + x^2 * (0xbf29fd8ff6b44592 + x^2 * 0x3ec6575d5e1f1064))))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffcb22c4 },
			{ .l = 0xbfc555553aa54ac2 },
			{ .l = 0x3f8111084ee41c54 },
			{ .l = 0xbf29fd8ff6b44592 },
			{ .l = 0x3ec6575d5e1f1064 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,(7.5 - 2 * pi)/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffff7739e0 + x^2 * (0x3fa55555304fc088 + x^2 * (0xbf56c15ec11398b4 + x^2 * (0x3ef9fd5c86236854 + x^2 * 0xbe91dda1b91d6496))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffff7739e0 },
			{ .l = 0x3fa55555304fc088 },
			{ .l = 0xbf56c15ec11398b4 },
			{ .l = 0x3ef9fd5c86236854 },
			{ .l = 0xbe91dda1b91d6496 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,(7.5 - 2 * pi)/1]); printexpansion(P);
		// x * (0x3fefffffffffdfc3 + x^2 * (0xbfc55555553dc014 + x^2 * (0x3f81111105c21b85 + x^2 * (0xbf2a01981c6ff184 + x^2 * (0x3ec71b52bd23483b + x^2 * 0xbe5a22bf245570b3)))))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffffdfc3 },
			{ .l = 0xbfc55555553dc014 },
			{ .l = 0x3f81111105c21b85 },
			{ .l = 0xbf2a01981c6ff184 },
			{ .l = 0x3ec71b52bd23483b },
			{ .l = 0xbe5a22bf245570b3 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,(7.5 - 2 * pi)/1], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffff9e0f + x^2 * (0x3fa555555530a760 + x^2 * (0xbf56c16c041b05e2 + x^2 * (0x3efa0197429fc28d + x^2 * (0xbe927c2b53e89b99 + x^2 * 0x3e216ad5da468948)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffff9e0f },
			{ .l = 0x3fa555555530a760 },
			{ .l = 0xbf56c16c041b05e2 },
			{ .l = 0x3efa0197429fc28d },
			{ .l = 0xbe927c2b53e89b99 },
			{ .l = 0x3e216ad5da468948 },
		},
	},
};

static const DL
coefficient_1_2[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/2]); printexpansion(P);
		// 0x3caffffffffffffe
		{
			{ .l = 0x3caffffffffffffe },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/2]); printexpansion(P);
		// 0x3fecd8916d6d8f14
		{
			{ .l = 0x3fecd8916d6d8f14 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,(7.5 - 2 * pi)/2]); printexpansion(P);
		// x * 0x3fef0031c084551d
		{
			{ .l = 0 },
			{ .l = 0x3fef0031c084551d },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,(7.5 - 2 * pi)/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdf2d97cc444719
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdf2d97cc444719 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,(7.5 - 2 * pi)/2]); printexpansion(P);
		// x * (0x3feffecf4a3d48c6 + x^2 * 0xbfc4f05cc7e3b1a4)
		{
			{ .l = 0 },
			{ .l = 0x3feffecf4a3d48c6 },
			{ .l = 0xbfc4f05cc7e3b1a4 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,(7.5 - 2 * pi)/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffe9e1acbb613 + x^2 * 0x3fa4f6d87a2c2556)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffe9e1acbb613 },
			{ .l = 0x3fa4f6d87a2c2556 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,(7.5 - 2 * pi)/2]); printexpansion(P);
		// x * (0x3fefffff539de63a + x^2 * (0xbfc554d4614734e7 + x^2 * 0x3f80d75e16bf8fee))
		{
			{ .l = 0 },
			{ .l = 0x3fefffff539de63a },
			{ .l = 0xbfc554d4614734e7 },
			{ .l = 0x3f80d75e16bf8fee },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,(7.5 - 2 * pi)/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffefd23c99e + x^2 * (0x3fa554d3c603846b + x^2 * 0xbf567704b5cab417))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffefd23c99e },
			{ .l = 0x3fa554d3c603846b },
			{ .l = 0xbf567704b5cab417 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,(7.5 - 2 * pi)/2]); printexpansion(P);
		// x * (0x3fefffffffc72c77 + x^2 * (0xbfc55555098c7051 + x^2 * (0x3f8110d18afb7b68 + x^2 * 0xbf29bd3c44329aa0)))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffc72c77 },
			{ .l = 0xbfc55555098c7051 },
			{ .l = 0x3f8110d18afb7b68 },
			{ .l = 0xbf29bd3c44329aa0 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,(7.5 - 2 * pi)/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffff975316 + x^2 * (0x3fa55555015c50bf + x^2 * (0xbf56c117c33cfcd8 + x^2 * 0x3ef9be8ee1d5fc41)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffff975316 },
			{ .l = 0x3fa55555015c50bf },
			{ .l = 0xbf56c117c33cfcd8 },
			{ .l = 0x3ef9be8ee1d5fc41 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,(7.5 - 2 * pi)/2]); printexpansion(P);
		// x * (0x3feffffffffff3bf + x^2 * (0xbfc55555553bbeec + x^2 * (0x3f811110eebe0d07 + x^2 * (0xbf2a015f7cfe4a7f + x^2 * 0x3ec6ec2590c44bbf))))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffff3bf },
			{ .l = 0xbfc55555553bbeec },
			{ .l = 0x3f811110eebe0d07 },
			{ .l = 0xbf2a015f7cfe4a7f },
			{ .l = 0x3ec6ec2590c44bbf },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,(7.5 - 2 * pi)/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffe542 + x^2 * (0x3fa555555535fbdc + x^2 * (0xbf56c16be6c39438 + x^2 * (0x3efa015fbd4d106a + x^2 * 0xbe925702660da70e))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffffe542 },
			{ .l = 0x3fa555555535fbdc },
			{ .l = 0xbf56c16be6c39438 },
			{ .l = 0x3efa015fbd4d106a },
			{ .l = 0xbe925702660da70e },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,(7.5 - 2 * pi)/2]); printexpansion(P);
		// x * (0x3feffffffffffffe + x^2 * (0xbfc5555555554f9c + x^2 * (0x3f8111111105fb0e + x^2 * (0xbf2a019ffa5a736d + x^2 * (0x3ec71dbaa87f44f0 + x^2 * 0xbe5ab53c5012e3c5)))))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffffffe },
			{ .l = 0xbfc5555555554f9c },
			{ .l = 0x3f8111111105fb0e },
			{ .l = 0xbf2a019ffa5a736d },
			{ .l = 0x3ec71dbaa87f44f0 },
			{ .l = 0xbe5ab53c5012e3c5 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,(7.5 - 2 * pi)/2], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffffffb + x^2 * (0x3fa5555555554d7b + x^2 * (0xbf56c16c16b0bd3a + x^2 * (0x3efa019ff926b820 + x^2 * (0xbe927e2ed67bff08 + x^2 * 0x3e21ce5ab4b9bf67)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffffffffb },
			{ .l = 0x3fa5555555554d7b },
			{ .l = 0xbf56c16c16b0bd3a },
			{ .l = 0x3efa019ff926b820 },
			{ .l = 0xbe927e2ed67bff08 },
			{ .l = 0x3e21ce5ab4b9bf67 },
		},
	},
};

static const DL
coefficient_1_4[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/4]); printexpansion(P);
		// 0x3caffffffffffffd
		{
			{ .l = 0x3caffffffffffffd },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/4]); printexpansion(P);
		// 0x3fef3f8439202e28
		{
			{ .l = 0x3fef3f8439202e28 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,(7.5 - 2 * pi)/4]); printexpansion(P);
		// x * 0x3fefc0a1a059f2cb
		{
			{ .l = 0 },
			{ .l = 0x3fefc0a1a059f2cb },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,(7.5 - 2 * pi)/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdfcb9a7751a5e9
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfcb9a7751a5e9 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,(7.5 - 2 * pi)/4]); printexpansion(P);
		// x * (0x3fefffed35a80f2e + x^2 * 0xbfc53c1217d9e284)
		{
			{ .l = 0 },
			{ .l = 0x3fefffed35a80f2e },
			{ .l = 0xbfc53c1217d9e284 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,(7.5 - 2 * pi)/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffea5cb5201f + x^2 * 0x3fa53dd706930caf)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffea5cb5201f },
			{ .l = 0x3fa53dd706930caf },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,(7.5 - 2 * pi)/4]); printexpansion(P);
		// x * (0x3feffffffd592dc2 + x^2 * (0xbfc5554d4e32d094 + x^2 * 0x3f8102a17941ac31))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffd592dc2 },
			{ .l = 0xbfc5554d4e32d094 },
			{ .l = 0x3f8102a17941ac31 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,(7.5 - 2 * pi)/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffc165b89 + x^2 * (0x3fa5554d5c2ab973 + x^2 * 0xbf56aee7e9ceaedc))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffc165b89 },
			{ .l = 0x3fa5554d5c2ab973 },
			{ .l = 0xbf56aee7e9ceaedc },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,(7.5 - 2 * pi)/4]); printexpansion(P);
		// x * (0x3fefffffffffc823 + x^2 * (0xbfc5555554287367 + x^2 * (0x3f81110d1acf3ace + x^2 * 0xbf29f0840baa110e)))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffffc823 },
			{ .l = 0xbfc5555554287367 },
			{ .l = 0x3f81110d1acf3ace },
			{ .l = 0xbf29f0840baa110e },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,(7.5 - 2 * pi)/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffff9b7b + x^2 * (0x3fa55555540d4b67 + x^2 * (0xbf56c166e18403a3 + x^2 * 0x3ef9f0ec0dc53c24)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffff9b7b },
			{ .l = 0x3fa55555540d4b67 },
			{ .l = 0xbf56c166e18403a3 },
			{ .l = 0x3ef9f0ec0dc53c24 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,(7.5 - 2 * pi)/4]); printexpansion(P);
		// x * (0x3feffffffffffffd + x^2 * (0xbfc5555555553c05 + x^2 * (0x3f8111111088788a + x^2 * (0xbf2a019c11bcb8b8 + x^2 * 0x3ec711724892e51e))))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffffffd },
			{ .l = 0xbfc5555555553c05 },
			{ .l = 0x3f8111111088788a },
			{ .l = 0xbf2a019c11bcb8b8 },
			{ .l = 0x3ec711724892e51e },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,(7.5 - 2 * pi)/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffffffa + x^2 * (0x3fa55555555537f3 + x^2 * (0xbf56c16c1608af28 + x^2 * (0x3efa019c290c1459 + x^2 * 0xbe92748fc49dec33))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffffffffa },
			{ .l = 0x3fa55555555537f3 },
			{ .l = 0xbf56c16c1608af28 },
			{ .l = 0x3efa019c290c1459 },
			{ .l = 0xbe92748fc49dec33 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,(7.5 - 2 * pi)/4]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555554 + x^2 * (0x3f811111111105d3 + x^2 * (0xbf2a01a01980297b + x^2 * (0x3ec71de10a194f5a + x^2 * 0xbe5ad9e55391ac69)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555554 },
			{ .l = 0x3f811111111105d3 },
			{ .l = 0xbf2a01a01980297b },
			{ .l = 0x3ec71de10a194f5a },
			{ .l = 0xbe5ad9e55391ac69 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,(7.5 - 2 * pi)/4], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c165a6 + x^2 * (0x3efa01a019b54965 + x^2 * (0xbe927e4e2ef007f9 + x^2 * 0x3e21e7a8f42f513e)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c165a6 },
			{ .l = 0x3efa01a019b54965 },
			{ .l = 0xbe927e4e2ef007f9 },
			{ .l = 0x3e21e7a8f42f513e },
		},
	},
};

static const DL
coefficient_1_8[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/8]); printexpansion(P);
		// 0x3caffffffffffff9
		{
			{ .l = 0x3caffffffffffff9 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/8]); printexpansion(P);
		// 0x3fefd06fb0e27ecc
		{
			{ .l = 0x3fefd06fb0e27ecc },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,(7.5 - 2 * pi)/8]); printexpansion(P);
		// x * 0x3feff031c0f1b513
		{
			{ .l = 0 },
			{ .l = 0x3feff031c0f1b513 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,(7.5 - 2 * pi)/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdff2e99819568b
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdff2e99819568b },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,(7.5 - 2 * pi)/8]); printexpansion(P);
		// x * (0x3feffffed4596c43 + x^2 * 0xbfc54f043162db47)
		{
			{ .l = 0 },
			{ .l = 0x3feffffed4596c43 },
			{ .l = 0xbfc54f043162db47 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,(7.5 - 2 * pi)/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffea792fc0c + x^2 * 0x3fa54f779933461d)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffea792fc0c },
			{ .l = 0x3fa54f779933461d },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,(7.5 - 2 * pi)/8]); printexpansion(P);
		// x * (0x3feffffffff56f34 + x^2 * (0xbfc55554d5030832 + x^2 * 0x3f810d74fc13997b))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffff56f34 },
			{ .l = 0xbfc55554d5030832 },
			{ .l = 0x3f810d74fc13997b },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,(7.5 - 2 * pi)/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffff078a1 + x^2 * (0x3fa55554d637c708 + x^2 * 0xbf56bccc41e9f957))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffff078a1 },
			{ .l = 0x3fa55554d637c708 },
			{ .l = 0xbf56bccc41e9f957 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,(7.5 - 2 * pi)/8]); printexpansion(P);
		// x * (0x3fefffffffffffc8 + x^2 * (0xbfc555555550a202 + x^2 * (0x3f811110d1aa5a56 + x^2 * 0xbf29fd58a046129d)))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffffffc8 },
			{ .l = 0xbfc555555550a202 },
			{ .l = 0x3f811110d1aa5a56 },
			{ .l = 0xbf29fd58a046129d },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,(7.5 - 2 * pi)/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffff9c + x^2 * (0x3fa5555555503969 + x^2 * (0xbf56c16bc38f0f83 + x^2 * 0x3ef9fd737b890bef)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffffff9c },
			{ .l = 0x3fa5555555503969 },
			{ .l = 0xbf56c16bc38f0f83 },
			{ .l = 0x3ef9fd737b890bef },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,(7.5 - 2 * pi)/8]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555543 + x^2 * (0x3f811111110f3368 + x^2 * (0xbf2a019fdd5de2f6 + x^2 * 0x3ec71ad9c52756dc))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555543 },
			{ .l = 0x3f811111110f3368 },
			{ .l = 0xbf2a019fdd5de2f6 },
			{ .l = 0x3ec71ad9c52756dc },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,(7.5 - 2 * pi)/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555548 + x^2 * (0xbf56c16c16bf6ed2 + x^2 * (0x3efa019fe575d410 + x^2 * 0xbe927c0b1d85b278))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555548 },
			{ .l = 0xbf56c16c16bf6ed2 },
			{ .l = 0x3efa019fe575d410 },
			{ .l = 0xbe927c0b1d85b278 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,(7.5 - 2 * pi)/8]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111111109d9 + x^2 * (0xbf2a01a01930ddd6 + x^2 * (0x3ec71dd9f6c65cf2 + x^2 * 0xbe5a940edbedbf10)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f811111111109d9 },
			{ .l = 0xbf2a01a01930ddd6 },
			{ .l = 0x3ec71dd9f6c65cf2 },
			{ .l = 0xbe5a940edbedbf10 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,(7.5 - 2 * pi)/8], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c1604f + x^2 * (0x3efa01a018dbb4b4 + x^2 * (0xbe927e438d6c9267 + x^2 * 0x3e219113adcd4044)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c1604f },
			{ .l = 0x3efa01a018dbb4b4 },
			{ .l = 0xbe927e438d6c9267 },
			{ .l = 0x3e219113adcd4044 },
		},
	},
};

static const DL
coefficient_1_16[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/16]); printexpansion(P);
		// 0x3caffffffffffff3
		{
			{ .l = 0x3caffffffffffff3 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/16]); printexpansion(P);
		// 0x3feff424ba59e474
		{
			{ .l = 0x3feff424ba59e474 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,(7.5 - 2 * pi)/16]); printexpansion(P);
		// x * 0x3feffc0d05decc4c
		{
			{ .l = 0 },
			{ .l = 0x3feffc0d05decc4c },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,(7.5 - 2 * pi)/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdffcba9492b45c
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffcba9492b45c },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,(7.5 - 2 * pi)/16]); printexpansion(P);
		// x * (0x3fefffffed498d6c + x^2 * 0xbfc553c1070421d6)
		{
			{ .l = 0 },
			{ .l = 0x3fefffffed498d6c },
			{ .l = 0xbfc553c1070421d6 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,(7.5 - 2 * pi)/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffea802d87 + x^2 * 0x3fa553de02ff3c50)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffea802d87 },
			{ .l = 0x3fa553de02ff3c50 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,(7.5 - 2 * pi)/16]); printexpansion(P);
		// x * (0x3fefffffffffd5c7 + x^2 * (0xbfc555554d50acad + x^2 * 0x3f81102a08c46baf))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffffd5c7 },
			{ .l = 0xbfc555554d50acad },
			{ .l = 0x3f81102a08c46baf },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,(7.5 - 2 * pi)/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffc201 + x^2 * (0x3fa555554d6545d0 + x^2 * 0xbf56c04434522306))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffffc201 },
			{ .l = 0x3fa555554d6545d0 },
			{ .l = 0xbf56c04434522306 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,(7.5 - 2 * pi)/16]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555554743 + x^2 * (0x3f8111110d80cf59 + x^2 * 0xbf2a009810618df2)))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555554743 },
			{ .l = 0x3f8111110d80cf59 },
			{ .l = 0xbf2a009810618df2 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,(7.5 - 2 * pi)/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555554bb2 + x^2 * (0xbf56c16c12f80257 + x^2 * 0x3efa00b278513158)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555554bb2 },
			{ .l = 0xbf56c16c12f80257 },
			{ .l = 0x3efa00b278513158 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,(7.5 - 2 * pi)/16]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110fb0f + x^2 * (0xbf2a01a0126565b2 + x^2 * 0x3ec71cd119f71943))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f8111111110fb0f },
			{ .l = 0xbf2a01a0126565b2 },
			{ .l = 0x3ec71cd119f71943 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,(7.5 - 2 * pi)/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c149a4 + x^2 * (0x3efa01a01088dac8 + x^2 * 0xbe927d47c5d60ad8))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c149a4 },
			{ .l = 0x3efa01a01088dac8 },
			{ .l = 0xbe927d47c5d60ad8 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,(7.5 - 2 * pi)/16]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110f441 + x^2 * (0xbf2a01a00d08cce5 + x^2 * (0x3ec71b8000a2a900 + x^2 * 0x3e6a4500143a83e7)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f8111111110f441 },
			{ .l = 0xbf2a01a00d08cce5 },
			{ .l = 0x3ec71b8000a2a900 },
			{ .l = 0x3e6a4500143a83e7 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,(7.5 - 2 * pi)/16], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c13d01 + x^2 * (0x3efa01a007ae0d45 + x^2 * (0xbe927b49d0d3551e + x^2 * 0xbe428f9c67cffdeb)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c13d01 },
			{ .l = 0x3efa01a007ae0d45 },
			{ .l = 0xbe927b49d0d3551e },
			{ .l = 0xbe428f9c67cffdeb },
		},
	},
};

static const DL
coefficient_1_32[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/32]); printexpansion(P);
		// 0x3cafffffffffffe6
		{
			{ .l = 0x3cafffffffffffe6 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/32]); printexpansion(P);
		// 0x3feffd09bb09c043
		{
			{ .l = 0x3feffd09bb09c043 },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,(7.5 - 2 * pi)/32]); printexpansion(P);
		// x * 0x3fefff034ad22909
		{
			{ .l = 0 },
			{ .l = 0x3fefff034ad22909 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,(7.5 - 2 * pi)/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdfff2ea8093018
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfff2ea8093018 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,(7.5 - 2 * pi)/32]); printexpansion(P);
		// x * (0x3feffffffed4a8ac + x^2 * 0xbfc554f0416b8ca3)
		{
			{ .l = 0 },
			{ .l = 0x3feffffffed4a8ac },
			{ .l = 0xbfc554f0416b8ca3 },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,(7.5 - 2 * pi)/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffea81eaf + x^2 * 0x3fa554f78287e3ac)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffea81eaf },
			{ .l = 0x3fa554f78287e3ac },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,(7.5 - 2 * pi)/32]); printexpansion(P);
		// x * (0x3fefffffffffff57 + x^2 * (0xbfc5555554d50649 + x^2 * 0x3f8110d74dee239c))
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffffff57 },
			{ .l = 0xbfc5555554d50649 },
			{ .l = 0x3f8110d74dee239c },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,(7.5 - 2 * pi)/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffffff08 + x^2 * (0x3fa5555554d65339 + x^2 * 0xbf56c1221d7588bd))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffffff08 },
			{ .l = 0x3fa5555554d65339 },
			{ .l = 0xbf56c1221d7588bd },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,(7.5 - 2 * pi)/32]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc555555555551d + x^2 * (0x3f81111110d806d8 + x^2 * 0xbf2a015e1474524d)))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc555555555551d },
			{ .l = 0x3f81111110d806d8 },
			{ .l = 0xbf2a015e1474524d },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,(7.5 - 2 * pi)/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa555555555552f + x^2 * (0xbf56c16c16850555 + x^2 * 0x3efa0164c52e2916)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa555555555552f },
			{ .l = 0xbf56c16c16850555 },
			{ .l = 0x3efa0164c52e2916 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,(7.5 - 2 * pi)/32]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110c6ea + x^2 * (0xbf2a019fcbfd01b7 + x^2 * 0x3ec7043ab9702112))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f8111111110c6ea },
			{ .l = 0xbf2a019fcbfd01b7 },
			{ .l = 0x3ec7043ab9702112 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,(7.5 - 2 * pi)/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c0ee70 + x^2 * (0x3efa019fa482e7d7 + x^2 * 0xbe925afc22035b78))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c0ee70 },
			{ .l = 0x3efa019fa482e7d7 },
			{ .l = 0xbe925afc22035b78 },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,(7.5 - 2 * pi)/32]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f81111111109dce + x^2 * (0xbf2a019f4a6bdc86 + x^2 * (0x3ec684f25e722aed + x^2 * 0x3ef3d67f6f203f32)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f81111111109dce },
			{ .l = 0xbf2a019f4a6bdc86 },
			{ .l = 0x3ec684f25e722aed },
			{ .l = 0x3ef3d67f6f203f32 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,(7.5 - 2 * pi)/32], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c0afb7 + x^2 * (0x3efa019ef4b04134 + x^2 * (0xbe91bcc08fc41554 + x^2 * 0xbec709e4f50a99b0)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16c0afb7 },
			{ .l = 0x3efa019ef4b04134 },
			{ .l = 0xbe91bcc08fc41554 },
			{ .l = 0xbec709e4f50a99b0 },
		},
	},
};

static const DL
coefficient_1_64[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/64]); printexpansion(P);
		// 0x3cafffffffffffcb
		{
			{ .l = 0x3cafffffffffffcb },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/64]); printexpansion(P);
		// 0x3fefff427787ea9f
		{
			{ .l = 0x3fefff427787ea9f },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,(7.5 - 2 * pi)/64]); printexpansion(P);
		// x * 0x3fefffc0d34a32df
		{
			{ .l = 0 },
			{ .l = 0x3fefffc0d34a32df },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,(7.5 - 2 * pi)/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdfffcbaa308347
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffcbaa308347 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,(7.5 - 2 * pi)/64]); printexpansion(P);
		// x * (0x3fefffffffed4aca + x^2 * 0xbfc5553c1055871d)
		{
			{ .l = 0 },
			{ .l = 0x3fefffffffed4aca },
			{ .l = 0xbfc5553c1055871d },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,(7.5 - 2 * pi)/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdfffffffea825a + x^2 * 0x3fa5553de0be5dda)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdfffffffea825a },
			{ .l = 0x3fa5553de0be5dda },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,(7.5 - 2 * pi)/64]); printexpansion(P);
		// x * (0x3feffffffffffffd + x^2 * (0xbfc55555554d1243 + x^2 * 0x3f8111027eaf402f))
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffffffd },
			{ .l = 0xbfc55555554d1243 },
			{ .l = 0x3f8111027eaf402f },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,(7.5 - 2 * pi)/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffffffc + x^2 * (0x3fa55555554d4672 + x^2 * 0xbf56c1597cc720b8))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffffffffc },
			{ .l = 0x3fa55555554d4672 },
			{ .l = 0xbf56c1597cc720b8 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,(7.5 - 2 * pi)/64]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555554 + x^2 * (0x3f811111110ca095 + x^2 * 0xbf2a018e0571839a)))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555554 },
			{ .l = 0x3f811111110ca095 },
			{ .l = 0xbf2a018e0571839a },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,(7.5 - 2 * pi)/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16be8f46 + x^2 * 0x3efa0192c8e98475)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16be8f46 },
			{ .l = 0x3efa0192c8e98475 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,(7.5 - 2 * pi)/64]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110fe8ac + x^2 * (0xbf2a019b3c7109b3 + x^2 * 0x3ec0be62452ed046))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f811111110fe8ac },
			{ .l = 0xbf2a019b3c7109b3 },
			{ .l = 0x3ec0be62452ed046 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,(7.5 - 2 * pi)/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16bf75aa + x^2 * (0x3efa0198c4275cbd + x^2 * 0xbe8361f5d70461ee))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16bf75aa },
			{ .l = 0x3efa0198c4275cbd },
			{ .l = 0xbe8361f5d70461ee },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,(7.5 - 2 * pi)/64]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110f4402 + x^2 * (0xbf2a01932082ae08 + x^2 * (0xbebe3de9df73d918 + x^2 * 0x3f73dd859c8568f8)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f811111110f4402 },
			{ .l = 0xbf2a01932082ae08 },
			{ .l = 0xbebe3de9df73d918 },
			{ .l = 0x3f73dd859c8568f8 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,(7.5 - 2 * pi)/64], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16be7a92 + x^2 * (0x3efa018dc4ab02a4 + x^2 * (0x3e9de6642628f5e1 + x^2 * 0xbf470ee4ebf1f11c)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16be7a92 },
			{ .l = 0x3efa018dc4ab02a4 },
			{ .l = 0x3e9de6642628f5e1 },
			{ .l = 0xbf470ee4ebf1f11c },
		},
	},
};

static const DL
coefficient_1_128[COEFFICIENTS_NUM][2][COEFFICIENTS_NUM] = {
	// 1 coefficient
	{
		// P = fpminimax(sin(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/128]); printexpansion(P);
		// 0x3cafffffffffff97
		{
			{ .l = 0x3cafffffffffff97 },
		},
		// P = fpminimax(cos(x), [|0|], [|D...|], [1b-53,(7.5 - 2 * pi)/128]); printexpansion(P);
		// 0x3fefffd09e6e4b6a
		{
			{ .l = 0x3fefffd09e6e4b6a },
		},
	},
	// 2 coefficients
	{
		// P = fpminimax(sin(x), [|1|], [|D...|], [1b-53,(7.5 - 2 * pi)/128]); printexpansion(P);
		// x * 0x3feffff034dbe747
		{
			{ .l = 0 },
			{ .l = 0x3feffff034dbe747 },
		},
		// P = fpminimax(cos(x), [|2|], [|D...|], [1b-53,(7.5 - 2 * pi)/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * 0xbfdffff2ea8f0402
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffff2ea8f0402 },
		},
	},
	// 3 coefficients
	{
		// P = fpminimax(sin(x), [|1,3|], [|D...|], [1b-53,(7.5 - 2 * pi)/128]); printexpansion(P);
		// x * (0x3feffffffffed4ae + x^2 * 0xbfc5554f0415652f)
		{
			{ .l = 0 },
			{ .l = 0x3feffffffffed4ae },
			{ .l = 0xbfc5554f0415652f },
		},
		// P = fpminimax(cos(x), [|2,4|], [|D...|], [1b-53,(7.5 - 2 * pi)/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfdffffffffea827 + x^2 * 0x3fa5554f7830cfa2)
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfdffffffffea827 },
			{ .l = 0x3fa5554f7830cfa2 },
		},
	},
	// 4 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5|], [|D...|], [1b-53,(7.5 - 2 * pi)/128]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc555555554f18f + x^2 * 0x3f81110db2877f89))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc555555554f18f },
			{ .l = 0x3f81110db2877f89 },
		},
		// P = fpminimax(cos(x), [|2,4,6|], [|D...|], [1b-53,(7.5 - 2 * pi)/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa55555555511e9 + x^2 * 0xbf56c1684d41bb32))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa55555555511e9 },
			{ .l = 0xbf56c1684d41bb32 },
		},
	},
	// 5 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7|], [|D...|], [1b-53,(7.5 - 2 * pi)/128]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110e608b + x^2 * 0xbf2a018a346250c8)))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f811111110e608b },
			{ .l = 0xbf2a018a346250c8 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8|], [|D...|], [1b-53,(7.5 - 2 * pi)/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16bcbbf9 + x^2 * 0x3efa017ece4a6516)))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16bcbbf9 },
			{ .l = 0x3efa017ece4a6516 },
		},
	},
	// 6 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,(7.5 - 2 * pi)/128]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f811111110c6f7b + x^2 * (0xbf2a015240d8c6d0 + x^2 * 0xbf080c1c1e3c62e5))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f811111110c6f7b },
			{ .l = 0xbf2a015240d8c6d0 },
			{ .l = 0xbf080c1c1e3c62e5 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,(7.5 - 2 * pi)/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16b9925e + x^2 * (0x3efa012abbd45ad8 + x^2 * 0x3ee106ca332f545d))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16b9925e },
			{ .l = 0x3efa012abbd45ad8 },
			{ .l = 0x3ee106ca332f545d },
		},
	},
	// 7 coefficients
	{
		// P = fpminimax(sin(x), [|1,3,5,7,9,11|], [|D...|], [1b-53,(7.5 - 2 * pi)/128]); printexpansion(P);
		// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111109dcd2 + x^2 * (0xbf2a00d081879994 + x^2 * (0xbf32f0415f70284d + x^2 * 0x3ff3dda127365792)))))
		{
			{ .l = 0 },
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfc5555555555555 },
			{ .l = 0x3f8111111109dcd2 },
			{ .l = 0xbf2a00d081879994 },
			{ .l = 0xbf32f0415f70284d },
			{ .l = 0x3ff3dda127365792 },
		},
		// P = fpminimax(cos(x), [|2,4,6,8,10,12|], [|D...|], [1b-53,(7.5 - 2 * pi)/128], 1); printexpansion(P);
		// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16b5a5fe + x^2 * (0x3efa007ac394f42b + x^2 * (0x3f080d7a92c116cb + x^2 * 0xbfc70f0b9bc330aa)))))
		{
			{ .l = 0x3ff0000000000000 },
			{ .l = 0xbfe0000000000000 },
			{ .l = 0x3fa5555555555555 },
			{ .l = 0xbf56c16c16b5a5fe },
			{ .l = 0x3efa007ac394f42b },
			{ .l = 0x3f080d7a92c116cb },
			{ .l = 0xbfc70f0b9bc330aa },
		},
	},
};


// link for all coefficients array
static const DL *link[COEFFICIENTS_VERSION];

static const double
interpolate[2][BITNUM] = {
	{
		0,
		9.5062216050696146663e-03,
		1.9011584130127399950e-02,
		2.8515228572796837841e-02,
		3.8016296085965010920e-02,
		4.7513928055396878469e-02,
		5.7007266177328479173e-02,
		6.6495452536032104152e-02,
		7.5977629681346386947e-02,
		8.5452940706164312590e-02,
		9.4920529323872182559e-02,
		1.0437953994573248573e-01,
		1.1382911775820366007e-01,
		1.2326840880018991031e-01,
		1.3269656004021385121e-01,
		1.4211271945350525958e-01,
		1.5151603609899874647e-01,
		1.6090566019623367411e-01,
		1.7028074320214883763e-01,
		1.7964043788776584698e-01,
		1.8898389841475313200e-01,
		1.9831028041186488098e-01,
		2.0761874105124694667e-01,
		2.1690843912460350262e-01,
		2.2617853511921723331e-01,
		2.3542819129381645227e-01,
		2.4465657175428190406e-01,
		2.5386284252918678295e-01,
		2.6304617164516314043e-01,
		2.7220572920208718770e-01,
		2.8134068744807805285e-01,
		2.9045022085430166614e-01,
		2.9953350618957413909e-01,
		3.0858972259475730970e-01,
		3.1761805165694018127e-01,
		3.2661767748339909367e-01,
		3.3558778677533007695e-01,
		3.4452756890134700329e-01,
		3.5343621597073848761e-01,
		3.6231292290647743037e-01,
		3.7115688751797570877e-01,
		3.7996731057357868711e-01,
		3.8874339587279188590e-01,
		3.9748435031823375896e-01,
		4.0618938398730836115e-01,
		4.1485771020359074601e-01,
		4.2348854560791904245e-01,
		4.3208111022918704869e-01,
		4.4063462755483034927e-01,
		4.4914832460100001521e-01,
		4.5762143198241733710e-01,
		4.6605318398190337392e-01,
		4.7444281861957710023e-01,
		4.8278957772171582352e-01,
		4.9109270698927132148e-01,
		4.9935145606603636992e-01,
		5.0756507860645427854e-01,
		5.1573283234306688261e-01,
		5.2385397915359288579e-01,
		5.3192778512763250198e-01,
		5.3995352063299129064e-01,
		5.4793046038161685729e-01,
		5.5585788349514364537e-01,
		5.6373507357003882490e-01,
		5.7156131874234372692e-01,
		5.7933591175200560563e-01,
		5.8705815000679240079e-01,
		5.9472733564578661447e-01,
		6.0234277560245108596e-01,
		6.0990378166726166853e-01,
		6.1740967054990070206e-01,
		6.2485976394100639641e-01,
		6.3225338857347179733e-01,
		6.3958987628328789476e-01,
		6.4686856406992632174e-01,
		6.5408879415625431619e-01,
		6.6124991404797861527e-01,
		6.6835127659261150956e-01,
		6.7539224003795395035e-01,
		6.8237216809009115792e-01,
		6.8929042997089451372e-01,
		6.9614640047502551745e-01,
		7.0293946002643514781e-01,
		7.0966899473435551826e-01,
		7.1633439644877772157e-01,
		7.2293506281540964586e-01,
		7.2947039733011165286e-01,
		7.3593980939280245757e-01,
		7.4234271436083199003e-01,
		7.4867853360181579880e-01,
		7.5494669454592588931e-01,
		7.6114663073763422219e-01,
		7.6727778188690376471e-01,
		7.7333959391982143305e-01,
		7.7933151902867048300e-01,
		7.8525301572143579865e-01,
		7.9110354887073863761e-01,
		7.9688258976219639163e-01,
		8.0258961614220258873e-01,
		8.0822411226512291815e-01,
		8.1378556893990383614e-01,
		8.1927348357608786866e-01,
		8.2468736022923261331e-01,
		8.3002670964573033174e-01,
		8.3529104930702080534e-01,
		8.4047990347319756488e-01,
		8.4559280322600005597e-01,
		8.5062928651119029677e-01,
		8.5558889818030881003e-01,
		8.6047119003180616570e-01,
		8.6527572085154746961e-01,
		8.7000205645268491317e-01,
		8.7464976971489538649e-01,
		8.7921844062297926925e-01,
		8.8370765630481762365e-01,
		8.8811701106868323752e-01,
		8.9244610643990363030e-01,
		8.9669455119687058176e-01,
		9.0086196140639551722e-01,
		9.0494796045840564247e-01,
		9.0895217909997794159e-01,
		9.1287425546870881732e-01,
		9.1671383512541604333e-01,
		9.2047057108616903154e-01,
		9.2414412385364619329e-01,
		9.2773416144781517545e-01,
		9.3124035943593441722e-01,
		9.3466240096187169772e-01,		
	},
	{
		1.0000000000000000000e+00,
		9.9995481485454895676e-01,
		9.9981926350159056049e-01,
		9.9959335819094008446e-01,
		9.9927711933772622732e-01,
		9.9887057552054592247e-01,
		9.9837376347888229233e-01,
		9.9778672810978419339e-01,
		9.9710952246380846198e-01,
		9.9634220774022652645e-01,
		9.9548485328149316498e-01,
		9.9453753656698007379e-01,
		9.9350034320597446769e-01,
		9.9237336692994193577e-01,
		9.9115670958405655000e-01,
		9.8985048111799656123e-01,
		9.8845479957600834719e-01,
		9.8696979108623894561e-01,
		9.8539558984933750541e-01,
		9.8373233812632776552e-01,
		9.8198018622575189429e-01,
		9.8013929249008657774e-01,
		9.7820982328143446516e-01,
		9.7619195296648919591e-01,
		9.7408586390077800399e-01,
		9.7189174641218256667e-01,
		9.6960979878373854124e-01,
		9.6724022723571689841e-01,
		9.6478324590698782970e-01,
		9.6223907683566867188e-01,
		9.5960794993905862427e-01,
		9.5689010299286059169e-01,
		9.5408578160969381710e-01,
		9.5119523921689741464e-01,
		9.4821873703362835606e-01,
		9.4515654404725479854e-01,
		9.4200893698904764051e-01,
		9.3877620030917252603e-01,
		9.3545862615098351878e-01,
		9.3205651432462233164e-01,
		9.2857017227992455499e-01,
		9.2499991507863477125e-01,
		9.2134606536593499637e-01,
		9.1760895334128667056e-01,
		9.1378891672859052786e-01,
		9.0988630074566656525e-01,
		9.0590145807305633152e-01,
		9.0183474882215153290e-01,
		8.9768654050265006550e-01,
		8.9345720798934447071e-01,
		8.8914713348824436778e-01,
		8.8475670650203619427e-01,
		8.8028632379488380710e-01,
		8.7573638935657305282e-01,
		8.7110731436600263855e-01,
		8.6639951715402596655e-01,
		8.6161342316564670796e-01,
		8.5674946492157089128e-01,
		8.5180808197911994650e-01,
		8.4678972089250781341e-01,
		8.4169483517248566695e-01,
		8.3652388524535825631e-01,
		8.3127733841137474435e-01,
		8.2595566880249871033e-01,
		8.2055935733956075762e-01,
		8.1508889168879761211e-01,
		8.0954476621778148626e-01,
		8.0392748195074403839e-01,
		7.9823754652329859116e-01,
		7.9247547413656482806e-01,
		7.8664178551070063072e-01,
		7.8073700783784405477e-01,
		7.7476167473447121736e-01,
		7.6871632619317287194e-01,
		7.6260150853385522129e-01,
		7.5641777435436907684e-01,
		7.5016568248057102775e-01,
		7.4384579791582239316e-01,
		7.3745869178992984327e-01,
		7.3100494130753201905e-01,
		7.2448512969593747979e-01,
		7.1789984615241797528e-01,
		7.1124968579096314869e-01,
		7.0453524958849944593e-01,
		6.9775714433057967057e-01,
		6.9091598255654773642e-01,
		6.8401238250418328057e-01,
		6.7704696805383124403e-01,
		6.7002036867202185988e-01,
		6.6293321935458537908e-01,
		6.5578616056926708477e-01,
		6.4857983819784859048e-01,
		6.4131490347777897476e-01,
		6.3399201294332219181e-01,
		6.2661182836622619785e-01,
		6.1917501669591812341e-01,
		6.1168224999923181961e-01,
		6.0413420539967355172e-01,
		5.9653156501622939256e-01,
		5.8887501590172253163e-01,
		5.8116524998072360830e-01,
		5.7340296398702172986e-01,
		5.6558885940066028208e-01,
		5.5772364238454386065e-01,
		5.4980802372062209660e-01,
		5.4184271874565648197e-01,
		5.3382844728657463662e-01,
		5.2576593359541967665e-01,
		5.1765590628389934746e-01,
		5.0949909825754102766e-01,
		5.0129624664945870993e-01,
		4.9304809275373867594e-01,
		4.8475538195844775080e-01,
		4.7641886367827285254e-01,
		4.6803929128679616634e-01,
		4.5961742204841227188e-01,
		4.5115401704989421816e-01,
		4.4264984113161376378e-01,
		4.3410566281842244418e-01,
		4.2552225425020001603e-01,
		4.1690039111207566336e-01,
		4.0824085256432934843e-01,
		3.9954442117197874751e-01,
		3.9081188283405848827e-01,
		3.8204402671259851676e-01,
		3.7324164516130714508e-01,
		3.6440553365396577412e-01,
		3.5553649071254178615e-01,
	}
};

static const DL width_inv[BIT + 1] =
{
	{.l = 0x3fea4c54e3510434}, // width_inv		1 / (7 - 2 * pi)
	{.l = 0x3ffa4c54e3510434}, // width_inv		2 / (7 - 2 * pi)
	{.l = 0x400a4c54e3510434}, // width_inv		4 / (7 - 2 * pi)
	{.l = 0x401a4c54e3510434}, // width_inv		8 / (7 - 2 * pi)
	{.l = 0x402a4c54e3510434}, // width_inv		16 / (7 - 2 * pi)
	{.l = 0x403a4c54e3510434}, // width_inv		32 / (7 - 2 * pi)
	{.l = 0x404a4c54e3510434}, // width_inv		64 / (7 - 2 * pi)
	{.l = 0x405a4c54e3510434}, // width_inv		128 / (7 - 2 * pi)
};

/*
static const double width_inv[BIT + 1] =
{
	8.21817821481210013e-01,
	1.64363564296242003e+00,
	3.28727128592484005e+00,
	6.57454257184968010e+00,
	1.31490851436993602e+01,
	2.62981702873987204e+01,
	5.25963405747974408e+01,
	1.05192681149594882e+02,	
}；
*/

static const DL width[BIT + 1] =
{
	{.l = 0x3ff37812aeef4b9f}, // width		(7 - 2 * pi) / 1
	{.l = 0x3fe37812aeef4b9f}, // width		(7 - 2 * pi) / 2
	{.l = 0x3fd37812aeef4b9f}, // width		(7 - 2 * pi) / 4
	{.l = 0x3fc37812aeef4b9f}, // width		(7 - 2 * pi) / 8
	{.l = 0x3fb37812aeef4b9f}, // width		(7 - 2 * pi) / 16
	{.l = 0x3fa37812aeef4b9f}, // width		(7 - 2 * pi) / 32
	{.l = 0x3f937812aeef4b9f}, // width		(7 - 2 * pi) / 64
	{.l = 0x3f837812aeef4b9f}, // width		(7 - 2 * pi) / 128
};

/*
static const double width[BIT + 1] =
{
	1.21681469282041352e+00,
	6.08407346410206762e-01,
	3.04203673205103381e-01,
	1.52101836602551690e-01,
	7.60509183012758452e-02,
	3.80254591506379226e-02,
	1.90127295753189613e-02,
	9.50636478765948065e-03,
}；
*/

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
	
	if ((func = fopen("sin6-7p5_gen.c", "w")) == (FILE *)0) {
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
		
		//fprintf(func, "static const double\n");
		//fprintf(func, "pio2_1 = 1.57079632673412561417e+00, // 0x3FF921FB, 0x54400000\n");
		//fprintf(func, "pio2_1t = 6.07710050650619224932e-11; // 0x3DD0B461, 0x1A626331\n");

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
		fprintf(func, "static const DL\n");
		// 2 * pi = 6.28318530717958647692
		// newA = 0x401921fb54440000, 6.28318530716933310032e+00 newB = 0x3da68c234c4c6629, 1.02533766063780756068e-11
		fprintf(func, "// basePoint = { .l = 0x401921fb54442d18},\n");
		fprintf(func, "// basePoint_1 = { .l = 0x401921fb54440000},\n");
		fprintf(func, "// basePoint_1t = { .l = 0x3da68c234c4c6629},\n");
		// newA = 0x3ff37812aeef0000, 1.216815e+00 newB = 0x3d92e7b9676733af, 4.298539e-12
		fprintf(func, "// width_1 = { .l = 0x3ff37812aeef0000}, // for BIT = 1\n");
		fprintf(func, "// width_1t = { .l = 0x3d92e7b9676733af}, // for BIT = 1\n");
		fprintf(func, "width = { .l = 0x%016lx},\n", width[bit]);
		fprintf(func, "width_inv = { .l = 0x%016lx};\n", width_inv[bit]);
		fprintf(func, "\n");
		fprintf(func, "static const double\n");
		fprintf(func, "basePoint = 6.28318530717958647692,\n");
		fprintf(func, "basePoint_1 = 6.28318530716933310032,\n");
		fprintf(func, "basePoint_1t = 1.02533766063780756068e-11;\n");

		// func
		//fprintf(func, "double sin_gen(double x) {\n");
		//fprintf(func, "\tdouble ix, iix, iiix, y, appro_s, appro_c;\n");
		fprintf(func, "double sin_gen(double x) {\n");
		fprintf(func, "\tdouble ix, iiix, appro_s, appro_c, z;\n");
		fprintf(func, "\tlong temp, flag, status_pi_2, table_order;\n");
		fprintf(func, "\tdouble newX;\n");
		fprintf(func, "\tDL y;\n");
		fprintf(func, "\n");
		
		fprintf(func, "\tstatus_pi_2 = 0;\n");
		fprintf(func, "\tnewX = x - basePoint_1;\n");
		fprintf(func, "\tnewX = newX - basePoint_1t;\n");
		fprintf(func, "\t// newX = x - basePoint;\n");
		fprintf(func, "\n");
		
		fprintf(func, "\ttemp = *((long int *)(&newX));\n");
		fprintf(func, "\tflag = temp & 0x8000000000000000;\n");
		fprintf(func, "\ttemp = temp & 0x7fffffffffffffff;\n");
		fprintf(func, "\tix = *((double *)(&temp)); // at this time, ix is absolute value\n");
		//fprintf(func, "\tix = x - X;\n");
		fprintf(func, "\t// if ix is too little, then return ix;\n");
		fprintf(func, "\tif (temp < 0x3e40000000000000) {\n");
		fprintf(func, "\t\treturn (1 - flag * 2) * ix;\n");
		fprintf(func, "\t}\n");
		fprintf(func, "\n");
		
		fprintf(func, "\ttable_order = (long)(ix * width_inv.d);\n");
		fprintf(func, "\tiiix = ix - ((double)table_order) * width.d;\n");
		fprintf(func, "\t// iiix = ix - ((double)table_order) / BITNUM * width_1.d;\n");
		fprintf(func, "\t// iiix = iiix - ((double)table_order) / BITNUM * width_1t.d;\n");
		fprintf(func, "\tz = iiix * iiix;\n");
		fprintf(func, "\n");
		
		fprintf(func, "\t// new, sin(x+x') = sin(x)cos(x') + sin(x')cos(x), x' = iiix\n");
		fprintf(func, "\t// OR, cos(x+x') = cos(x)cos(x') - sin(x)sin(x'), x' = iiix\n");

		if (degree == 0) {
			fprintf(func, "\tappro_s = coefficient[0][0].d;\n");
			fprintf(func, "\tappro_c = coefficient[1][0].d;\n");
			fprintf(func, "\n");
			fprintf(func, "\ty.d = interpolate[status_pi_2][table_order] * appro_c + interpolate[1 - status_pi_2][table_order] * appro_s;\n");
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
			fprintf(func, "\ty.d = interpolate[status_pi_2][table_order] * appro_c + interpolate[1 - status_pi_2][table_order] * appro_s;\n");
			fprintf(func, "\ty.d += interpolate[status_pi_2][table_order] * coefficient[1][0].d;\n");
		}
		
		fprintf(func, "\ty.l = y.l | flag;\n");
		fprintf(func, "\n");
		
		fprintf(func, "\treturn y.d;\n");
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