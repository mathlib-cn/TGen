#include <stdio.h>
#include <stdlib.h>
#include "myhead.h"

#define DEGREE 8
#define COEFFICIENTS 8
#define COEFFICIENTS_VERSION 8
#define	FORMAT 64
#define BIT 7
#define BITNUM 128
#define BITNUM_1 127

struct constraint {
	double start;
	double end;
	int precision;
	int bit;
	int fnum;
	int degree;
};

static const DL
ln2byHI[BIT] = {
	{.l = 0x3fd62e42fee00000 }, // 1
	{.l = 0x3fc62e42fee00000 }, // 2
	{.l = 0x3fb62e42fee00000 }, // 3
	{.l = 0x3fa62e42fee00000 }, // 4
	{.l = 0x3f962e42fee00000 }, // 5
	{.l = 0x3f862e42fee00000 }, // 6
	{.l = 0x3f762e42fee00000 }, // 7
},
ln2byLO[BIT] = {
	{.l = 0x3dda39ef35793c76 }, // 1
	{.l = 0x3dca39ef35793c76 }, // 2
	{.l = 0x3dba39ef35793c76 }, // 3
	{.l = 0x3daa39ef35793c76 }, // 4
	{.l = 0x3d9a39ef35793c76 }, // 5
	{.l = 0x3d8a39ef35793c76 }, // 6
	{.l = 0x3d7a39ef35793c76 }, // 7
},
invln2by[BIT] = {
	{.l = 0x40071547652b82fe }, // 1
	{.l = 0x40171547652b82fe }, // 2
	{.l = 0x40271547652b82fe }, // 3
	{.l = 0x40371547652b82fe }, // 4
	{.l = 0x40471547652b82fe }, // 5
	{.l = 0x40571547652b82fe }, // 6
	{.l = 0x40671547652b82fe }, // 7
};

// fpminmax: [0, ln2/128]
static const DL
coefficient_ln2div_128[COEFFICIENTS][COEFFICIENTS] = { 
	// P = fpminimax(exp(x), 0, [|D...|], [1b-53,log(2)/128]); printexpansion(P);
	// 0x3ff00b171fb8cce7
	{
		{.l = 0x3ff00b171fb8cce7}
	},
	// P = fpminimax(exp(x),[| 1 | ],[| D... | ],[1b - 53,log(2) / 128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * 0x3ff00932cd89057f
	// P = fpminimax(exp(x), 1, [|D...|], [1b-53,log(2)/128]); printexpansion(P);
	// 0x3feffffc28085a53 + x * 0x3ff00b1a55ea36dc
	{
		{.l = 0x3feffffc28085a53},
		{.l = 0x3ff00b1a55ea36dc}
	},
	// P = fpminimax(exp(x), [|1,2|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3feffffb9477c883 + x * 0x3fe00a4e59041d9d)
	// P = fpminimax(exp(x), 2, [|D...|], [1b-53,log(2)/128]); printexpansion(P);
	// 0x3ff000000038d610 + x * (0x3feffffa3a7e7402 + x * 0x3fe00b1a7f051224)
	{
		{.l = 0x3ff000000038d610},
		{.l = 0x3feffffa3a7e7402},
		{.l = 0x3fe00b1a7f051224}
	},
	// P = fpminimax(exp(x), [|1,2,3|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000538109 + x * (0x3fdffffa494bd1eb + x * 0x3fc5638d68876c7f))
	// P = fpminimax(exp(x), 3, [| D... | ], [1b - 53, log(2) / 128]); printexpansion(P);
	// 0x3feffffffffff627 + x * (0x3ff000000071c5c4 + x * (0x3fdffff995f16a5d + x * 0x3fc5642370510f12))
	{
		{.l = 0x3feffffffffff627},
		{.l = 0x3ff000000071c5c4},
		{.l = 0x3fdffff995f16a5d},
		{.l = 0x3fc5642370510f12}
	},
	// P = fpminimax(exp(x), [|0,1,2,3,4|], [|D...|], [1b-53,log(2)/128]); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3fefffffffffedee + x * (0x3fe00000007d30f8 + x * (0x3fc555511976763b + x * 0x3fa563cf3c344455)))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3fefffffffffedee},
		{.l = 0x3fe00000007d30f8},
		{.l = 0x3fc555511976763b},
		{.l = 0x3fa563cf3c344455}
	},
	// P = fpminimax(exp(x), [|0,1,2,3,4,5|], [|D...|], [1b-53,log(2)/128]); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000000001 + x * (0x3fdfffffffffdc17 + x * (0x3fc555555631bb85 + x * (0x3fa5555099ee69d1 + x * 0x3f811d0bfc4aaa45))))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff0000000000001},
		{.l = 0x3fdfffffffffdc17},
		{.l = 0x3fc555555631bb85},
		{.l = 0x3fa5555099ee69d1},
		{.l = 0x3f811d0bfc4aaa45}
	},
	// P = fpminimax(exp(x), [|0,1,2,3,4,5,6|], [|D...|], [1b-53,log(2)/128]); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000000000 + x * (0x3fe0000000000001 + x * (0x3fc5555555553ced + x * (0x3fa5555556351b25 + x * (0x3f81110d40415ef6 + x * 0x3f56d16f7657ba80)))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fe0000000000001 },
		{.l = 0x3fc5555555553ced },
		{.l = 0x3fa5555556351b25 },
		{.l = 0x3f81110d40415ef6 },
		{.l = 0x3f56d16f7657ba80 },
	},
	// P = fpminimax(exp(x), [|0,1,2,3,4,5,6,7|], [|D...|], [1b-53,log(2)/128]); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000000000 + x * (0x3fe0000000000000 + x * (0x3fc5555555555556 + x * (0x3fa5555555554678 + x * (0x3f811111119057bf + x * (0x3f56c167f90022e5 + x * 0x3f2a12305f6902c9))))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fe0000000000000 },
		{.l = 0x3fc5555555555565 },
		{.l = 0x3fa5555555554678 },
		{.l = 0x3f811111119057bf },
		{.l = 0x3f56c167f90022e5 },
		{.l = 0x3f2a12305f6902c9 },
	}
};

