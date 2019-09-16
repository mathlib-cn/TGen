#include "libm.h"

/* coefficients for polynomial approximation of asin on +/- sqrt(1 - (63/64)**2) */

static const du	P[] =
{
{D(0x3ff00000, 0x00000000)},
{D(0x3fc55555, 0x55555578)},
{D(0x3fb33333, 0x333256c4)},
{D(0x3fa6db6d, 0xb8b92be5)},
{D(0x3f9f1c6f, 0xdcc39d32)},
{D(0x3f96e940, 0x7e05b546)},
{D(0x3f91b0c9, 0xc6fc9434)},
{D(0x3f8fb249, 0x579227f4)},
};

/* table of angles asin(j/32), j=-32, -31, ..., 31, 32	*/

/* Angles are chosen so that xk[j] is exact and the sum
   of the ulps error in sin(xk[j]) + cos(xk[j]) is
   minimized over the nearest 2000 angles; consequently
   most of the tabulated values are nearly exact, thus
   helping to minimize roundoff error in the computations.
*/

static const	du	xk[] =
{
{D(0xbff921fb, 0x54442d18)},
{D(0xbff51f4b, 0xd13f862e)},
{D(0xbff3722d, 0x2feb25a8)},
{D(0xbff22634, 0x618206fc)},
{D(0xbff10c06, 0x6d3e6b91)},
{D(0xbff01155, 0x91d29988)},
{D(0xbfee5985, 0x567b66f4)},
{D(0xbfecb17c, 0x849c6f02)},
{D(0xbfeb2353, 0x15c68019)},
{D(0xbfe9aa01, 0xbabefab1)},
{D(0xbfe841de, 0xb5114857)},
{D(0xbfe6e825, 0x383cc18f)},
{D(0xbfe59aad, 0x71ced0ea)},
{D(0xbfe457bf, 0x318fe898)},
{D(0xbfe31df4, 0x0fbd352f)},
{D(0xbfe1ec23, 0x0c714e6a)},
{D(0xbfe0c152, 0x382d758a)},
{D(0xbfdf3958, 0xaecddd8d)},
{D(0xbfdcfaf2, 0x7460fe79)},
{D(0xbfdac62f, 0xec0b26e2)},
{D(0xbfd899f4, 0xedc95fb8)},
{D(0xbfd67544, 0x13299ae7)},
{D(0xbfd45739, 0x3b90e4bd)},
{D(0xbfd23f05, 0x23c5dae8)},
{D(0xbfd02be9, 0xce0b8573)},
{D(0xbfcc3a6f, 0x13aae951)},
{D(0xbfc82494, 0xed0e7a60)},
{D(0xbfc41510, 0xcb011615)},
{D(0xbfc00abe, 0x0c12a191)},
{D(0xbfb80909, 0x2913e89d)},
{D(0xbfb002ab, 0xde9537c5)},
{D(0xbfa000aa, 0xbde0b958)},
{D(0x00000000, 0x00000000)},
{D(0x3fa000aa, 0xbde0b958)},
{D(0x3fb002ab, 0xde9537c5)},
{D(0x3fb80909, 0x2913e89d)},
{D(0x3fc00abe, 0x0c12a191)},
{D(0x3fc41510, 0xcb011615)},
{D(0x3fc82494, 0xed0e7a60)},
{D(0x3fcc3a6f, 0x13aae951)},
{D(0x3fd02be9, 0xce0b8573)},
{D(0x3fd23f05, 0x23c5dae8)},
{D(0x3fd45739, 0x3b90e4bd)},
{D(0x3fd67544, 0x13299ae7)},
{D(0x3fd899f4, 0xedc95fb8)},
{D(0x3fdac62f, 0xec0b26e2)},
{D(0x3fdcfaf2, 0x7460fe79)},
{D(0x3fdf3958, 0xaecddd8d)},
{D(0x3fe0c152, 0x382d758a)},
{D(0x3fe1ec23, 0x0c714e6a)},
{D(0x3fe31df4, 0x0fbd352f)},
{D(0x3fe457bf, 0x318fe898)},
{D(0x3fe59aad, 0x71ced0ea)},
{D(0x3fe6e825, 0x383cc18f)},
{D(0x3fe841de, 0xb5114857)},
{D(0x3fe9aa01, 0xbabefab1)},
{D(0x3feb2353, 0x15c68019)},
{D(0x3fecb17c, 0x849c6f02)},
{D(0x3fee5985, 0x567b66f4)},
{D(0x3ff01155, 0x91d29988)},
{D(0x3ff10c06, 0x6d3e6b91)},
{D(0x3ff22634, 0x618206fc)},
{D(0x3ff3722d, 0x2feb25a8)},
{D(0x3ff51f4b, 0xd13f862e)},
{D(0x3ff921fb, 0x54442d18)},
};

