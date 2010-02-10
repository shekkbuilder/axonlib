/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file axMath.h
 * \brief math approximations and routines
 * axonlib's math library with a collection of optimized functions for trigonometry,
 * algebra and analysis. <br>
 * some of the methods have a 'f' suffix version (e.g.: axTanf) <br>
 * such versions use FPU instructions for the calculation.
 * the versions without 'f' suffix are most likely less cpu heavy approximations,
 * but with reduced accuracy and acceptable input range (e.g. axLog). <br> 
 * \code
 * TODO:
 * - short performance table
 * \endcode
 */

// ---------------------------------------------------------------------------
// NOTES:
// the empty inline asm declaration: __asm__ __volatile__ ("":::);
// may look absurd but i've discovered that the inline asm that follows such,
// returns proper values (sometimes a 'non-volatile' delcared methods can return incorrect)
// yet execution times are much faster. we may as well leave it this way.
// ---------------------------------------------------------------------------

#ifndef axMath_included
#define axMath_included

#include <math.h> // <- could this be optional since most ? 
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include "axDebug.h"
#include "axDefines.h"
#include "axUtils.h"

/// square of x
#define axSqr(x) ((x)*(x))

/// cube of x
#define axCube(x) ((x)*(x)*(x))

/**
 * newton step 1 (newton's method) <br>
 * \code 
 * new_approx = (old_approx + x/old_approx) / 2;
 * \endcode 
*/
#define axNewtonStep(y, x) { (y)=((y)+(x)/(y))*0.5f; }
/**
 * newton step 2 (newton's method) <br>
 * as seen in axInvSqrt <br> 
 * \code 
 * y = x*(1.5f - (x/2)*x*x);
 * \endcode 
*/
#define axNewtonStep2(x) { (x) = (x)*(1.5f - (x)*0.5f)*(x)*(x); }

/**
 * returns the floor of a floating point number
 * \code
 * // -1.6 -> -1
 * // -1.1 -> -1
 * // 1.6 -> 1
 * // 1.1 -> 1 
 * \endcode  
 * @param[in] value float
 * @return result float
 */
inline float axFloor(const float value)
{
  return (float)(int)(value);
}

/**
 * returns the ceil of a floating point number
 * @param[in] value float
 * @return result float
 */
inline float axCeil(const float value)
{
  return (float)(int)(value + 1);
}

/**
 * rounds a floating point number
 * @param[in] value float
 * @return result float
*/
inline float axRound(const float value)
{
  return (float)(int)(value + 0.5);
}

/**
 * fast modf() for floating point values. <br> 
 * returns the fractional part of a floating point number
 * and stores the integer part in the second argument.
 * \code
 * // example:
 * value_pi = 3.141592;
 * float integer;  
 * float fraction = axModf(value_pi, &integer);
 * // fraction = 0.141592, integer = 3   
 * \endcode 
 * @param[in] value float - input variable
 * @param[in] intpart float* - pointer to integer part variable
 * @return float - fractional part
 */
inline float axModf(const float value, float* intpart)
{
  *intpart = (float)(int)value;
  return (value - *intpart);
}

/**
 * returns the remainder of the division of two arguments
 * \code
 * // example: 
 * float numerator = 5.3;
 * float denominator = 2;    
 * float result = numerator - (floorf(numerator/denominator) * denominator);
 * // result = 1.3  
 * \endcode  
 * param[in] x float - numerator (divident) 
 * param[in] y float - denominator (devisor or modulus)  
 */
inline float axFmod(const float x, const float y)
  {
  register float value;
  __asm__ __volatile__ ("":::);
  __asm__
  (
    // gets remainder; copy floating point status register into ax register;
    // copy the ah register into the condition code bits (cc);
    // jump if parity;
    "1: fprem;"    "fstsw %%ax;"   "sahf;"   "jp 1b;"
    : "=t" (value) : "0" (x), "u" (y) : "ax", "cc"
  );
  return value;
}

/**
 * returns the absolute value of a floating point number
 * @param[in] value float
 * @return value float
*/
inline float axAbs(const float value)
{
  int i=((*(int*)&value)&0x7fffffff);
  return (*(float*)&i);
}

/**
 * returns the negative of a floating point number
 * @param[in] value float
 * @return value float
*/
inline float axNeg(float value)
{
  __asm__ __volatile__ ("":::);
  __asm__ ( "xorl $0x80000000, %0;"    : "=r" (value)    : "0" (value) );
  return value;
}

/**
 * returns the sign (-1 or 1) of a floating point number
 * @param[in] value float
 * @return value float
*/
inline float axSign(const float value)
{
  return 1.0f + (((*(int*)&value) >> 31) << 1);
}