// fpminmax: [0, ln2/64]
static const DL
coefficient_ln2div_64[COEFFICIENTS][COEFFICIENTS] = {
	// P = fpminimax(exp(x), 0, [|D...|], [1b-53,log(2)/64]); printexpansion(P);
	// 0x3ff0162e34c980bb
	{
		{.l = 0x3ff0162e34c980bb}
	},
	// P = fpminimax(exp(x), 1, [|D...|], [1b-53,log(2)/64]); printexpansion(P);
	// 0x3feffff0a022a478 + x * 0x3ff0163b166fb309
	{
		{.l = 0x3feffff0a022a478},
		{.l = 0x3ff0163b166fb309}
	},
	// P = fpminimax(exp(x), 2, [|D...|], [1b-53,log(2)/64]); printexpansion(P);
	// 0x3ff0000001c6b064 + x * (0x3fefffe8e3c16fd5 + x * 0x3fe0163bbb53255a)
	{
		{.l = 0x3ff0000001c6b064},
		{.l = 0x3fefffe8e3c16fd5},
		{.l = 0x3fe0163bbb53255a}
	},
	// P = fpminimax(exp(x), 3, [|D...|], [1b-53,log(2)/64]); printexpansion(P);
	// 0x3fefffffffff626b + x * (0x3ff00000038f0a2d + x * (0x3fdfffe64f26a8c7 + x * 0x3fc572fad279172d))
	{
		{.l = 0x3fefffffffff626b},
		{.l = 0x3ff00000038f0a2d},
		{.l = 0x3fdfffe64f26a8c7},
		{.l = 0x3fc572fad279172d}
	},
	// P = fpminimax(exp(x), 4, [|D...|], [1b-53,log(2)/64]); printexpansion(P);
	// 0x3ff000000000000b + x * (0x3feffffffffe74ac + x * (0x3fe00000047483da + x * (0x3fc5554356945409 + x * 0x3fa572fc5cd74a87)))
	{
		{.l = 0x3ff000000000000b},
		{.l = 0x3feffffffffe74ac},
		{.l = 0x3fe00000047483da},
		{.l = 0x3fc5554356945409},
		{.l = 0x3fa572fc5cd74a87}
	},
	// P = fpminimax(exp(x), 5, [|D...|], [1b-53,log(2)/64]); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000000018 + x * (0x3fdffffffffe1d6f + x * (0x3fc555555b88b12d + x * (0x3fa555437944ad86 + x * 0x3f812872cdebb4c5))))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff0000000000018},
		{.l = 0x3fdffffffffe1d6f},
		{.l = 0x3fc555555b88b12d},
		{.l = 0x3fa555437944ad86},
		{.l = 0x3f812872cdebb4c5}
	},
	// P = fpminimax(exp(x), 6, [|D...|], [1b-53,log(2)/64]); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000000000 + x * (0x3fe0000000000012 + x * (0x3fc55555555452e6 + x * (0x3fa555555a9c7dfe + x * (0x3f81110437350ca8 + x * 0x3f56df023bc86d0c)))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fe0000000000012 },
		{.l = 0x3fc55555555452e6 },
		{.l = 0x3fa555555a9c7dfe },
		{.l = 0x3f81110437350ca8 },
		{.l = 0x3f56df023bc86d0c },
	},
	// P = fpminimax(exp(x), 7, [|D...|], [1b-53,log(2)/64]); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000000000 + x * (0x3fe0000000000000 + x * (0x3fc5555555555565 + x * (0x3fa555555554975b + x * (0x3f81111114798f0e + x * (0x3f56c15d1d27863e + x * 0x3f2a2155f216d174))))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fe0000000000000 },
		{.l = 0x3fc5555555555565 },
		{.l = 0x3fa555555554975b },
		{.l = 0x3f81111114798f0e },
		{.l = 0x3f56c15d1d27863e },
		{.l = 0x3f2a2155f216d174 },
	}
};

