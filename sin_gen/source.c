#include <stdio.h>
#include <stdlib.h>
#include "myhead.h"

#define NUM 100
#define DEGREE 7
#define COEFFICIENTS 7
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
// pi/4/128
static const DL
coefficient_pi_4_128[COEFFICIENTS][2][COEFFICIENTS] = {
	// 1 coefficients
	// 0x3e4ffffd74191a12
	// 0x3fefffec42bb182f
	{
		{
			{.l = 0x3e4ffffd74191a12}
		},
		{
			{.l = 0x3fefffec42bb182f}
		}
	},
	// 2 coefficients
	// 0x3d2a51aaa4577386 + x * 0x3feffff96b389608
	// 0x3ff0000277a907c0 + x * 0xbf6921fc09879ff4
	{
		{
			{.l = 0x3d2a51aaa4577386},
			{.l = 0x3feffff96b389608}
		},
		{
			{.l = 0x3ff0000277a907c0},
			{.l = 0xbf6921fc09879ff4}
		}
	},
	// 3 coefficients
	// 0xbd0a6081327b77eb + x * (0x3ff00000d3f2460f + x * 0xbf50c60cab8e9315)
	// 0x3ff00000000041f1 + x * (0xbe43664f6b411c35 + x * 0xbfdffff4148eb6b7)
	{
		{
			{.l = 0xbd0a6081327b77eb},
			{.l = 0x3ff00000d3f2460f},
			{.l = 0xbf50c60cab8e9315}
		},
		{
			{.l = 0x3ff00000000041f1},
			{.l = 0xbe43664f6b411c35},
			{.l = 0xbfdffff4148eb6b7}
		}
	},
	// 4 coefficients
	// // 0xbbca7d59979393ef + x * (0x3ff0000000000d53 + x * (0xbe1f1f04f59599b9 + x * 0xbfc555508f75ffbc))
	// 0x3fefffffffffefc4 + x * (0x3e24abb2cf0603fd + x * (0xbfe000041cc0f168 + x * 0x3f40c14fcbc76a91))
	{
		{
			{.l = 0xbbca7d59979393ef},
			{.l = 0x3ff0000000000d53},
			{.l = 0xbe1f1f04f59599b9},
			{.l = 0xbfc555508f75ffbc}
		},
		{
			{.l = 0x3fefffffffffefc4},
			{.l = 0x3e24abb2cf0603fd},
			{.l = 0xbfe000041cc0f168},
			{.l = 0x3f40c14fcbc76a91}
		}
	},
	// 5 coefficients
	// 0x3b9a24fd6f71e4f0 + x * (0x3feffffffffffcb4 + x * (0x3e00999ee5af3132 + x * (0xbfc55558a0d459da + x * 0x3f1ad2a9831248b0)))
	// 0x3ff0000000000000 + x * (0x3ce5434093bdf722 + x * (0xbfe00000000077d5 + x * (0x3e1923479c224098 + x * 0x3fa5554e47f80e37)))
	{
		{
			{.l = 0x3b9a24fd6f71e4f0},
			{.l = 0x3feffffffffffcb4},
			{.l = 0x3e00999ee5af3132},
			{.l = 0xbfc55558a0d459da},
			{.l = 0x3f1ad2a9831248b0}
		},
		{
			{.l = 0x3ff0000000000000},
			{.l = 0x3ce5434093bdf722},
			{.l = 0xbfe00000000077d5},
			{.l = 0x3e1923479c224098},
			{.l = 0x3fa5554e47f80e37}
		}
	},
	// 6 coefficients
	// 0xb984d64a29b505c5 + x * (0x3ff0000000000000 + x * (0x3cb845aef92c1d42 + x * (0xbfc55555555599c3 + x * (0x3decb7924858362b + x * 0x3f81110d09aab89d))))
	// 0x3ff0000000000000 + x * (0xbcb61a0a5c31efb7 + x * (0xbfdfffffffffcec1 + x * (0xbe01ea30fe505df3 + x * (0x3fa5555b0928dfe9 + x * 0xbefa66ee34c3b903))))
	{
		{
			{.l = 0xb984d64a29b505c5},
			{.l = 0x3ff0000000000000},
			{.l = 0x3cb845aef92c1d42},
			{.l = 0xbfc55555555599c3},
			{.l = 0x3decb7924858362b},
			{.l = 0x3f81110d09aab89d}
		},
		{
			{.l = 0x3ff0000000000000},
			{.l = 0xbcb61a0a5c31efb7},
			{.l = 0xbfdfffffffffcec1},
			{.l = 0xbe01ea30fe505df3},
			{.l = 0x3fa5555b0928dfe9},
			{.l = 0xbefa66ee34c3b903}

		}
	},
	// 7 coefficients
	// 0x39558228bf002ff9 + x * (0x3ff0000000000000 + x * (0xbc893a0de469b1fb + x * (0xbfc5555555554746 + x * (0xbdd47714d2fce3ba + x * (0x3f811114530de4ce + x * 0xbece2bb5fc6e149b)))))
	// 0x3ff0000000000000 + x * (0x3b63918d3d8b63e0 + x * (0xbfe0000000000000 + x * (0xbcb1f0881cdb7fed + x * (0x3fa555555555c054 + x * (0xbdcd70054d47aeb5 + x * 0xbf56c165ff0982a8)))))
	{
		{
			{.l = 0x39558228bf002ff9},
			{.l = 0x3ff0000000000000},
			{.l = 0xbc893a0de469b1fb},
			{.l = 0xbfc5555555554746},
			{.l = 0xbdd47714d2fce3ba},
			{.l = 0x3f811114530de4ce},
			{.l = 0xbece2bb5fc6e149b}
		},
		{
			{.l = 0x3ff0000000000000},
			{.l = 0x3b63918d3d8b63e0},
			{.l = 0xbfe0000000000000},
			{.l = 0xbcb1f0881cdb7fed},
			{.l = 0x3fa555555555c054},
			{.l = 0xbdcd70054d47aeb5},
			{.l = 0xbf56c165ff0982a8}
		}
	}
};

