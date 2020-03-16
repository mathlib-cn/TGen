/** target func:	sin
*** target domain:	[0.000000, 1.000000]
*** target precision:	23
**/

#include <stdio.h>
#include "myhead.h"

#define NUM 100
#define BIT 5
#define BITNUM 32
#define BITNUM_1 31
#define DEGREE 6

static const DL
coefficient[2][DEGREE] = {
	// P = fpminimax(sin(x), [|1,3,5,7,9|], [|D...|], [1b-53,1/32]); printexpansion(P);
	// x * (0x3ff0000000000000 + x^2 * (0xbfc5555555555555 + x^2 * (0x3f8111111110a356 + x^2 * (0xbf2a019f6f43317c + x^2 * 0x3ec6cb0757c5b38c))))

	{
		{.l = 0x0},
		{.l = 0x3ff0000000000000},
		{.l = 0xbfc5555555555555},
		{.l = 0x3f8111111110a356},
		{.l = 0xbf2a019f6f43317c},
		{.l = 0x3ec6cb0757c5b38c}
	},
	// P = fpminimax(cos(x), [|2,4,6,8,10|], [|D...|], [1b-53,1/32],1); printexpansion(P);
	// 0x3ff0000000000000 + x^2 * (0xbfe0000000000000 + x^2 * (0x3fa5555555555555 + x^2 * (0xbf56c16c16c0b21a + x^2 * (0x3efa019f18a85975 + x^2 * 0xbe920bf3d573e5aa))))

	{
		{.l = 0x3ff0000000000000},
		{.l = 0xbfe0000000000000},
		{.l = 0x3fa5555555555555},
		{.l = 0xbf56c16c16c0b21a},
		{.l = 0x3efa019f18a85975},
		{.l = 0xbe920bf3d573e5aa}
	}
};
static const double
interpolate[2][BITNUM] = {
	/*
	{
		0.00000000000000000e+00,
		4.90676743274180149e-02,
		9.80171403295606036e-02,
		1.46730474455361748e-01,
		1.95090322016128276e-01,
		2.42980179903263899e-01,
		2.90284677254462387e-01,
		3.36889853392220051e-01,
		3.82683432365089782e-01,
		4.27555093430282085e-01,
		4.71396736825997642e-01,
		5.14102744193221772e-01,
		5.55570233019602178e-01,
		5.95699304492433357e-01,
		6.34393284163645488e-01,
		6.71558954847018441e-01,
		7.07106781186547573e-01,
		7.40951125354959106e-01,
		7.73010453362736993e-01,
		8.03207531480644943e-01,
		8.31469612302545236e-01,
		8.57728610000272118e-01,
		8.81921264348355050e-01,
		9.03989293123443338e-01,
		9.23879532511286738e-01,
		9.41544065183020806e-01,
		9.56940335732208824e-01,
		9.70031253194543974e-01,
		9.80785280403230431e-01,
		9.89176509964781014e-01,
		9.95184726672196929e-01,
		9.98795456205172405e-01
	},
	{
		1.00000000000000000e+00,
		9.98795456205172405e-01,
		9.95184726672196929e-01,
		9.89176509964781014e-01,
		9.80785280403230431e-01,
		9.70031253194543974e-01,
		9.56940335732208824e-01,
		9.41544065183020806e-01,
		9.23879532511286738e-01,
		9.03989293123443338e-01,
		8.81921264348355050e-01,
		8.57728610000272118e-01,
		8.31469612302545236e-01,
		8.03207531480644943e-01,
		7.73010453362736993e-01,
		7.40951125354959106e-01,
		7.07106781186547573e-01,
		6.71558954847018441e-01,
		6.34393284163645488e-01,
		5.95699304492433357e-01,
		5.55570233019602178e-01,
		5.14102744193221772e-01,
		4.71396736825997642e-01,
		4.27555093430282085e-01,
		3.82683432365089782e-01,
		3.36889853392220051e-01,
		2.90284677254462387e-01,
		2.42980179903263899e-01,
		1.95090322016128276e-01,
		1.46730474455361748e-01,
		9.80171403295606036e-02,
		4.90676743274180149e-02
	}
	*/
	{
		8.4147098480789650488e-01,
		8.5794184281248342394e-01,
		8.7357493516707107606e-01,
		8.8835499642227300221e-01,
		9.0226759409909518261e-01,
		9.1529914278200663613e-01,
		9.2743691738486766507e-01,
		9.3866906557675977574e-01,
		9.4898461935558620084e-01,
		9.5837350575813973208e-01,
		9.6682655669618022820e-01,
		9.7433551790891725908e-01,
		9.8089305702315565672e-01,
		9.8649277071323371313e-01,
		9.9112919095376161849e-01,
		9.9479779035905591122e-01,
		9.9749498660405444550e-01,
		9.9921814592239599495e-01,
		9.9996558567824889163e-01,
		9.9973657600937559931e-01,
		9.9853134053983161333e-01,
		9.9635105616159957975e-01,
		9.9319785188537490495e-01,
		9.8907480676162262689e-01,
		9.8398594687393692304e-01,
		9.7793624140763857344e-01,
		9.7093159779745052962e-01,
		9.6297885595898724542e-01,
		9.5408578160969381710e-01,
		9.4426105868575449520e-01,
		9.3351428086237620008e-01,
		9.2185594218572775027e-01,		
	},
	{
		5.4030230586813976501e-01,
		5.1374681931036803029e-01,
		4.8668966770196331328e-01,
		4.5915727189230409744e-01,
		4.3117651679866619840e-01,
		4.0277472515355744465e-01,
		3.7397963082453317130e-01,
		3.4481935173254513183e-01,
		3.1532236239526867383e-01,
		2.8551746612221973187e-01,
		2.5543376688881169745e-01,
		2.2510064091681744602e-01,
		1.9454770798898718409e-01,
		1.6380480252583334844e-01,
		1.3290194445282521585e-01,
		1.0186930988644112006e-01,
		7.0737201667702906405e-02,
		3.9536019771965788538e-02,
		8.2962316238583775491e-03,
		-2.2951657653640416407e-02,
		-5.4177135026936322493e-02,
		-8.5349709347279173466e-02,
		-1.1643894112485225767e-01,
		-1.4741447225241752217e-01,
		-1.7824605564949208558e-01,
		-2.0890358479810910985e-01,
		-2.3935712314132159828e-01,
		-2.6957693331574222873e-01,
		-2.9953350618957413909e-01,
		-3.2919758967777718039e-01,
		-3.5854021730623281128e-01,
		-3.8753273649701414083e-01,
	}
};

