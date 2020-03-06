#include <stdio.h>
#include <stdlib.h>
#include "myhead.h"
//#include "libm.h"

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

const DL logtabhi[] =
{
	{.l = 0x0000000000000000 },
	{.l = 0x3f7fe02a6b200000 }, // ln(1+1/128)
	{.l = 0x3f8fc0a8b1000000 }, // ln(1+2/128)
	{.l = 0x3f97b91b07d80000 },
	{.l = 0x3f9f829b0e780000 },
	{.l = 0x3fa39e87ba000000 },
	{.l = 0x3fa77458f6340000 },
	{.l = 0x3fab42dd71180000 },
	{.l = 0x3faf0a30c0100000 },
	{.l = 0x3fb16536eea40000 },
	{.l = 0x3fb341d7961c0000 },
	{.l = 0x3fb51b073f060000 },
	{.l = 0x3fb6f0d28ae60000 },
	{.l = 0x3fb8c345d6320000 },
	{.l = 0x3fba926d3a4a0000 },
	{.l = 0x3fbc5e548f5c0000 },
	{.l = 0x3fbe27076e2a0000 },
	{.l = 0x3fbfec9131dc0000 },
	{.l = 0x3fc0d77e7cd10000 },
	{.l = 0x3fc1b72ad52f0000 },
	{.l = 0x3fc29552f8200000 },
	{.l = 0x3fc371fc201f0000 },
	{.l = 0x3fc44d2b6ccb0000 },
	{.l = 0x3fc526e5e3a20000 },
	{.l = 0x3fc5ff3070a80000 },
	{.l = 0x3fc6d60fe71a0000 },
	{.l = 0x3fc7ab8902110000 },
	{.l = 0x3fc87fa065210000 },
	{.l = 0x3fc9525a9cf40000 },
	{.l = 0x3fca23bc1fe30000 },
	{.l = 0x3fcaf3c94e810000 },
	{.l = 0x3fcbc286742e0000 },
	{.l = 0x3fcc8ff7c79b0000 },
	{.l = 0x3fcd5c216b500000 },
	{.l = 0x3fce27076e2b0000 },
	{.l = 0x3fcef0adcbdc0000 },
	{.l = 0x3fcfb9186d5e0000 },
	{.l = 0x3fd0402594b50000 },
	{.l = 0x3fd0a324e2738000 },
	{.l = 0x3fd1058bf9ae8000 },
	{.l = 0x3fd1675cabab8000 },
	{.l = 0x3fd1c898c1698000 },
	{.l = 0x3fd22941fbcf8000 },
	{.l = 0x3fd2895a13de8000 },
	{.l = 0x3fd2e8e2bae10000 },
	{.l = 0x3fd347dd9a988000 },
	{.l = 0x3fd3a64c55698000 },
	{.l = 0x3fd4043086868000 },
	{.l = 0x3fd4618bc21c8000 },
	{.l = 0x3fd4be5f95778000 },
	{.l = 0x3fd51aad872e0000 },
	{.l = 0x3fd5767717458000 },
	{.l = 0x3fd5d1bdbf580000 },
	{.l = 0x3fd62c82f2ba0000 },
	{.l = 0x3fd686c81e9b0000 },
	{.l = 0x3fd6e08eaa2b8000 },
	{.l = 0x3fd739d7f6bc0000 },
	{.l = 0x3fd792a55fdd8000 },
	{.l = 0x3fd7eaf83b828000 },
	{.l = 0x3fd842d1da1e8000 },
	{.l = 0x3fd89a3386c18000 },
	{.l = 0x3fd8f11e87368000 },
	{.l = 0x3fd947941c210000 },
	{.l = 0x3fd99d9581180000 },
	{.l = 0x3fd9f323ecbf8000 },
	{.l = 0xbfd214456d0e8000 },
	{.l = 0xbfd1bf99635a8000 },
	{.l = 0xbfd16b5ccbad0000 },
	{.l = 0xbfd1178e82280000 },
	{.l = 0xbfd0c42d67618000 },
	{.l = 0xbfd07138604d8000 },
	{.l = 0xbfd01eae56270000 },
	{.l = 0xbfcf991c6cb40000 },
	{.l = 0xbfcef5ade4dd0000 },
	{.l = 0xbfce530effe70000 },
	{.l = 0xbfcdb13db0d50000 },
	{.l = 0xbfcd1037f2650000 },
	{.l = 0xbfcc6ffbc6f00000 },
	{.l = 0xbfcbd087383c0000 },
	{.l = 0xbfcb31d8575c0000 },
	{.l = 0xbfca93ed3c8b0000 },
	{.l = 0xbfc9f6c407090000 },
	{.l = 0xbfc95a5adcf70000 },
	{.l = 0xbfc8beafeb390000 },
	{.l = 0xbfc823c165520000 },
	{.l = 0xbfc7898d85440000 },
	{.l = 0xbfc6f0128b750000 },
	{.l = 0xbfc6574ebe8c0000 },
	{.l = 0xbfc5bf406b540000 },
	{.l = 0xbfc527e5e4a20000 },
	{.l = 0xbfc4913d83340000 },
	{.l = 0xbfc3fb45a5990000 },
	{.l = 0xbfc365fcb0160000 },
	{.l = 0xbfc2d1610c870000 },
	{.l = 0xbfc23d712a4a0000 },
	{.l = 0xbfc1aa2b7e240000 },
	{.l = 0xbfc1178e82280000 },
	{.l = 0xbfc08598b59e0000 },
	{.l = 0xbfbfe89139dc0000 },
	{.l = 0xbfbec739830a0000 },
	{.l = 0xbfbda72763840000 },
	{.l = 0xbfbc885801bc0000 },
	{.l = 0xbfbb6ac88dae0000 },
	{.l = 0xbfba4e7640b20000 },
	{.l = 0xbfb9335e5d5a0000 },
	{.l = 0xbfb8197e2f400000 },
	{.l = 0xbfb700d30aea0000 },
	{.l = 0xbfb5e95a4d980000 },
	{.l = 0xbfb4d3115d200000 },
	{.l = 0xbfb3bdf5a7d20000 },
	{.l = 0xbfb2aa04a4480000 },
	{.l = 0xbfb1973bd1460000 },
	{.l = 0xbfb08598b59e0000 },
	{.l = 0xbfaeea31c0080000 },
	{.l = 0xbfaccb73cddc0000 },
	{.l = 0xbfaaaef2d0fc0000 },
	{.l = 0xbfa894aa14a00000 },
	{.l = 0xbfa67c94f2d40000 },
	{.l = 0xbfa466aed42c0000 },
	{.l = 0xbfa252f32f8c0000 },
	{.l = 0xbfa0415d89e80000 },
	{.l = 0xbf9c63d2ec180000 },
	{.l = 0xbf98492528c80000 },
	{.l = 0xbf9432a925980000 },
	{.l = 0xbf90205658900000 },
	{.l = 0xbf882448a3900000 },
	{.l = 0xbf80101575900000 },
	{.l = 0xbf70080559600000 },
	{.l = 0x0000000000000000 },
};