// sin & cos
// coefficient[0] is for sin; coefficient[1] is for cos;
// pi/4/64
static const DL
coefficient_pi_4_64[COEFFICIENTS][2][COEFFICIENTS] = {
	// 1 coefficients
	// 0x3e4ffffeba0aedef
	// 0x3fefffb10a8af759
	{
		{
			{.l = 0x3e4ffffeba0aedef}
		},
		{
			{.l = 0x3fefffb10a8af759}
		}
	},
	// 2 coefficients
	// 0x3d4a51b016418ff7 + x * 0x3fefffe5ad663073
	// 0x3ff00009dea4cf1d + x * 0xbf7921f72953d200
	{
		{
			{.l = 0x3d4a51b016418ff7},
			{.l = 0x3fefffe5ad663073}
		},
		{
			{.l = 0x3ff00009dea4cf1d},
			{.l = 0xbf7921f72953d200}
		}
	},
	// 3 coefficients
	// 0xbd2a5c2194f2e9a0 + x * (0x3ff000034e258aca + x * 0xbf60c4a5e2afe6cb)
	// 0x3ff0000000041f0d + x * (0xbe7366517d2c1105 + x * 0xbfdfffd05239dd23)
	{
		{
			{.l = 0xbd2a5c2194f2e9a0},
			{.l = 0x3ff000034e258aca},
			{.l = 0xbf60c4a5e2afe6cb}
		},
		{
			{.l = 0x3ff0000000041f0d},
			{.l = 0xbe7366517d2c1105},
			{.l = 0xbfdfffd05239dd23}
		}
	},
	// 4 coefficients
	// 0xbc0a74cb6bfe2a66 + x * (0x3ff000000000d48c + x * (0xbe4f18dbb1db2dca + x * 0xbfc555423f5fca0f))
	// 0x3feffffffffefc3c + x * (0x3e54abc68b0b1952 + x * (0xbfe00010730708f5 + x * 0x3f50c14e50ee4777))
	{
		{
			{.l = 0xbc0a74cb6bfe2a66},
			{.l = 0x3ff000000000d48c},
			{.l = 0xbe4f18dbb1db2dca},
			{.l = 0xbfc555423f5fca0f}
		},
		{
			{.l = 0x3feffffffffefc3c},
			{.l = 0x3e54abc68b0b1952},
			{.l = 0xbfe00010730708f5},
			{.l = 0x3f50c14e50ee4777}
		}
	},
	// 5 coefficients
	// 0x3bda18d9de49ab89 + x * (0x3fefffffffffcb7b + x * (0x3e3094ecf9a8384e + x * (0xbfc5556281d0b611 + x * 0x3f2ad18e4f0827b1)))
	// 0x3ff0000000000000 + x * (0x3d35436df932e463 + x * (0xbfe0000000077d5a + x * (0x3e49235b48445bed + x * 0x3fa555391fd536fd)))
	{
		{
			{.l = 0x3bda18d9de49ab89},
			{.l = 0x3fefffffffffcb7b},
			{.l = 0x3e3094ecf9a8384e},
			{.l = 0xbfc5556281d0b611},
			{.l = 0x3f2ad18e4f0827b1}
		},
		{
			{.l = 0x3ff0000000000000},
			{.l = 0x3d35436df932e463},
			{.l = 0xbfe0000000077d5a},
			{.l = 0x3e49235b48445bed},
			{.l = 0x3fa555391fd536fd}
		}
	},
	// 6 coefficients
	// 0xb9d4fc64a5efeb6a + x * (0x3ff0000000000000 + x * (0x3d0847eb23176753 + x * (0xbfc5555555599c6c + x * (0x3e1cb882e949c36c + x * 0x3f811100f32d3a83))))
	// 0x3ff0000000000000 + x * (0xbd0619f83dcccbf2 + x * (0xbfdffffffffcec12 + x * (0xbe31ea26f2eb19dd + x * (0x3fa5556c2498abda + x * 0xbf0a66e583c1d006))))
	{
		{
			{.l = 0xb9d4fc64a5efeb6a},
			{.l = 0x3ff0000000000000},
			{.l = 0x3d0847eb23176753},
			{.l = 0xbfc5555555599c6c},
			{.l = 0x3e1cb882e949c36c},
			{.l = 0x3f811100f32d3a83}
		},
		{
			{.l = 0x3ff0000000000000},
			{.l = 0xbd0619f83dcccbf2},
			{.l = 0xbfdffffffffcec12},
			{.l = 0xbe31ea26f2eb19dd},
			{.l = 0x3fa5556c2498abda},
			{.l = 0xbf0a66e583c1d006}
		}
	},
	// 7 coefficients
	// 0x39a5b32c373a103b + x * (0x3ff0000000000000 + x * (0xbcd93d23df47f8f2 + x * (0xbfc5555555547450 + x * (0xbe0477ee9a14db48 + x * (0x3f81111e194f108f + x * 0xbede2bfcaa5c4d71)))))
	// 0x3ff0000000000000 + x * (0xbbe6aedc6ad30a82 + x * (0xbfdfffffffffffff + x * (0xbd1e56ec42a5c878 + x * (0x3fa5555555626754 + x * (0xbe05c0b21cd411e2 + x * 0xbf56c14de38f7e23)))))
	{
		{
			{.l = 0x39a5b32c373a103b},
			{.l = 0x3ff0000000000000},
			{.l = 0xbcd93d23df47f8f2},
			{.l = 0xbfc5555555547450},
			{.l = 0xbe0477ee9a14db48},
			{.l = 0x3f81111e194f108f},
			{.l = 0xbede2bfcaa5c4d71}
		},
		{
			{.l = 0x3ff0000000000000},
			{.l = 0xbbe6aedc6ad30a82},
			{.l = 0xbfdfffffffffffff},
			{.l = 0xbd1e56ec42a5c878},
			{.l = 0x3fa5555555626754},
			{.l = 0xbe05c0b21cd411e2},
			{.l = 0xbf56c14de38f7e23}
		}
	}
};

// sin & cos
// coefficient[0] is for sin; coefficient[1] is for cos;
// pi/4/32
static const DL
coefficient_pi_4_32[COEFFICIENTS][2][COEFFICIENTS] = {
	// 1 coefficients
	// 0x3e4fffff5d024f68
	// 0x3feffec424152730
	{
		{
			{.l = 0x3e4fffff5d024f68}
		},
		{
			{.l = 0x3feffec424152730}
		}
	},
	// 2 coefficients
	// 0x3d6a51c988d7d7ca + x * 0x3fefff96b68cea95
	// 0x3ff000277abf3c54 + x * 0xbf8921e7288b3eed
	{
		{
			{.l = 0x3d6a51c988d7d7ca},
			{.l = 0x3fefff96b68cea95}
		},
		{
			{.l = 0x3ff000277abf3c54},
			{.l = 0xbf8921e7288b3eed}
		}
	},
	// 3 coefficients
	// 0xbd4a58fa078e1bfe + x * (0x3ff0000d33ec85c8 + x * 0xbf70c39c66553991)
	// 0x3ff000000041f1ec + x * (0xbea3667f01b48d34 + x * 0xbfdfff4148411c57)
	{
		{
			{.l = 0xbd4a58fa078e1bfe},
			{.l = 0x3ff0000d33ec85c8},
			{.l = 0xbf70c39c66553991}
		},
		{
			{.l = 0x3ff000000041f1ec},
			{.l = 0xbea3667f01b48d34},
			{.l = 0xbfdfff4148411c57}
		}
	},
	// 4 coefficients
	// 0xbc4a6ed4a82f0a57 + x * (0x3ff00000000d418e + x * (0xbe7f148843c5557a + x * 0xbfc5550901db79c2))
	// 0x3fefffffffefc3b8 + x * (0x3e84abc68a85b7ed + x * (0xbfe00041cbebd6e8 + x * 0x3f60c13f41810855))
	{
		{
			{.l = 0xbc4a6ed4a82f0a57},
			{.l = 0x3ff00000000d418e},
			{.l = 0xbe7f148843c5557a},
			{.l = 0xbfc5550901db79c2}
		},
		{
			{.l = 0x3fefffffffefc3b8},
			{.l = 0x3e84abc68a85b7ed},
			{.l = 0xbfe00041cbebd6e8},
			{.l = 0x3f60c13f41810855}
		}
	},
	// 5 coefficients
	// 0x3c1a0fa756bd3f67 + x * (0x3feffffffffcba5e + x * (0x3e60917579930fe4 + x * (0xbfc5558a02b11613 + x * 0x3f3ad0abbc522eb7)))
	// 0x3feffffffffffff0 + x * (0x3d8dc087f14a5031 + x * (0xbfe00000008b49eb + x * (0x3e7b33bfc68376b4 + x * 0x3fa554dfd36ee1b1)))
	{
		{
			{.l = 0x3c1a0fa756bd3f67},
			{.l = 0x3feffffffffcba5e},
			{.l = 0x3e60917579930fe4},
			{.l = 0xbfc5558a02b11613},
			{.l = 0x3f3ad0abbc522eb7}
		},
		{
			{.l = 0x3feffffffffffff0},
			{.l = 0x3d8dc087f14a5031},
			{.l = 0xbfe00000008b49eb},
			{.l = 0x3e7b33bfc68376b4},
			{.l = 0x3fa554dfd36ee1b1}
		}
	},
	// 6 coefficients
	// 0x3b0fd4acbb1215cd + x * (0x3feffffffffffffe + x * (0x3d6064617071654d + x * (0xbfc5555555a38704 + x * (0x3e4eca6b4cb8c93e + x * 0x3f8110ce41aee4f3))))
	// 0x3ff0000000000001 + x * (0xbd616abbb1cc8ecc + x * (0xbfdfffffffc3682c + x * (0xbe63fa7ba182ef14 + x * (0x3fa555b5d381c878 + x * 0xbf1b02895e60699f))))
	{
		{
			{.l = 0x3b0fd4acbb1215cd},
			{.l = 0x3feffffffffffffe},
			{.l = 0x3d6064617071654d},
			{.l = 0xbfc5555555a38704},
			{.l = 0x3e4eca6b4cb8c93e},
			{.l = 0x3f8110ce41aee4f3}
		},
		{
			{.l = 0x3ff0000000000001},
			{.l = 0xbd616abbb1cc8ecc},
			{.l = 0xbfdfffffffc3682c},
			{.l = 0xbe63fa7ba182ef14},
			{.l = 0x3fa555b5d381c878},
			{.l = 0xbf1b02895e60699f}
		}
	},
	// 7 coefficients
	// 0x39f5d597b0149658 + x * (0x3ff0000000000000 + x * (0xbd293e830c12c95d + x * (0xbfc555555547449a + x * (0xbe347837ff39b44f + x * (0x3f81114532518e39 + x * 0xbeee2c01eb2f7feb)))))
	// 0x3ff0000000000000 + x * (0xbc54257f5753f4a9 + x * (0xbfdfffffffffffc5 + x * (0xbd6cab8626df20da + x * (0x3fa55555561e7a88 + x * (0xbe3533fe6b89710e + x * 0xbf56c0f51c7eacc2)))))
	{
		{
			{.l = 0x39f5d597b0149658},
			{.l = 0x3ff0000000000000},
			{.l = 0xbd293e830c12c95d},
			{.l = 0xbfc555555547449a},
			{.l = 0xbe347837ff39b44f},
			{.l = 0x3f81114532518e39},
			{.l = 0xbeee2c01eb2f7feb}
		},
		{
			{.l = 0x3ff0000000000000},
			{.l = 0xbc54257f5753f4a9},
			{.l = 0xbfdfffffffffffc5},
			{.l = 0xbd6cab8626df20da},
			{.l = 0x3fa55555561e7a88},
			{.l = 0xbe3533fe6b89710e},
			{.l = 0xbf56c0f51c7eacc2}
		}
	}
};