// fpminmax: [0, ln2/32]
static const DL
coefficient_ln2div_32[COEFFICIENTS][COEFFICIENTS] = {
	// P = fpminmax(exp(x), 0, [|D...|], [1b-53,log(2)/32]); printexpansion(P);
	// 0x3ff02c5c14532e9c
	{
		{.l = 0x3ff02c5c14532e9c}
	},
	// P = fpminimax(exp(x), 1, [|D...|], [1b-53,log(2)/32]); printexpansion(P);
	// 0x3fefffc2809e44c3 + x * 0x3ff02c8fe1e8584c
	{
		{.l = 0x3fefffc2809e44c3},
		{.l = 0x3ff02c8fe1e8584c}
	},
	// P = fpminimax(exp(x), 2, [|D...|], [1b-53,log(2)/32]); printexpansion(P);
	// 0x3ff000000e357f4d + x * (0x3fefffa35d3b9258 + x * 0x3fe02c92791bccea)
	{
		{.l = 0x3ff000000e357f4d},
		{.l = 0x3fefffa35d3b9258},
		{.l = 0x3fe02c92791bccea}
	},
	// P = fpminimax(exp(x), 3, [|D...|], [1b-53,log(2)/32]); printexpansion(P);
	// 0x3feffffffff626ad + x * (0x3ff000001c85a218 + x * (0x3fdfff98f8298e5b + x * 0x3fc590c55eb4a9d3))
	{
		{.l = 0x3feffffffff626ad},
		{.l = 0x3ff000001c85a218},
		{.l = 0x3fdfff98f8298e5b},
		{.l = 0x3fc590c55eb4a9d3}
	},
	// P = fpminimax(exp(x), 4, [|D...|], [1b-53,log(2)/32]); printexpansion(P);
	// 0x3ff000000000015e + x * (0x3fefffffffe74a5f + x * (0x3fe0000023b47d2f + x * (0x3fc5550d2c09b74b + x * 0x3fa590c73530f0fe)))
	{
		{.l = 0x3ff000000000015e},
		{.l = 0x3fefffffffe74a5f},
		{.l = 0x3fe0000023b47d2f},
		{.l = 0x3fc5550d2c09b74b},
		{.l = 0x3fa590c73530f0fe}
	},
	// P = fpminimax(exp(x), 5, [|D...|], [1b-53,log(2)/32]); printexpansion(P);
	// 0x3fefffffffffffff + x * (0x3ff00000000004c6 + x * (0x3fdfffffffdaad44 + x * (0x3fc555558ce76101 + x * (0x3fa555097d6d0f86 + x * 0x3f81410ba1e01a92))))
	{
		{.l = 0x3fefffffffffffff},
		{.l = 0x3ff00000000004c6},
		{.l = 0x3fdfffffffdaad44},
		{.l = 0x3fc555558ce76101},
		{.l = 0x3fa555097d6d0f86},
		{.l = 0x3f81410ba1e01a92}
	},
	// P = fpminimax(exp(x), 6, [|D...|], [1b-53,log(2)/32]); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3feffffffffffffe + x * (0x3fe0000000000659 + x * (0x3fc5555555396115 + x * (0x3fa555558f0f844c + x * (0x3f8110d42461a6d8 + x * 0x3f57010ed2532e59)))))	
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3feffffffffffffe },
		{.l = 0x3fe0000000000659 },
		{.l = 0x3fc5555555396115 },
		{.l = 0x3fa555558f0f844c },
		{.l = 0x3f8110d42461a6d8 },
		{.l = 0x3f57010ed2532e59 },
	},
	// P = fpminimax(exp(x), 7, [|D...|], [1b-53,log(2)/32]); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000000000 + x * (0x3fdfffffffffffff + x * (0x3fc5555555555a89 + x * (0x3fa5555555419729 + x * (0x3f81111135650b5d + x * (0x3f56c1262900e0f7 + x * 0x3f2a455dd754eece))))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fdfffffffffffff },
		{.l = 0x3fc5555555555a89 },
		{.l = 0x3fa5555555419729 },
		{.l = 0x3f81111135650b5d },
		{.l = 0x3f56c1262900e0f7 },
		{.l = 0x3f2a455dd754eece },
	}
};

// fpminmax: [0, ln2/16]
static const DL
coefficient_ln2div_16[COEFFICIENTS][COEFFICIENTS] = {
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/16]); printexpansion(P);i=i+1;
	// 0x3ff058b57ec67ce8
	{
		{.l = 0x3ff058b57ec67ce8}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/16]); printexpansion(P);i=i+1;
	// 0x3fefff0a03b44b4e + x * 0x3ff05986ec6dc5ea
	{
		{.l = 0x3fefff0a03b44b4e},
		{.l = 0x3ff05986ec6dc5ea}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/16]); printexpansion(P);i=i+1;
	// 0x3ff0000071ab7f85 + x * (0x3feffe8be6283015 + x * 0x3fe05991671450d0)
	{
		{.l = 0x3ff0000071ab7f85},
		{.l = 0x3feffe8be6283015},
		{.l = 0x3fe05991671450d0}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/16]); printexpansion(P);i=i+1;
	// 0x3fefffffff626b66 + x * (0x3ff00000e501ed1e + x * (0x3fdffe61bb8ed3b0 + x * 0x3fc5ccca3fd3a3b9))
	{
		{.l = 0x3fefffffff626b66},
		{.l = 0x3ff00000e501ed1e},
		{.l = 0x3fdffe61bb8ed3b0},
		{.l = 0x3fc5ccca3fd3a3b9}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/16]); printexpansion(P);i=i+1;
	// 0x3ff0000000002bb1 + x * (0x3feffffffe738982 + x * (0x3fe000011f0340cf + x * (0x3fc5543311ab5298 + x * 0x3fa5cccfd8b7c53c)))
	{
		{.l = 0x3ff0000000002bb1},
		{.l = 0x3feffffffe738982},
		{.l = 0x3fe000011f0340cf},
		{.l = 0x3fc5543311ab5298},
		{.l = 0x3fa5cccfd8b7c53c}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/16]); printexpansion(P);i=i+1;
	// 0x3fefffffffffffd8 + x * (0x3ff0000000008376 + x * (0x3fdffffffdd412d3 + x * (0x3fc55557023e90ac + x * (0x3fa5542ab580ecf6 + x * 0x3f8170a5ec861e7b))))
	{
		{.l = 0x3fefffffffffffd8},
		{.l = 0x3ff0000000008376},
		{.l = 0x3fdffffffdd412d3},
		{.l = 0x3fc55557023e90ac},
		{.l = 0x3fa5542ab580ecf6},
		{.l = 0x3f8170a5ec861e7b}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/16]); printexpansion(P);i=i+1;
	// 0x3ff0000000000000 + x * (0x3fefffffffffff9b + x * (0x3fe000000000afc5 + x * (0x3fc5555553bc6d73 + x * (0x3fa555570b2ecffe + x * (0x3f8110240cb95772 + x * 0x3f573f7397883f32)))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fefffffffffff9b },
		{.l = 0x3fe000000000afc5 },
		{.l = 0x3fc5555553bc6d73 },
		{.l = 0x3fa555570b2ecffe },
		{.l = 0x3f8110240cb95772 },
		{.l = 0x3f573f7397883f32 },
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/16]); printexpansion(P);i=i+1;
	// 0x3ff0000000000000 + x * (0x3ff0000000000000 + x * (0x3fdfffffffffffb0 + x * (0x3fc555555556167c + x * (0x3fa5555553f804d1 + x * (0x3f81111247bd705f + x * (0x3f56c0489a6295dd + x * 0x3f2a8bea84866969))))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3ff0000000000000 },
		{.l = 0x3fdfffffffffffb0 },
		{.l = 0x3fc555555556167c },
		{.l = 0x3fa5555553f804d1 },
		{.l = 0x3f81111247bd705f },
		{.l = 0x3f56c0489a6295dd },
		{.l = 0x3f2a8bea84866969 },
	}
};

