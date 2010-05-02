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
 
/**
 * \file axMath.h
 * \brief math approximations and routines
 *
 * axonlib's math header with a collection of optimized functions for trigonometry,
 * algebra and analysis. <br>
 * some of the methods have a 'f' suffix version (e.g.: axTanf).
 * such versions use FPU instructions for the calculation (FPU x87 required).
 * the versions without 'f' suffix are most likely less cpu heavy approximations,
 * but with reduced accuracy and acceptable input range (e.g. axCos). <br>
 * <br> 
 * <b>notes:</b> <br> <br>
 * gcc 4.x outperforms 3.x many times with -O2, -O3 <br>
 * you can get gcc 4.x for windows at http://www.tdragon.net/recentgcc/    
  <br>
  <br>
  <b>short performance table for axMath.h</b>: <br>
  <br>
  cpu: amd athlon 1800+ <br>
  parameters: -O3 -msse -mfpmath=sse,387 <br>
  input values: pi, -pi, 1, -1, other <br>
  iterations: 10e+5 or more <br>
  <br>
  *some comparisons may use a weighting function to reduce the number
  of iterations <br>    
  <br>
  <table>  <tr>  <td>
  axFloor: 5ms <br>  floorf: 25ms <br>  
  axRound: 6ms <br>  roundf: 42ms <br>  
  axAbs: 36ms <br>  fabs: 63ms <br>  
  axLog2: 7ms <br>  axLog2f: 69ms <br>  log2f: 75ms <br>  
  axLogf: 20ms <br>  logf: 80ms <br>   
  axLog10: 30ms <br>  axLog10f: 69ms <br>  log10f: 75ms <br>
  </td>
  <td>
  axPowf: 96ms <br>  powf: 161ms <br>  
  axExp: 3ms <br>  axExpf: 34ms <br>  expf: 137ms <br>  
  axSqrt: <1ms <br>  axSqrtf: <1ms <br>  sqrtf: 27ms <br>  
  axSin: 56ms <br>  axSinf: 98ms <br>  sinf: 114ms <br>  
  axAcosf: <1ms <br>  acosf: 75ms <br>  
  axTanhf: 70ms <br>  tanf: 194ms <br>
  </td>  </tr>  </table>
  <br>
  */

// -----------------------------------------------------------------------------
#ifndef axMath_included
#define axMath_included

#include "axDefines.h"

#ifdef AX_USE_HOT_INLINE
  #define __axmath_inline __hotinline
#else
  #define __axmath_inline inline
#endif

//strip:
//---------------
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
//----


// ### set ( | test only) some deprecated warnings on compile time
__deprecated float fabs(float x) { return __builtin_fabs(x); }
__deprecated float sinf(float x) { return __builtin_sinf(x); }
__deprecated float cosf(float x) { return __builtin_cosf(x); }
__deprecated float tanf(float x) { return __builtin_tanf(x); }
__deprecated int sin(int x) { return __builtin_sin(x); }
__deprecated int cos(int x) { return __builtin_cos(x); }
__deprecated int tan(int x) { return __builtin_tan(x); }
__deprecated int pow(int x, int y) { return __builtin_pow(x, y); }
__deprecated float powf(double x, float y) { return __builtin_powf(x, y); }

/**
 * invert of x: (x^2)
 */
#define axInv(x) (1/(x))


// #### deprecated (?)
/**
 * square of x: (x^2)
 */
#define axSqr(x) ((x)*(x))
// ####  ------------

/**
 * cube of x: (x^3)
 */
#define axCube(x) ((x)*(x)*(x))

// #### deprecated
/**
 * newton step 1 (newton's method) <br>
 * as seen in axInvSqrt <br>
 * \code
 * y = x*(1.5f - (x/2)*x*x);
 * \endcode
*/
#define axNewtonStep(x) { (x) = (x)*(1.5f - (x)*0.5f)*(x)*(x); }
// ####  ------------

// #### deprecated
/**
 * newton step 2 (newton's method) <br>
 * \code
 * new_approx = (old_approx + x/old_approx) / 2;
 * \endcode
*/
#define axNewtonStep2(y, x) { (y)=((y)+(x)/(y))*0.5f; }
// #### -------------

/**
 * convert radians to degrees <br>
 * \code
 * deg = rad*(180/pi);
 * \endcode
 */
#define axRad2Deg(x) ( (x)*57.2957795130823f )