// sin & cos
// coefficient[0] is for sin; coefficient[1] is for cos;
// pi/4/16
static const DL
coefficient_pi_4_16[COEFFICIENTS][2][COEFFICIENTS] = {
	// 1 coefficients
	// 0x3e4fffffae7addd5
	// 0x3feffb102ee13dbc
	{
		{
			{.l = 0x3e4fffffae7addd5}
		},
		{
			{.l = 0x3feffb102ee13dbc}
		}
	},
	// 2 coefficients
	// 0x3d8a523129a77c77 + x * 0x3feffe5ad7215caf
	// 0x3ff0009dedff43be + x * 0xbf9921a8e5b84b2d
	{
		{
			{.l = 0x3d8a523129a77c77},
			{.l = 0x3feffe5ad7215caf}
		},
		{
			{.l = 0x3ff0009dedff43be},
			{.l = 0xbf9921a8e5b84b2d}
		}
	},
	// 3 coefficients
	// 0xbd6a567ed53fd466 + x * (0x3ff00034c2036306 + x * 0xbf80c2b10ee9a5de)
	// 0x3ff00000041f6b79 + x * (0xbed3673c23f0b55a + x * 0xbfdffd05162da912)
	{
		{
			{.l = 0xbd6a567ed53fd466},
			{.l = 0x3ff00034c2036306},
			{.l = 0xbf80c2b10ee9a5de}
		},
		{
			{.l = 0x3ff00000041f6b79},
			{.l = 0xbed3673c23f0b55a},
			{.l = 0xbfdffd05162da912}
		}
	},
	// 4 coefficients
	// 0xbc8a6aea61243945 + x * (0x3ff0000000d3c9f7 + x * (0xbeaf1192aac26e18 + x * 0xbfc55424145a70a2))
	// 0x3feffffffefc35c3 + x * (0x3eb4abd997932194 + x * (0xbfe001072d0ba831 + x * 0x3f70c105394e49f9))
	{
		{
			{.l = 0xbc8a6aea61243945},
			{.l = 0x3ff0000000d3c9f7},
			{.l = 0xbeaf1192aac26e18},
			{.l = 0xbfc55424145a70a2}
		},
		{
			{.l = 0x3feffffffefc35c3},
			{.l = 0x3eb4abd997932194},
			{.l = 0xbfe001072d0ba831},
			{.l = 0x3f70c105394e49f9}
		}
	},
	// 5 coefficients
	// 0x3c5a08c70e2300f0 + x * (0x3fefffffffcbc4e6 + x * (0x3e908ebe4d100a7e + x * (0xbfc55627fa67fdd4 + x * 0x3f4acfb5cc36f278)))
	// 0x3feffffffffffbf7 + x * (0x3dddd4845478db8f + x * (0xbfe0000008b78aef + x * (0x3eab38b419a37c78 + x * 0x3fa5537f22216dce)))
	{
		{
			{.l = 0x3c5a08c70e2300f0},
			{.l = 0x3fefffffffcbc4e6},
			{.l = 0x3e908ebe4d100a7e},
			{.l = 0xbfc55627fa67fdd4},
			{.l = 0x3f4acfb5cc36f278}
		},
		{
			{.l = 0x3feffffffffffbf7},
			{.l = 0x3dddd4845478db8f},
			{.l = 0xbfe0000008b78aef},
			{.l = 0x3eab38b419a37c78},
			{.l = 0x3fa5537f22216dce}
		}
	},
	// 6 coefficients
	// 0x3b728e29523aaf26 + x * (0x3fefffffffffff6b + x * (0x3db116fc9b61762c + x * (0xbfc555555a52084a + x * (0x3e7f21422dc9145f + x * 0x3f8110044b2b6bd5))))
	// 0x3ff000000000002b + x * (0xbdaea813481f61a6 + x * (0xbfdffffffc721afb + x * (0xbe934cd8de340b65 + x * (0x3fa556d0616203b7 + x * 0xbf2acf051176b508))))
	{
		{
			{.l = 0x3b728e29523aaf26},
			{.l = 0x3fefffffffffff6b},
			{.l = 0x3db116fc9b61762c},
			{.l = 0xbfc555555a52084a},
			{.l = 0x3e7f21422dc9145f},
			{.l = 0x3f8110044b2b6bd5}
		},
		{
			{.l = 0x3ff000000000002b},
			{.l = 0xbdaea813481f61a6},
			{.l = 0xbfdffffffc721afb},
			{.l = 0xbe934cd8de340b65},
			{.l = 0x3fa556d0616203b7},
			{.l = 0xbf2acf051176b508}
		}
	},
	// 7 coefficients
	// 0xbb37e3679de565ed + x * (0x3ff0000000000006 + x * (0xbd816700b6864321 + x * (0xbfc555555452325e + x * (0xbe6604f31d4bce51 + x * (0x3f8111e978ee7681 + x * 0xbefea0a7102bb852)))))
	// 0x3ff0000000000000 + x * (0xbcc3f2a79a39d131 + x * (0xbfdffffffffff159 + x * (0xbdbc8a23e46b1056 + x * (0x3fa5555561ddaa7a + x * (0xbe6528ed7f67d61b + x * 0xbf56bf90c1d7fc9d)))))
	{
		{
			{.l = 0xbb37e3679de565ed},
			{.l = 0x3ff0000000000006 },
			{.l = 0xbd816700b6864321 },
			{.l = 0xbfc555555452325e },
			{.l = 0xbe6604f31d4bce51 },
			{.l = 0x3f8111e978ee7681 },
			{.l = 0xbefea0a7102bb852 }
		},
		{
			{.l = 0x3ff0000000000000},
			{.l = 0xbcc3f2a79a39d131},
			{.l = 0xbfdffffffffff159},
			{.l = 0xbdbc8a23e46b1056},
			{.l = 0x3fa5555561ddaa7a},
			{.l = 0xbe6528ed7f67d61b},
			{.l = 0xbf56bf90c1d7fc9d}
		}
	}
};