// fpminmax: [0, ln2/8]
static const DL
coefficient_ln2div_8[COEFFICIENTS][COEFFICIENTS] = {
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/8]); printexpansion(P);i=i+1;
	// 0x3ff0b155b2640b16
	{
		{.l = 0x3ff0b155b2640b16}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/8]); printexpansion(P);i=i+1;
	// 0x3feffc2822873ad1 + x * 0x3ff0b4ad186dfd80
	{
		{.l = 0x3feffc2822873ad1},
		{.l = 0x3ff0b4ad186dfd80}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/8]); printexpansion(P);i=i+1;
	// 0x3ff000038d4ca023 + x * (0x3feffa231b4ec420 + x * 0x3fe0b4d7fdb177c3)
	{
		{.l = 0x3ff000038d4ca023},
		{.l = 0x3feffa231b4ec420},
		{.l = 0x3fe0b4d7fdb177c3}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/8]); printexpansion(P);i=i+1;
	// 0x3feffffff626dabc + x * (0x3ff00007355f2420 + x * (0x3fdff975a9ab9c56 + x * 0x3fc6469791b2ab36))
		{
		{.l = 0x3feffffff626dabc},
		{.l = 0x3ff00007355f2420},
		{.l = 0x3fdff975a9ab9c56},
		{.l = 0x3fc6469791b2ab36}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/8]); printexpansion(P);i=i+1;
	// 0x3ff0000000057609 + x * (0x3fefffffe71099c8 + x * (0x3fe000090ee71ccd + x * (0x3fc550bf0b88487e + x * 0x3fa646ae671bde9a)))
	{
		{.l = 0x3ff0000000057609},
		{.l = 0x3fefffffe71099c8},
		{.l = 0x3fe000090ee71ccd},
		{.l = 0x3fc550bf0b88487e},
		{.l = 0x3fa646ae671bde9a}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/8]); printexpansion(P);i=i+1;
	// 0x3feffffffffff5e8 + x * (0x3ff000000010911e + x * (0x3fdfffffdce42ea7 + x * (0x3fc55562e5e571d4 + x * (0x3fa5509c18862623 + x * 0x3f81d2322045c89c))))
	{
		{.l = 0x3feffffffffff5e8},
		{.l = 0x3ff000000010911e},
		{.l = 0x3fdfffffdce42ea7},
		{.l = 0x3fc55562e5e571d4},
		{.l = 0x3fa5509c18862623},
		{.l = 0x3f81d2322045c89c}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/8]); printexpansion(P);i=i+1;
	// 0x3ff0000000000004 + x * (0x3fefffffffffdc50 + x * (0x3fe000000019ebf9 + x * (0x3fc555553927111b + x * (0x3fa55563e4911d21 + x * (0x3f810d36f9436e4e + x * 0x3f57c2faf6f30cda)))))
	{
		{.l = 0x3ff0000000000004 },
		{.l = 0x3fefffffffffdc50 },
		{.l = 0x3fe000000019ebf9 },
		{.l = 0x3fc555553927111b },
		{.l = 0x3fa55563e4911d21 },
		{.l = 0x3f810d36f9436e4e },
		{.l = 0x3f57c2faf6f30cda },
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/8]); printexpansion(P);i=i+1;
	// 0x3ff0000000000000 + x * (0x3ff000000000000b + x * (0x3fdfffffffffccec + x * (0x3fc55555557d5474 + x * (0x3fa5555537b1f6df + x * (0x3f81111cca819ad5 + x * (0x3f56bc5613d2d58b + x * 0x3f2b250e621abf42))))))
	{
		{.l = 0x3ff0000000000000 },
		{.l = 0x3ff000000000000b },
		{.l = 0x3fdfffffffffccec },
		{.l = 0x3fc55555557d5474 },
		{.l = 0x3fa5555537b1f6df },
		{.l = 0x3f81111cca819ad5 },
		{.l = 0x3f56bc5613d2d58b },
		{.l = 0x3f2b250e621abf42 },
	}
};

// fpminmax: [0, ln2/4]
static const DL
coefficient_ln2div_4[COEFFICIENTS][COEFFICIENTS] = {
	// P = fpminmax(exp(x), i, [|D...|], [1b-53,log(2)/4]); printexpansion(P);i=i+1;
	// 0x3ff1620185e56214
	{
		{.l = 0x3ff1620185e56214}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/4]); printexpansion(P);i=i+1;
	// 0x3feff0a1c61fee42 + x * 0x3ff16feba2bf45a4
	{
		{.l = 0x3feff0a1c61fee42},
		{.l = 0x3ff16feba2bf45a4}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/4]); printexpansion(P);i=i+1;
	// 0x3ff0001c686f1e7b + x * (0x3fefe82817b9ec9d + x * 0x3fe1709fd3e9d201)
	{
		{.l = 0x3ff0001c686f1e7b},
		{.l = 0x3fefe82817b9ec9d},
		{.l = 0x3fe1709fd3e9d201}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/4]); printexpansion(P);i=i+1;
	// 0x3fefffff6276c135 + x * (0x3ff0003a8025b98b + x * (0x3fdfe54ac47ad26b + x * 0x3fc7416421ca0778))
	{
		{.l = 0x3fefffff6276c135},
		{.l = 0x3ff0003a8025b98b},
		{.l = 0x3fdfe54ac47ad26b},
		{.l = 0x3fc7416421ca0778}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/4]); printexpansion(P);i=i+1;
	// 0x3ff0000000aeb856 + x * (0x3feffffe6c0a4aab + x * (0x3fe00049e7dab9da + x * (0x3fc54290a5d78482 + x * 0x3fa741c37b948b28)))
	{
		{.l = 0x3ff0000000aeb856},
		{.l = 0x3feffffe6c0a4aab},
		{.l = 0x3fe00049e7dab9da},
		{.l = 0x3fc54290a5d78482},
		{.l = 0x3fa741c37b948b28}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/4]); printexpansion(P);i=i+1;
	// 0x3feffffffffd7a14 + x * (0x3ff000000217fb2d + x * (0x3fdffffdc3f0ecfd + x * (0x3fc555c4525631d8 + x * (0x3fa541fbc5ef509b + x * 0x3f829b394603f446))))
	{
		{.l = 0x3feffffffffd7a14},
		{.l = 0x3ff000000217fb2d},
		{.l = 0x3fdffffdc3f0ecfd},
		{.l = 0x3fc555c4525631d8},
		{.l = 0x3fa541fbc5ef509b},
		{.l = 0x3f829b394603f446}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/4]); printexpansion(P);i=i+1;
	// 0x3ff0000000000200 + x * (0x3feffffffff6fd8d + x * (0x3fe00000034b82d1 + x * (0x3fc5555388a12f5e + x * (0x3fa555ccac73932d + x * (0x3f8101465aecd1a3 + x * 0x3f58cf2e727a6005)))))
	{
		{.l = 0x3ff0000000000200 },
		{.l = 0x3feffffffff6fd8d },
		{.l = 0x3fe00000034b82d1 },
		{.l = 0x3fc5555388a12f5e },
		{.l = 0x3fa555ccac73932d },
		{.l = 0x3f8101465aecd1a3 },
		{.l = 0x3f58cf2e727a6005 },
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/4]); printexpansion(P);i=i+1;
	// 0x3feffffffffffffa + x * (0x3ff0000000000856 + x * (0x3fdffffffff02ffa + x * (0x3fc555555b10620c + x * (0x3fa555534b9630e2 + x * (0x3f8111759d2233e0 + x * (0x3f56ac08b231ac58 + x * 0x3f2c5b1bc70ff176))))))
	{
		{.l = 0x3feffffffffffffa },
		{.l = 0x3ff0000000000856 },
		{.l = 0x3fdffffffff02ffa },
		{.l = 0x3fc555555b10620c },
		{.l = 0x3fa555534b9630e2 },
		{.l = 0x3f8111759d2233e0 },
		{.l = 0x3f56ac08b231ac58 },
		{.l = 0x3f2c5b1bc70ff176 },
	}
};