/**
 * returns the smaller of two floating point numbers
 * @param[in] a float
 * @param[in] b float
 * @return result float
 */
inline float axMin(const float a, const float b)
{
  return (a < b) ? a : b;
}

/**
 * returns the larger of two floating point numbers
 * @param[in] a float
 * @param[in] b float
 * @return result float
 */
inline float axMax(const float a, const float b)
{
  return (a > b) ? a : b;
}

/**
 * limits a floating point number to [-limit, limit]
 * @param[in] input float
 * @param[in] limit float
 * @return result float
 */
inline float axLimit(const float input, const float limit)
{
  return axMin(axMax(input, -limit), limit);
}

/**
 * returns the smaller of two integer numbers
 * @param[in] a int
 * @param[in] b int
 * @return result float
 */
inline int axMinInt(const int a, const int b)
{
  return (a < b) ? a : b;
}

/**
 * returns the larger of two integer numbers
 * @param[in] a int
 * @param[in] b int
 * @return result float
 */
inline int axMaxInt(const int a, const int b)
{
  return (a > b) ? a : b;
}

/**
 * limits an integer number to [-limit, limit]
 * @param[in] input int
 * @param[in] limit int
 * @return a_or_b float
 */
inline int axLimitInt(const int input, const int limit)
{
  return axMinInt(axMaxInt(input, -limit), limit);
}

/**
 * calculates min(floor(a*b), c - 1)
 * @param[in] a float
 * @param[in] b float
 * @param[in] c float
 * @return result float
 */
inline float axCalcStep(const float a, const float b, const float c)
{
  return axMin(axFloor(a*b), c - 1);
}

/**
 * calculates (a*b + c)
 * @param[in] a float
 * @param[in] b float
 * @param[in] c float
 * @return result float
 */
inline float axCalcValuep(const float a, const float b, const float c)
{
  return (a*b + c);
}

/**
 * passes a seed to the random number generator
 * @param[in] aSeed int default value -> use ctime
 */
inline void axRandomize(const int aSeed = (unsigned)time(0))
{
    srand(aSeed);
}

/**
 * returns a random float number between [0..f]
 * @param[in] f float default value (1)
 * @return result float
 */
inline float axRandom(const float f = 1)
{
  return (f * (float)rand() / (float)RAND_MAX);
}

/**
 * returns a random integer between [0..i]
 * @param[in] i int
 * @return result int
 */
inline int axRandomInt(const int i)
{
  //const float f = axRandom(i + 1);
  //return axMinInt(i, (int)axFloor(f));
  return axMinInt(i, (int)axFloor(axRandom(i + 1)));
}

/**
 * returns a random floating point number between [-1..1]
 * @return result float
 */
inline float axRandomSigned(void)
{
  return (2 * (float)rand() / (float)RAND_MAX) - 1;
}

/**
 * returns a random float number between [aLow..aHigh]
 * @param[in] aLow float
 * @param[in] aHigh float
 * @return result float
 */
inline float axRandom(const float aLow, const float aHigh)
{
  /*
    float range = aHigh-aLow;
    float rnd = axRandom();
    return aLow + rnd*range;
  */
  return aLow + axRandom()*(aHigh - aLow);
}

/**
 * calculates the logarithm base 2 of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axLog2f(float value)
{
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fld1;"   "fxch;"  "fyl2x;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the logarithm base 2 function
 * based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
 * @param[in] val float
 * @return result float
 */
inline float axLog2(float val)
{
  assert (val > 0);
  int* const exp_ptr = reinterpret_cast<int*>(&val);
  int x = *exp_ptr;
  const int log_2 = ((x >> 23) & 255) - 128;
  x &= ~(255 << 23);
  x += 127 << 23;
  *exp_ptr = x;
  return (val + log_2);
}

/**
 * calculates the natural logarithm (base e) of a floating point number
 * @param[in] value float
 * @return value float
 */