const DL logtablo[] =
{
	{.l = 0x0000000000000000 },
	{.l = 0xbd6f30ee07912df9 },
	{.l = 0xbd5fe0e183092c59 },
	{.l = 0xbd62772ab6c0559c },
	{.l = 0x3d2980267c7e09e4 },
	{.l = 0xbd642a056fea4dfd },
	{.l = 0xbd62303b9cb0d5e1 },
	{.l = 0x3d671bec28d14c7e },
	{.l = 0x3d662a6617cc9717 },
	{.l = 0xbd60a3e2f3b47d18 },
	{.l = 0xbd4717b6b33e44f8 },
	{.l = 0x3d383f69278e686a },
	{.l = 0xbd62968c836cc8c2 },
	{.l = 0xbd5937c294d2f567 },
	{.l = 0x3d6aac6ca17a4554 },
	{.l = 0xbd4c5e7514f4083f },
	{.l = 0x3d6e5cbd3d50fffc },
	{.l = 0xbd354555d1ae6607 },
	{.l = 0xbd6c69a65a23a170 },
	{.l = 0x3d69e80a41811a39 },
	{.l = 0xbd35b967f4471dfc },
	{.l = 0xbd6c22f10c9a4ea8 },
	{.l = 0x3d6f4799f4f6543e },
	{.l = 0xbd62f21746ff8a47 },
	{.l = 0xbd6b0b0de3077d7e },
	{.l = 0xbd56f1b955c4d1da },
	{.l = 0xbd537b720e4a694b },
	{.l = 0xbd5b77b7effb7f41 },
	{.l = 0x3d65ad1d904c1d4e },
	{.l = 0xbd62a739b23b93e1 },
	{.l = 0xbd600349cc67f9b2 },
	{.l = 0xbd6cca75818c5dbc },
	{.l = 0xbd697794f689f843 },
	{.l = 0xbd611ba91bbca682 },
	{.l = 0xbd3a342c2af0003c },
	{.l = 0x3d664d948637950e },
	{.l = 0x3d5f1546aaa3361c },
	{.l = 0xbd67df928ec217a5 },
	{.l = 0x3d50e35f73f7a018 },
	{.l = 0xbd6a9573b02faa5a },
	{.l = 0x3d630701ce63eab9 },
	{.l = 0x3d59fafbc68e7540 },
	{.l = 0xbd3a6976f5eb0963 },
	{.l = 0x3d3a8d7ad24c13f0 },
	{.l = 0x3d5d309c2cc91a85 },
	{.l = 0xbd25594dd4c58092 },
	{.l = 0xbd6d0b1c68651946 },
	{.l = 0x3d63f1de86093efa },
	{.l = 0xbd609ec17a426426 },
	{.l = 0xbd3d7c92cd9ad824 },
	{.l = 0xbd3f4bd8db0a7cc1 },
	{.l = 0xbd62c9d5b2a49af9 },
	{.l = 0x3d4394a11b1c1ee4 },
	{.l = 0xbd6c356848506ead },
	{.l = 0x3d54aec442be1015 },
	{.l = 0x3d60f1c609c98c6c },
	{.l = 0xbd67fcb18ed9d603 },
	{.l = 0xbd6c2ec1f512dc03 },
	{.l = 0x3d67e1b259d2f3da },
	{.l = 0x3d462e927628cbc2 },
	{.l = 0xbd6ed2a52c73bf78 },
	{.l = 0xbd5d3881e8962a96 },
	{.l = 0x3d56faba4cdd147d },
	{.l = 0xbd5f753456d113b8 },
	{.l = 0x3d584bf2b68d766f },
	{.l = 0xbd6c6a1f7ae91aec },
	{.l = 0x3d546b2255d70849 },
	{.l = 0x3d323299042d74bf },
	{.l = 0x3d5b8421cc74be04 },
	{.l = 0x3d5d1cee9d3862a3 },
	{.l = 0x3d63cec649d22563 },
	{.l = 0x3d6cb78460a42f4a },
	{.l = 0x3d6321a099af9906 },
	{.l = 0x3cca211565bb8e11 },
	{.l = 0xbd401212276041f4 },
	{.l = 0x3d6daff2af715b03 },
	{.l = 0xbd679ed606292424 },
	{.l = 0xbd3ee138d3a69d43 },
	{.l = 0x3d53a9788b2a8257 },
	{.l = 0x3d58e1ac6a75670d },
	{.l = 0x3d530e486a0ac42d },
	{.l = 0x3d6a66fb2979a7e8 },
	{.l = 0xbd07f22858a0ff6f },
	{.l = 0x3d073d54aae92cd1 },
	{.l = 0x3d670f91232ce70c },
	{.l = 0xbd631ccf7c7b75e8 },
	{.l = 0xbd6aaee721a63de2 },
	{.l = 0xbd4339f1658785cf },
	{.l = 0xbd5ed8fdc149767e },
	{.l = 0x3d629cc171a96824 },
	{.l = 0x3d62a7c86ab02498 },
	{.l = 0xbd5465c4f032b956 },
	{.l = 0x3d6bfa7415f72d23 },
	{.l = 0x3d6fb18a4cd1f92d },
	{.l = 0x3d5eff2dc702c20a },
	{.l = 0x3d31ac38dde3b366 },
	{.l = 0x3d4b8421cc74be04 },
	{.l = 0xbd5d034451fecdfb },
	{.l = 0x3d454d3593e842bf },
	{.l = 0xbd311fcba80cdd10 },
	{.l = 0xbd51a89401fa7173 },
	{.l = 0xbd52c8da38cb559a },
	{.l = 0x3d649c84015fbb47 },
	{.l = 0x3d50f215b5ca203e },
	{.l = 0x3d66ceea3c542b82 },
	{.l = 0xbd6c7e0373e5bff8 },
	{.l = 0xbd681c1e8da99ded },
	{.l = 0x3d5b8d20c78ba3a4 },
	{.l = 0xbd5fab1769f42c78 },
	{.l = 0x3d319bd0ad125895 },
	{.l = 0x3d6d0b6e8ae9c698 },
	{.l = 0xbd5559b4553e4c2d },
	{.l = 0xbd4d034451fecdfb },
	{.l = 0x3d647844f93f24ee },
	{.l = 0xbd6b2cb86dc13ec0 },
	{.l = 0x3d5de07cb958897a },
	{.l = 0x3d432f32ba0b44d0 },
	{.l = 0xbd576b08209f3283 },
	{.l = 0xbd6e3e98c8a4202d },
	{.l = 0xbd6183e9ae021b68 },
	{.l = 0x3d577771fd187145 },
	{.l = 0x3d6aa8739c0614d1 },
	{.l = 0xbd4957d173697cf3 },
	{.l = 0xbd098139928637fe },
	{.l = 0xbd6ac23a4f91d083 },
	{.l = 0x3d6d75577da74f64 },
	{.l = 0x3d6c863b5ccce95f },
	{.l = 0x3d5dd32a0699c731 },
	{.l = 0x0000000000000000 },
};