// fpminmax: [0, ln2/2]
static const DL
coefficient_ln2div_2[COEFFICIENTS][COEFFICIENTS] = {
	// P = fpminmax(exp(x), i, [|D...|], [1b-53,log(2)/2]); printexpansion(P);i=i+1;
	// 0x3ff2bec333018867
	{
		{.l = 0x3ff2bec333018867}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/2]); printexpansion(P);i=i+1;
	// 0x3fefc29b010051c2 + x * 0x3ff2fab7f2ad6eeb

	{
		{.l = 0x3fefc29b010051c2},
		{.l = 0x3ff2fab7f2ad6eeb}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/2]); printexpansion(P);i=i+1;
	// 0x3ff000e306506de4 + x * (0x3fef9d7799061f80 + x * 0x3fe2fddbb12352e3)
	{
		{.l = 0x3ff000e306506de4},
		{.l = 0x3fef9d7799061f80},
		{.l = 0x3fe2fddbb12352e3}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/2]); printexpansion(P);i=i+1;
	// 0x3feffff629b11fc1 + x * (0x3ff001e1576384f6 + x * (0x3fdf90b009c3fc5d + x * 0x3fc954e96fec627c))
	{
		{.l = 0x3feffff629b11fc1},
		{.l = 0x3ff001e1576384f6},
		{.l = 0x3fdf90b009c3fc5d},
		{.l = 0x3fc954e96fec627c}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/2]); printexpansion(P);i=i+1;
	// 0x3ff0000015d2abe5 + x * (0x3fefffe6212787aa + x * (0x3fe00266c06c7cc4 + x * (0x3fc506cb0bab2ab6 + x * 0x3fa95688fa87c4c0)))
	{
		{.l = 0x3ff0000015d2abe5},
		{.l = 0x3fefffe6212787aa},
		{.l = 0x3fe00266c06c7cc4},
		{.l = 0x3fc506cb0bab2ab6},
		{.l = 0x3fa95688fa87c4c0}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/2]); printexpansion(P);i=i+1;
	// 0x3fefffffff5ea175 + x * (0x3ff000004472dc4a + x * (0x3fdfffdaef74e215 + x * (0x3fc558f5e5ad8ee7 + x * (0x3fa504293563ec3b + x * 0x3f844627e16e4a23))))
	{
		{.l = 0x3fefffffff5ea175},
		{.l = 0x3ff000004472dc4a},
		{.l = 0x3fdfffdaef74e215},
		{.l = 0x3fc558f5e5ad8ee7},
		{.l = 0x3fa504293563ec3b},
		{.l = 0x3f844627e16e4a23}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/2]); printexpansion(P);i=i+1;
	// 0x3ff000000000ffb0 + x * (0x3feffffffdb462a6 + x * (0x3fe000006cfa2eda + x * (0x3fc555374888f46c + x * (0x3fa5593f61401642 + x * (0x3f80ceb6103be07e + x * 0x3f5b0922b1531ec0)))))
	{
		{.l = 0x3ff000000000ffb0 },
		{.l = 0x3feffffffdb462a6 },
		{.l = 0x3fe000006cfa2eda },
		{.l = 0x3fc555374888f46c },
		{.l = 0x3fa5593f61401642 },
		{.l = 0x3f80ceb6103be07e },
		{.l = 0x3f5b0922b1531ec0 },
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)/2]); printexpansion(P);i=i+1;
	// 0x3feffffffffffa76 + x * (0x3ff00000000423e2 + x * (0x3fdffffffbf98430 + x * (0x3fc5555612d9bd6e + x * (0x3fa555334e960925 + x * (0x3f81145d0d37f475 + x * (0x3f5667966a599798 + x * 0x3f2ee6b1cfd8d0fd))))))
	{
		{.l = 0x3feffffffffffa76 },
		{.l = 0x3ff00000000423e2 },
		{.l = 0x3fdffffffbf98430 },
		{.l = 0x3fc5555612d9bd6e },
		{.l = 0x3fa555334e960925 },
		{.l = 0x3f81145d0d37f475 },
		{.l = 0x3f5667966a599798 },
		{.l = 0x3f2ee6b1cfd8d0fd },
	}
};

