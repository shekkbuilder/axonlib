#ifndef axMath_included
#define axMath_included
//----------------------------------------------------------------------

#include <math.h>
#include <stdlib.h>
#include "axDebug.h"

//----------------------------------------------------------------------
// fast floorf() / int()
//----------------------------------------------------------------------
inline float axFloor(float value)
{
	int j;
	__asm__ volatile
	(
		"flds %1;"		"fistpl %0;"
		: "=m" (j)
		: "m" (value)
	);
	return j;
}

//----------------------------------------------------------------------
// fast ceilf()
//----------------------------------------------------------------------
inline float axCeil(float value)
{
	int j;
	float im = 1.0f;
	__asm__ volatile
	(
		"flds %1;"		"flds %2;"		"faddp;"		"fistpl %0;"
		: "=m" (j)
		: "m" (value), "m" (im)
	);
	return j;
}

//----------------------------------------------------------------------
// fast roundf()
//----------------------------------------------------------------------
inline float axRound(float value)
{
	int j;
	float im = 0.5f;
	__asm__ volatile
	(
		"flds %1;"		"fistpl %0;"		"flds %2;"		"faddp;"
		: "=m" (j)
		: "m" (value), "m" (im)
	);
	return j;
}

//-----------------------------------------------------------------------
// fast fabs()
//-----------------------------------------------------------------------
inline float axAbs(float value)
{
	__asm__ volatile
	(
		"andl $0x7fffffff, %0;"
		: "=r" (value)
		: "0" (value)
	);
	return value;
}

//-----------------------------------------------------------------------
// x = -x
//-----------------------------------------------------------------------
inline float axNeg(float value)
{
	__asm__ volatile
	(
		"xorl $0x80000000, %0;"
		: "=r" (value)
		: "0" (value)
	);
	return value;
}

//-----------------------------------------------------------------------
// fast sign(x)
// NOTE: gas with -O3 optimizes better in this case
//-----------------------------------------------------------------------
inline float axSign(const float value)
{
	return 1.0f + (((*(int*)&value) >> 31) << 1);
}

//----------------------------------------------------------------------
// min(x,y)
//----------------------------------------------------------------------
inline float axMin(const float a, const float b)
{
	return (a < b) ? a : b;
}

//----------------------------------------------------------------------
// max(x,y)
//----------------------------------------------------------------------
inline float axMax(const float a, const float b)
{
	return (a > b) ? a : b;
}

//----------------------------------------------------------------------
// limit(input,limit_value)
//----------------------------------------------------------------------
inline float axLimit(const float input, const float limit)
{
	return axMin(axMax(input, -limit), limit);
}

//----------------------------------------------------------------------
// minInt(x,y)
//----------------------------------------------------------------------
inline int axMinInt(const int a, const int b)
{
	return (a < b) ? a : b;
}

//----------------------------------------------------------------------
// maxInt(x,y)
//----------------------------------------------------------------------
inline int axMaxInt(const int a, const int b)
{
	return (a > b) ? a : b;
}

//----------------------------------------------------------------------
// limitInt(input,limit_value)
//----------------------------------------------------------------------
inline int axLimitInt(const int input, const int limit)
{
	return axMinInt(axMaxInt(input, -limit), limit);
}

//----------------------------------------------------------------------
// axCalcStep
//----------------------------------------------------------------------
inline float axCalcStep(const float v, const float s)
{
	return axMin(axFloor(v*s), s - 1);
}

//----------------------------------------------------------------------
// axCalcValue
//----------------------------------------------------------------------
inline float axCalcValuep(const float v, const float m, const float a)
{
	return (v*m+a);
}
//----------------------------------------------------------------------
// axRandomize -> default seed: 19
//----------------------------------------------------------------------
inline void axRandomize(const int aSeed = 19)
{
    srand(aSeed);
}

//----------------------------------------------------------------------
// axRandom -> range: 0..1 (default)
//----------------------------------------------------------------------
inline float axRandom(const float f = 1)
{
	return (f * (float)rand() / (float)RAND_MAX);
}

//----------------------------------------------------------------------
// axRandomInt -> 0..i (int, inclusive)
//----------------------------------------------------------------------
inline int axRandomInt(const int i)
{
	const float f = axRandom(i + 1);
	return (int)axMin(i, axFloor(f));
}

