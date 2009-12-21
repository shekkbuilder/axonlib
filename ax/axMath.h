#ifndef axMath_included
#define axMath_included
//----------------------------------------------------------------------

#include <math.h>
#include <stdlib.h>

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
    return axMin( i, floorf(f) );

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

//----------------------------------------------------------------------
#endif
