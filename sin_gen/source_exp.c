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
	{
		{.l = 0x3ff0000000000000}
	},
	// P = fpminimax(exp(x),[| 1 | ],[| D... | ],[1b - 53,log(2) / 128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * 0x3ff00932cd89057f
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff00932cd89057f}
	},
	// P = fpminimax(exp(x), [|1,2|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3feffffb9477c883 + x * 0x3fe00a4e59041d9d)
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3feffffb9477c883},
		{.l = 0x3fe00a4e59041d9d}
	},
	// P = fpminimax(exp(x), [|1,2,3|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000538109 + x * (0x3fdffffa494bd1eb + x * 0x3fc5638d68876c7f))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff0000000538109},
		{.l = 0x3fdffffa494bd1eb},
		{.l = 0x3fc5638d68876c7f}
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

// fpminmax: [0, ln2/32]
static const DL
coefficient_ln2div_32[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3ff0000000000000}
	},
	// P = fpminimax(exp(x),[| 1 | ],[| D... | ],[1b - 53,log(2) / 128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * 0x3ff00932cd89057f
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff00932cd89057f}
	},
	// P = fpminimax(exp(x), [|1,2|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3feffffb9477c883 + x * 0x3fe00a4e59041d9d)
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3feffffb9477c883},
		{.l = 0x3fe00a4e59041d9d}
	},
	// P = fpminimax(exp(x), [|1,2,3|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000538109 + x * (0x3fdffffa494bd1eb + x * 0x3fc5638d68876c7f))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff0000000538109},
		{.l = 0x3fdffffa494bd1eb},
		{.l = 0x3fc5638d68876c7f}
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

// fpminmax: [0, ln2/16]
static const DL
coefficient_ln2div_16[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3ff0000000000000}
	},
	// P = fpminimax(exp(x),[| 1 | ],[| D... | ],[1b - 53,log(2) / 128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * 0x3ff00932cd89057f
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff00932cd89057f}
	},
	// P = fpminimax(exp(x), [|1,2|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3feffffb9477c883 + x * 0x3fe00a4e59041d9d)
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3feffffb9477c883},
		{.l = 0x3fe00a4e59041d9d}
	},
	// P = fpminimax(exp(x), [|1,2,3|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000538109 + x * (0x3fdffffa494bd1eb + x * 0x3fc5638d68876c7f))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff0000000538109},
		{.l = 0x3fdffffa494bd1eb},
		{.l = 0x3fc5638d68876c7f}
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

// fpminmax: [0, ln2/8]
static const DL
coefficient_ln2div_8[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3ff0000000000000}
	},
	// P = fpminimax(exp(x),[| 1 | ],[| D... | ],[1b - 53,log(2) / 128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * 0x3ff00932cd89057f
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff00932cd89057f}
	},
	// P = fpminimax(exp(x), [|1,2|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3feffffb9477c883 + x * 0x3fe00a4e59041d9d)
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3feffffb9477c883},
		{.l = 0x3fe00a4e59041d9d}
	},
	// P = fpminimax(exp(x), [|1,2,3|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000538109 + x * (0x3fdffffa494bd1eb + x * 0x3fc5638d68876c7f))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff0000000538109},
		{.l = 0x3fdffffa494bd1eb},
		{.l = 0x3fc5638d68876c7f}
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

// fpminmax: [0, ln2/4]
static const DL
coefficient_ln2div_4[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3ff0000000000000}
	},
	// P = fpminimax(exp(x),[| 1 | ],[| D... | ],[1b - 53,log(2) / 128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * 0x3ff00932cd89057f
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff00932cd89057f}
	},
	// P = fpminimax(exp(x), [|1,2|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3feffffb9477c883 + x * 0x3fe00a4e59041d9d)
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3feffffb9477c883},
		{.l = 0x3fe00a4e59041d9d}
	},
	// P = fpminimax(exp(x), [|1,2,3|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000538109 + x * (0x3fdffffa494bd1eb + x * 0x3fc5638d68876c7f))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff0000000538109},
		{.l = 0x3fdffffa494bd1eb},
		{.l = 0x3fc5638d68876c7f}
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

// fpminmax: [0, ln2/2]
static const DL
coefficient_ln2div_2[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3ff0000000000000}
	},
	// P = fpminimax(exp(x),[| 1 | ],[| D... | ],[1b - 53,log(2) / 128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * 0x3ff00932cd89057f
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff00932cd89057f}
	},
	// P = fpminimax(exp(x), [|1,2|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3feffffb9477c883 + x * 0x3fe00a4e59041d9d)
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3feffffb9477c883},
		{.l = 0x3fe00a4e59041d9d}
	},
	// P = fpminimax(exp(x), [|1,2,3|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000538109 + x * (0x3fdffffa494bd1eb + x * 0x3fc5638d68876c7f))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff0000000538109},
		{.l = 0x3fdffffa494bd1eb},
		{.l = 0x3fc5638d68876c7f}
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

// fpminmax: [0, ln2/1]
static const DL
coefficient_ln2div_1[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3ff0000000000000}
	},
	// P = fpminimax(exp(x),[| 1 | ],[| D... | ],[1b - 53,log(2) / 128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * 0x3ff00932cd89057f
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff00932cd89057f}
	},
	// P = fpminimax(exp(x), [|1,2|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3feffffb9477c883 + x * 0x3fe00a4e59041d9d)
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3feffffb9477c883},
		{.l = 0x3fe00a4e59041d9d}
	},
	// P = fpminimax(exp(x), [|1,2,3|], [|D...|], [1b-53,log(2)/128],1); printexpansion(P);
	// 0x3ff0000000000000 + x * (0x3ff0000000538109 + x * (0x3fdffffa494bd1eb + x * 0x3fc5638d68876c7f))
	{
		{.l = 0x3ff0000000000000},
		{.l = 0x3ff0000000538109},
		{.l = 0x3fdffffa494bd1eb},
		{.l = 0x3fc5638d68876c7f}
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