// fpminmax: [0, ln2/1]
static const DL
coefficient_ln2div_1[COEFFICIENTS][COEFFICIENTS] = {
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)]); printexpansion(P);i=i+1;
	// 0x3ff5555555555556
	{
		{.l = 0x3ff5555555555556}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)]); printexpansion(P);i=i+1;
	// 0x3fef0bb4660a65a0 + x * 0x3ff6650e9bd9da77
	{
		{.l = 0x3fef0bb4660a65a0},
		{.l = 0x3ff6650e9bd9da77}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)]); printexpansion(P);i=i+1;
	// 0x3ff007108b969adb + x * (0x3fee5c4ff49148ff + x * 0x3fe675460ce511a4)
	{
		{.l = 0x3ff007108b969adb},
		{.l = 0x3fee5c4ff49148ff},
		{.l = 0x3fe675460ce511a4}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)]); printexpansion(P);i=i+1;
	// 0x3fefff632c1cb846 + x * (0x3ff00fdfdbd11725 + x * (0x3fde1d265f206d05 + x * 0x3fcdfd3b669fce68))
	{
		{.l = 0x3fefff632c1cb846},
		{.l = 0x3ff00fdfdbd11725},
		{.l = 0x3fde1d265f206d05},
		{.l = 0x3fcdfd3b669fce68}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)]); printexpansion(P);i=i+1;
	// 0x3ff00002b82713a0 + x * (0x3feffe4e4c936d40 + x * (0x3fe014bcdddc1e3e + x * (0x3fc3fdac7f267a70 + x * 0x3fae04ecd7565c41)))
	{
		{.l = 0x3ff00002b82713a0},
		{.l = 0x3feffe4e4c936d40},
		{.l = 0x3fe014bcdddc1e3e},
		{.l = 0x3fc3fdac7f267a70},
		{.l = 0x3fae04ecd7565c41}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)]); printexpansion(P);i=i+1;
	// 0x3fefffffd7c4cf55 + x * (0x3ff00008ea0bb50d + x * (0x3fdffd83adc2400b + x * (0x3fc57509839df7ec + x * (0x3fa3f04be6d4b744 + x * 0x3f880856eae91239))))
	{
		{.l = 0x3fefffffd7c4cf55},
		{.l = 0x3ff00008ea0bb50d},
		{.l = 0x3fdffd83adc2400b},
		{.l = 0x3fc57509839df7ec},
		{.l = 0x3fa3f04be6d4b744},
		{.l = 0x3f880856eae91239}
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)]); printexpansion(P);i=i+1;
	// 0x3ff00000007f8794 + x * (0x3fefffff67a2006e + x * (0x3fe0000e8843bc7a + x * (0x3fc5534a3d00cef1 + x * (0x3fa577ce7a2c8a42 + x * (0x3f7fd8561b37732b + x * 0x3f6007c2b4d0a481)))))
	{
		{.l = 0x3ff00000007f8794 },
		{.l = 0x3fefffff67a2006e },
		{.l = 0x3fe0000e8843bc7a },
		{.l = 0x3fc5534a3d00cef1 },
		{.l = 0x3fa577ce7a2c8a42 },
		{.l = 0x3f7fd8561b37732b },
		{.l = 0x3f6007c2b4d0a481 },
	},
	// P = fpminimax(exp(x), i, [|D...|], [1b-53,log(2)]); printexpansion(P);i=i+1;
	// 0x3feffffffffa7933 + x * (0x3ff000000223a698 + x * (0x3fdffffeee95ea7e + x * (0x3fc5556efb1563ba + x * (0x3fa552ff9841c69d + x * (0x3f812e413bf82e32 + x * (0x3f5533dd87f4dc2e + x * 0x3f3253f8202f16f3))))))
	{
		{.l = 0x3feffffffffa7933 },
		{.l = 0x3ff000000223a698 },
		{.l = 0x3fdffffeee95ea7e },
		{.l = 0x3fc5556efb1563ba },
		{.l = 0x3fa552ff9841c69d },
		{.l = 0x3f812e413bf82e32 },
		{.l = 0x3f5533dd87f4dc2e },
		{.l = 0x3f3253f8202f16f3 },
	}
};

// link for all coefficients array
static const DL *link[COEFFICIENTS_VERSION];