const DL log_ru[] =
{
	{.l = 0x3ff0000000000000 },
	{.l = 0x3fefc07f01fc07f0 },
	{.l = 0x3fef81f81f81f820 },
	{.l = 0x3fef44659e4a4271 },
	{.l = 0x3fef07c1f07c1f08 },
	{.l = 0x3feecc07b301ecc0 },
	{.l = 0x3fee9131abf0b767 },
	{.l = 0x3fee573ac901e574 },
	{.l = 0x3fee1e1e1e1e1e1e },
	{.l = 0x3fede5d6e3f8868a },
	{.l = 0x3fedae6076b981db },
	{.l = 0x3fed77b654b82c34 },
	{.l = 0x3fed41d41d41d41d },
	{.l = 0x3fed0cb58f6ec074 },
	{.l = 0x3fecd85689039b0b },
	{.l = 0x3feca4b3055ee191 },
	{.l = 0x3fec71c71c71c71c },
	{.l = 0x3fec3f8f01c3f8f0 },
	{.l = 0x3fec0e070381c0e0 },
	{.l = 0x3febdd2b899406f7 },
	{.l = 0x3febacf914c1bad0 },
	{.l = 0x3feb7d6c3dda338b },
	{.l = 0x3feb4e81b4e81b4f },
	{.l = 0x3feb2036406c80d9 },
	{.l = 0x3feaf286bca1af28 },
	{.l = 0x3feac5701ac5701b },
	{.l = 0x3fea98ef606a63be },
	{.l = 0x3fea6d01a6d01a6d },
	{.l = 0x3fea41a41a41a41a },
	{.l = 0x3fea16d3f97a4b02 },
	{.l = 0x3fe9ec8e951033d9 },
	{.l = 0x3fe9c2d14ee4a102 },
	{.l = 0x3fe999999999999a },
	{.l = 0x3fe970e4f80cb872 },
	{.l = 0x3fe948b0fcd6e9e0 },
	{.l = 0x3fe920fb49d0e229 },
	{.l = 0x3fe8f9c18f9c18fa },
	{.l = 0x3fe8d3018d3018d3 },
	{.l = 0x3fe8acb90f6bf3aa },
	{.l = 0x3fe886e5f0abb04a },
	{.l = 0x3fe8618618618618 },
	{.l = 0x3fe83c977ab2bedd },
	{.l = 0x3fe8181818181818 },
	{.l = 0x3fe7f405fd017f40 },
	{.l = 0x3fe7d05f417d05f4 },
	{.l = 0x3fe7ad2208e0ecc3 },
	{.l = 0x3fe78a4c8178a4c8 },
	{.l = 0x3fe767dce434a9b1 },
	{.l = 0x3fe745d1745d1746 },
	{.l = 0x3fe724287f46debc },
	{.l = 0x3fe702e05c0b8170 },
	{.l = 0x3fe6e1f76b4337c7 },
	{.l = 0x3fe6c16c16c16c17 },
	{.l = 0x3fe6a13cd1537290 },
	{.l = 0x3fe6816816816817 },
	{.l = 0x3fe661ec6a5122f9 },
	{.l = 0x3fe642c8590b2164 },
	{.l = 0x3fe623fa77016240 },
	{.l = 0x3fe6058160581606 },
	{.l = 0x3fe5e75bb8d015e7 },
	{.l = 0x3fe5c9882b931057 },
	{.l = 0x3fe5ac056b015ac0 },
	{.l = 0x3fe58ed2308158ed },
	{.l = 0x3fe571ed3c506b3a },
	{.l = 0x3fe5555555555555 },
	{.l = 0x3fe5390948f40feb },
	{.l = 0x3fe51d07eae2f815 },
	{.l = 0x3fe5015015015015 },
	{.l = 0x3fe4e5e0a72f0539 },
	{.l = 0x3fe4cab88725af6e },
	{.l = 0x3fe4afd6a052bf5b },
	{.l = 0x3fe49539e3b2d067 },
	{.l = 0x3fe47ae147ae147b },
	{.l = 0x3fe460cbc7f5cf9a },
	{.l = 0x3fe446f86562d9fb },
	{.l = 0x3fe42d6625d51f87 },
	{.l = 0x3fe4141414141414 },
	{.l = 0x3fe3fb013fb013fb },
	{.l = 0x3fe3e22cbce4a902 },
	{.l = 0x3fe3c995a47babe7 },
	{.l = 0x3fe3b13b13b13b14 },
	{.l = 0x3fe3991c2c187f63 },
	{.l = 0x3fe3813813813814 },
	{.l = 0x3fe3698df3de0748 },
	{.l = 0x3fe3521cfb2b78c1 },
	{.l = 0x3fe33ae45b57bcb2 },
	{.l = 0x3fe323e34a2b10bf },
	{.l = 0x3fe30d190130d190 },
	{.l = 0x3fe2f684bda12f68 },
	{.l = 0x3fe2e025c04b8097 },
	{.l = 0x3fe2c9fb4d812ca0 },
	{.l = 0x3fe2b404ad012b40 },
	{.l = 0x3fe29e4129e4129e },
	{.l = 0x3fe288b01288b013 },
	{.l = 0x3fe27350b8812735 },
	{.l = 0x3fe25e22708092f1 },
	{.l = 0x3fe2492492492492 },
	{.l = 0x3fe23456789abcdf },
	{.l = 0x3fe21fb78121fb78 },
	{.l = 0x3fe20b470c67c0d9 },
	{.l = 0x3fe1f7047dc11f70 },
	{.l = 0x3fe1e2ef3b3fb874 },
	{.l = 0x3fe1cf06ada2811d },
	{.l = 0x3fe1bb4a4046ed29 },
	{.l = 0x3fe1a7b9611a7b96 },
	{.l = 0x3fe19453808ca29c },
	{.l = 0x3fe1811811811812 },
	{.l = 0x3fe16e0689427379 },
	{.l = 0x3fe15b1e5f75270d },
	{.l = 0x3fe1485f0e0acd3b },
	{.l = 0x3fe135c81135c811 },
	{.l = 0x3fe12358e75d3033 },
	{.l = 0x3fe1111111111111 },
	{.l = 0x3fe0fef010fef011 },
	{.l = 0x3fe0ecf56be69c90 },
	{.l = 0x3fe0db20a88f4696 },
	{.l = 0x3fe0c9714fbcda3b },
	{.l = 0x3fe0b7e6ec259dc8 },
	{.l = 0x3fe0a6810a6810a7 },
	{.l = 0x3fe0953f39010954 },
	{.l = 0x3fe0842108421084 },
	{.l = 0x3fe073260a47f7c6 },
	{.l = 0x3fe0624dd2f1a9fc },
	{.l = 0x3fe05197f7d73404 },
	{.l = 0x3fe0410410410410 },
	{.l = 0x3fe03091b51f5e1a },
	{.l = 0x3fe0204081020408 },
	{.l = 0x3fe0101010101010 },
	{.l = 0x3fe0000000000000 },
};

