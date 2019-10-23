#define	QNANHI	0xfff80000
#define	QNANLO	0x00000000

#define	QNANF	0xffc00000

extern	const double	__libm_qnan_d;
extern	const float	__libm_qnan_f;
extern	const double	__libm_inf_d;
extern	const float	__libm_inf_f;
extern	const double	__libm_neginf_d;
extern	const float	__libm_neginf_f;

#define D(h,l) l,h

extern	int	errno;

#define SETERRNO(x)     \
        {       \
                errno = x; \
        }

typedef union
{
	struct
	{
		unsigned int lo;
		unsigned int hi;
	} word;

	double	d;
} du;

typedef union
{
	unsigned int	i;
	float	f;
} fu;

#define	ISNANF(x)	((x)!=(x))
#define	ISNAND(x)	((x)!=(x))
#define	EXPBIAS 0x7f
#define	EXPMASK	0x807fffff
#define	SIGNMASK	0x7fffffff

#define	EXPWIDTH	8

#define	DBLHI2INT(x, n)	n = *((int *)&x + 1)
#define	DBLLO2INT(x, n)	n = *(int *)&x
#define	INT2DBLHI(n, x)	*((int *)&x + 1) = n
#define	INT2DBLLO(n, x)	*(int *)&x = n

#define	__float80	long double

#define	DEXPBIAS 0x3ff
#define	DEXPWIDTH	11

#define	ROUNDF(f)	__builtin_round_f2ll(f)
#define	ROUND(d)	__builtin_round_d2ll(d)
#define	ROUNDED(ed)	__builtin_round_ed2ll(ed)

#define	FLT2INT(x, n)	n = __builtin_cast_f2i(x)
#define	INT2FLT(n, x)	x = __builtin_cast_i2f(n)

#define	DBL2LL(x, n)	n = __builtin_cast_d2ll(x)
#define	LL2DBL(n, x)	x = __builtin_cast_ll2d(n)

#define	EDHI2INT(x, n)	n = __builtin_getf_exp(x)
#define	INT2EDHI(n, x)	x = __builtin_setf_exp(x, n)
#define	EDLO2LL(x, n)	n = __builtin_getf_sig(x)
#define	LL2EDLO(n, x)	x = __builtin_setf_sig(x, n)

#define	FMERGE(x, y)	__builtin_fmerge_se(x, y)

#define	INT	long long
#define	UINT	unsigned long long
#define	DEXPMASK	0x800fffffffffffffll
#define	DSIGNMASK	0x7fffffffffffffffll
#define	DMANTWIDTH	52
#define	MANTWIDTH	23

#define	MAXEDEXP	0x1ffff
#define	EDEXPBIAS	0xffff
#define	EDSIGNMASK	0xffff
#define	EDEXPMASK	0x10000
#define	EDEXPWIDTH	17

#define	RFSIGNMASK	0x1ffff
#define	RFEXPBIAS	0xffff
#define	RFEXPWIDTH	17
#define	RFEXPMASK	0x20000ll

#define ISNANED(x)	((x)!=(x))

#define	EDH2LL(x, n) \
	n = *(int *)&x; \
	n >>= 52; \
	n &= 0xfff

#define LL2EDH(n, x) \
	*(long long *)&x &= (DSIGNMASK & DEXPMASK); \
	*(long long *)&x |= (n << 52)