// sin & cos
// coefficient[0] is for sin; coefficient[1] is for cos;
// pi/4/8
static const DL
coefficient_pi_4_8[COEFFICIENTS][2][COEFFICIENTS] = {
	// 1 coefficients
	// 0x3e4fffffd730d932
	// 0x3fefec3aa24f71e9
	{
		{
			{.l = 0x3e4fffffd730d932}
		},
		{
			{.l = 0x3fefec3aa24f71e9}
		}
	},
	// 2 coefficients
	// 0x3daa53d095f5494d + x * 0x3feff96afd3f2101
	// 0x3ff00277e8bd04cf + x * 0xbfa920b0bf612cc7
	{
		{
			{.l = 0x3daa53d095f5494d},
			{.l = 0x3feff96afd3f2101}
		},
		{
			{.l = 0x3ff00277e8bd04cf},
			{.l = 0xbfa920b0bf612cc7}
		}
	},
	// 3 coefficients
	// 0xbd8a53bc9d5394bc + x * (0x3ff000d2d953d6cf + x * 0xbf90c14bf6605d55)
	// 0x3ff00000420a0b7f + x * (0xbf036a342ed271a6 + x * 0xbfdff413aa5d7bb1)
	{
		{
			{.l = 0xbd8a53bc9d5394bc},
			{.l = 0x3ff000d2d953d6cf},
			{.l = 0xbf90c14bf6605d55}
		},
		{
			{.l = 0x3ff00000420a0b7f},
			{.l = 0xbf036a342ed271a6},
			{.l = 0xbfdff413aa5d7bb1}
		}
	},
	// 4 coefficients
	// 0xbcca695a192d6976 + x * (0x3ff000000d39bd9f + x * (0xbedf0fdf4e6ee2fb + x * 0xbfc550907f7ab04b))
	// 0x3fefffffefc1dcac + x * (0x3ee4ac2b4c41e45d + x * (0xbfe0041c8a66061d + x * 0x3f80c01dc40aec6a))
	{
		{
			{.l = 0xbcca695a192d6976},
			{.l = 0x3ff000000d39bd9f},
			{.l = 0xbedf0fdf4e6ee2fb},
			{.l = 0xbfc550907f7ab04b}
		},
		{
			{.l = 0x3fefffffefc1dcac},
			{.l = 0x3ee4ac2b4c41e45d},
			{.l = 0xbfe0041c8a66061d},
			{.l = 0x3f80c01dc40aec6a}
		}
	},
	// 5 coefficients
	// 0x3c9a027f22c2d623 + x * (0x3feffffffcbdda86 + x * (0x3ec08bc5c5c12fa7 + x * (0xbfc5589f84861c34 + x * 0x3f5acdb187526a7f)))
	// 0x3feffffffffefd79 + x * (0x3e2dda6ebe52df83 + x * (0xbfe000008b8a1b9e + x * (0x3edb3a861aa1b427 + x * 0x3fa54dfc5aa396df)))
	{
		{
			{.l = 0x3c9a027f22c2d623},
			{.l = 0x3feffffffcbdda86},
			{.l = 0x3ec08bc5c5c12fa7},
			{.l = 0xbfc5589f84861c34},
			{.l = 0x3f5acdb187526a7f}
		},
		{
			{.l = 0x3feffffffffefd79},
			{.l = 0x3e2dda6ebe52df83},
			{.l = 0xbfe000008b8a1b9e},
			{.l = 0x3edb3a861aa1b427},
			{.l = 0x3fa54dfc5aa396df}
		}
	},
	// 6 coefficients
	// 0x3bd27bef69d3c65f + x * (0x3fefffffffffdaef + x * (0x3e011107ace24f82 + x * (0xbfc55555a5125ae3 + x * (0x3eaf1ddeb9818ebe + x * 0x3f810cde44c6b1b7))))
	// 0x3ff0000000000ab0 + x * (0xbdfe9bc50a5f055c + x * (0xbfdfffffc72e560e + x * (0xbec349bc15b20cd2 + x * (0x3fa55b40c6341343 + x * 0xbf3accbf7275d995))))
	{
		{
			{.l = 0x3bd27bef69d3c65f},
			{.l = 0x3fefffffffffdaef},
			{.l = 0x3e011107ace24f82},
			{.l = 0xbfc55555a5125ae3},
			{.l = 0x3eaf1ddeb9818ebe},
			{.l = 0x3f810cde44c6b1b7}
		},
		{
			{.l = 0x3ff0000000000ab0},
			{.l = 0xbdfe9bc50a5f055c},
			{.l = 0xbfdfffffc72e560e},
			{.l = 0xbec349bc15b20cd2},
			{.l = 0x3fa55b40c6341343},
			{.l = 0xbf3accbf7275d995}
		}
	},
	// 7 coefficients
	// 0xbb987b482165ea8b + x * (0x3ff0000000000189 + x * (0xbdd182868496b421 + x * (0xbfc555554517bf44 + x * (0xbe960c3af2ce7fcc + x * (0x3f811473233f751e + x * 0xbf0ea176be4a8715)))))
	// 0x3ff0000000000000 + x * (0xbd33f80196911c8b + x * (0xbfdffffffffc5589 + x * (0xbe0c8dfc128e7167 + x * (0x3fa555561debb5a2 + x * (0xbe9529e1d03eeda3 + x * 0xbf56b9fea6cc4993)))))
	{
		{
			{.l = 0xbb987b482165ea8b},
			{.l = 0x3ff0000000000189},
			{.l = 0xbdd182868496b421},
			{.l = 0xbfc555554517bf44},
			{.l = 0xbe960c3af2ce7fcc},
			{.l = 0x3f811473233f751e},
			{.l = 0xbf0ea176be4a8715}
		},
		{
			{.l = 0x3ff0000000000000},
			{.l = 0xbd33f80196911c8b},
			{.l = 0xbfdffffffffc5589},
			{.l = 0xbe0c8dfc128e7167},
			{.l = 0x3fa555561debb5a2},
			{.l = 0xbe9529e1d03eeda3},
			{.l = 0xbf56b9fea6cc4993}
		}
	}
};