// fpminmax: [0, 1/128]
static const DL
coefficient_1div_128[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3cafffffffffff7f}
	},
	// P = fpminimax(log(1 + x),[| 1 | ],[| D... | ],[1b - 53,1 / 128]); printexpansion(P);
	// x * 0x3feff00d46bd8a01
	{
		{.l = 0},
		{.l = 0x3feff00d46bd8a01}
	},
	// P = fpminimax(log(1 + x),[| 1,2 | ],[| D... | ],[1b - 53,1 / 128]); printexpansion(P);
	// x * (0x3feffffab3f0f240 + x * 0xbfdfd58faa8ef660)
	{
		{.l = 0},
		{.l = 0x3feffffab3f0f240},
		{.l = 0xbfdfd58faa8ef660}
	},
	// P = fpminimax(log(1 + x),[| 1,2,3 | ],[| D... | ],[1b - 53,1 / 128]); printexpansion(P);
	// x * (0x3feffffffe055b24 + x * (0xbfdfffdc5752c6a4 + x * 0x3fd525ad86013187))
	{
		{.l = 0},
		{.l = 0x3feffffffe055b24},
		{.l = 0xbfdfffdc5752c6a4},
		{.l = 0x3fd525ad86013187}
	},
	// P = fpminimax(log(1 + x),[| 1,2,3,4 | ],[| D... | ],[1b - 53,1 / 128]); printexpansion(P);
	// x * (0x3fefffffffff361b + x * (0xbfdfffffe6be5aea + x * (0x3fd5551619f1fd31 + x * 0xbfcf9a869bd56952)))
	{
		{.l = 0},
		{.l = 0x3fefffffffff361b},
		{.l = 0xbfdfffffe6be5aea},
		{.l = 0x3fd5551619f1fd31},
		{.l = 0xbfcf9a869bd56952}
	},
	// P = fpminimax(log(1 + x),[| 1,2,3,4,5 | ],[| D... | ],[1b - 53,1 / 128]); printexpansion(P);
	// x * (0x3fefffffffffffac + x * (0xbfdfffffffef9c86 + x * (0x3fd5555513bbc9c0 + x * (0xbfcfff4804af2d01  + x * 0x3fc93017b07f7536))))
	{
		{.l = 0},
		{.l = 0x3fefffffffffffac},
		{.l = 0xbfdfffffffef9c86},
		{.l = 0x3fd5555513bbc9c0},
		{.l = 0xbfcfff4804af2d01},
		{.l = 0x3fc93017b07f7536}
	},
	// P = fpminimax(log(1 + x),[| 1,2,3,4,5,6 | ],[| D... | ],[1b - 53,1 / 128]); printexpansion(P);
	// x * (0x3ff0000000000000 + x * (0xbfdffffffffff8ba + x * (0x3fd55555552260cf + x * (0xbfcfffff16c63d6a  + x * (0x3fc998afe2adfada + x * 0xbfc4eaa610c545e3)))))
	{
		{.l = 0 },
		{.l = 0x3ff0000000000000 },
		{.l = 0xbfdffffffffff8ba },
		{.l = 0x3fd55555552260cf },
		{.l = 0xbfcfffff16c63d6a },
		{.l = 0x3fc998afe2adfada },
		{.l = 0xbfc4eaa610c545e3 },
	},
	// P = fpminimax(log(1+x), [|1,2,3,4,5,6,7|], [|D...|], [1b-53,1/128]);
	// x * (0x3ff0000000000000 + x * (0xbfdffffffffffffc + x * (0x3fd5555555552dde + x * (0xbfcffffffefe562d + x * (0x3fc9999817d3a50f + x * (0xbfc554317b3f67a5 + x * 0x3fc1dc5c45e09c18))))))
	{
		{.l = 0 },
		{.l = 0x3ff0000000000000},
		{.l = 0xbfdffffffffffffc},
		{.l = 0x3fd5555555552dde},
		{.l = 0xbfcffffffefe562d},
		{.l = 0x3fc9999817d3a50f},
		{.l = 0xbfc554317b3f67a5},
		{.l = 0x3fc1dc5c45e09c18},
	}
};

// fpminmax: [0, 1/64]
static const DL
coefficient_1div_64[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3cafffffffffffbf}
	},
	
			
	// P = fpminimax(log(1 + x),[| 1 | ],[| D... | ],[1b - 53,1 / 64]); printexpansion(P);
	// x * 0x3fefe034e12c4aef
	{
		{.l = 0},
		{.l = 0x3fefe034e12c4aef}
	},
	// P = fpminimax(log(1 + x),[| 1,2 | ],[| D... | ],[1b - 53,1 / 64]); printexpansion(P);
	// x * (0x3fefffeaf465fea1 + x * 0xbfdfab92addcb37a)
	{
		{.l = 0},
		{.l = 0x3fefffeaf465fea1},
		{.l = 0xbfdfab92addcb37a}
	},
	// P = fpminimax(log(1 + x),[| 1,2,3 | ],[| D... | ],[1b - 53,1 / 64]); printexpansion(P);
	// x * (0x3feffffff05500cb + x * (0xbfdfff72b5405e9d + x * 0x3fd4f6b3aff4950f))
	{
		{.l = 0},
		{.l = 0x3feffffff05500cb},
		{.l = 0xbfdfff72b5405e9d},
		{.l = 0x3fd4f6b3aff4950f}
	},
	// P = fpminimax(log(1 + x),[| 1,2,3,4 | ],[| D... | ],[1b - 53,1 / 64]); printexpansion(P);
	// x * (0x3feffffffff38f34 + x * (0xbfdfffff38a4cf76 + x * (0x3fd5545b6bf73938 + x * 0xbfcf36df996eaaee)))
	{
		{.l = 0},
		{.l = 0x3feffffffff38f34},
		{.l = 0xbfdfffff38a4cf76},
		{.l = 0x3fd5545b6bf73938},
		{.l = 0xbfcf36df996eaaee}
	},
	// P = fpminimax(log(1 + x),[| 1,2,3,4,5 | ],[| D... | ],[1b - 53,1 / 64]); printexpansion(P);
	// x * (0x3feffffffffff5b6 + x * (0xbfdffffffefe7380 + x * (0x3fd55553512bad77 + x * (0xbfcffd2ac86ed02d + x / * /0x3fc8c8e1cf94fd7d))))
	{
		{.l = 0},
		{.l = 0x3feffffffffff5b6},
		{.l = 0xbfdffffffefe7380},
		{.l = 0x3fd55553512bad77},
		{.l = 0xbfcffd2ac86ed02d},
		{.l = 0x3fc8c8e1cf94fd7d}
	},
	// P = fpminimax(log(1 + x),[| 1,2,3,4,5,6 | ],[| D... | ],[1b - 53,1 / 64]); printexpansion(P);
	// x * (0x3feffffffffffff7 + x * (0xbfdffffffffec21c + x * (0x3fd5555551b86ce4 + x * (0xbfcffff8495e15a1 + x * // (0x3fc995de6aeeb477 + x * 0xbfc47f5fe348ff14)))))
	{
		{.l = 0 },
		{.l = 0x3feffffffffffff7 },
		{.l = 0xbfdffffffffec21c },
		{.l = 0x3fd5555551b86ce4 },
		{.l = 0xbfcffff8495e15a1 },
		{.l = 0x3fc995de6aeeb477 },
		{.l = 0xbfc47f5fe348ff14 },
	},
	// P = fpminimax(log(1 + x),[| 1,2,3,4,5,6,7 | ],[| D... | ],[1b - 53,1 / 64]); printexpansion(P);
	// x * (0x3ff0000000000000 + x * (0xbfdffffffffffef6 + x * (0x3fd555555550561f + x * (0xbfcfffffefe73664 + x * // (0x3fc9998da3c5c4d6 + x * (0xbfc550d3680b0081 + x * 0x3fc171ff31e1c8fe))))))
	{
		{.l = 0 },
		{.l = 0x3ff0000000000000 },
		{.l = 0xbfdffffffffffef6 },
		{.l = 0x3fd555555550561f },
		{.l = 0xbfcfffffefe73664 },
		{.l = 0x3fc9998da3c5c4d6 },
		{.l = 0xbfc550d3680b0081 },
		{.l = 0x3fc171ff31e1c8fe },
	}
};

