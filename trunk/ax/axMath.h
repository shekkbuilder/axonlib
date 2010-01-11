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

// *** asm translations bellow
// ------------------------------------
float approx_sqrt( float f )
{
  //assert( sizeof(unsigned int) == sizeof(float) );
  unsigned int* u;
  u = (unsigned int*)&f;
  *u -= 0x3F800000;
  *u >>= 1;
  *u += 0x3F800000;
  return *(float*)(u);
}

float rttof( float x, long root )
{
  long l = *(long*)&x;
  l -= 0x3F800000;
  l >>= root-1;
  l += 0x3F800000;
  return *(float*)&l;
}
// ------------------------------------
// *** asm translations bellow

//-----------------------------------------------------------------------
// approximation: n-th root of x
//----------------------------------------------------------------------
float axNrt (float radicand, long root)
{
		float result;
	 	__asm__ volatile
		(
			"movl %1, %%eax;"
			"subl $0x3f800000, %%eax;"
			"subl $1, %2;"
			"shrl %b2, %%eax;"
			"addl $0x3f800000, %%eax;"
			"movl %%eax, %0;"
	    : "=r" (result)
	    : "r" (radicand), "c" (root)
			: "%eax"
		);
    return result;    
}

//----------------------------------------------------------------------
// approximation: square root of x
//----------------------------------------------------------------------
float axSqrt (float radicand)
{
	float result;
 	__asm__ volatile
	(
		"movl %1, %%eax;"
		"subl $0x3f800000, %%eax;"			
		"shrl $1, %%eax;"
		"addl $0x3f800000, %%eax;"
		"movl %%eax,%0;"
    : "=r" (result)
    : "r" (radicand)
		: "%eax"
	);
	return result;
}

//----------------------------------------------------------------------
// approximation: invert square root of x 
// * at&t asm pending
//----------------------------------------------------------------------
float axInvSqrt (float x)
{
    float xhalf = 0.5f*x;
    int i = *(int*)&x;
    i = 0x5f3759df - (i>>1);
    x = *(float*)&i;
    return x*(1.5f - xhalf*x*x);
}

//----------------------------------------------------------------------
// approximation: powf(x, n) [currently 4.37 times faster than powf()]
// * at&t asm pending
//----------------------------------------------------------------------
float axPow (float x, long n)
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
// * at&t asm pending
//----------------------------------------------------------------------
float axExp(float exponent)
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