//----------------------------------------------------------------------
// axRandomSigned
//----------------------------------------------------------------------
inline float axRandomSigned(void)
{
	return (2 * (float)rand() / (float)RAND_MAX) - 1;
}

//----------------------------------------------------------------------
// axRandom -> range: aLow..aHigh
//----------------------------------------------------------------------
//
inline float axRandom(const float aLow, const float aHigh)
{
	float range = aHigh-aLow;
	float rnd = axRandom();
	return aLow + rnd*range;
}

//----------------------------------------------------------------------
// FPU ln(x)
//----------------------------------------------------------------------
inline float axLogf(float value)
{
	__asm__ volatile
	(
		"fld %0;"		"fldln2;"		"fxch;"		"fyl2x;"		"fst %0;"
		: "=m" (value)
		: "m" (value)
	);
	return value;
}

//----------------------------------------------------------------------
// FPU log2(x) log base 2
//----------------------------------------------------------------------
inline float axLog2f(float value)
{
	__asm__ volatile
	(
		"fld1;"		"fld %0;"    "fyl2x;"		"fst %0;"
		: "=m" (value)
		: "m" (value)
	);
	return value;
}

//----------------------------------------------------------------------
// FPU sinf(x)
//----------------------------------------------------------------------
inline float axSinf(float value)
{
	__asm__ volatile
	(
		"fsin %1, %0;"
		: "=f" (value)
		: "f" (value)
  );
	return value;
}

//----------------------------------------------------------------------
// FPU cosf(x)
//----------------------------------------------------------------------
inline float axCosf(float value)
{
	__asm__ volatile
	(
		"fcos %1, %0;"
		: "=f" (value)
		: "f" (value)
  );
	return value;
}

//----------------------------------------------------------------------
// FPU tanf(x)
//----------------------------------------------------------------------
inline float axTanf(float value)
{
	__asm__ volatile
	(
		"fld %0;"		"fsincos;"    "fdivrp;"    "fst %0;"
		: "=m" (value)
		: "m" (value)
  );
	return value;
}

//----------------------------------------------------------------------
// FPU asinf(x)
// asin(x)=atan(sqrt(x*x/(1-x*x)))
//----------------------------------------------------------------------
inline float axAsinf(float value)
{
	float tmp;
	__asm__ volatile
	(
		"fld %0;"		"fld %0;"		"fmulp;"		"fst %1;"		"fld1;"		"fsubp;"
		"fld %1;"		"fdivp;"		"fsqrt;"		"fld1;"		"fpatan;"		"fst %0;"
		: "=m" (value)
		: "m" (tmp)
		: "eax"
  );
	return value;
}

//----------------------------------------------------------------------
// FPU acosf(x)
// acos(x) = atan(sqrt((1-x*x)/(x*x)))
//----------------------------------------------------------------------
inline float axAcosf(float value)
{
	float tmp;
	__asm__ volatile
	(
		"fld %0;"		"fld %0;"		"fmulp;"		"fst %1;"		"fld1;"		"fsubp;"
		"fld %1;"		"fdivrp;"		"fsqrt;"		"fld1;"		"fpatan;"		"fst %0;"
		: "=m" (value)
		: "m" (tmp)
		: "eax"
  );
	return value;
}

//----------------------------------------------------------------------
// FPU atanf(x)
//----------------------------------------------------------------------
inline float axAtanf(float value)
{
	__asm__ volatile
	(
		"fld %0;"		"fld1;"		"fpatan;"		"fst %0;"
		: "=m" (value)
		: "m" (value)
  );
	return value;
}

//-----------------------------------------------------------------------
// approximation: n-th root of x
//-----------------------------------------------------------------------
inline float axNrt(float value, long root)
{
	__asm__ volatile
	(
		"subl $0x3f800000, %0;"		"subl $1, %2;"
		"shrl %b2, %0;"		"addl $0x3f800000, %0;"
		: "=r" (value)
		: "0" (value), "c" (root)
	);
	return value;
}

//----------------------------------------------------------------------
// approximation: square root of x
//----------------------------------------------------------------------
inline float axSqrt(float value)
{
	__asm__ volatile
	(
		"subl $0x3f800000, %0;"		"shrl $1, %0;"
		"addl $0x3f800000, %0;"		: "=r" (value)
		: "0" (value)
	);
	return value;
}

//----------------------------------------------------------------------
// approximation: invert square root of x
// q3
//----------------------------------------------------------------------
inline float axInvSqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i>>1);
	x = *(float*)&i;
	return x*(1.5f - xhalf*x*x);
}

