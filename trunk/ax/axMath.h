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
 * @file
 * \brief math approximations and routines
 */

/**
 * \brief math approximations and routines
 *
 */

#ifndef axMath_included
#define axMath_included

#include <math.h>
#include <stdlib.h>
#include "axDebug.h"
#include "axDefines.h"


/**
 * returns the floor of a floating point number
 * @param[in] value float
 * @return j float
 */

//inline float axFloor(float value)
//{
//  int j;
//  __asm__
//  (
//    "flds %1;"    "fistpl %0;"
//    : "=m" (j)
//    : "m" (value)
//  );
//  return j;
//}

//TRACE("axFloor( 1.4) = %f\n",axFloor( 1.4)); // returns  1
//TRACE("axFloor( 1.6) = %f\n",axFloor( 1.6)); // returns  2, should return 1
//TRACE("axFloor(-1.4) = %f\n",axFloor(-1.4)); // returns -1
//TRACE("axFloor(-1.6) = %f\n",axFloor(-1.6)); // returns -2  (should it floor towards zero, or floor always down?)
//  so, this looks rounding, not flooring
//  quick fix: (returns 1,1,-2,-2)
//  TODO: look into how we're using this throughout the library

inline float axFloor(float value)
  {
    return floorf(value);
  }


/**
 * returns the ceil of a floating point number
 * @param[in] value float
 * @return j float
 */
inline float axCeil(float value)
{
  int j;
  float im = 1.0f;
  __asm__
  (
    "flds %1;"    "flds %2;"    "faddp;"    "fistpl %0;"
    : "=m" (j)
    : "m" (value), "m" (im)
  );
  return j;
}

/**
 * rounds a floating point number
 * @param[in] value float
 * @return j float
*/
inline float axRound(float value)
{
  int j;
  float im = 0.5f;
  __asm__
  (
    "flds %1;"    "fistpl %0;"    "flds %2;"    "faddp;"
    : "=m" (j)
    : "m" (value), "m" (im)
  );
  return j;
}

/**
 * returns the absolute value of a floating point number
 * @param[in] value float
 * @return value float
*/
inline float axAbs(float value)
{
  __asm__
  (
    "andl $0x7fffffff, %0;"
    : "=r" (value)
    : "0" (value)
  );
  return value;
}

/**
 * returns the negative of a floating point number
 * @param[in] value float
 * @return value float
*/
inline float axNeg(float value)
{
  __asm__
  (
    "xorl $0x80000000, %0;"
    : "=r" (value)
    : "0" (value)
  );
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
 * @param[in] aSeed int default value (19)
 */
inline void axRandomize(const int aSeed = 19)
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
  const float f = axRandom(i + 1);
  return axMinInt(i, (int)axFloor(f));
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
  float range = aHigh-aLow;
  float rnd = axRandom();
  return aLow + rnd*range;
}

/**
 * calculates the natural logarithm of a floating point number
 * @param[in] value float
 * @return value float
 */
inline float axLogf(float value)
{
  __asm__
  (
    "fld %0;"    "fldln2;"    "fxch;"    "fyl2x;"    "fst %0;"
    : "=m" (value)
    : "m" (value)
  );
  return value;
}

/**
 * calculates the logarithm base 2 of a floating point number
 * @param[in] value float
 * @return value float
 */
inline float axLog2f(float value)
{
  __asm__
  (
    "fld1;"    "fld %0;"    "fyl2x;"    "fst %0;"
    : "=m" (value)
    : "m" (value)
  );
  return value;
}

/**
 * calculates the sine of a floating point number
 * @param[in] value float
 * @return value float
 */
inline float axSinf(float value)
{
  __asm__
  (
    "fsin;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the cosine of a floating point number
 * @param[in] value float
 * @return value float
 */
inline float axCosf(float value)
{
  __asm__
  (
    "fcos;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the tangens of a floating point number
 * @param[in] value float
 * @return value float
 */
inline float axTanf(float value)
{
  __asm__
  (
    "fsincos;"    "fdivrp;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the arc-sine of a floating point number
 * @param[in] value float
 * @return value float
 */
inline float axAsinf(float value)
{
  // asin(x)=atan(sqrt(x*x/(1-x*x)))
  float tmp;
  __asm__
  (
    "fld %0;"    "fld %0;"    "fmulp;"    "fst %1;"    "fld1;"    "fsubp;"
    "fld %1;"    "fdivp;"    "fsqrt;"    "fld1;"    "fpatan;"    "fst %0;"
    : "=m" (value)
    : "m" (tmp)
    : "eax"
  );
  return value;
}

/**
 * calculates the arc-cosine of a floating point number
 * @param[in] value float
 * @return value float
 */
inline float axAcosf(float value)
{
  // acos(x) = atan(sqrt((1-x*x)/(x*x)))
  float tmp;
  __asm__
  (
    "fld %0;"    "fld %0;"    "fmulp;"    "fst %1;"    "fld1;"    "fsubp;"
    "fld %1;"    "fdivrp;"    "fsqrt;"    "fld1;"    "fpatan;"    "fst %0;"
    : "=m" (value)
    : "m" (tmp)
    : "eax"
  );
  return value;
}

/**
 * calculates the arc-tangens of a floating point number
 * @param[in] value float
 * @return value float
 */
inline float axAtanf(float value)
{
  __asm__
  (
    "fld %0;"    "fld1;"    "fpatan;"    "fst %0;"
    : "=m" (value)
    : "m" (value)
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
  __asm__
  (
    "subl $0x3f800000, %0;"    "subl $1, %2;"
    "shrl %b2, %0;"    "addl $0x3f800000, %0;"
    : "=r" (value)
    : "0" (value), "c" (root)
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
  __asm__
  (
    "subl $0x3f800000, %0;"    "shrl $1, %0;"    "addl $0x3f800000, %0;"
    : "=r" (value)
    : "0" (value)
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
 * performs fast and accurate pow(float, integer) approximation
 * @param[in] x float
 * @param[in] n int
 * @return
 */
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

/**
 * fast approximation of [e] to the power of a number
 * based on code found in http://theoval.sys.uea.ac.uk/publications/pdf/nc2000a.pdf
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
 * fast approximation of the logarithm base 2 function
 * based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
 * @param[in] val float
 * @return result float
 */
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

/**
 * fast approximation of the natural logarithm
 * based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
 * @param[in] val float
 * @return result float
 */
inline float axLog(const float &val)
{
  return (axLog2 (val) * 0.69314718f);
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
  return M_PI_2 - sqrtf(1 - x)*(1.5707288 - x*(0.2121144 + x*(0.0742610 -
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
  return LOG2DB * logf(lin);
}

/**
 * converts decibel value to linear
 * @param[in] dB float
 * @return result float
 */
inline float dB2lin(const float dB)
{
  return expf(DB2LOG * dB);
}

#endif