// fpminmax: [0, 1/32]
static const DL
coefficient_1div_32[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3cafffffffffffdf}
	},
	// > P = fpminimax(log(1 + x),[| 1 | ],[| D... | ],[1b - 53,1 / 32]); printexpansion(P);
	// x * 0x3fefc0d1bd3af815
	{
		{.l = 0},
		{.l = 0x3fefc0d1bd3af815}
	},
	// > P = fpminimax(log(1 + x),[| 1,2 | ],[| D... | ],[1b - 53,1 / 32]); printexpansion(P);
	// x * (0x3fefffacf13a5185 + x * 0xbfdf58eb0405ef4c)
	{
		{.l = 0},
		{.l = 0x3fefffacf13a5185},
		{.l = 0xbfdf58eb0405ef4c}
	},
	// > P = fpminimax(log(1 + x),[| 1,2,3 | ],[| D... | ],[1b - 53,1 / 32]); printexpansion(P);
	// x * (0x3fefffff853a5bec + x * (0xbfdffdd556776870 + x * 0x3fd49abbe20bc9ef))
	{
		{.l = 0},
		{.l = 0x3fefffff853a5bec},
		{.l = 0xbfdffdd556776870},
		{.l = 0x3fd49abbe20bc9ef}
	},
	// > P = fpminimax(log(1 + x),[| 1,2,3,4 | ],[| D... | ],[1b - 53,1 / 32]); printexpansion(P);
	// x * (0x3fefffffff3e79bc + x * (0xbfdffff9ef0ee27c + x * (0x3fd551852d305e43 + x * 0xbfce74dc82554192)))
	{
		{.l = 0},
		{.l = 0x3fefffffff3e79bc},
		{.l = 0xbfdffff9ef0ee27c},
		{.l = 0x3fd551852d305e43},
		{.l = 0xbfce74dc82554192}
	},
		
	// > P = fpminimax(log(1 + x),[| 1,2,3,4,5 | ],[| D... | ],[1b - 53,1 / 32]); printexpansion(P);
	// x * (0x3feffffffffec249 + x * (0xbfdffffff0728d39 + x * (0x3fd55545b770fff4 + x * (0xbfcff4fd6e5f4f4b + x /  * /0x3fc8010f3572ce04))))
	{
		{.l = 0},
		{.l = 0x3feffffffffec249},
		{.l = 0xbfdffffff0728d39},
		{.l = 0x3fd55545b770fff4},
		{.l = 0xbfcff4fd6e5f4f4b},
		{.l = 0x3fc8010f3572ce04}
	},
	// > P = fpminimax(log(1 + x),[| 1,2,3,4,5,6 | ],[| D... | ],[1b - 53,1 / 32]); printexpansion(P);
	// x * (0x3feffffffffffde8 + x * (0xbfdfffffffda3553 + x * (0x3fd555551e0936b7 + x * (0xbfcfffc4b7b0fd94 + x * /		(0x3fc98b2fbd76a2e7 + x * 0xbfc3b44337129fd2)))))	
	{
		{.l = 0 },
		{.l = 0x3feffffffffffde8 },
		{.l = 0xbfdfffffffda3553 },
		{.l = 0x3fd555551e0936b7 },
		{.l = 0xbfcfffc4b7b0fd94 },
		{.l = 0x3fc98b2fbd76a2e7 },
		{.l = 0xbfc3b44337129fd2 },
	},
	// > P = fpminimax(log(1 + x),[| 1,2,3,4,5,6,7 | ],[| D... | ],[1b - 53,1 / 32]); printexpansion(P);
	// x * (0x3feffffffffffffc + x * (0xbfdfffffffffa4a2 + x * (0x3fd5555554a0d8c6 + x * (0xbfcffffef1feef16 + x * /		(0x3fc99938f50727b8 + x * (0xbfc5437b5b9ec820 + x * 0x3fc0a21611e66632))))))
	{
		{.l = 0 },
		{.l = 0x3feffffffffffffc },
		{.l = 0xbfdfffffffffa4a2 },
		{.l = 0x3fd5555554a0d8c6 },
		{.l = 0xbfcffffef1feef16 },
		{.l = 0x3fc99938f50727b8 },
		{.l = 0xbfc5437b5b9ec820 },
		{.l = 0x3fc0a21611e66632 },
	}
};

// fpminmax: [0, 1/16]
static const DL
coefficient_1div_16[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3cafffffffffffef},
	},

	// P = fpminimax(log(1+x), [|1|], [|D...|], [1b-53,1/16]);
	// x * 0x3fef833922b1a8ab
	{
		{.l = 0},
		{.l = 0x3fef833922b1a8ab },
	},

	// P = fpminimax(log(1+x), [|1,2|], [|D...|], [1b-53,1/16]);
	// x * (0x3feffebc6eb9549c + x * 0xbfdeb8b14dd7c2c2)
	{
		{.l = 0},
		{.l = 0x3feffebc6eb9549c },
		{.l = 0xbfdeb8b14dd7c2c2 },
	},

	// P = fpminimax(log(1+x), [|1,2,3|], [|D...|], [1b-53,1/16]);
	// x * (0x3feffffc510d8d1e + x * (0xbfdff7a5b276b735 + x * 0x3fd3ea5e7d65af6f))
	{
		{.l = 0},
		{.l = 0x3feffffc510d8d1e },
		{.l = 0xbfdff7a5b276b735 },
		{.l = 0x3fd3ea5e7d65af6f },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4|], [|D...|], [1b-53,1/16]);
	// x * (0x3feffffff48e56d3 + x * (0xbfdfffd1f21e6466 + x * (0x3fd546c6e62248ab + x * 0xbfcd04de34c177c2)))
	{
		{.l = 0},
		{.l = 0x3feffffff48e56d3 },
		{.l = 0xbfdfffd1f21e6466 },
		{.l = 0x3fd546c6e62248ab },
		{.l = 0xbfcd04de34c177c2 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5|], [|D...|], [1b-53,1/16]);
	// x * (0x3fefffffffdaf948 + x * (0xbfdfffff176dd7b2 + x * (0x3fd554e01798c219 + x * (0xbfcfd660f3c17fa6 + x * 0x3fc68a3714355af0))))
	{
		{.l = 0},
		{.l = 0x3fefffffffdaf948 },
		{.l = 0xbfdfffff176dd7b2 },
		{.l = 0x3fd554e01798c219 },
		{.l = 0xbfcfd660f3c17fa6 },
		{.l = 0x3fc68a3714355af0 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5,6|], [|D...|], [1b-53,1/16]);
	// x * (0x3fefffffffff84ce + x * (0xbfdffffffba682ce + x * (0x3fd55552243558e4 + x * (0xbfcffe47d8387eac + x * (0x3fc963a3fb819d2a + x * 0xbfc23b478efc825b)))))
	{
		{.l = 0},
		{.l = 0x3fefffffffff84ce },
		{.l = 0xbfdffffffba682ce },
		{.l = 0x3fd55552243558e4 },
		{.l = 0xbfcffe47d8387eac },
		{.l = 0x3fc963a3fb819d2a },
		{.l = 0xbfc23b478efc825b },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5,6,7|], [|D...|], [1b-53,1/16]);
	// x * (0x3feffffffffffe5e + x * (0xbfdfffffffebe8f6 + x * (0x3fd5555541228f3b + x * (0xbfcffff0bc2afadd + x * (0x3fc996d799cf9eb6 + x * (0xbfc51355ab854158 + x * 0x3fbe54a6235cc1fa))))))
	{
		{.l = 0},
		{.l = 0x3feffffffffffe5e },
		{.l = 0xbfdfffffffebe8f6 },
		{.l = 0x3fd5555541228f3b },
		{.l = 0xbfcffff0bc2afadd },
		{.l = 0x3fc996d799cf9eb6 },
		{.l = 0xbfc51355ab854158 },
		{.l = 0x3fbe54a6235cc1fa },
	},
};