// sin & cos
// coefficient[0] is for sin; coefficient[1] is for cos;
// pi/4/4
static const DL
coefficient_pi_4_4[COEFFICIENTS][2][COEFFICIENTS] = {
	// 1 coefficients
	// 0x3e4fffffeb7f263d
	// 0x3fefb088755db288
	{
		{
			{.l = 0x3e4fffffeb7f263d}
		},
		{
			{.l = 0x3fefb088755db288}
		}
	},
	// 2 coefficients
	// 0x3dca5a4e69de3c2f + x * 0x3fefe5a58cc90760
	// 0x3ff009e2b5c37a68 + x * 0xbfb91cd0ec57c6fe
	{
		{
			{.l = 0x3dca5a4e69de3c2f},
			{.l = 0x3fefe5a58cc90760}
		},
		{
			{.l = 0x3ff009e2b5c37a68},
			{.l = 0xbfb91cd0ec57c6fe}
		}
	},
	// 3 coefficients
	// 0xbdaa4dc2e2f728ce + x * (0x3ff0034a604d10bd + x * 0xbfa0bd544e804e07)
	// 0x3ff0000425810070 + x * (0xbf33762ce6dc17bb + x * 0xbfdfd043a7ef0fc1)
	{
		{
			{.l = 0xbdaa4dc2e2f728ce},
			{.l = 0x3ff0034a604d10bd},
			{.l = 0xbfa0bd544e804e07}
		},
		{
			{.l = 0x3ff0000425810070},
			{.l = 0xbf33762ce6dc17bb},
			{.l = 0xbfdfd043a7ef0fc1}
		}
	},
	// 4 coefficients
	// 0xbd0a6d12cd6a4f6e + x * (0x3ff00000d3a1e6a4 + x * (0xbf0f103d1400ed4c + x * 0xbfc54243183bbbf1))
	// 0x3feffffefbbc7f9f + x * (0x3f14ad7e9388218b + x * (0xbfe0106f91a92d5b + x * 0x3f90bc81e83dda86))
	{
		{
			{.l = 0xbd0a6d12cd6a4f6e},
			{.l = 0x3ff00000d3a1e6a4},
			{.l = 0xbf0f103d1400ed4c},
			{.l = 0xbfc54243183bbbf1}
		},
		{
			{.l = 0x3feffffefbbc7f9f},
			{.l = 0x3f14ad7e9388218b},
			{.l = 0xbfe0106f91a92d5b},
			{.l = 0x3f90bc81e83dda86}
		}
	},
	// 5 coefficients
	// 0x3cd9f861ed669817 + x * (0x3fefffffcbfa83fb + x * (0x3ef085772a93af38 + x * (0xbfc5627986bf9c6b + x * 0x3f6ac6eb28e67b4e)))
	// 0x3fefffffffbf1399 + x * (0x3e7df2b0677cc0c2 + x * (0xbfe00008bd14c7a8 + x * (0x3f0b41f8a98b058e + x * 0x3fa537ee739e6cb8)))
	{
		{
			{.l = 0x3cd9f861ed669817},
			{.l = 0x3fefffffcbfa83fb},
			{.l = 0x3ef085772a93af38},
			{.l = 0xbfc5627986bf9c6b},
			{.l = 0x3f6ac6eb28e67b4e}
		},
		{
			{.l = 0x3fefffffffbf1399},
			{.l = 0x3e7df2b0677cc0c2},
			{.l = 0xbfe00008bd14c7a8},
			{.l = 0x3f0b41f8a98b058e},
			{.l = 0x3fa537ee739e6cb8}
		}
	},
	// 6 coefficients
	// 0x3c327cafa27860fe + x * (0x3feffffffff6bd37 + x * (0x3e51111eba33a411 + x * (0xbfc5555a5102bb09 + x * (0x3edf1bd2dcdde00c + x * 0x3f81004755cee660))))
	// 0x3ff000000002ad15 + x * (0xbe4ea0f6e1fcc808 + x * (0xbfdffffc7306a0f0 + x * (0xbef3461cf4c2b0bc + x * (0x3fa56cfcc2c9ce15 + x * 0xbf4ac643a9e29636))))
	{
		{
			{.l = 0x3c327cafa27860fe},
			{.l = 0x3feffffffff6bd37},
			{.l = 0x3e51111eba33a411},
			{.l = 0xbfc5555a5102bb09},
			{.l = 0x3edf1bd2dcdde00c},
			{.l = 0x3f81004755cee660}
		},
		{
			{.l = 0x3ff000000002ad15},
			{.l = 0xbe4ea0f6e1fcc808},
			{.l = 0xbfdffffc7306a0f0},
			{.l = 0xbef3461cf4c2b0bc},
			{.l = 0x3fa56cfcc2c9ce15},
			{.l = 0xbf4ac643a9e29636}
		}
	},
	// 7 coefficients
	// 0xbbf86ca2b1e587ef + x * (0x3ff00000000061ed + x * (0xbe2179bfee9687b0 + x * (0xbfc5555451f24138 + x * (0xbec602c8d8b0a55f + x * (0x3f811e943e3100b8 + x * 0xbf1e9956e922c465)))))
	// 0x3ff000000000007a + x * (0xbdac865673189f40 + x * (0xbfdffffffee99a86 + x * (0xbe5f8c3060609178 + x * (0x3fa55562a9e7d329 + x * (0xbec5f8177ad15767 + x * 0xbf56a316222252f5)))))
	{
		{
			{.l = 0xbbf86ca2b1e587ef},
			{.l = 0x3ff00000000061ed},
			{.l = 0xbe2179bfee9687b0},
			{.l = 0xbfc5555451f24138},
			{.l = 0xbec602c8d8b0a55f},
			{.l = 0x3f811e943e3100b8},
			{.l = 0xbf1e9956e922c465}
		},
		{
			{.l = 0x3ff000000000007a},
			{.l = 0xbdac865673189f40},
			{.l = 0xbfdffffffee99a86},
			{.l = 0xbe5f8c3060609178},
			{.l = 0x3fa55562a9e7d329},
			{.l = 0xbec5f8177ad15767},
			{.l = 0xbf56a316222252f5}
		}
	}
};

// sin & cos
// coefficient[0] is for sin; coefficient[1] is for cos;
// pi/4/2
static const DL
coefficient_pi_4_2[COEFFICIENTS][2][COEFFICIENTS] = {
	// 1 coefficients
	// 0x3e4ffffff58c28bb
	// 0x3feebbdfcfc213ab
	{
		{
			{.l = 0x3e4ffffff58c28bb}
		},
		{
			{.l = 0x3feebbdfcfc213ab}
		}
	},
	// 2 coefficients
	// 0x3dea743fe875fb49 + x * 0x3fef962ea3b318ec
	// 0x3ff027bd8a3d0e3a + x * 0xbfc90d5928ccd076
	{
		{
			{.l = 0x3dea743fe875fb49},
			{.l = 0x3fef962ea3b318ec}
		},
		{
			{.l = 0x3ff027bd8a3d0e3a},
			{.l = 0xbfc90d5928ccd076}
		}
	},
	// 3 coefficients
	// 0xbdca394c1d5f8234 + x * (0x3ff00d1e7f809cc5 + x * 0xbfb0ae9349031150)
	// 0x3ff000439afdf9d9 + x * (0xbf63a7970b742dbe + x * 0xbfdf405786c5868c)
	{
		{
			{.l = 0xbdca394c1d5f8234},
			{.l = 0x3ff00d1e7f809cc5},
			{.l = 0xbfb0ae9349031150}
		},
		{
			{.l = 0x3ff000439afdf9d9},
			{.l = 0xbf63a7970b742dbe},
			{.l = 0xbfdf405786c5868c}
		}
	},
	// 4 coefficients
	// 0xbd4a832c19cd646e + x * (0x3ff0000d441fc46b + x * (0xbf3f16d5b8d6e1d4 + x * 0xbfc50918ee2f3c82))
	// 0x3fefffefa27a2e33 + x * (0x3f44b373eeb1e289 + x * (0xbfe04195c92f17a7 + x * 0x3fa0ae2ec495b438))
	{
		{
			{.l = 0xbd4a832c19cd646e},
			{.l = 0x3ff0000d441fc46b},
			{.l = 0xbf3f16d5b8d6e1d4},
			{.l = 0xbfc50918ee2f3c82}
		},
		{
			{.l = 0x3fefffefa27a2e33},
			{.l = 0x3f44b373eeb1e289},
			{.l = 0xbfe04195c92f17a7},
			{.l = 0x3fa0ae2ec495b438}
		}
	},
	// 5 coefficients
	// 0x3d19da61dd452824 + x * (0x3feffffcc3ca7e11 + x * (0x3f20701f3db64d97 + x * (0xbfc589a259abffb4 + x * 0x3f7aacbc5517e71b)))
	// 0x3fefffffef77855a + x * (0x3ece56ff49fc2761 + x * (0xbfe0008cf7cc930e + x * (0x3f3b60ece9cd2dba + x * 0x3fa4df878bfc1945)))
	{
		{
			{.l = 0x3d19da61dd452824},
			{.l = 0x3feffffcc3ca7e11},
			{.l = 0x3f20701f3db64d97},
			{.l = 0xbfc589a259abffb4},
			{.l = 0x3f7aacbc5517e71b}
		},
		{
			{.l = 0x3fefffffef77855a},
			{.l = 0x3ece56ff49fc2761},
			{.l = 0xbfe0008cf7cc930e},
			{.l = 0x3f3b60ece9cd2dba},
			{.l = 0x3fa4df878bfc1945}
		}
	},
	// 6 coefficients
	// 0x3c9289f7d2ddbf19 + x * (0x3feffffffdadf797 + x * (0x3ea117480ed2a7f8 + x * (0xbfc555a5147fbd70 + x * (0x3f0f1660b971a53c + x * 0x3f80cdfe6e071c96))))
	// 0x3ff0000000ac638c + x * (0xbe9eb707034512f1 + x * (0xbfdfffc737272085 + x * (0xbf2337f78bb10cc7 + x * (0x3fa5b38ead1cedff + x * 0xbf5aac78a45a7bc9))))
	{
		{
			{.l = 0x3c9289f7d2ddbf19},
			{.l = 0x3feffffffdadf797},
			{.l = 0x3ea117480ed2a7f8},
			{.l = 0xbfc555a5147fbd70},
			{.l = 0x3f0f1660b971a53c},
			{.l = 0x3f80cdfe6e071c96}
		},
		{
			{.l = 0x3ff0000000ac638c},
			{.l = 0xbe9eb707034512f1},
			{.l = 0xbfdfffc737272085},
			{.l = 0xbf2337f78bb10cc7},
			{.l = 0x3fa5b38ead1cedff},
			{.l = 0xbf5aac78a45a7bc9}
		}
	},
	// 7 coefficients
	// 0xbc584ab2be4711b9 + x * (0x3ff00000001854f9 + x * (0xbe715f7bcfd221a6 + x * (0xbfc5554538c9c83a + x * (0xbef5dfde116fecf7 + x * (0x3f8146cfbb62fc73 + x * 0xbf2e79b667c060e9)))))
	// 0x3ff0000000007bea + x * (0xbe1ce9715c3ca2c2 + x * (0xbfdfffffb9b0e743 + x * (0xbeafc573352bb253 + x * (0x3fa5562b8fe02d84 + x * (0xbef604a5002299de + x * 0xbf5648025a6e4c90)))))
	{
		{
			{.l = 0xbc584ab2be4711b9},
			{.l = 0x3ff00000001854f9},
			{.l = 0xbe715f7bcfd221a6},
			{.l = 0xbfc5554538c9c83a},
			{.l = 0xbef5dfde116fecf7},
			{.l = 0x3f8146cfbb62fc73},
			{.l = 0xbf2e79b667c060e9}
		},
		{
			{.l = 0x3ff0000000007bea},
			{.l = 0xbe1ce9715c3ca2c2},
			{.l = 0xbfdfffffb9b0e743},
			{.l = 0xbeafc573352bb253},
			{.l = 0x3fa5562b8fe02d84},
			{.l = 0xbef604a5002299de},
			{.l = 0xbf5648025a6e4c90}
		}
	}
};