inline float axLogf(float value)
{
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fld %0;"    "fldln2;"    "fxch;"    "fyl2x;"
    : "=t" (value)
    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the natural logarithm function
 * based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
 * @param[in] val float
 * @return result float
 */
inline float axLog(const float &val)
{
  return (axLog2(val)*0.69314718f);
}

/**
 * calculates the logarithm base 10 of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axLog10f(float value)
{
  __asm__ __volatile__("":::);
  __asm__
  (
    "fldlg2;"    "fxch;"    "fyl2x;"
    : "=t" (value): "0" (value)
  );
  return value;
}

/**
 * calculates the logarithm base 10 of a floating point number
 * @param[in] x float
 * @return value float
 */
inline float axLog10(const float x)
{
  // log10(e) = 0.4342945239647
  // also: log10(x) = log2(x) - ln(x)
  const float y = (x - 1)/(x + 1);
  const float y2 = y*y;
  return (2.f*y*(1 + y2*0.3333333333f + y2*y2*0.2f))*0.4342945239647f;
}

/**
 * performs fast and accurate powf(float, float) approximation (fpu)
 * optimized for fractional exponent. <br>
 * for (long) integer exponent use axPow() <br>
 * \code
 * axPow(3.5f, 2);
 * axPowf(3.5f, 2.5f);
 * \endcode  
 * @param[in] x float - base
 * @param[in] y float - exponent
 * @return float
 */
inline float axPowf(const float x, const float y)
{
  register float value, exponent;
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fld1;"                       // |  
    "fxch;"                       // |
    "fyl2x;"                      // log2(x)
    "fmul %%st(1);"               // y*log2(x)
    "fst %%st(1);"                // |
    "frndint;"                    // int(y*log2(x))
    "fxch;"                       // |
    "fsub %%st(1);"               // fract(y*log2(x))
    "f2xm1;"                      // 2^(fract(y*log2(x))) - 1
    "fld1;"                       // |
    "faddp;"                      // += 1    
    "fscale;"                     // x*(2^exp)
    : "=t" (value), "=u" (exponent)   :"0" (x), "1" (y)
  );
  return value;
}

/**
 * performs fast pow(float, integer)
 * @param[in] x float
 * @param[in] n int
 * @return result float
 */
inline float axPow(float x, unsigned int n)
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

/**
 * approximation of [e] to the power of a number (fpu) 
 * @param[in] x float input value
 * @return value float
 */
inline float axExpf(const float x)
{
  register float value, exponent;
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fldl2e;"               // e^x = 2^(x*log2(e))
    "fmul %%st(1);"         // x * log2(e)"
    "fst %%st(1);"          // |
    "frndint;"              // int(x*log2(e))
    "fxch;"                 // |
    "fsub %%st(1);"         // fract(x*log2(e))
    "f2xm1;"                // 2^(fract(x*log2(e)))
    "fld1;"                 // |
    "faddp;"                // += 1
    "fscale;"               // x*(2^exp)
    : "=t" (value), "=u" (exponent) : "0" (x)
  );
  return value;
}

/**
 * fast approximation of [e] to the power of a number <br> 
 * based on http://theoval.sys.uea.ac.uk/publications/pdf/nc2000a.pdf <br> 
 * note: original is for double precision (has a double to float cast)
 * @param[in] exponent float
 * @return result float
 */
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

/**
 * Returns the result of x*(2^floor(y)) <br>
 * ( significand (x) multiplied by the exponent(2^y) )
 * \code
 * // example: 
 * float sig = 2.f;
 * float exponent = 4.1f; // will be truncated to 4.0f
 * float result = axLdexp(
 * // result = 32.f
 * \endcode  
 * param[in] x float - significand
 * param[in] y float - denominator (devisor or modulus)  
 */
float axLdexp(const float x, const float y)
{
  register float value;
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fscale;"  : "=t" (value) : "0" (x), "u" (y)
  );
  return value;
}

/**
 * fast approximation of a N-th root function
 * @param[in] value float
 * @param[in] root long
 * @return value float
 */
inline float axNrt(float value, long root)
{
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "subl $0x3f800000, %0;"    "subl $1, %2;"
    "shrl %b2, %0;"            "addl $0x3f800000, %0;"
    : "=r" (value)
    : "0" (value), "c" (root)
  );
  return value;
}

/**
 * returns the squre root of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axSqrtf(float value)
{
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fsqrt;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the squre root function
 * @param[in] value float
 * @return value float
 */
inline float axSqrt(float value)
{
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "subl $0x3f800000, %0;"    "shrl $1, %0;"    "addl $0x3f800000, %0;"
    : "=r" (value)
    : "0" (value)
  );
  return value;
}

/**
 * returns the invert squre root of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axInvSqrtf(float value)
{
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fsqrt;"  "fld1;"   "fdivp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the invert squre root function
 * based on code found in 'quake 3 arena' by 'id software'
 * @param[in] x float
 * @return result float
 */
inline float axInvSqrt(float x)
{
  float xhalf = 0.5f*x;
  int i = *(int*)&x;
  i = 0x5f3759df - (i>>1);
  x = *(float*)&i;
  return x*(1.5f - xhalf*x*x);
}