// fpminmax: [0, 1/8]
static const DL
coefficient_1div_8[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3caffffffffffff7},
	},

	// P = fpminimax(log(1+x), [|1|], [|D...|], [1b-53,1/8]);
	// x * 0x3fef0c7c1880280d
	{
		{.l = 0},
		{.l = 0x3fef0c7c1880280d },
	},

	// P = fpminimax(log(1+x), [|1,2|], [|D...|], [1b-53,1/8]);
	// x * (0x3feffb32412073b2 + x * 0xbfdd8b191c5403bf)
	{
		{.l = 0},
		{.l = 0x3feffb32412073b2 },
		{.l = 0xbfdd8b191c5403bf },
	},

	// P = fpminimax(log(1+x), [|1,2,3|], [|D...|], [1b-53,1/8]);
	// x * (0x3fefffe4c49b62b9 + x * (0xbfdfe0e3ce17349c + x * 0x3fd2a547ab062403))
	{
		{.l = 0},
		{.l = 0x3fefffe4c49b62b9 },
		{.l = 0xbfdfe0e3ce17349c },
		{.l = 0x3fd2a547ab062403 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4|], [|D...|], [1b-53,1/8]);
	// x * (0x3fefffff5b7ea4c1 + x * (0xbfdffeb30df7b2fe + x * (0x3fd520219cee80d4 + x * 0xbfca6cd57fe891cc)))
	{
		{.l = 0},
		{.l = 0x3fefffff5b7ea4c1 },
		{.l = 0xbfdffeb30df7b2fe },
		{.l = 0x3fd520219cee80d4 },
		{.l = 0xbfca6cd57fe891cc },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5|], [|D...|], [1b-53,1/8]);
	// x * (0x3feffffffbf56d72 + x * (0xbfdffff33f2e186d + x * (0x3fd55217d797f93c + x * (0xbfcf6ab305e7ab18 + x * 0x3fc3f42a32cd6fc9))))
	{
		{.l = 0},
		{.l = 0x3feffffffbf56d72 },
		{.l = 0xbfdffff33f2e186d },
		{.l = 0x3fd55217d797f93c },
		{.l = 0xbfcf6ab305e7ab18 },
		{.l = 0x3fc3f42a32cd6fc9 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5,6|], [|D...|], [1b-53,1/8]);
	// x * (0x3fefffffffe5de14 + x * (0xbfdfffff896b218e + x * (0x3fd555298d5edd13 + x * (0xbfcff417c5a44d85 + x * (0x3fc8db9c9ba29e3a + x * 0xbfbf5f4541f27e47)))))
	{
		{.l = 0},
		{.l = 0x3fefffffffe5de14 },
		{.l = 0xbfdfffff896b218e },
		{.l = 0x3fd555298d5edd13 },
		{.l = 0xbfcff417c5a44d85 },
		{.l = 0x3fc8db9c9ba29e3a },
		{.l = 0xbfbf5f4541f27e47 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5,6,7|], [|D...|], [1b-53,1/8]);
	// x * (0x3fefffffffff538a + x * (0xbfdffffffbd7f25b + x * (0x3fd555533bc775ab + x * (0xbfcfff335e2c6c4a + x * (0x3fc986e7200f6804 + x * (0xbfc47137024abb37 + x * 0x3fb95b9f1194e98c))))))
	{
		{.l = 0},
		{.l = 0x3fefffffffff538a },
		{.l = 0xbfdffffffbd7f25b },
		{.l = 0x3fd555533bc775ab },
		{.l = 0xbfcfff335e2c6c4a },
		{.l = 0x3fc986e7200f6804 },
		{.l = 0xbfc47137024abb37 },
		{.l = 0x3fb95b9f1194e98c },
	},
};


// fpminmax: [0, 1/4]
static const DL
coefficient_1div_4[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3caffffffffffffb},
	},

	// P = fpminimax(log(1+x), [|1|], [|D...|], [1b-53,1/4]);
	// x * 0x3fee2f0203c7c9ad
	{
		{.l = 0},
		{.l = 0x3fee2f0203c7c9ad },
	},

	// P = fpminimax(log(1+x), [|1,2|], [|D...|], [1b-53,1/4]);
	// x * (0x3fefee8c032150e5 + x * 0xbfdb7141f1fdc47c)
	{
		{.l = 0},
		{.l = 0x3fefee8c032150e5 },
		{.l = 0xbfdb7141f1fdc47c },
	},

	// P = fpminimax(log(1+x), [|1,2,3|], [|D...|], [1b-53,1/4]);
	// x * (0x3fefff442b6396e3 + x * (0xbfdf932ae8999b44 + x * 0x3fd078140bf24956))
	{
		{.l = 0},
		{.l = 0x3fefff442b6396e3 },
		{.l = 0xbfdf932ae8999b44 },
		{.l = 0x3fd078140bf24956 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4|], [|D...|], [1b-53,1/4]);
	// x * (0x3feffff797eb79ad + x * (0xbfdff76600e1cb1a + x * (0x3fd4a19df30c3a2a + x * 0xbfc62780b13108c1)))
	{
		{.l = 0},
		{.l = 0x3feffff797eb79ad },
		{.l = 0xbfdff76600e1cb1a },
		{.l = 0x3fd4a19df30c3a2a },
		{.l = 0xbfc62780b13108c1 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5|], [|D...|], [1b-53,1/4]);
	// x * (0x3fefffff9bc73530 + x * (0xbfdfff606bcf1ce8 + x * (0x3fd540bf85717280 + x * (0xbfce18ba70d5383f + x * 0x3fbfba88394bd971))))
	{
		{.l = 0},
		{.l = 0x3fefffff9bc73530 },
		{.l = 0xbfdfff606bcf1ce8 },
		{.l = 0x3fd540bf85717280 },
		{.l = 0xbfce18ba70d5383f },
		{.l = 0x3fbfba88394bd971 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5,6|], [|D...|], [1b-53,1/4]);
	// x * (0x3feffffffb33e10d + x * (0xbfdffff507e71934 + x * (0x3fd55348b807b133 + x * (0xbfcfb74b5526dcd6 + x * (0x3fc7423f6aecbed1 + x * 0xbfb7a4254cc0b206)))))
	{
		{.l = 0},
		{.l = 0x3feffffffb33e10d },
		{.l = 0xbfdffff507e71934 },
		{.l = 0x3fd55348b807b133 },
		{.l = 0xbfcfb74b5526dcd6 },
		{.l = 0x3fc7423f6aecbed1 },
		{.l = 0xbfb7a4254cc0b206 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5,6,7|], [|D...|], [1b-53,1/4]);
	// x * (0x3fefffffffc40424 + x * (0xbfdfffff45b2cafa + x * (0x3fd55525cfb60417 + x * (0xbfcff6d2c2ef3dac + x * (0x3fc92bd4d0a196f7 + x * (0xbfc29d67a1dabd1d + x * 0x3fb21b26d6ff468f))))))
	{
		{.l = 0},
		{.l = 0x3fefffffffc40424 },
		{.l = 0xbfdfffff45b2cafa },
		{.l = 0x3fd55525cfb60417 },
		{.l = 0xbfcff6d2c2ef3dac },
		{.l = 0x3fc92bd4d0a196f7 },
		{.l = 0xbfc29d67a1dabd1d },
		{.l = 0x3fb21b26d6ff468f },
	},
};