// sin & cos
// coefficient[0] is for sin; coefficient[1] is for cos;
// pi/4/1
static const DL
coefficient_pi_4_1[COEFFICIENTS][2][COEFFICIENTS] = {
	// 1 coefficients
	// 0x3e4ffffffa57d867
	// 0x3fea827999fcef32
	{
		{
			{.l = 0x3e4ffffffa57d867}
		},
		{
			{.l = 0x3fea827999fcef32}
		}
	},
	// 2 coefficients
	// 0x3e0adb8ebe66d8ae + x * 0x3fee52462b6a17cd
	// 0x3ff0a298a934a038 + x * 0xbfd8d0849caea8a9
	{
		{
			{.l = 0x3e0adb8ebe66d8ae},
			{.l = 0x3fee52462b6a17cd}
		},
		{
			{.l = 0x3ff0a298a934a038},
			{.l = 0xbfd8d0849caea8a9}
		}
	},
	// 3 coefficients
	// 0xbde9e78e6b11bc83 + x * (0x3ff033d44688d4de + x * 0xbfc073f325fce6d6)
	// 0x3ff0049774d691fa + x * (0xbf948a72dac87113 + x * 0xbfdcf3d5d05424df)
	{
		{
			{.l = 0xbde9e78e6b11bc83},
			{.l = 0x3ff033d44688d4de},
			{.l = 0xbfc073f325fce6d6}
		},
		{
			{.l = 0x3ff0049774d691fa},
			{.l = 0xbf948a72dac87113},
			{.l = 0xbfdcf3d5d05424df}
		}
	},
	// 4 coefficients
	// 0xbd8ae35e18682dad + x * (0x3ff000d737e982b5 + x * (0xbf6f35e927f7e33e + x * 0xbfc4251983c88fbc))
	// 0x3feffef2a624cb12 + x * (0x3f74d826afd60504 + x * (0xbfe1041e784aef3c + x * 0x3fb07704f670adf6))
	{
		{
			{.l = 0xbd8ae35e18682dad},
			{.l = 0x3ff000d737e982b5},
			{.l = 0xbf6f35e927f7e33e},
			{.l = 0xbfc4251983c88fbc}
		},
		{
			{.l = 0x3feffef2a624cb12},
			{.l = 0x3f74d826afd60504},
			{.l = 0xbfe1041e784aef3c},
			{.l = 0x3fb07704f670adf6}
		}
	},
	// 5 coefficients
	// 0x3d59672253924af3 + x * (0x3fefffcd279d7cc4 + x * (0x3f501c861b6c4271 + x * (0xbfc62256e03e2e4d + x * 0x3f8a44afa65d3254)))
	// 0x3feffffb840b67b0 + x * (0x3f2012e0e506fede + x * (0xbfe009248deedc11 + x * (0x3f6bf2a315dc5af4 + x * 0x3fa37a25a71af850)))
	{
		{
			{.l = 0x3d59672253924af3},
			{.l = 0x3fefffcd279d7cc4},
			{.l = 0x3f501c861b6c4271},
			{.l = 0xbfc62256e03e2e4d},
			{.l = 0x3f8a44afa65d3254}
		},
		{
			{.l = 0x3feffffb840b67b0},
			{.l = 0x3f2012e0e506fede},
			{.l = 0xbfe009248deedc11},
			{.l = 0x3f6bf2a315dc5af4},
			{.l = 0x3fa37a25a71af850}
		}
	},
	// 6 coefficients
	// 0x3cf2c81e1df2a50d + x * (0x3fefffff699b194e + x * (0x3ef134feabceba82 + x * (0xbfc55a5318de6f54 + x * (0x3f3f031bafd36271 + x * 0x3f8005ff40e73192))))
	// 0x3ff000002c6f6faa + x * (0xbeef2aa87cf0ded6 + x * (0xbfdffc73127aa3ba + x * (0xbf53066d8cc094a5 + x * (0x3fa6c846ef68d357 + x * 0xbf6a4815af673afc))))
	{
		{
			{.l = 0x3cf2c81e1df2a50d},
			{.l = 0x3fefffff699b194e},
			{.l = 0x3ef134feabceba82},
			{.l = 0xbfc55a5318de6f54},
			{.l = 0x3f3f031bafd36271},
			{.l = 0x3f8005ff40e73192}
		},
		{
			{.l = 0x3ff000002c6f6faa},
			{.l = 0xbeef2aa87cf0ded6},
			{.l = 0xbfdffc73127aa3ba},
			{.l = 0xbf53066d8cc094a5},
			{.l = 0x3fa6c846ef68d357},
			{.l = 0xbf6a4815af673afc}

		}
	},
	// 7 coefficients
	// 0xbcb7cbd21946ff99 + x * (0x3ff0000005f4bb6a + x * (0xbec0fa22e820a1ea + x * (0xbfc55459ddfff2d4 + x * (0xbf25556ba593269a + x * (0x3f81e333a4b9e05d + x * 0xbf3dfbf9b4139d97)))))
	// 0x3ff0000000864d2e + x * (0xbe8ece0fd28a9d8d + x * (0xbfdfffed91f6c602 + x * (0xbf006c4655abc12f + x * (0x3fa56302ae7e8e59 + x * (0xbf26454679f224d0 + x * 0xbf54d9e98b9890b7)))))
	{
		{
			{.l = 0xbcb7cbd21946ff99},
			{.l = 0x3ff0000005f4bb6a},
			{.l = 0xbec0fa22e820a1ea},
			{.l = 0xbfc55459ddfff2d4},
			{.l = 0xbf25556ba593269a},
			{.l = 0x3f81e333a4b9e05d},
			{.l = 0xbf3dfbf9b4139d97}
		},
		{
			{.l = 0x3ff0000000864d2e},
			{.l = 0xbe8ece0fd28a9d8d},
			{.l = 0xbfdfffed91f6c602},
			{.l = 0xbf006c4655abc12f},
			{.l = 0x3fa56302ae7e8e59},
			{.l = 0xbf26454679f224d0},
			{.l = 0xbf54d9e98b9890b7}
		}
	}
};

// link for all coefficients array
static const DL *link[COEFFICIENTS_VERSION];