static const double start = 1;
static const double end = 2;
static const double step = 1.0 / 32.0;
static const double invstep = 32.0;

double sin_gen(double x) {
	double ix, iix, iiix, y, appro_s, appro_c, z;
	long temp, flag, sign, temp1, temp2, status_pi_2, status_pi_1, table_order;
	int i1;
	
	/*
	ix = x - start;
	iix = ix * step;
	table_order = (long)iix;
	iiix = (iix - table_order) * step;
	*/
	ix = x - start;
	iix = ix * invstep;
	table_order = (long)iix;
	iiix = ix - table_order * step;
	
	// if ix is too little, then return ix;
	//if (temp < 0x3e40000000000000) {
	//	return (1 - flag * 2) * ix;
	//}

	// new, sin(x+x') = sin(x)cos(x') + sin(x')cos(x), x' = iiix
	// OR, cos(x+x') = cos(x)cos(x') - sin(x)sin(x'), x' = iiix
	z = iiix * iiix;
	appro_s = 0.0 + iiix * (coefficient[0][1].d + z * (coefficient[0][2].d + z * (coefficient[0][3].d + z * (coefficient[0][4].d + z * (coefficient[0][5].d)))));
	appro_c = 1.0 + z * (coefficient[1][1].d + z * (coefficient[1][2].d + z * (coefficient[1][3].d + z * (coefficient[1][4].d + z * (coefficient[1][5].d)))));

	y = interpolate[0][table_order] * appro_c + interpolate[1][table_order] * appro_s;
	//y += interpolate[0][table_order] * coefficient[1][0].d;

	return y;
}

