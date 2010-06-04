/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

/*
  axonlib's builtin random number generators
  64 bit integers are used so you might need to disable the 'long long'
  warning in g++: -Wno-long-long
*/

#ifndef axRand_included
#define axRand_included
//------------------------------------------------------------------------------

#ifdef AX_HOT_INLINE_RAND
  #define __axrand_inline __hotinline
#else
  #define __axrand_inline inline
#endif

#ifndef AX_NO_RAND      // if not defined: use axonlib's axRand()

#include "core/axDefines.h"

/*
  ------------------------------------------------------------------------------
  axRand()
  - "rdtsc" 64 bit register dependant
  - seeded from "rdtsc"
  - uniform distribution
  - returns integer or floats in specified ranges
  author: lubomir i. ivanov (lii) for axonlib
  ------------------------------------------------------------------------------
*/

#define AXRAND_MAX        2147483647L
#define INV_AXRAND_MAX    4.656612875245796924105750827168e-10f
#define AXRAND_MAXLL      9223372036854775806LL

#define _AXRAND_FLOAT     ( ( (float)( ( a * b ) >> 2) ) * 0.00000000093f )
#define _AXRAND_FLOAT_C   s * ( (float)( ( a * b ) >> 1) ) * INV_AXRAND_MAX

#ifdef __AX64__
  // 64bit untested
  unsigned int _axrnd;
  #define _AXRAND_ARCH \
    register unsigned long low, high; \
    register unsigned int  a, b; \
    if (!_axrnd) \
    { \
      __asm__ ( "rdtsc;" : "=a" (low), "=d" (high) ) \
      _axrnd = (unsigned int)( (low) | ( (unsigned long)(high) << 32 ) ) \
    } else if (_axrnd > AXRAND_MAXLL) _axrnd = 0; \
    a = _axrnd++;
#endif
#ifdef __AX32__
  unsigned long long _axrnd;
  #define _AXRAND_ARCH \
    register unsigned long a, b; \
    if (!_axrnd) \
      __asm__ ( "rdtsc;" : "=A" (_axrnd) ); \
    else if (_axrnd > AXRAND_MAXLL) _axrnd = 0; \
    a = (unsigned long) _axrnd++
#endif

#define _AXRAND \
  _AXRAND_ARCH; \
  b = a; \
  a = ( (a * -29999531) & -30000469 ) * 20000469; \
  b = ( (b * -20000085) & -19999915 ) * 19999915;

__axrand_inline float axRand(void)
{ _AXRAND; return _AXRAND_FLOAT; }

__axrand_inline float axRand(const float s)
{ _AXRAND;  return _AXRAND_FLOAT_C; }

__axrand_inline float axRandSigned(void)
{ _AXRAND; return ( 2.f * _AXRAND_FLOAT - 1.f ); }

__axrand_inline unsigned long axRandInt(void)
{ _AXRAND; return ( a * b ); }

__axrand_inline unsigned long axRandInt(const unsigned long s)
{ _AXRAND;
  return (unsigned long)( _AXRAND_FLOAT_C ); }

/*
  ------------------------------------------------------------------------------
  axRandSinf()
  - non-uniform, trigonometric, "negative" distribution
    (more values towards min, max)
  - uses class constructor
  - recursive
  - seeded with "rdtsc" on initialization
  - thread unsafe
  - returns integer or floats in specified ranges
  author: lubomir i. ivanov (lii) for axonlib
  ------------------------------------------------------------------------------
*/

#define AXRANDSINF_MAX    3.40282347e+38F
#define AXRANDSF_MAX      2147483647L

#define _AXRANDSINF \
  if (x >= AXRANDSINF_MAX ) x = 0.f; \
  __asm__ \
  ( \
    "fld1;" \
    "faddp;" \
    "fxch;" \
    "fmul %%st(1);" \
    "fsin;" \
    : "=t" (y), "=u" (x) : "0" (x), "1" (y) \
  )

#define _AXRANDSINF_SHIFT \
  (y + 1.f) * 0.5f

class axRandSinf
{
  private:
    float x, y;
  
  public:
    axRandSinf(void)
    {
      #ifdef __AX64__
      // 64bit untested
        register unsigned long low, high;
        register unsigned int _x;
        __asmv ( "rdtsc;" : "=a" (low), "=d" (high) );
        _x = (unsigned int) ( (low) | ( (unsigned long)(high) << 32 ) );
      #endif
      #ifdef __AX32__
        register unsigned long long _x;
        __asmv ( "rdtsc;" : "=A" (_x) );
      #endif
      x = (float)(_x >> 16);
      y = 1.f;
    }
      
    axRandSinf(const unsigned long _x)  { x = (float)_x; }
    axRandSinf(const float _x)          { x = _x;        }

    ~axRandSinf() {}

    __axrand_inline float rand(void)
    { _AXRANDSINF; return _AXRANDSINF_SHIFT; }

    __axrand_inline float rand(const float _top)
    { _AXRANDSINF; return _top * _AXRANDSINF_SHIFT; }

    __axrand_inline float randSigned(void)
    { _AXRANDSINF; return y; }

    __axrand_inline unsigned long randInt(void)
    { _AXRANDSINF;  return (unsigned long) (AXRANDSF_MAX * _AXRANDSINF_SHIFT); }

    __axrand_inline unsigned long randInt(const unsigned long _top)
    { _AXRANDSINF; return (unsigned long) ( _top * _AXRANDSINF_SHIFT ); }
};

#else // AX_NO_RAND

/*
  ------------------------------------------------------------------------------
  stdlib based methods rand()
  ------------------------------------------------------------------------------
*/

#include "math.h"
#include "time.h"
#include "stdarg.h"
#include "stdlib.h"

__deprecated ___axrand_inline void axRandomize
  (const int aSeed = (unsigned int)time(0))
{ srand(aSeed); }

__deprecated __axrand_inline float axRand(const float f = 1)
{ return (f * (float)rand() / (float)RAND_MAX); }

__deprecated __axrand_inline float axRand(const float aLow, const float aHigh)
{ return aLow + ( (float)rand() / (float)RAND_MAX ) *(aHigh - aLow); }

__deprecated __axmath_inline int axRandomInt(const int i)
{ return axMinInt
    (i, (int)axFloor( (i + 1) * (float)rand() / (float)RAND_MAX ) ); }

__deprecated __axrand_inline float axRandomSigned(void)
{ return (2 * (float)rand() / (float)RAND_MAX) - 1; }

#endif // AX_NO_RAND

#endif // axRand_included