/**
 * calculates the sine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axSinf(float value)
{
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fsin;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the cosine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axCosf(float value)
{
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fcos;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the tangens of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axTanf(float value)
{
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fptan;"  "fstp %1;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}


/**
 * calculates both the sine and cosine of a floating point number (fpu)
 * \code
 * // example:
 * float sinx;
 * float cosx;
 * axSinCosf(x, &sinx, &cosx);
 * // sinx and cosx will recieve the results
 * \endcode
 * @param[in] x float input variable
 * @param[in] sin float* pointer to sin value
 * @param[in] cos float* pointer to cos value
 * @return void
 */
inline void axSinCosf(float x, float* sin, float* cos)
{
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fsincos;" : "=t" (*cos), "=u" (*sin) : "0" (x)
  );
}

/**
 * calculates the arc-sine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axAsinf(float value)
{
  // asin(x)=atan(sqrt(x*x/(1-x*x)))
  register float tmp;
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fld %0;"    "fld %0;"    "fmulp;"    "fst %1;"    "fld1;"    "fsubp;"
    "fld %1;"    "fdivp;"    "fsqrt;"    "fld1;"    "fpatan;"   "fst %0;"
    : "=m" (value)  : "m" (tmp)
  );
  return value;
}

/**
 * calculates the arc-cosine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axAcosf(float value)
{
  // acos(x) = atan(sqrt((1-x*x)/(x*x)))
  register float tmp;
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fld %0;"    "fld %0;"    "fmulp;"    "fst %1;"    "fld1;"    "fsubp;"
    "fld %1;"    "fdivrp;"    "fsqrt;"    "fld1;"    "fpatan;"    "fst %0;"
    : "=m" (value)    : "m" (tmp)
  );
  return value;
}

/**
 * calculates the arc-tangens of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axAtanf(float value)
{
  // from partial tangens  
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fld1;"    "fpatan;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the angle in radians between the positive x-axis of a plane
 * and the point given by the coordinates (x, y) on it.
 * @param[in] x float - x coordinate
 * @param[in] y float - y coordinate
 * @return value float
 */
inline float axAtan2f(const float y, const float x)
{
  register double value;
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fpatan;"    "fld %%st(0);"    : "=t" (value) : "0" (x), "u" (y)
  );
  return value;
}

/**
 * calculates the contangens of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axCotanf(float value)
{
  // cotan(x) = 1/tan(x)
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fsincos;"    "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the cosecant of a a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axCscf(float value)
{
  // csc(x) = 1/sin(x)
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fsin;"   "fld1;"   "fdivrp;"    
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the secant of a a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axSecf(float value)
{
  // sec(x) = 1/cos(x)
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fcos;"   "fld1;"   "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the arccotangent of a a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axAcotanf(float value)
{
  // arccotan(x) = atan(1/x)
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fld1;"   "fxch;"   "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the arcsecant of a a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axAsecf(float value) (fpu)
{
  // asec(x) = atan(sqrt(x*x-1))  
  __asm__ __volatile__ ("":::);
  __asm__
  (
  "fld %0;"   "fmulp;"    "fld1;"   "fsubp;"    "fsqrt;"
  "fld1;"     "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the arccosecant of a a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
inline float axAcscf(float value)
{
  // acsc(x) = atan(sqrt(1/(x*x-1)))
  __asm__ __volatile__ ("":::);
  __asm__
  (
    "fld %0;"   "fmulp;"    "fld1;"   "fsubp;"    "fld1;"   "fdivrp;"
    "fsqrt;"    "fld1;"     "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the sine function for range [-pi, pi]
 * @param[in] x float
 * @return result float
 */
inline float axSin(float x)
{
  x = x * (1.2732395447f - 0.4052847345f * axAbs(x));
  return 0.225f * (x * axAbs(x) - x) + x;
}

/**
 * fast approximation of the cosine function for range [-pi, pi]
 * @param[in] x float
 * @return result float
 */
inline float axCos(float x)
{
  const float x2 = x*x;
  return (15120 + x2*(-6900 + 313*x2)) / (15120 + x2*(660 + 13*x2));
}

/**
 * fast approximation of the tangens function for range [-pi, pi]
 * @param[in] x float
 * @return result float
 */
inline float axTan(float x)
{
  const float x2 = x*x;
  return (x*(105 - 10*x2)) / (105 - x*(45*x - x*x2));
}

/**
 * fast approximation of the arc-sine function for range [-1, 1]
 * @param[in] x float
 * @return result float
 */
inline float axAsin(const float x)
{
  return M_PI_2 - axSqrtf(1 - x)*(1.5707288 - x*(0.2121144 + x*(0.0742610 -
  x*(0.0187293 + 0.395*x))));
}