static const double
interpolate[2][BITNUM] = {
	{
		0.00000000000000000e+00,
		6.13588464915447536e-03,
		1.22715382857199261e-02,
		1.84067299058048209e-02,
		2.45412285229122880e-02,
		3.06748031766366259e-02,
		3.68072229413588323e-02,
		4.29382569349408231e-02,
		4.90676743274180142e-02,
		5.51952443496899398e-02,
		6.13207363022085778e-02,
		6.74439195636640579e-02,
		7.35645635996674235e-02,
		7.96824379714301211e-02,
		8.57973123444398905e-02,
		9.19089564971327286e-02,
		9.80171403295606020e-02,
		1.04121633872054579e-01,
		1.10222207293883059e-01,
		1.16318630911904767e-01,
		1.22410675199216198e-01,
		1.28498110793793172e-01,
		1.34580708507126186e-01,
		1.40658239332849231e-01,
		1.46730474455361752e-01,
		1.52797185258443428e-01,
		1.58858143333861442e-01,
		1.64913120489969921e-01,
		1.70961888760301226e-01,
		1.77004220412148756e-01,
		1.83039887955140959e-01,
		1.89068664149806213e-01,
		1.95090322016128268e-01,
		2.01104634842091911e-01,
		2.07111376192218550e-01,
		2.13110319916091374e-01,
		2.19101240156869797e-01,
		2.25083911359792836e-01,
		2.31058108280671120e-01,
		2.37023605994367207e-01,
		2.42980179903263890e-01,
		2.48927605745720168e-01,
		2.54865659604514572e-01,
		2.60794117915275518e-01,
		2.66712757474898386e-01,
		2.72621355449948984e-01,
		2.78519689385053105e-01,
		2.84407537211271844e-01,
		2.90284677254462367e-01,
		2.96150888243623824e-01,
		3.02005949319228067e-01,
		3.07849640041534893e-01,
		3.13681740398891476e-01,
		3.19502030816015678e-01,
		3.25310292162262934e-01,
		3.31106305759876401e-01,
		3.36889853392220051e-01,
		3.42660717311994397e-01,
		3.48418680249434569e-01,
		3.54163525420490382e-01,
		3.59895036534988149e-01,
		3.65612997804773870e-01,
		3.71317193951837543e-01,
		3.77007410216418256e-01,
		3.82683432365089772e-01,
		3.88345046698826292e-01,
		3.93992040061048108e-01,
		3.99624199845646828e-01,
		4.05241314004989871e-01,
		4.10843171057903942e-01,
		4.16429560097637182e-01,
		4.22000270799799686e-01,
		4.27555093430282094e-01,
		4.33093818853151969e-01,
		4.38616238538527637e-01,
		4.44122144570429232e-01,
		4.49611329654606600e-01,
		4.55083587126343823e-01,
		4.60538710958240024e-01,
		4.65976495767966178e-01,
		4.71396736825997649e-01,
		4.76799230063322133e-01,
		4.82183772079122748e-01,
		4.87550160148435955e-01,
		4.92898192229784037e-01,
		4.98227666972781852e-01,
		5.03538383725717559e-01,
		5.08830142543107037e-01,
		5.14102744193221726e-01,
		5.19355990165589587e-01,
		5.24589682678468906e-01,
		5.29803624686294669e-01,
		5.34997619887097211e-01,
		5.40171472729892881e-01,
		5.45324988422046422e-01,
		5.50457972936604803e-01,
		5.55570233019602224e-01,
		5.60661576197336024e-01,
		5.65731810783613198e-01,
		5.70780745886967280e-01,
		5.75808191417845300e-01,
		5.80813958095764546e-01,
		5.85797857456438861e-01,
		5.90759701858874228e-01,
		5.95699304492433343e-01,
		6.00616479383868927e-01,
		6.05511041404325514e-01,
		6.10382806276309453e-01,
		6.15231590580626845e-01,
		6.20057211763289179e-01,
		6.24859488142386377e-01,
		6.29638238914927025e-01,
		6.34393284163645498e-01,
		6.39124444863775744e-01,
		6.43831542889791465e-01,
		6.48514401022112445e-01,
		6.53172842953776764e-01,
		6.57806693297078656e-01,
		6.62415777590171761e-01,
		6.66999922303637507e-01,
		6.71558954847018400e-01,
		6.76092703575315960e-01,
		6.80600997795453050e-01,
		6.85083667772700381e-01,
		6.89540544737066925e-01,
		6.93971460889654009e-01,
		6.98376249408972853e-01,
		7.02754744457225303e-01
	},
	{
		1.00000000000000000e+00,
		9.99981175282601143e-01,
		9.99924701839144541e-01,
		9.99830581795823422e-01,
		9.99698818696204220e-01,
		9.99529417501093163e-01,
		9.99322384588349501e-01,
		9.99077727752645383e-01,
		9.98795456205172393e-01,
		9.98475580573294753e-01,
		9.98118112900149207e-01,
		9.97723066644191610e-01,
		9.97290456678690217e-01,
		9.96820299291165715e-01,
		9.96312612182778013e-01,
		9.95767414467659794e-01,
		9.95184726672196886e-01,
		9.94564570734255452e-01,
		9.93906970002356041e-01,
		9.93211949234794533e-01,
		9.92479534598709998e-01,
		9.91709753669099523e-01,
		9.90902635427780025e-01,
		9.90058210262297105e-01,
		9.89176509964780974e-01,
		9.88257567730749491e-01,
		9.87301418157858383e-01,
		9.86308097244598648e-01,
		9.85277642388941245e-01,
		9.84210092386929073e-01,
		9.83105487431216328e-01,
		9.81963869109555264e-01,
		9.80785280403230449e-01,
		9.79569765685440534e-01,
		9.78317370719627633e-01,
		9.77028142657754351e-01,
		9.75702130038528545e-01,
		9.74339382785575861e-01,
		9.72939952205560146e-01,
		9.71503890986251776e-01,
		9.70031253194543992e-01,
		9.68522094274417316e-01,
		9.66976471044852109e-01,
		9.65394441697689375e-01,
		9.63776065795439867e-01,
		9.62121404269041596e-01,
		9.60430519415565811e-01,
		9.58703474895871556e-01,
		9.56940335732208865e-01,
		9.55141168305770722e-01,
		9.53306040354193837e-01,
		9.51435020969008369e-01,
		9.49528180593036667e-01,
		9.47585591017741135e-01,
		9.45607325380521326e-01,
		9.43593458161960361e-01,
		9.41544065183020779e-01,
		9.39459223602189912e-01,
		9.37339011912574923e-01,
		9.35183509938947577e-01,
		9.32992798834738887e-01,
		9.30766961078983732e-01,
		9.28506080473215566e-01,
		9.26210242138311342e-01,
		9.23879532511286756e-01,
		9.21514039342041943e-01,
		9.19113851690057744e-01,
		9.16679059921042663e-01,
		9.14209755703530655e-01,
		9.11706032005429851e-01,
		9.09167983090522377e-01,
		9.06595704514915365e-01,
		9.03989293123443332e-01,
		9.01348847046022014e-01,
		8.98674465693953844e-01,
		8.95966249756185156e-01,
		8.93224301195515320e-01,
		8.90448723244757890e-01,
		8.87639620402853948e-01,
		8.84797098430937780e-01,
		8.81921264348355030e-01,
		8.79012226428633478e-01,
		8.76070094195406607e-01,
		8.73094978418290099e-01,
		8.70086991108711419e-01,
		8.67046245515692652e-01,
		8.63972856121586738e-01,
		8.60866938637767279e-01,
		8.57728610000272070e-01,
		8.54557988365400521e-01,
		8.51355193105265142e-01,
		8.48120344803297252e-01,
		8.44853565249707074e-01,
		8.41554977436898410e-01,
		8.38224705554838043e-01,
		8.34862874986380056e-01,
		8.31469612302545237e-01,
		8.28045045257755752e-01,
		8.24589302785025264e-01,
		8.21102514991104680e-01,
		8.17584813151583697e-01,
		8.14036329705948362e-01,
		8.10457198252594792e-01,
		8.06847553543799272e-01,
		8.03207531480644910e-01,
		7.99537269107905033e-01,
		7.95836904608883536e-01,
		7.92106577300212352e-01,
		7.88346427626606262e-01,
		7.84556597155575233e-01,
		7.80737228572094479e-01,
		7.76888465673232450e-01,
		7.73010453362736960e-01,
		7.69103337645579640e-01,
		7.65167265622458927e-01,
		7.61202385484261814e-01,
		7.57208846506484548e-01,
		7.53186799043612482e-01,
		7.49136394523459326e-01,
		7.45057785441465962e-01,
		7.40951125354959091e-01,
		7.36816568877369875e-01,
		7.32654271672412835e-01,
		7.28464390448225196e-01,
		7.24247082951466921e-01,
		7.20002507961381630e-01,
		7.15730825283818655e-01,
		7.11432195745216441e-01
	}
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
	
	if ((func = fopen("sin_gen.c", "w")) == (FILE *)0) {
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
		fprintf(func, "static const double\n");
		fprintf(func, "invpio4 = 1.2732395447351626861510701069801,\n");
		fprintf(func, "invpio512 = 162.97466172610082382733697369345,\n");
		fprintf(func, "pi_4 = 0.78539816339744830961566084581988,\n");
		fprintf(func, "pi_2 = 1.5707963267948966192313216916398,\n");
		fprintf(func, "pi_512 = 0.00613592315154256491887235035797;\n");
		//fprintf(func, "X = 0;\n");
		//fprintf(func, "X = %.17lf;\n", X);
		/*fprintf(func, "table[TableNum] = {\t// pi_2 * T, T = 0, 1, 2, ..., %d\n", btemp);
		for (i = 0; i < btemp; i++)
		{
			fprintf(func, "\t%.17lf,\n", table[i]);
		}
		fprintf(func, "\t%.17lf\n ", table[i]);
		fprintf(func, "};\n");
		fprintf(func, "extern double k_sin(double);\n");
		fprintf(func, "extern double k_cos(double);\n");*/
		fprintf(func, "\n");

		fprintf(func, "static const double\n");
		fprintf(func, "pio2_1 = 1.57079632673412561417e+00, /* 0x3FF921FB, 0x54400000 */\n");
		fprintf(func, "pio2_1t = 6.07710050650619224932e-11; /* 0x3DD0B461, 0x1A626331 */\n");
		fprintf(func, "\n");

		fprintf(func, "static const DL\n");
		fprintf(func, "pi_4_1 = { .l = 0x3FE921FB54400000 },\n");
		fprintf(func, "pi_4_1t = { .l = 0x3DC0B4611A626331 };\n");

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
		fprintf(func, "\t\t%.17e\n", interpolate[0][i * (1 << (7 - bit))]);
		fprintf(func, "\t},\n");
		fprintf(func, "\t{\n");
		for (i = 0; i < bitnum - 1; i++) {
			fprintf(func, "\t\t%.17e,\n", interpolate[1][i * (1 << (7 - bit))]);
		}
		fprintf(func, "\t\t%.17e\n", interpolate[1][i * (1 << (7 - bit))]);
		fprintf(func, "\t}\n");
		fprintf(func, "};\n\n");

		// func
		//fprintf(func, "double sin_gen(double x) {\n");
		//fprintf(func, "\tdouble ix, iix, iiix, y, appro_s, appro_c;\n");
		fprintf(func, "double sin_gen(double ix) {\n");
		fprintf(func, "\tdouble iix, iiix, y, appro_s, appro_c;\n");
		fprintf(func, "\tlong int temp, table_order, status_pi_4, status_pi_2, status_pi_1, flag, sign, sin_or_cos;\n");
		fprintf(func, "\tint i;\n");
		fprintf(func, "\n");
		//fprintf(func, "\tix = x - X;\n");
		fprintf(func, "\ttemp = *((long int *)(&ix));\n");
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
		fprintf(func, "\t// iix = ix - [ix * invpio4] * pi_4\n");
		fprintf(func, "\t// iiix = iix - i / %d * pi_4\n", bitnum);
		fprintf(func, "\ttemp = (long int)(ix * invpio4 * BITNUM); // why not ix * invpio_512;\n");
		fprintf(func, "\tstatus_pi_4 = (temp >> BIT) & 0x1; \n");
		fprintf(func, "\tstatus_pi_2 = (temp >> (BIT + 1)) & 0x1;\n");
		fprintf(func, "\tstatus_pi_1 = (temp >> (BIT + 2)) & 0x1;\n");
		fprintf(func, "\tiix = ix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_1;\n");
		fprintf(func, "\tiix = iix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_1t;\n");
		fprintf(func, "\t//iix = iix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_2;\n");
		fprintf(func, "\t//iix = iix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_2t;\n");
		fprintf(func, "\t//iix = iix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_3;\n");
		fprintf(func, "\t//iix = iix - ((temp >> (BIT + 1)) + status_pi_4) * pio2_3t;\n");
		fprintf(func, "\ttable_order = temp & (BITNUM_1); // BITNUM - 1 = (2^BIT -1)\n");
		fprintf(func, "\ttable_order = table_order - status_pi_4 * (BITNUM_1);\n");
		fprintf(func, "\ttable_order = (1 - status_pi_4 * 2) * table_order;\n");
		fprintf(func, "\ttemp = *((long int *)(&iix));\n");
		fprintf(func, "\ttemp = temp & 0x7fffffffffffffff;\n");
		fprintf(func, "\tiix = *((double *)(&temp)); // at this time, iix is absolute value\n");
		fprintf(func, "\tiiix = iix - ((double)table_order) / BITNUM * pi_4_1.d;\n");
		fprintf(func, "\tiiix = iiix - ((double)table_order) / BITNUM * pi_4_1t.d;\n");
		fprintf(func, "\t//iiix = iiix - ((double)table_order) / BITNUM * pi_4_2.d;\n");
		fprintf(func, "\t//iiix = iiix - ((double)table_order) / BITNUM * pi_4_2t.d;\n");
		fprintf(func, "\t//iiix = iiix - ((double)table_order) / BITNUM * pi_4_3.d;\n");
		fprintf(func, "\t//iiix = iiix - ((double)table_order) / BITNUM * pi_4_3t.d;\n");
		fprintf(func, "\tsign = flag ^ status_pi_1; // ornot，0 is +, 1 is -\n");
		fprintf(func, "\tsin_or_cos = status_pi_2 ^ status_pi_4; // ornot, 0 is sin, 1 is cos\n");
		fprintf(func, "\n");
		
		fprintf(func, "\t// new, sin(x+x') = sin(x)cos(x') + sin(x')cos(x), x' = iiix\n");
		fprintf(func, "\t// OR, cos(x+x') = cos(x)cos(x') - sin(x)sin(x'), x' = iiix\n");

		//fprintf(func, "\tappro_s = coefficient[0][0].d");
		fprintf(func, "\tappro_s = 0.0");
		for (i = 1; i <= degree; i++) {
			fprintf(func, " + iiix * (coefficient[0][%ld].d", i);
		}
		for (i = 1; i <= degree; i++) {
			fprintf(func, ")");
		}
		fprintf(func, ";\n");
		//fprintf(func, "\tappro_c = coefficient[1][0].d");
		fprintf(func, "\tappro_c = 0.0");
		for (i = 1; i <= degree; i++) {
			fprintf(func, " + iiix * (coefficient[1][%ld].d", i);
		}
		for (i = 1; i <= degree; i++) {
			fprintf(func, ")");
		}
		fprintf(func, ";\n");

		//fprintf(func, "\tappro_s = coefficient[0][0].d + iiix * (coefficient[0][1].d + iiix * (coefficient[0][2].d + iiix * (coefficient[0][3].d + iiix * (coefficient[0][4].d + iiix * coefficient[0][5].d))));\n");
		//fprintf(func, "\tappro_c = coefficient[1][0].d + iiix * (coefficient[1][1].d + iiix * (coefficient[1][2].d + iiix * (coefficient[1][3].d + iiix * (coefficient[1][4].d + iiix * coefficient[1][5].d))));\n");
		fprintf(func, "\n");
		fprintf(func, "\ty = interpolate[sin_or_cos][table_order] * appro_c + (1 - sin_or_cos * 2) * interpolate[1 - sin_or_cos][table_order] * appro_s;\n");
		fprintf(func, "\ty += (1 - sin_or_cos * 2) * interpolate[1 - sin_or_cos][table_order] * coefficient[0][0].d + interpolate[sin_or_cos][table_order] * coefficient[1][0].d;\n");
		fprintf(func, "\ty = (1 - sign * 2) * y;\n");
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

	link[7] = &(coefficient_pi_4_128[0][0][0]);
	link[6] = &(coefficient_pi_4_64[0][0][0]);
	link[5] = &(coefficient_pi_4_32[0][0][0]);
	link[4] = &(coefficient_pi_4_16[0][0][0]);
	link[3] = &(coefficient_pi_4_8[0][0][0]);
	link[2] = &(coefficient_pi_4_4[0][0][0]);
	link[1] = &(coefficient_pi_4_2[0][0][0]);
	link[0] = &(coefficient_pi_4_1[0][0][0]);

	input_parameter.start = a;
	input_parameter.end = b;
	input_parameter.precision = precision;
	input_parameter.bit = bit;
	input_parameter.fnum = fnum;
	input_parameter.degree = degree;
	gen(input_parameter);

	return 0;
}