// fpminmax: [0, 1/2]
static const DL
coefficient_1div_2[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3caffffffffffffd},
	},

	// P = fpminimax(log(1+x), [|1|], [|D...|], [1b-53,1/2]);
	// x * 0x3feca8b74ab36a40
	{
		{.l = 0},
		{.l = 0x3feca8b74ab36a40 },
	},

	// P = fpminimax(log(1+x), [|1,2|], [|D...|], [1b-53,1/2]);
	// x * (0x3fefc53bdbc48c41 + x * 0xbfd806febaff5f80)
	{
		{.l = 0},
		{.l = 0x3fefc53bdbc48c41 },
		{.l = 0xbfd806febaff5f80 },
	},

	// P = fpminimax(log(1+x), [|1,2,3|], [|D...|], [1b-53,1/2]);
	// x * (0x3feffb7fbd3f4c4a + x * (0xbfdea9651cf11bfe + x * 0x3fca5a3ddeb9c9e1))
	{
		{.l = 0},
		{.l = 0x3feffb7fbd3f4c4a },
		{.l = 0xbfdea9651cf11bfe },
		{.l = 0x3fca5a3ddeb9c9e1 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4|], [|D...|], [1b-53,1/2]);
	// x * (0x3fefffa231fa3a05 + x * (0xbfdfcf098eb3f383 + x * (0x3fd3410e0a3c833c + x * 0xbfc0253690f06363)))
	{
		{.l = 0},
		{.l = 0x3fefffa231fa3a05 },
		{.l = 0xbfdfcf098eb3f383 },
		{.l = 0x3fd3410e0a3c833c },
		{.l = 0xbfc0253690f06363 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5|], [|D...|], [1b-53,1/2]);
	// x * (0x3feffff80fc0ef4a + x * (0xbfdff99328bb71d6 + x * (0x3fd4e8285f823ee4 + x * (0xbfcaaf92f9f16741 + x * 0x3fb5075187ab2534))))
	{
		{.l = 0},
		{.l = 0x3feffff80fc0ef4a },
		{.l = 0xbfdff99328bb71d6 },
		{.l = 0x3fd4e8285f823ee4 },
		{.l = 0xbfcaaf92f9f16741 },
		{.l = 0x3fb5075187ab2534 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5,6|], [|D...|], [1b-53,1/2]);
	// x * (0x3fefffff4f548f0a + x * (0xbfdfff3328d8c7b4 + x * (0x3fd541c46bf83620 + x * (0xbfce98220336300a + x * (0x3fc36d739bc40485 + x * 0xbfac78e6eeed3f1d)))))
	{
		{.l = 0},
		{.l = 0x3fefffff4f548f0a },
		{.l = 0xbfdfff3328d8c7b4 },
		{.l = 0x3fd541c46bf83620 },
		{.l = 0xbfce98220336300a },
		{.l = 0x3fc36d739bc40485 },
		{.l = 0xbfac78e6eeed3f1d },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5,6,7|], [|D...|], [1b-53,1/2]);
	// x * (0x3feffffff055cbc3 + x * (0xbfdfffe75f539094 + x * (0x3fd552226ea04527 + x * (0xbfcfaebad52c4f63 + x * (0x3fc79dd9d0e9ce20 + x * (0xbfbd1b3f004d10ae + x * 0x3fa3cd0612bcfa39))))))
	{
		{.l = 0},
		{.l = 0x3feffffff055cbc3 },
		{.l = 0xbfdfffe75f539094 },
		{.l = 0x3fd552226ea04527 },
		{.l = 0xbfcfaebad52c4f63 },
		{.l = 0x3fc79dd9d0e9ce20 },
		{.l = 0xbfbd1b3f004d10ae },
		{.l = 0x3fa3cd0612bcfa39 },
	},
};