//----------------------------------------------------------------------
// approximation: pow(float, integer)
//----------------------------------------------------------------------
inline float axPow(float x, int n)
{
	float res = 1;
	while (n > 0)
	{
		if (n & 1) res *= x;
		n >>= 1;
		x *= x;
	}
	return res;
}

//----------------------------------------------------------------------
// approximation: exp(x)
// http://theoval.sys.uea.ac.uk/publications/pdf/nc2000a.pdf
//----------------------------------------------------------------------
inline float axExp(const float exponent)
{
	union
	{
		double d;
		struct
		{
			#ifdef LITTLE_ENDIAN
				int i, j;
			#else
				int j, i;
			#endif
		} s;
	} u;
	u.s.i = (int)(1512775*(double)exponent) + 1072632447;
	u.s.j = 0;
	return (float)u.d;
}

//----------------------------------------------------------------------
// approximation: log2(x)
// http://www.flipcode.com/archives/Fast_log_Function.shtml
//----------------------------------------------------------------------
inline float axLog2(float val)
{
	assert (val > 0);
	int * const  exp_ptr = reinterpret_cast <int *> (&val);
	int          x = *exp_ptr;
	const int    log_2 = ((x >> 23) & 255) - 128;
	x &= ~(255 << 23);
	x += 127 << 23;
	*exp_ptr = x;
	return (val + log_2);
}

//----------------------------------------------------------------------
// approximation: ln(x)
//----------------------------------------------------------------------
inline float axLog(const float &val)
{
	return (axLog2 (val) * 0.69314718f);
}

//----------------------------------------------------------------------
// approximation: sin(x)
//----------------------------------------------------------------------
inline float axSin(float v)
{
	v = v * (1.2732395447f - 0.4052847345f * axAbs(v));
	return 0.225f * (v * axAbs(v) - v) + v;
}

//----------------------------------------------------------------------
// approximation: cos(x)
//----------------------------------------------------------------------
inline float axCos(float x)
{
	const float x2 = x*x;
	return (15120 + x2*(-6900 + 313*x2)) / (15120 + x2*(660 + 13*x2));
}

//----------------------------------------------------------------------
// approximation: tan(x)
//----------------------------------------------------------------------
inline float axTan(float x)
{
	const float x2 = x*x;
	return (x*(105 - 10*x2)) / (105 - x*(45*x - x*x2));
}

//----------------------------------------------------------------------
// approximation: asin(x)
//----------------------------------------------------------------------
inline float axAsin(const float x)
{
	return M_PI_2 - sqrtf(1 - x)*(1.5707288 - x*(0.2121144 + x*(0.0742610
	- x*(0.0187293 + 0.2698391*x))));
}

//----------------------------------------------------------------------
// approximation: acos(x)
//----------------------------------------------------------------------
inline float axAcos(const float x)
{
	const float x2 = x*x;
	return (15120 + x2*(-6900 + 313*x2)) / (15120 + x2*(660 + 13*x2));
}

//----------------------------------------------------------------------
// approximation: atan(x)
//----------------------------------------------------------------------
inline float axAtan(const float x)
{
	const float x2 = x*x;
	return (x*(105 + 55*x2)) / (105 + x2*(90 + 9*x2));
}

//----------------------------------------------------------------------
// approximation: sinh(x)
//----------------------------------------------------------------------
inline float axSinh(const float x)
{
	const float x2 = x*x;
	return x*(x2*(0.012*x2 + 0.156) + 1.004);
}

//----------------------------------------------------------------------
// approximation: cosh(x)
//----------------------------------------------------------------------
inline float axCosh(const float x)
{
	const float x2 = x*x;
	return x2*(0.065*x2 + 0.428) + 1.025;
}

//----------------------------------------------------------------------
// approximation: tanh(x)
//----------------------------------------------------------------------
inline float axTanh(const float x)
{
	const float x2 = x*x;
	return x*(27 + x2) / (27 + 9*x*x);
}

//----------------------------------------------------------------------
// lin2dB: convert linear -> dB
//----------------------------------------------------------------------
inline float lin2dB(const float lin)
{
	return LOG2DB * logf(lin);
}

//----------------------------------------------------------------------
// dB2lin: convert dB -> linear
//----------------------------------------------------------------------
inline float dB2lin(const float dB)
{
	return expf(DB2LOG * dB);
}

#endif