static const DL
interpolate[BITNUM] = {
	{.l = 0x3ff0000000000000},
	{.l = 0x3ff0163da9fb3335},
	{.l = 0x3ff02c9a3e778061},
	{.l = 0x3ff04315e86e7f85},
	{.l = 0x3ff059b0d3158574},
	{.l = 0x3ff0706b29ddf6de},
	{.l = 0x3ff0874518759bc8},
	{.l = 0x3ff09e3ecac6f383},
	{.l = 0x3ff0b5586cf9890f},
	{.l = 0x3ff0cc922b7247f7},
	{.l = 0x3ff0e3ec32d3d1a2},
	{.l = 0x3ff0fb66affed31b},
	{.l = 0x3ff11301d0125b51},
	{.l = 0x3ff12abdc06c31cc},
	{.l = 0x3ff1429aaea92de0},
	{.l = 0x3ff15a98c8a58e51},
	{.l = 0x3ff172b83c7d517b},
	{.l = 0x3ff18af9388c8dea},
	{.l = 0x3ff1a35beb6fcb75},
	{.l = 0x3ff1bbe084045cd4},
	{.l = 0x3ff1d4873168b9aa},
	{.l = 0x3ff1ed5022fcd91d},
	{.l = 0x3ff2063b88628cd6},
	{.l = 0x3ff21f49917ddc96},
	{.l = 0x3ff2387a6e756238},
	{.l = 0x3ff251ce4fb2a63f},
	{.l = 0x3ff26b4565e27cdd},
	{.l = 0x3ff284dfe1f56381},
	{.l = 0x3ff29e9df51fdee1},
	{.l = 0x3ff2b87fd0dad990},
	{.l = 0x3ff2d285a6e4030b},
	{.l = 0x3ff2ecafa93e2f56},
	{.l = 0x3ff306fe0a31b715},
	{.l = 0x3ff32170fc4cd831},
	{.l = 0x3ff33c08b26416ff},
	{.l = 0x3ff356c55f929ff1},
	{.l = 0x3ff371a7373aa9cb},
	{.l = 0x3ff38cae6d05d866},
	{.l = 0x3ff3a7db34e59ff7},
	{.l = 0x3ff3c32dc313a8e5},
	{.l = 0x3ff3dea64c123422},
	{.l = 0x3ff3fa4504ac801c},
	{.l = 0x3ff4160a21f72e2a},
	{.l = 0x3ff431f5d950a897},
	{.l = 0x3ff44e086061892d},
	{.l = 0x3ff46a41ed1d0057},
	{.l = 0x3ff486a2b5c13cd0},
	{.l = 0x3ff4a32af0d7d3de},
	{.l = 0x3ff4bfdad5362a27},
	{.l = 0x3ff4dcb299fddd0d},
	{.l = 0x3ff4f9b2769d2ca7},
	{.l = 0x3ff516daa2cf6642},
	{.l = 0x3ff5342b569d4f82},
	{.l = 0x3ff551a4ca5d920f},
	{.l = 0x3ff56f4736b527da},
	{.l = 0x3ff58d12d497c7fd},
	{.l = 0x3ff5ab07dd485429},
	{.l = 0x3ff5c9268a5946b7},
	{.l = 0x3ff5e76f15ad2148},
	{.l = 0x3ff605e1b976dc09},
	{.l = 0x3ff6247eb03a5585},
	{.l = 0x3ff6434634ccc320},
	{.l = 0x3ff6623882552225},
	{.l = 0x3ff68155d44ca973},
	{.l = 0x3ff6a09e667f3bcd},
	{.l = 0x3ff6c012750bdabf},
	{.l = 0x3ff6dfb23c651a2f},
	{.l = 0x3ff6ff7df9519484},
	{.l = 0x3ff71f75e8ec5f74},
	{.l = 0x3ff73f9a48a58174},
	{.l = 0x3ff75feb564267c9},
	{.l = 0x3ff780694fde5d3f},
	{.l = 0x3ff7a11473eb0187},
	{.l = 0x3ff7c1ed0130c132},
	{.l = 0x3ff7e2f336cf4e62},
	{.l = 0x3ff80427543e1a12},
	{.l = 0x3ff82589994cce13},
	{.l = 0x3ff8471a4623c7ad},
	{.l = 0x3ff868d99b4492ed},
	{.l = 0x3ff88ac7d98a6699},
	{.l = 0x3ff8ace5422aa0db},
	{.l = 0x3ff8cf3216b5448c},
	{.l = 0x3ff8f1ae99157736},
	{.l = 0x3ff9145b0b91ffc6},
	{.l = 0x3ff93737b0cdc5e5},
	{.l = 0x3ff95a44cbc8520f},
	{.l = 0x3ff97d829fde4e50},
	{.l = 0x3ff9a0f170ca07ba},
	{.l = 0x3ff9c49182a3f090},
	{.l = 0x3ff9e86319e32323},
	{.l = 0x3ffa0c667b5de565},
	{.l = 0x3ffa309bec4a2d33},
	{.l = 0x3ffa5503b23e255d},
	{.l = 0x3ffa799e1330b358},
	{.l = 0x3ffa9e6b5579fdbf},
	{.l = 0x3ffac36bbfd3f37a},
	{.l = 0x3ffae89f995ad3ad},
	{.l = 0x3ffb0e07298db666},
	{.l = 0x3ffb33a2b84f15fb},
	{.l = 0x3ffb59728de5593a},
	{.l = 0x3ffb7f76f2fb5e47},
	{.l = 0x3ffba5b030a1064a},
	{.l = 0x3ffbcc1e904bc1d2},
	{.l = 0x3ffbf2c25bd71e09},
	{.l = 0x3ffc199bdd85529c},
	{.l = 0x3ffc40ab5fffd07a},
	{.l = 0x3ffc67f12e57d14b},
	{.l = 0x3ffc8f6d9406e7b5},
	{.l = 0x3ffcb720dcef9069},
	{.l = 0x3ffcdf0b555dc3fa},
	{.l = 0x3ffd072d4a07897c},
	{.l = 0x3ffd2f87080d89f2},
	{.l = 0x3ffd5818dcfba487},
	{.l = 0x3ffd80e316c98398},
	{.l = 0x3ffda9e603db3285},
	{.l = 0x3ffdd321f301b460},
	{.l = 0x3ffdfc97337b9b5f},
	{.l = 0x3ffe264614f5a129},
	{.l = 0x3ffe502ee78b3ff6},
	{.l = 0x3ffe7a51fbc74c83},
	{.l = 0x3ffea4afa2a490da},
	{.l = 0x3ffecf482d8e67f1},
	{.l = 0x3ffefa1bee615a27},
	{.l = 0x3fff252b376bba97},
	{.l = 0x3fff50765b6e4540},
	{.l = 0x3fff7bfdad9cbe14},
	{.l = 0x3fffa7c1819e90d8},
	{.l = 0x3fffd3c22b8f71f1},
};