// fpminmax: [0, 1/1]
static const DL
coefficient_1div_1[COEFFICIENTS][COEFFICIENTS] = {
	{
		{.l = 0x3caffffffffffffe},
	},

	// P = fpminimax(log(1+x), [|1|], [|D...|], [1b-53,1/1]);
	// x * 0x3fea33587dad899e
	{
		{.l = 0},
		{.l = 0x3fea33587dad899e },
	},

	// P = fpminimax(log(1+x), [|1,2|], [|D...|], [1b-53,1/1]);
	// x * (0x3fef4f9058185381 + x * 0xbfd337325ce5c2d6)
	{
		{.l = 0},
		{.l = 0x3fef4f9058185381 },
		{.l = 0xbfd337325ce5c2d6 },
	},

	// P = fpminimax(log(1+x), [|1,2,3|], [|D...|], [1b-53,1/1]);
	// x * (0x3fefe8d9edb91e0a + x * (0xbfdc66e25a6f5563 + x * 0x3fc2239788eb02ed))
	{
		{.l = 0},
		{.l = 0x3fefe8d9edb91e0a },
		{.l = 0xbfdc66e25a6f5563 },
		{.l = 0x3fc2239788eb02ed },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4|], [|D...|], [1b-53,1/1]);
	// x * (0x3feffcc7ddd1dd04 + x * (0xbfdf2168bfcb01ed + x * (0x3fd041f215a17f99 + x * 0xbfb3082645894c27)))
	{
		{.l = 0},
		{.l = 0x3feffcc7ddd1dd04 },
		{.l = 0xbfdf2168bfcb01ed },
		{.l = 0x3fd041f215a17f99 },
		{.l = 0xbfb3082645894c27 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5|], [|D...|], [1b-53,1/1]);
	// x * (0x3fefff890b910a8a + x * (0xbfdfce7bff28e3d2 + x * (0x3fd39a6f36b0ceb5 + x * (0xbfc426932158cbab + x * 0x3fa52b79400211bd))))
	{
		{.l = 0},
		{.l = 0x3fefff890b910a8a },
		{.l = 0xbfdfce7bff28e3d2 },
		{.l = 0x3fd39a6f36b0ceb5 },
		{.l = 0xbfc426932158cbab },
		{.l = 0x3fa52b79400211bd },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5,6|], [|D...|], [1b-53,1/1]);
	// x * (0x3fefffee614cd66b + x * (0xbfdff5895e927a81 + x * (0x3fd4d02fadfe33cb + x * (0xbfcae2e01eda7194 + x * (0x3fb9e9b422be7da7 + x * 0xbf987127a7bbc937)))))
	{
		{.l = 0},
		{.l = 0x3fefffee614cd66b },
		{.l = 0xbfdff5895e927a81 },
		{.l = 0x3fd4d02fadfe33cb },
		{.l = 0xbfcae2e01eda7194 },
		{.l = 0x3fb9e9b422be7da7 },
		{.l = 0xbf987127a7bbc937 },
	},

	// P = fpminimax(log(1+x), [|1,2,3,4,5,6,7|], [|D...|], [1b-53,1/1]);
	// x * (0x3feffffd56ff8cb9 + x * (0xbfdffddd4ca20aed + x * (0x3fd530a6f1903cea + x * (0xbfce15beb7d6ece1 + x * (0x3fc319c499d2296a + x * (0xbfb0ffb55b7f7c00 + x * 0x3f8cf5f81f618fd2))))))
	{
		{.l = 0},
		{.l = 0x3feffffd56ff8cb9 },
		{.l = 0xbfdffddd4ca20aed },
		{.l = 0x3fd530a6f1903cea },
		{.l = 0xbfce15beb7d6ece1 },
		{.l = 0x3fc319c499d2296a },
		{.l = 0xbfb0ffb55b7f7c00 },
		{.l = 0x3f8cf5f81f618fd2 },
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
	int i, num, bit, bitnum, bitnum_1, degree, fnum, format;
	num = 100;

	if ((func = fopen("atanh_gen.c", "w")) == (FILE *)0) {
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
		fprintf(func, "#include %cmath.h%c\n", '<', '>');
		fprintf(func, "#include %cmyhead.h%c\n", '"', '"');
		fprintf(func, "\n");
		fprintf(func, "#define BIT %d\n", bit);
		fprintf(func, "#define BITNUM %d\n", bitnum);
		fprintf(func, "#define DEGREE %d\n", degree + 1); // 常数项被省略
		fprintf(func, "\n");
		fprintf(func, "static const long long	twop%d =\n", bit);
		fprintf(func, "{ 0x%03x0000000000000ll }; // %d\n", (0x3ff + bit), bitnum);
		fprintf(func, "\n");
		fprintf(func, "static const double twopm%d = %-.10e;\n", bit, 1.0/bitnum);
		fprintf(func, "\n");
		fprintf(func, "static const DL	log2_lead =\n");
		fprintf(func, "{ .l = 0x3fe62e42fefa4000 };\n");
		fprintf(func, "\n");
		fprintf(func, "static const DL	log2_trail =\n");
		fprintf(func, "{ .l = 0xbd48432a1b0e2634 };\n");
		fprintf(func, "\n");
		// degree
		if (degree > 0)	{
			//fprintf(func, "// P = fpminimax(log(1+x), [|1,2,3,4,5,6,7|], [|D...|], [1b-53,1/128]);\n");
			//fprintf(func, "// x * (0x3ff0000000000000 + x * (0xbfdffffffffffffc + x * (0x3fd5555555552dde + x * (0xbfcffffffefe562d + x * (0x3fc9999817d3a50f + x * (0xbfc554317b3f67a5 + x * 0x3fc1dc5c45e09c18))))))\n");
			fprintf(func, "static const DL\n");
			fprintf(func, "C[DEGREE - 1] = {\n");
			for (i = 1; i <= degree; i++) {
				fprintf(func, "\t{.l = 0x%lx},\n", link[bit][degree * COEFFICIENTS + i].l);
				//fprintf(func, "\t{.l = 0x%lx},\n", coefficient[degree][i].l);
			}
			fprintf(func, "};\n");
		}
		else {
			fprintf(func, "static const DL\n");
			fprintf(func, "C = {\n");
				fprintf(func, "\t.l = 0x%lx\n", link[bit][0].l);
			fprintf(func, "};\n");
		}
		// loghi
		fprintf(func, "static const DL\n");
		fprintf(func, "logtabhi[BITNUM] = {\n");
		for (i = 0; i < bitnum_1; i++) {
			fprintf(func, "\t{.l = 0x%016lx},\n", logtabhi[i * (1 << (BIT - bit))].l);
		}
		fprintf(func, "\t{.l = 0x%016lx}\n", logtabhi[i * (1 << (BIT - bit))].l);
		fprintf(func, "};\n\n");
		// loglo
		fprintf(func, "static const DL\n");
		fprintf(func, "logtablo[BITNUM] = {\n");
		for (i = 0; i < bitnum_1; i++) {
			fprintf(func, "\t{.l = 0x%016lx},\n", logtablo[i * (1 << (BIT - bit))].l);
		}
		fprintf(func, "\t{.l = 0x%016lx}\n", logtablo[i * (1 << (BIT - bit))].l);
		fprintf(func, "};\n\n");
		// log_ru
		fprintf(func, "static const DL\n");
		fprintf(func, "log_ru[BITNUM] = {\n");
		for (i = 0; i < bitnum_1; i++) {
			fprintf(func, "\t{.l = 0x%016lx},\n", log_ru[i * (1 << (BIT - bit))].l);
		}
		fprintf(func, "\t{.l = 0x%016lx}\n", log_ru[i * (1 << (BIT - bit))].l);
		fprintf(func, "};\n\n");
	}
	{
		fprintf(func, "double atanh_gen(double x) {\n");
		fprintf(func, "	_UL ix;\n");
		fprintf(func, "	int	m, k;\n");
		fprintf(func, "	double	w, u, t, xmu, q, l_lead, l_trail, result;\n");
		fprintf(func, "\n");
		fprintf(func, "	/* extract exponent and sign of x for some quick screening */\n");
		fprintf(func, "\n");
		fprintf(func, "\tx = (1 + x) / (1 - x);\n");
		fprintf(func, "	ix = *(unsigned long long *)(&x);	/* copy arg to a long long */\n");
		fprintf(func, "\n");
		fprintf(func, "	m = (ix >> DMANTWIDTH);		/* shift off mantissa	*/\n");
		fprintf(func, "\n");
		fprintf(func, "	m -= DEXPBIAS;\n");
		fprintf(func, "\n");
		fprintf(func, "	/* normalize x and compute the nearest 1/%dth to x */\n", bitnum);
		fprintf(func, "\n");
		fprintf(func, "	ix &= DMANTISSA;	/* get the mantissa of x */\n");
		fprintf(func, "	ix |= twop%d;	/* set exponent of x to %d */\n", bit, bit);
		fprintf(func, "\n");
		fprintf(func, "	w = *(double *)(&ix);\n");
		fprintf(func, "	k = w;\n");
		fprintf(func, "	u = k;\n");
		fprintf(func, "\n");
		fprintf(func, "	k -= BITNUM; // the number %d here is for zhengshu bit %c1%c\n", bitnum, '"', '"');
		fprintf(func, "\n");
		fprintf(func, "	xmu = twopm%d*(w - u);\n", bit);
		fprintf(func, "\n");
		fprintf(func, "	t = log_ru[k].d*xmu;\n");
		fprintf(func, "\n");
		fprintf(func, "	/* avoid loss of significance for values of x near two\n");
		fprintf(func, "		by adjusting index; effectively u is divided by two.\n");
		fprintf(func, "		The logtable has been adjusted for this.\n");
		fprintf(func, "	*/\n");
		fprintf(func, "\n");
		fprintf(func, "	if (k > %d)\n", bitnum/2);
		fprintf(func, "		m++;\n");
		fprintf(func, "\n");

		//fprintf(func, "	q = (((((C[6].d*t + C[5].d)*t + C[4].d)*t + C[3].d)*t + C[2].d)*t + C[1].d)*(t*t);\n");
		if (degree > 1) {
			fprintf(func, "\tq = (t * t) * (C[1].d");
			for (i = 2; i < degree ; i++) {
				fprintf(func, " + t * (C[%d].d", i);
			}
			for (i = 1; i < degree ; i++) {
				fprintf(func, ")");
			}
			fprintf(func, ";\n");
			fprintf(func, "\n");
			fprintf(func, "	l_lead = logtabhi[k].d;\n");
			fprintf(func, "	l_trail = logtablo[k].d;\n");
			fprintf(func, "	l_lead += m * log2_lead.d;\n");
			fprintf(func, "	l_trail += m * log2_trail.d;\n");
			fprintf(func, "\n");
			fprintf(func, "	result = l_lead + (C[0].d * t + (q + l_trail));\n");
		}
		else if (degree == 1) {
			fprintf(func, "	l_lead = logtabhi[k].d;\n");
			fprintf(func, "	l_trail = logtablo[k].d;\n");
			fprintf(func, "	l_lead += m * log2_lead.d;\n");
			fprintf(func, "	l_trail += m * log2_trail.d;\n");
			fprintf(func, "\n");
			fprintf(func, "	result = l_lead + (C[0].d * t + l_trail);\n");
		}
		else {
			fprintf(func, "	l_lead = logtabhi[k].d;\n");
			fprintf(func, "	l_trail = logtablo[k].d;\n");
			fprintf(func, "	l_lead += m * log2_lead.d;\n");
			fprintf(func, "	l_trail += m * log2_trail.d;\n");
			fprintf(func, "\n");
			fprintf(func, "	result = l_lead + (l_trail + C.d);\n");
			fprintf(func, "if (x == 1) {\n");
			fprintf(func, "\tresult = 0;\n");
			fprintf(func, "}\n");
		}

		fprintf(func, "\n");
		fprintf(func, "	return 0.5 * result;\n");
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

	link[7] = &(coefficient_1div_128[0][0]);
	link[6] = &(coefficient_1div_64[0][0]);
	link[5] = &(coefficient_1div_32[0][0]);
	link[4] = &(coefficient_1div_16[0][0]);
	link[3] = &(coefficient_1div_8[0][0]);
	link[2] = &(coefficient_1div_4[0][0]);
	link[1] = &(coefficient_1div_2[0][0]);
	link[0] = &(coefficient_1div_1[0][0]);

	input_parameter.start = a;
	input_parameter.end = b;
	input_parameter.precision = precision;
	input_parameter.bit = bit;
	input_parameter.fnum = fnum;
	input_parameter.degree = degree;
	gen(input_parameter);

	return 0;
}