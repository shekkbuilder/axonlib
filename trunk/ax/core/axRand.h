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

#ifndef axRand_included
#define axRand_included
//------------------------------------------------------------------------------

#include "axDefines.h"

#ifdef AX_USE_HOT_INLINE
  #define __axrandinline __hotinline
#else
  #define __axrandinline inline
#endif

/*
  ------------------------------------------------------------------------------
  axRand()
  - "rdtsc" 64 bit register dependant
  - constantly seeded from "rdtsc"
  - uniform distribution
  - non-recursive
  - returns integer or floats in specified ranges
  author: lubomir i. ivanov (lii) for axonlib
  ------------------------------------------------------------------------------
*/

#define AXRAND_MAX        2147483647L
#define INV_AXRAND_MAX    4.656612875245796924105750827168e-10f

#define _AXRAND_FLOAT     ( ( (float)( ( a * b ) >> 2) ) * 0.00000000093f )
#define _AXRAND_FLOAT_C   s * ( (float)( ( a * b ) >> 1) ) * INV_AXRAND_MAX

#ifdef __AX64__
  // 64bit untested
  #define _AXRAND_ARCH \
    register unsigned long low, high; \
    register unsigned int  a, b; \
    __asmv ( "rdtsc;" : "=a" (low), "=d" (high) ) \
    a = (unsigned int)( (low) | ( (unsigned long)(high) << 32 ) )
#endif
#ifdef __AX32__
  #define _AXRAND_ARCH \
    register unsigned long long _a; \
    register unsigned long a, b; \
    __asmv ( "rdtsc;" : "=A" (_a) ); \
    a = (unsigned long) _a;
#endif

#define _AXRAND \
  _AXRAND_ARCH; \
  b = a; \
  a = ( (a * -29999531) & -30000469 ) * 20000469; \
  b = ( (b * -20000085) & -19999915 ) * 19999915;

__axrandinline float axRand(void)
{ _AXRAND; return _AXRAND_FLOAT; }

__axrandinline float axRand(const float s)
{ _AXRAND;  return _AXRAND_FLOAT_C; }

__axrandinline float axRandSigned(void)
{ _AXRAND; return ( 2.f * _AXRAND_FLOAT - 1.f ); }

__axrandinline unsigned long axRandInt(void)
{ _AXRAND; return ( a * b ); }

__axrandinline unsigned long axRandInt(const unsigned long s)
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

#define AXRANDSINF_MAX    AX_FLT_MAX
#define AXRAND_MAX        2147483647L

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
      // 64bit untested
      #ifdef __AX64__
        register unsigned long low, high;
        register unsigned int _x;
        __asmv ( "rdtsc;" : "=a" (low), "=d" (high) );
        _x = (unsigned int) ( (low) | ( (unsigned long)(high) << 32 ) );
      #endif
      #ifdef __AX32__
        register unsigned long _x;
        __asmv ( "rdtsc;" : "=a" (_x) );
      #endif
      x = (float)(_x >> 16);
      y = 1.f;
    }

    axRandSinf(const unsigned long _x)  { x = (float)_x; }
    axRandSinf(const float _x)          { x = _x;        }

    ~axRandSinf() {}

    __axrandinline float rand(void)
    { _AXRANDSINF; return _AXRANDSINF_SHIFT; }

    __axrandinline float rand(const float _top)
    { _AXRANDSINF; return _top * _AXRANDSINF_SHIFT; }

    __axrandinline float randSigned(void)
    { _AXRANDSINF; return y; }

    __axrandinline unsigned long randInt(void)
    { _AXRANDSINF;  return (unsigned long) AXRAND_MAX * _AXRANDSINF_SHIFT; }

    __axrandinline unsigned long randInt(const unsigned long _top)
    { _AXRANDSINF; return (unsigned long) ( _top * _AXRANDSINF_SHIFT ); }
};

//------------------------------------------------------------------------------
#endif