int gen(struct constraint input_parameter) {
	double a, b;
	int precision;
	double X;
	FILE *func;
	// init
	int i, num, bit, bitnum, bitnum_1, degree, fnum, format;
	num = 100;

	if ((func = fopen("exp_gen.c", "w")) == (FILE *)0) {
		printf("open file error!\n");
		return 1;
	}
	a = input_parameter.start;
	b = input_parameter.end;
	precision = input_parameter.precision;
	bit = input_parameter.bit;
	bit = 7;
	bitnum = 1 << bit;
	bitnum_1 = bitnum - 1;
	fnum = input_parameter.fnum;
	degree = input_parameter.degree;
	//degree = 7;
	// format --> float number format: double is 64, float is 32
	format = 64;

	/*
	printf("please input [a, b]: ");
	scanf("%lf %lf", &a, &b);
	printf("please input target precision: ");
	scanf("%d", &precision);
	printf("[a,b] = [%lf, %lf]\nprecision = %d\n", a, b, precision);
	*/

	// generate code for exp_gen
	{
		fprintf(func, "#include %cmyhead.h%c\n", '"', '"');
		fprintf(func, "\n");
		fprintf(func, "#define BIT %d\n", bit);
		fprintf(func, "#define BITNUM %d\n", bitnum);
		fprintf(func, "#define BITNUM_1 %d\n", bitnum_1);
		fprintf(func, "#define DEGREE %d\n", degree + 1);
		fprintf(func, "\n");
		fprintf(func, "static const DL\n");
		fprintf(func, "ln2by%dHI = { .l = 0x%lx },\n", bitnum, ln2byHI[bit-1].l);
		fprintf(func, "ln2by%dLO = { .l = 0x%lx },\n", bitnum, ln2byLO[bit-1].l);
		fprintf(func, "invln2by%d = { .l = 0x%lx };\n", bitnum, invln2by[bit-1].l);
		fprintf(func, "\n");
		fprintf(func, "// fpminmax: [-1/%d*ln2, 1/%d*ln2]\n", bitnum, bitnum);
		
		fprintf(func, "static const DL\n");
		fprintf(func, "C[DEGREE] = {\n");
		for (i = 0; i <= degree; i++) {
			fprintf(func, "\t{.l = 0x%lx},\n", link[bit][degree * COEFFICIENTS + i].l);
			//fprintf(func, "\t{.l = 0x%lx},\n", coefficient[degree][i].l);
		}
		fprintf(func, "};\n");

		// interpolate
		fprintf(func, "static const DL\n");
		fprintf(func, "interpolate[BITNUM] = {\n");
		for (i = 0; i < bitnum_1; i++) {
			fprintf(func, "\t{.l = 0x%lx},\n", interpolate[i * (1 << (BIT - bit))].l);
		}
		fprintf(func, "\t{.l = 0x%lx}\n", interpolate[i * (1 << (BIT - bit))].l);
		fprintf(func, "};\n\n");
	}
	{
		fprintf(func, "double exp_gen(double x) {\n");
		fprintf(func, "	double temp;\n");
		fprintf(func, "	double T;\n");
		fprintf(func, "	long int T_int;\n");
		fprintf(func, "	double r, r1, r2, rr;\n");
		fprintf(func, "	double r_poly;\n");
		fprintf(func, "	unsigned long int hi, lo;\n");
		fprintf(func, "	double result;\n");
		fprintf(func, "	double r_hi, r_lo;\n");
		fprintf(func, "\n");
		fprintf(func, "	T = x * invln2by%d.d;\n", bitnum);
		fprintf(func, "	T_int = T;\n");
	
		if (degree > 1) {
			fprintf(func, "	// 0 <= r <= 1/k1\n");
			fprintf(func, "	r1 = x - T_int * ln2by%dHI.d;\n", bitnum);
			fprintf(func, "	r2 = T_int * ln2by%dLO.d;\n", bitnum);
			fprintf(func, "	r = r1 - r2;\n");
			fprintf(func, "\n");

			fprintf(func, "\tr_poly = (r * r) * (C[2].d");
			for (i = 3; i <= degree; i++) {
				fprintf(func, " + r * (C[%d].d", i);
			}
			for (i = 2; i <= degree; i++) {
				fprintf(func, ")");
			}
			//fprintf(func, " - r2 * C[1].d + r1 * C[1].d;\n\n");
			fprintf(func, " - r2 * C[1].d + r1 * C[1].d + C[0].d;\n\n");
		}
		else if (degree == 1) {
			fprintf(func, "	// 0 <= r <= 1/k1\n");
			fprintf(func, "	r1 = x - T_int * ln2by%dHI.d;\n", bitnum);
			fprintf(func, "	r2 = T_int * ln2by%dLO.d;\n", bitnum);
			fprintf(func, "	r = r1 - r2;\n");
			fprintf(func, "\n");

			//fprintf(func, "\tr_poly = - r2 * C[1].d + r1 * C[1].d;\n\n");
			fprintf(func, "\tr_poly = - r2 * C[1].d + r1 * C[1].d + C[0].d;\n\n");
		} 
		else {
			//fprintf(func, "\tr_poly = 0\n\n");
			fprintf(func, "\tr_poly = C[0].d;\n\n");
		}
		
		fprintf(func, "	lo = T_int & BITNUM_1;\n");
		fprintf(func, "	hi = T_int >> BIT;\n");
		fprintf(func, "	hi = (hi + 0x3ff) << 52;\n");
		fprintf(func, "	r_hi = *((double *)&hi);\n");
		fprintf(func, "	//r_lo = pow(2, (((double)lo) / ((double)k)));\n");
		fprintf(func, "	r_lo = interpolate[lo].d;\n");
		//fprintf(func, "	result = r_hi * (r_lo + r_lo * r_poly);\n");
		fprintf(func, "	result = r_hi * (r_lo * r_poly);\n");
		fprintf(func, "\n");
		fprintf(func, "	return result;\n");
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

	link[7] = &(coefficient_ln2div_128[0][0]);
	link[6] = &(coefficient_ln2div_64[0][0]);
	link[5] = &(coefficient_ln2div_32[0][0]);
	link[4] = &(coefficient_ln2div_16[0][0]);
	link[3] = &(coefficient_ln2div_8[0][0]);
	link[2] = &(coefficient_ln2div_4[0][0]);
	link[1] = &(coefficient_ln2div_2[0][0]);
	link[0] = &(coefficient_ln2div_1[0][0]);
	
	input_parameter.start = a;
	input_parameter.end = b;
	input_parameter.precision = precision;
	input_parameter.bit = bit;
	input_parameter.fnum = fnum;
	input_parameter.degree = degree;
	gen(input_parameter);

	return 0;
}