#ifndef axMath_included
#define axMath_included
//----------------------------------------------------------------------

#include <math.h>
#include <stdlib.h>
#include "axDebug.h"

//------------------------------

#define axMin(a,b)  ((a<b)?a:b)
#define axMax(a,b)  ((a>b)?a:b)

#define axCalcStep(v,s)     (axMin(floorf(v*s),s-1))
#define axCalcValue(v,m,a)  (v*m+a)

//----------------------------------------------------------------------

void axRandomize(int aSeed=19)
  {
    srand(aSeed);
  }

//----------

// 0..f
float axRandom(float f)
  {
    return ( f * (float)rand() / (float)RAND_MAX );
  }

//----------

// 0..i (int, inclusive)
int axRandomInt(int i)
  {
    float f = axRandom(i+1);
    return (int)axMin( i, floorf(f) );

  }

//----------

// 0..1
float axRandom(void)
  {
    return ( (float)rand() / (float)RAND_MAX );
  }

//----------

// -1..1
float axRandomSigned(void)
  {
    return ( 2 * (float)rand() / (float)RAND_MAX ) - 1;
  }

//----------

// aLow..aHigh
float axRandom(float aLow, float aHigh)
  {
    float range = aHigh-aLow;
    float rnd = axRandom();//(float)rand() / (float)RAND_MAX;
    return aLow + rnd*range;
  }

//----------


//----------------------------------------------------------------------

unsigned int axBitReverse( unsigned int aInput )
  {
    unsigned int in  = (unsigned int)aInput;
    unsigned int out = 0;
    for( unsigned int i=0; i<32; i++ )
    {
      unsigned int bit = (1<<i);
      unsigned int rev = (0x80000000>>i);
      if (in&bit) out+=rev;
    };
    return out;
  }

//----------------------------------------------------------------------

// convert linear -> dB
float lin2dB( float lin )
{
  float precalc_a = 20.0 / log(10.0);
  return precalc_a * log(lin);
}

// convert dB-> linear
float dB2lin( float dB )
{
  float precalc_b = 0.05 * log(10.0);
  return exp( precalc_b * dB );
}

//----------

//-----------------------------------------------------------------------
// fast fabs()
//-----------------------------------------------------------------------
inline float axAbs (float value)
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
inline float axNeg (float value)
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
//-----------------------------------------------------------------------
inline float axSign (const float value)
{
	return 1.0f + (((*(int*)&value) >> 31) << 1);
}

//-----------------------------------------------------------------------
// approximation: n-th root of x
//-----------------------------------------------------------------------
inline float axNrt (float value, long root)
{
	__asm__ volatile
	(
		"subl $0x3f800000, %0;"
		"subl $1, %2;"
		"shrl %b2, %0;"
		"addl $0x3f800000, %0;"
		: "=r" (value)
		: "0" (value), "c" (root)
	);
	return value;
}

//----------------------------------------------------------------------
// approximation: square root of x
//----------------------------------------------------------------------
inline float axSqrt (float value)
{
	__asm__ volatile
	(
		"subl $0x3f800000, %0;"
		"shrl $1, %0;"
		"addl $0x3f800000, %0;"
		: "=r" (value)
		: "0" (value)		
	);
	return value;
}

//----------------------------------------------------------------------
// approximation: invert square root of x 
//----------------------------------------------------------------------
inline float axInvSqrt (float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i>>1);
	x = *(float*)&i;
	return x*(1.5f - xhalf*x*x);
}

//----------------------------------------------------------------------
// approximation: powf(x, n) [currently 4.37 times faster than powf()]
//----------------------------------------------------------------------
inline float axPow (float x, long n)
{
	const int org_n = n;
	float z = 1;
	if (n < 0) n = -n;
	while (n != 0)
	{
		if ((n & 1) != 0) z *= x;
		n >>= 1;
		x *= x;
	}	
	return ((org_n < 0) ? 1.0f / z : z);
}

//----------------------------------------------------------------------
// approximation: exp(x)
//----------------------------------------------------------------------
inline float axExp (const float exponent)
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
// approximation: fsin(x) for range [-pi, pi]
//----------------------------------------------------------------------
inline float axSin (float v)
{
	v = v * (1.2732395447f - 0.4052847345f * axAbs(v));
	return 0.225f * (v * axAbs(v) - v) + v;
	//---------------------------------------------------------
	/*
	// ## tmp / gas
	// ------  
	float result;
  float a = -1.2732395447f;
	float b = -0.4052847345f;
	float c = 0.225f;	
  __asm__ volatile
	(
		"movl %0, %%eax;"		"andl $0x7fffffff, %%eax;"		"fld %2;"		"fld %3;"
		"fmulp;"		"fld %2;"		"fldl %%eax;"		"faddp;"		"fstp %0;"		
		"fld %0;"		"fld %%eax;"		"fmulp;"		"fstp %0;"		"movl %0, %%eax;"
		"andl $0x7fffffff, %%eax;"		"fld %0;"		"fld %%eax;"		"fmulp;"
		"fstp %%eax;"		"fld %0;"		"fld %%eax;"			"fsubp;"		"fstp %%eax;"
		"fld %%eax;"		"fld %4;"		"fmulp;"		"fld %%eax;"
		"fld %0;"		"faddp;"		"fstp %0;"
		: "=m" (result)
		: "g" (value), "g" (a), "g" (b), "g" (c)
		: "eax"			
  );
  return result;
  */  
}


//----------------------------------------------------------------------
// approximation: fcos(x) for range [-pi, pi] from identity
//----------------------------------------------------------------------
inline float axCos (float v)
{
    return axSin(v + PI_2);
}

//----------------------------------------------------------------------
// fsin(x)
//----------------------------------------------------------------------
inline float axFsin (float value)
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
// fcos(x)
//----------------------------------------------------------------------
inline float axFcos (float value)
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
// not really math related, but it have to stay here, until we find a
// better 'home'

void axRadix (long *source, long *dest, long N, int byte)
{
  int i;
  long count[256];
  long index[256];
  memset (count, 0, sizeof (count));
  for ( i=0; i<N; i++ ) count[((source[i])>>(byte*8))&0xff]++;
  index[0]=0;
  for ( i=1; i<256; i++ ) index[i]=index[i-1]+count[i-1];
  for ( i=0; i<N; i++ ) dest[index[((source[i])>>(byte*8))&0xff]++] = source[i];
}

//i = 1;
//loop( size-1,
//  cur = buf[i];
//  j = i;
//  buf[j-1] > cur ? (
//    while(
//      buf[j] = buf[j-1];
//      j -= 1;
//      ( (j>0) && (buf[j-1]>cur) )
//    );
//  );
//  i += 1;
//);

//----------------------------------------------------------------------

#endif