/**
 * convert degrees to radians
 * \code
 * rad = deg*(pi/180);
 * \endcode
 */
#define axDeg2Rad(x) ( (x)*0.01745329251994f )

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
__axmath_inline float axFloor(const float value)
{
  return (float)(int)(value);
}

/**
 * returns the ceil of a floating point number
 * @param[in] value float
 * @return result float
 */
__axmath_inline float axCeil(const float value)
{
  return (float)(int)(value + 1.f);
}

/**
 * rounds a floating point number
 * @param[in] value float
 * @return result float
*/
__axmath_inline float axRound(const float value)
{
  return (float)(int)(value + 0.5f);
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
__axmath_inline float axModf(const float value, float* intpart)
{
  *intpart = (float)(int)value;
  return (value - *intpart);
}

/**
 * returns the remainder of the division of two arguments
 * \code
 * // example:
 * float numerator = 5.3f;
 * float denominator = 2.f;
 * float result = numerator - (floorf(numerator/denominator) * denominator);
 * // result = 1.3
 * \endcode
 * @param[in] x float - numerator (divident)
 * @param[in] y float - denominator (devisor or modulus)
 */
__axmath_inline float axFmod(const float x, const float y)
  {
  register float value;
  //__asmv ("":::);
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
__axmath_inline float axAbs(const float value)
{
  // alt: fpu fabs is slower
  union
  {
    int i;
    float j;
  } u;
  u.j = value;
  u.i &= 0x7fffffff;
  return u.j;  
}

/**
 * returns the negative of a floating point number
 * @param[in] value float
 * @return value float
*/
__axmath_inline float axNeg(float value)
{
  if (value != 0.f)
  {
    //__asmv ("":::);
    __asm__ ( "xorl $0x80000000, %0;"    : "=r" (value)    : "0" (value) );
    return value;
  } else { return 0.f; }
}

/**
 * returns the sign (-1, 1 or 0) of a floating point number
 * @param[in] value float
 * @return value float
*/
__axmath_inline float axSign(const float value)
{
  if (value != 0.f)
  {
    int p = (int)value;
    return (1 | (p >> 31));
  } else { return 0.f; }
}

/**
 * returns the smaller of two floating point numbers
 * @param[in] a float
 * @param[in] b float
 * @return result float
 */
__axmath_inline float axMin(const float a, const float b)
{
  return (a < b) ? a : b;
}

/**
 * returns the larger of two floating point numbers
 * @param[in] a float
 * @param[in] b float
 * @return result float
 */
__axmath_inline float axMax(const float a, const float b)
{
  return (a > b) ? a : b;
}

/**
 * limits a floating point number to [-limit, limit]
 * @param[in] input float
 * @param[in] limit float
 * @return result float
 */
__axmath_inline float axLimit(const float input, const float limit)
{
  register float _t = (input > -limit) ? input : -limit;
  return (_t > limit) ? _t : limit;
}

/**
 * returns the smaller of two integer numbers
 * @param[in] a int
 * @param[in] b int
 * @return result float
 */
__axmath_inline int axMinInt(const int a, const int b)
{
  return (a < b) ? a : b;
}

/**
 * returns the larger of two integer numbers
 * @param[in] a int
 * @param[in] b int
 * @return result float
 */
__axmath_inline int axMaxInt(const int a, const int b)
{
  return (a > b) ? a : b;
}

/**
 * limits an integer number to [-limit, limit]
 * @param[in] input int
 * @param[in] limit int
 * @return a_or_b float
 */
__axmath_inline int axLimitInt(const int input, const int limit)
{
  register float _t = (input > -limit) ? input : -limit;
  return (_t > limit) ? _t : limit;
}

/**
 * calculates min(floor(a*b), c - 1)
 * @param[in] a float
 * @param[in] b float
 * @param[in] c float
 * @return result float
 */
__deprecated __axmath_inline float axCalcStep(const float a, const float b, const float c)
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
__deprecated __axmath_inline float axCalcValuep(const float a, const float b, const float c)
{
  return (a*b + c);
}

// ###################################
// depreciated stdlib.h -> rand()
// 
// using: axRand.h
// ###################################

/**
 * passes a seed to the random number generator
 * @param[in] aSeed int default value -> use ctime
 */
 
__deprecated __axmath_inline void axRandomize(const int aSeed = (unsigned)time(0))
{
    srand(aSeed);
}

/**
 * returns a random float number between [0..f]
 * @param[in] f float default value (1)
 * @return result float
 */

__deprecated __axmath_inline float axRandom(const float f = 1)
{
  return (f * (float)rand() / (float)RAND_MAX);
}

/**
 * returns a random integer between [0..i]
 * @param[in] i int
 * @return result int
 */

__deprecated __axmath_inline int axRandomInt(const int i)
{
  //const float f = axRandom(i + 1);
  //return axMinInt(i, (int)axFloor(f));
  return axMinInt
    (i, (int)axFloor( (i + 1) * (float)rand() / (float)RAND_MAX ) );
}

/**
 * returns a random floating point number between [-1..1]
 * @return result float
 */
 
__deprecated __axmath_inline float axRandomSigned(void)
{
  return (2 * (float)rand() / (float)RAND_MAX) - 1;
}

/**
 * returns a random float number between [aLow..aHigh]
 * @param[in] aLow float
 * @param[in] aHigh float
 * @return result float
 */
 
__deprecated __axmath_inline float axRandom(const float aLow, const float aHigh)
{
  
    //float range = aHigh-aLow;
    //float rnd = axRandom();
    //return aLow + rnd*range;
  
  return aLow + ( (float)rand() / (float)RAND_MAX ) *(aHigh - aLow);
}

// #####################


/**
 * calculates the logarithm base 2 of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axLog2f(float value)
{
  //__asmv ("":::);
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
__axmath_inline float axLog2(const float val)
{
  if (val > 0.f)
  {
    union
    {
      int i;
      float j;
    } u;
    u.j = val;
    const int log_2 = ((u.i >> 23) & 255) - 128;
    u.i &= ~(255 << 23);
    u.i += 127 << 23;
    return (0.05f + u.j + (float)log_2);
  }
  else
    return 0.f;
}

/**
 * calculates the natural logarithm (base e) of a floating point number
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axLogf(float value)
{
  //__asmv ("":::);
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
__axmath_inline float axLog(const float &val)
{
  return (axLog2(val)*0.69314718f);
}

/**
 * calculates the logarithm base 10 of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axLog10f(float value)
{
  //__asmv("":::);
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
__axmath_inline float axLog10(const float x)
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
__axmath_inline float axPowf(const float x, const float y)
{
  register float value, exponent;
  //__asmv ("":::);
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
__axmath_inline float axPow(float x, unsigned long n)
{
  register float res = 1;
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
__axmath_inline float axExpf(const float x)
{
  register float value, exponent;
  //__asmv ("":::);
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
__axmath_inline float axExp(const float exponent)
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
 * returns the result of x*(2^floor(y)) <br>
 * ( significand (x) multiplied by the exponent(2^y) )
 * \code
 * // example:
 * float sig = 2.f;
 * float exponent = 4.1f; // will be truncated to 4.0f
 * float result = axFscale(sig, exponent);
 * // result = 32.f
 * \endcode
 * @param[in] x float - significand
 * @param[in] y float - exponent
 * @return value float
 */
__axmath_inline float axFscale(const float x, const float y)
{
  register float value;
  //__asmv ("":::);
  __asm__
  (
    "fscale;"  : "=t" (value) : "0" (x), "u" (y)
  );
  return value;
}

/**
 * separates the input variable x into significand*(2^exponent)<br>
 * uses IEEE logb(x)
 * \code
 * // example:
 * float value = 3.141592;
 * float exponent;
 * float significand = axFxtract(value, &exponent);
 * // significand = 1.5708, exponent = 1
 * // 3.141592 = 1.5708 * (2^1)
 * // -----------------------------------
 * // NOTE: using frexp() from libm
 * // significand = 0.785398, exponent = 2
 * // 3.141592 = 0.785398 * (2^2)
 * \endcode
 * @param[in] value float - input value
 * @param[in] _exp float* - pointer to the exponent variable
 * @return sig float - value of significand
 */
__axmath_inline float axFxtract(float value, float* _exp)
{
  register float sig;
  __asmv("":::);
  __asm__
  (
    "fxtract;"   :"=t"(sig), "=u"(*_exp)   :"0"(value)
  );
  return sig;
}

/**
 * rought approximation of a N-th root function (inaccurate) <br>
 * use axPowf(x, 1/n) instead.
 * @param[in] value float
 * @param[in] root long
 * @return value float
 */
__axmath_inline float axNrt(float value, long root)
{
  // alt: newton-ramson, is still slower than the fpu axPowf
  // xn = (1/n)*((n-1)*xn + x/pow(xn, n-1));
  //__asmv ("":::);
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
 * returns the square root of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axSqrtf(float value)
{
  //__asmv ("":::);
  __asm__
  (
    "fsqrt;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the squre root function <br>
 * based on: http://www.azillionmonkeys.com/qed/sqroot.html
 * @param[in] x float
 * @return value float
 */
__axmath_inline float axSqrt(const float x)
{
  const float halfx = x*0.5;
  union
  {
    int i;
    float j;    
  } u;
  u.j = x;
  u.i = (0xbe6ec85f - u.i) >> 1;   // good initial guess  
  return x*u.j*(1.5f - u.j*u.j*halfx) + 0.001f; // newton iteration
}

/**
 * returns the invert squre root of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axInvSqrtf(float value)
{
  //__asmv ("":::);
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
__axmath_inline float axInvSqrt(const float x)
{
  const float halfx = 0.5f*x;
  union
  {
    float j;
    int i;
  } u;
  u.j = x;
  u.i = 0x5f3759df - (u.i >> 1); // good initial guess
  return u.j*(1.5f - u.j*u.j*halfx) + 0.001f; // newton iteration  
}

/**
 * calculates the sine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axSinf(float value)
{
  //__asmv ("":::);
  __asm__
  (
    "fsin;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the sine function for range [-pi, pi]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axSin(float x)
{
  x = x * (1.2732395447f - 0.4052847345f * axAbs(x));
  return 0.225f * (x * axAbs(x) - x) + x;
}

/**
 * calculates the cosine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axCosf(float value)
{
  //__asmv ("":::);
  __asm__
  (
    "fcos;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the cosine function for range [-pi, pi]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axCos(const float x)
{
  const float x2 = x*x;
  return (15120 + x2*(-6900 + 313*x2)) / (15120 + x2*(660 + 13*x2));
}

/**
 * calculates the tangens of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axTanf(float value)
{
  //__asmv ("":::);
  __asm__
  (
    "fptan;"  "fstp %1;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the tangens function for range [-pi, pi]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axTan(const float x)
{
  const float x2 = x*x;
  return (x*(105 - 10*x2)) / (105 - x2*(45 - x2));
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
__axmath_inline void axSinCosf(const float x, float* sin, float* cos)
{
  //__asmv ("":::);
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
__axmath_inline float axAsinf(float value)
{
  // asin(x)=atan(sqrt(x*x/(1-x*x)))
  register float tmp;
  //__asmv ("":::);
  __asm__
  (
    "fld %0;"    "fld %0;"    "fmulp;"    "fst %1;"    "fld1;"    "fsubp;"
    "fld %1;"    "fdivp;"    "fsqrt;"    "fld1;"    "fpatan;"   "fst %0;"
    : "=t" (value)  : "0" (tmp)
  );
  return value;
}

/**
 * fast approximation of the arc-sine function for range [-1, 1]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axAsin(const float x)
{
  return M_PI_2 - axSqrtf(1 - x)*(1.5707288 - x*(0.2121144 + x*(0.0742610 -
  x*(0.0187293 + 0.395*x))));
}

/**
 * calculates the arc-cosine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAcosf(float value)
{
  // acos(x) = atan(sqrt((1-x*x)/(x*x)))
  register float tmp;
  //__asmv ("":::);
  __asm__
  (
    "fld %0;"    "fld %0;"    "fmulp;"    "fst %1;"    "fld1;"    "fsubp;"
    "fld %1;"    "fdivrp;"    "fsqrt;"    "fld1;"    "fpatan;"    "fst %0;"
    : "=t" (value)    : "0" (tmp)
  );
  return value;
}

/**
 * fast approximation of the arc-cosine function for range [-1, 1]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axAcos(const float x)
{
  const float x2 = x*x;
  return x*(x2*(-0.55*x2 + 0.097) - 1.008) + 1.571;
}

/**
 * calculates the arc-tangens of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAtanf(float value)
{
  // from partial tangens
  //__asmv ("":::);
  __asm__
  (
    "fld1;"    "fpatan;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the arc-tangens function for range [-2, 2]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axAtan(const float x)
{
  const float x2 = x*x;
  return (x*(105 + 55*x2)) / (105 + x2*(90 + 9*x2));
}

/**
 * calculates the angle in radians between the positive x-axis of a plane
 * and the point given by the coordinates (x, y) on it.
 * @param[in] x float - x coordinate
 * @param[in] y float - y coordinate
 * @return value float
 */
__axmath_inline float axAtan2f(const float y, const float x)
{
  register float value;
  //__asmv ("":::);
  __asm__
  (
    "fpatan;"    "fld %%st(0);"    : "=t" (value) : "0" (x), "u" (y)
  );
  return value;
}

/**
 * calculates the cotangens of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axCotanf(float value)
{
  // cotan(x) = 1/tan(x)
  //__asmv ("":::);
  __asm__
  (
    "fsincos;"    "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the cosecant of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axCscf(float value)
{
  // csc(x) = 1/sin(x)
  //__asmv ("":::);
  __asm__
  (
    "fsin;"   "fld1;"   "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the secant of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axSecf(float value)
{
  // sec(x) = 1/cos(x)
  //__asmv ("":::);
  __asm__
  (
    "fcos;"   "fld1;"   "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the arccotangent of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAcotanf(float value)
{
  // arccotan(x) = atan(1/x)
  //__asmv ("":::);
  __asm__
  (
    "fld1;"   "fxch;"   "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the arcsecant of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAsecf(float value)
{
  // asec(x) = atan(sqrt(x*x-1))
  //__asmv ("":::);
  __asm__
  (
  "fld %0;"   "fmulp;"    "fld1;"   "fsubp;"    "fsqrt;"
  "fld1;"     "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the arccosecant of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAcscf(float value)
{
  // acsc(x) = atan(sqrt(1/(x*x-1)))
  //__asmv ("":::);
  __asm__
  (
    "fld %0;"   "fmulp;"    "fld1;"   "fsubp;"    "fld1;"   "fdivrp;"
    "fsqrt;"    "fld1;"     "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * approximation of the hyperbolic-sine function for range (fpu)
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axSinhf(const float x)
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
__axmath_inline float axSinh(const float x)
{
  const float x2 = x*x;
  return x*(x2*(0.012*x2 + 0.156) + 1.004);
}

/**
 * approximation of the hyperbolic-cosine function (fpu)
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axCoshf(const float x)
{
  const float _e = axExpf(axAbs(x));
  return (_e + 1.0f/_e)*0.5f;
}

/**
 * fast approximation of the hyperbolic-cosine function for range [-3.5, 3.5]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axCosh(const float x)
{
  const float x2 = x*x;
  return x2*(0.065*x2 + 0.428) + 1.025;
}

/**
 * approximation of the hyperbolic-tangens function for range [-50, 50] (fpu)
 * @param[in] value const float
 * @return result float
 */
__axmath_inline float axTanhf(const float value)
{
  if (value > 50)
    return 1;
  else if (value < -50)
    return -1;
  else
  {
    const float _e = axExpf(value);
    const float _er = 1.f/_e;
    return (_e - _er) / (_e + _er);
  }
}

/**
 * fast approximation of the hyperbolic-tangens function for range [-4.2, 4.2]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axTanh(const float x)
{
  const float x2 = x*x;
  return x*(27 + x2) / (27 + 9*x2);
}

/**
 * calculate the average value of a set of floats
 * example: <br>
 * \code
 * float ar[] = { -1.f, 2.f, 3.f, 4.f, 5.f };
 * float result = axAvrg(5, ar); // result is 2.6f
 * \endcode
 * @param n unsigned int - number of elements (n)
 * @param ar float* - array of floats
 * @return float
 */
__axmath_inline float axAvrg(const unsigned int n, const float* ar)
{
  float total = 0;
  for(unsigned int i = 0; i < n; i++) total += ar[i];
  return total/n;
}

/**
 * calculate the average value of a set of integers
 * example: <br>
 * \code
 * int ar[] = { -1, 2, 3, 4, 5 };
 * int result = axAvrgInt(5, ar); // result is 2 (truncated)
 * \endcode
 * @param n unsigned int - number of elements (n)
 * @param ar int* - array of integers
 * @return int
 */
__axmath_inline int axAvrgInt(const unsigned int n, const int* ar)
{
  int total = 0;
  for(unsigned int i = 0; i < n; i++) total += ar[i];
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
__axmath_inline float axRMS(const unsigned int n, const float* ar)
{
  float numr = 0;
  for (unsigned int i=0; i<n; i++) numr += axSqr(ar[i]); // ar[i] * ar[i]
  return axSqrtf(numr/n);
}

#endif
