#ifndef MYHEAD
#define MYHEAD

#define RUN_COUNT	1000000
#define DATA_COUNT	1000000
#define RUNTIME	1000
#define PERCENT	0.1

#define MAXEXP	0x7ffu
#define	MINEXP	0x001u
#define	DEXPBIAS	0x3ff
#define DMANTISSA	0x000fffffffffffffll
#define	DMANTWIDTH	52
#define	DEXPWIDTH	11

typedef unsigned long int _UL;
typedef long int _L;
typedef union
{
	double d;
	unsigned long int l;
} DL;

#define D(h,l) l,h
typedef union
{
	struct
	{
		unsigned int lo;
		unsigned int hi;
	} word;

	double	d;
}  du;
#define	QNANHI	0xfff80000
#define	QNANLO	0x00000000
#define	QNANF	0xffc00000

#endif
