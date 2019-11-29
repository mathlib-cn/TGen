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

const DL _logtabhi[] =
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

const DL _logtablo[] =
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

const DL _log_ru[] =
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

// fpminmax: [0, 1/64]
static const DL
coefficient_1div_64[COEFFICIENTS][COEFFICIENTS] = {
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

// fpminmax: [0, 1/32]
static const DL
coefficient_1div_32[COEFFICIENTS][COEFFICIENTS] = {
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

// fpminmax: [0, 1/16]
static const DL
coefficient_1div_16[COEFFICIENTS][COEFFICIENTS] = {
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

// fpminmax: [0, 1/8]
static const DL
coefficient_1div_8[COEFFICIENTS][COEFFICIENTS] = {
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

// fpminmax: [0, 1/4]
static const DL
coefficient_1div_4[COEFFICIENTS][COEFFICIENTS] = {
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

// fpminmax: [0, 1/2]
static const DL
coefficient_1div_2[COEFFICIENTS][COEFFICIENTS] = {
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

// fpminmax: [0, 1/1]
static const DL
coefficient_1div_1[COEFFICIENTS][COEFFICIENTS] = {
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

int gen(struct constraint input_parameter) {
	double a, b;
	int precision;
	double X;
	FILE *func;
	// init
	int i, num, bit, bitnum, bitnum_1, degree, fnum, format;
	num = 100;

	if ((func = fopen("log_gen.c", "w")) == (FILE *)0) {
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
	degree = 7;
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
		fprintf(func, "#include %clibm.h%c\n", '"', '"');
		fprintf(func, "\n");
		fprintf(func, "extern const du _logtabhi[];\n");
		fprintf(func, "extern const du _logtablo[];\n");
		fprintf(func, "extern const du _log_ru[];\n");
		fprintf(func, "\n");
		fprintf(func, "static const long long twop7 =\n");
		fprintf(func, "{ 0x4060000000000000ll }; // 128\n");
		fprintf(func, "\n");
		fprintf(func, "static const du twopm7 =\n");
		fprintf(func, "{ D(0x3f800000, 0x00000000) }; // 1/128\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	log2_lead =\n");
		fprintf(func, "{ D(0x3fe62e42, 0xfefa4000) };\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	log2_trail =\n");
		fprintf(func, "{ D(0xbd48432a, 0x1b0e2634) };\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	Scaleup =\n");
		fprintf(func, "{ D(0x43300000, 0x00000000) };\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	Qnan =\n");
		fprintf(func, "{ D(QNANHI, QNANLO) };\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	Inf =\n");
		fprintf(func, "{ D(0x7ff00000, 0x00000000) };\n");
		fprintf(func, "\n");
		fprintf(func, "static const du	Neginf =\n");
		fprintf(func, "{ D(0xfff00000, 0x00000000) };\n");
		fprintf(func, "\n");
		fprintf(func, "/* coefficients for polynomial approximation of log(1 + t) on +/- 1/256   */\n");
		fprintf(func, "\n");
		fprintf(func, "// x * (0x3ff0000000000000 + x * (0xbfdffffffffffffc + x * (0x3fd5555555552dde + x * (0xbfcffffffefe562c + x * (0x3fc9999817d3a30f + x * (0xbfc554317b3e1c57 + x * 0x3fc1dc5c459adbd8))))))\n");
		fprintf(func, "static const du	P[] =\n");
		fprintf(func, "{\n");
		fprintf(func, "{D(0x3ff00000, 0x00000000)},\n");
		fprintf(func, "{D(0xbfdfffff, 0xfffffffc)},\n");
		fprintf(func, "{D(0x3fd55555, 0x55552dde)},\n");
		fprintf(func, "{D(0xbfcfffff, 0xfefe562c)},\n");
		fprintf(func, "{D(0x3fc99998, 0x17d3a30f)},\n");
		fprintf(func, "{D(0xbfc55431, 0x7b3e1c57)},\n");
		fprintf(func, "{D(0x3fc1dc5c, 0x459adbd8)}\n");
		fprintf(func, "};\n");
		fprintf(func, "\n");
		fprintf(func, "#define MAXEXP	0x7ffu\n");
		fprintf(func, "\n");
		fprintf(func, "#define	MINEXP	0x001u\n");
		fprintf(func, "\n");	
	}
	{
		fprintf(func, "double log_gen(double x) {\n");
		fprintf(func, "	unsigned long long ix;\n");
		fprintf(func, "	int	j;\n");
		fprintf(func, "	int	m;\n");
		fprintf(func, "	int	k;\n");
		fprintf(func, "	double	u;\n");
		fprintf(func, "	double	t;\n");
		fprintf(func, "	double	xmu;\n");
		fprintf(func, "	double	q;\n");
		fprintf(func, "	double	l_lead, l_trail;\n");
		fprintf(func, "	double	w;\n");
		fprintf(func, "	double	result;\n");
		fprintf(func, "\n");
		fprintf(func, "	/* extract exponent and sign of x for some quick screening */\n");
		fprintf(func, "\n");
		fprintf(func, "	ix = *(unsigned long long *)(&x);	/* copy arg to a long long */\n");
		fprintf(func, "\n");
		fprintf(func, "	m = (ix >> DMANTWIDTH);		/* shift off mantissa	*/\n");
		fprintf(func, "	j = m - MINEXP;\n");
		fprintf(func, "\n");
		fprintf(func, "	m -= DEXPBIAS;\n");
		fprintf(func, "\n");
		fprintf(func, "	/* normalize x and compute the nearest 1/128th to x */\n");
		fprintf(func, "\n");
		fprintf(func, "	ix &= (DSIGNMASK & DEXPMASK);	/* mask off sign and exponent\n");
		fprintf(func, "						* bits of x\n");
		fprintf(func, "						*/\n");
		fprintf(func, "	ix |= twop7;	/* set exponent of x to 0x406 */\n");
		fprintf(func, "\n");
		fprintf(func, "	/* adjust scaled arg	*/\n");
		fprintf(func, "\n");
		fprintf(func, "	//LL2DBL(ix, w);\n");
		fprintf(func, "	w = *(double *)(&ix);\n");
		fprintf(func, "\n");
		fprintf(func, "	//k = ROUND(w);\n");
		fprintf(func, "	k = w + 0.5; // !!! +0.5 !!!\n");
		fprintf(func, "\n");
		fprintf(func, "	u = k;\n");
		fprintf(func, "\n");
		fprintf(func, "	k -= 128;\n");
		fprintf(func, "\n");
		fprintf(func, "	xmu = twopm7.d*(w - u);\n");
		fprintf(func, "\n");
		fprintf(func, "	t = _log_ru[k].d*xmu;\n");
		fprintf(func, "\n");
		fprintf(func, "	/* avoid loss of significance for values of x near two\n");
		fprintf(func, "		by adjusting index; effectively u is divided by two.\n");
		fprintf(func, "		The logtable has been adjusted for this.\n");
		fprintf(func, "	*/\n");
		fprintf(func, "\n");
		fprintf(func, "	if (k > 64) {\n");
		fprintf(func, "		m++;\n");
		fprintf(func, "	}\n");
		fprintf(func, "\n");
		fprintf(func, "	q = (((((P[6].d*t + P[5].d)*t + P[4].d)*t + P[3].d)*t + P[2].d)*t + P[1].d)*(t*t);\n");
		fprintf(func, "	\n");
		fprintf(func, "	l_lead = _logtabhi[k].d;\n");
		fprintf(func, "	l_trail = _logtablo[k].d;\n");
		fprintf(func, "\n");
		fprintf(func, "	l_lead += m * log2_lead.d;\n");
		fprintf(func, "	l_trail += m * log2_trail.d;\n");
		fprintf(func, "\n");
		fprintf(func, "	result = l_lead + (t + (q + l_trail));\n");
		fprintf(func, "	\n");
		fprintf(func, "	/* take care of negative args, NaNs, and Infinities     */\n");
		fprintf(func, "\n");
		fprintf(func, "	if (j >= (MAXEXP - MINEXP)) {\n");
		fprintf(func, "		result = Qnan.d;\n");
		fprintf(func, "	}\n");
		fprintf(func, "\n");
		fprintf(func, "	if (x == 0.0){\n");
		fprintf(func, "		result = Neginf.d;\n");
		fprintf(func, "	}\n");
		fprintf(func, "\n");
		fprintf(func, "	if (x == Inf.d) {\n");
		fprintf(func, "		result = Inf.d;\n");
		fprintf(func, "	}\n");
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