/**
 * fast approximation of the arc-cosine function for range [-1, 1]
 * @param[in] x float
 * @return result float
 */
inline float axAcos(const float x)
{
  const float x2 = x*x;
  return x*(x2*(-0.55*x2 + 0.097) - 1.008) + 1.571;
}

/**
 * fast approximation of the arc-tangens function for range [-2, 2]
 * @param[in] x float
 * @return result float
 */
inline float axAtan(const float x)
{
  const float x2 = x*x;
  return (x*(105 + 55*x2)) / (105 + x2*(90 + 9*x2));
}

/**
 * approximation of the hyperbolic-sine function for range (fpu)
 * @param[in] x float
 * @return result float
 */
inline float axSinhf(const float x)
{
  if(x >= 0.0f)
  {
    const float _e = axExpf(x);
    return (_e - 1.0f/_e)*0.5f;
  }
  else
  {
    const float _eneg = axExpf(-x);
    return (1.0f/_eneg - _eneg)*0.5f;
  }
}

/**
 * fast approximation of the hyperbolic-sine function for range [-3.5, 3.5]
 * @param[in] x float
 * @return result float
 */
inline float axSinh(const float x)
{
  const float x2 = x*x;
  return x*(x2*(0.012*x2 + 0.156) + 1.004);
}

/**
 * approximation of the hyperbolic-cosine function (fpu)
 * @param[in] x float
 * @return result float
 */
inline float axCoshf(const float x)
{
  const float _e = axExpf(axAbs(x));
  return (_e + 1.0f/_e)*0.5f;
}

/**
 * fast approximation of the hyperbolic-cosine function for range [-3.5, 3.5]
 * @param[in] x float
 * @return result float
 */
inline float axCosh(const float x)
{
  const float x2 = x*x;
  return x2*(0.065*x2 + 0.428) + 1.025;
}

/**
 * approximation of the hyperbolic-tangens function for range [-50.f, 50.f] (fpu)
 * @param[in] x float
 * @return result float
 */
inline float axTanf(const float x)
{
  if (x > 50)
    return 1;
  else if (x < -50)
    return -1;
  else
  {
    const float _e = axExpf(x);
    const float _er = 1.f/_e;
    return (_e - _er) / (_e + _er);
  }
}

/**
 * fast approximation of the hyperbolic-tangens function for range [-4.2, 4.2]
 * @param[in] x float
 * @return result float
 */
inline float axTanh(const float x)
{
  const float x2 = x*x;
  return x*(27 + x2) / (27 + 9*x2);
}

/**
 * converts linear value to decibel
 * @param[in] lin float
 * @return result float
 */
inline float lin2dB(const float lin)
{
  return LOG2DB * axLogf(lin);
}

/**
 * converts decibel value to linear
 * @param[in] dB float
 * @return result float
 */
inline float dB2lin(const float dB)
{
  return axExpf(DB2LOG * dB);
}

/**
 * calculate the average value of a set of floating point numbers
 * example: <br>
 * \code
 * axAvrg(3, -1.f, 3.5f, 5.f); // result is 2.5
 * \endcode
 * @param[in] count unsigned int - number of elements (n)
 * @param[in] elements[0-n] float - elements
 * @return float
 */
inline float axAvrg(const unsigned int n,...)
{
  va_list ap;
  float total = 0.f;
  va_start(ap, n);
  for(unsigned int i = 0; i < n; i++)
    total += va_arg(ap, double);
  va_end(ap);
  return total/n;
}

/**
 * calculate the average value of a set of integers
 * example: <br>
 * \code
 * axAvrgInt(5, -2, -1, 1, 5, 7); // result is 2
 * \endcode
 * @param[in] count unsigned int - number of elements (n)
 * @param[in] elements[0-n] int - elements
 * @return int
 */
inline int axAvrgInt(const unsigned int n,...)
{
  va_list ap;
  int total = 0;
  va_start(ap, n);
  for(unsigned int i = 0; i < n; i++)
    total += va_arg(ap, int);
  va_end(ap);
  return total/n;
}

/**
 * calculate the RMS of a set (array) of float numbers
 * example:
 * \code
 * float ar[] = { 1.f, 2.f, 3.f, 4.f, 5.f };
 * float result = axRMS(5, ar);   // result = 3.31662
 * \endcode
 * @param[in] n unsigned int - size of the array
 * @param[in] ar float* - array of floats  
 *   
 * @return float
 */
inline float axRMS(const unsigned int n, const float* ar)
{
  float numr = 0;
  for (unsigned int i=0; i<n; i++)
  {
    numr += axSqr(ar[i]); // ar[i] * ar[i]
  }
  return axSqrtf(numr/n);
}

#endif