/* sines of the above angles */

static const	du	sk[] =
{
{D(0xbff00000, 0x00000000)},
{D(0xbfef0000, 0x0000004e)},
{D(0xbfee0000, 0x0000009c)},
{D(0xbfecffff, 0xfffffcbd)},
{D(0xbfec0000, 0x0000024c)},
{D(0xbfeaffff, 0xfffffc33)},
{D(0xbfea0000, 0x00000058)},
{D(0xbfe8ffff, 0xfffffdcd)},
{D(0xbfe7ffff, 0xffffff7f)},
{D(0xbfe70000, 0x00000250)},
{D(0xbfe5ffff, 0xfffffd8f)},
{D(0xbfe4ffff, 0xfffffe20)},
{D(0xbfe40000, 0x000000ab)},
{D(0xbfe30000, 0x000002d2)},
{D(0xbfe20000, 0x000002cc)},
{D(0xbfe10000, 0x0000033e)},
{D(0xbfe00000, 0x000001db)},
{D(0xbfddffff, 0xfffffec3)},
{D(0xbfdbffff, 0xffffffde)},
{D(0xbfd9ffff, 0xfffffca1)},
{D(0xbfd7ffff, 0xfffffd1f)},
{D(0xbfd60000, 0x00000252)},
{D(0xbfd40000, 0x000001f8)},
{D(0xbfd1ffff, 0xfffffeca)},
{D(0xbfcfffff, 0xfffffb72)},
{D(0xbfcc0000, 0x00000100)},
{D(0xbfc80000, 0x0000015e)},
{D(0xbfc40000, 0x000001ec)},
{D(0xbfc00000, 0x0000036c)},
{D(0xbfb80000, 0x0000036b)},
{D(0xbfb00000, 0x000001ab)},
{D(0xbf9fffff, 0xffffff20)},
{D(0x00000000, 0x00000000)},
{D(0x3f9fffff, 0xffffff20)},
{D(0x3fb00000, 0x000001ab)},
{D(0x3fb80000, 0x0000036b)},
{D(0x3fc00000, 0x0000036c)},
{D(0x3fc40000, 0x000001ec)},
{D(0x3fc80000, 0x0000015e)},
{D(0x3fcc0000, 0x00000100)},
{D(0x3fcfffff, 0xfffffb72)},
{D(0x3fd1ffff, 0xfffffeca)},
{D(0x3fd40000, 0x000001f8)},
{D(0x3fd60000, 0x00000252)},
{D(0x3fd7ffff, 0xfffffd1f)},
{D(0x3fd9ffff, 0xfffffca1)},
{D(0x3fdbffff, 0xffffffde)},
{D(0x3fddffff, 0xfffffec3)},
{D(0x3fe00000, 0x000001db)},
{D(0x3fe10000, 0x0000033e)},
{D(0x3fe20000, 0x000002cc)},
{D(0x3fe30000, 0x000002d2)},
{D(0x3fe40000, 0x000000ab)},
{D(0x3fe4ffff, 0xfffffe20)},
{D(0x3fe5ffff, 0xfffffd8f)},
{D(0x3fe70000, 0x00000250)},
{D(0x3fe7ffff, 0xffffff7f)},
{D(0x3fe8ffff, 0xfffffdcd)},
{D(0x3fea0000, 0x00000058)},
{D(0x3feaffff, 0xfffffc33)},
{D(0x3fec0000, 0x0000024c)},
{D(0x3fecffff, 0xfffffcbd)},
{D(0x3fee0000, 0x0000009c)},
{D(0x3fef0000, 0x0000004e)},
{D(0x3ff00000, 0x00000000)},
};

/* cosines of the above angles */

static const	du	ck[] =
{
{D(0x00000000, 0x00000000)},
{D(0x3fcfbfbf, 0x7ebc709c)},
{D(0x3fd64564, 0x0568be7b)},
{D(0x3fdb0e35, 0x269b46f1)},
{D(0x3fdefbde, 0xb14f468c)},
{D(0x3fe12cf1, 0xc3c6a80d)},
{D(0x3fe2a79e, 0x3a2cd26b)},
{D(0x3fe3f998, 0x9320bab8)},
{D(0x3fe52a7f, 0xa9d2f97c)},
{D(0x3fe63fa3, 0xf3c026fe)},
{D(0x3fe73ce7, 0x04fb7d73)},
{D(0x3fe82538, 0x78ae2faa)},
{D(0x3fe8fae0, 0xc15ad301)},
{D(0x3fe9bfb0, 0x76d234d6)},
{D(0x3fea751f, 0x9447b53d)},
{D(0x3feb1c62, 0xdb2562f6)},
{D(0x3febb67a, 0xe8584b98)},
{D(0x3fec443f, 0x1d4d2303)},
{D(0x3fecc665, 0xb032862a)},
{D(0x3fed3d89, 0xbe176132)},
{D(0x3fedaa2f, 0xefaae26d)},
{D(0x3fee0cca, 0x12e97828)},
{D(0x3fee65b9, 0xedeba33b)},
{D(0x3feeb553, 0x7b143507)},
{D(0x3feefbde, 0xb14f4f25)},
{D(0x3fef3998, 0xf1b1885e)},
{D(0x3fef6eb6, 0x2d2772fc)},
{D(0x3fef9b61, 0xd023723d)},
{D(0x3fefbfbf, 0x7ebc7543)},
{D(0x3fefdbeb, 0xa917c3eb)},
{D(0x3fefeffb, 0xfdfebf1c)},
{D(0x3feffbff, 0xbff7fec0)},
{D(0x3ff00000, 0x00000000)},
{D(0x3feffbff, 0xbff7fec0)},
{D(0x3fefeffb, 0xfdfebf1c)},
{D(0x3fefdbeb, 0xa917c3eb)},
{D(0x3fefbfbf, 0x7ebc7543)},
{D(0x3fef9b61, 0xd023723d)},
{D(0x3fef6eb6, 0x2d2772fc)},
{D(0x3fef3998, 0xf1b1885e)},
{D(0x3feefbde, 0xb14f4f25)},
{D(0x3feeb553, 0x7b143507)},
{D(0x3fee65b9, 0xedeba33b)},
{D(0x3fee0cca, 0x12e97828)},
{D(0x3fedaa2f, 0xefaae26d)},
{D(0x3fed3d89, 0xbe176132)},
{D(0x3fecc665, 0xb032862a)},
{D(0x3fec443f, 0x1d4d2303)},
{D(0x3febb67a, 0xe8584b98)},
{D(0x3feb1c62, 0xdb2562f6)},
{D(0x3fea751f, 0x9447b53d)},
{D(0x3fe9bfb0, 0x76d234d6)},
{D(0x3fe8fae0, 0xc15ad301)},
{D(0x3fe82538, 0x78ae2faa)},
{D(0x3fe73ce7, 0x04fb7d73)},
{D(0x3fe63fa3, 0xf3c026fe)},
{D(0x3fe52a7f, 0xa9d2f97c)},
{D(0x3fe3f998, 0x9320bab8)},
{D(0x3fe2a79e, 0x3a2cd26b)},
{D(0x3fe12cf1, 0xc3c6a80d)},
{D(0x3fdefbde, 0xb14f468c)},
{D(0x3fdb0e35, 0x269b46f1)},
{D(0x3fd64564, 0x0568be7b)},
{D(0x3fcfbfbf, 0x7ebc709c)},
{D(0x00000000, 0x00000000)},
};

static const	du	Twopm28 =
{ D(0x3e300000, 0x00000000) };

static const	du	Qnan =
{ D(QNANHI, QNANLO) };


double asin(double x)
{
	int	j;
	double	y, z;
	double	z, w;
	double	zsq, poly;
	double	result;

	if (fabs(x) > 1.0)
		x = Qnan.d;

	y = x;

	if (x != x)
		y = 0.0;

	j = ROUND(32.0*y);
	j += 32;
	z = fabs(y);

	z = y * ck[j].d - sk[j].d*sqrt(1.0 - z + z * (1.0 - z));
	w = z;

	/* guard against underflow for small args	*/

	if (fabs(z) < Twopm28.d)
		w = 0.0;

	zsq = w * w;

	poly = ((((((P[7].d*zsq + P[6].d)*zsq + P[5].d)*zsq +
		P[4].d)*zsq + P[3].d)*zsq + P[2].d)*zsq +
		P[1].d)*(zsq*z) + z;

	result = xk[j].d + poly;

	if (x != x)
		result = Qnan.d;

	return result;

}
