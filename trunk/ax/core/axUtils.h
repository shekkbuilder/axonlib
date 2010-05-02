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
 * @file axUtils.h
 * \brief contains various utility methods.
 */

/*
TODO:
  lii:  i've started writing some of these in axStdLib.h, but they can be here
        as well.
  
  axMemcpy, axMemsey
  axStrCpy,Strdup, etc...
  strcmp
  sprintf (doesn't need to be that advanced..)
  stdlib & std replacements
*/

#ifndef axUtils_included
#define axUtils_included

#include "axDefines.h"
#include "axStdlib.h"
#include "core/axMath.h"

#ifdef AX_USE_HOT_INLINE
  #define __axutils_inline __hotinline
#else
  #define __axutils_inline inline
#endif

//strip:
// --------
#include <memory.h>
#include <stdio.h>
#include <sstream>
using namespace std;
// --------

/**
 return the filename from the __FILE__ flag: <br>
 (can be used for any path as well)
 \code
  printf("%s\n", axGetFileName(__FILE__));
 \endcode
 @param[in] path const char*
 @return const char*
 */
__axutils_inline const char* axGetFileName(const char* path)
{
  const char *slash, *backslash;
  //slash = axStrrchr(path, '/');
  //backslash = axStrrchr(path, '\\') + 1;
  slash = strrchr(path, '/');
  backslash = strrchr(path, '\\') + 1;
  if (slash) return slash + 1;
    return backslash;
}

/**
 * swap the values of two variables <br>
 \code
 * origin: http://graphics.stanford.edu/~seander/bithacks.html
 * note: type of first variable is used
 * float x = 5.f;
 * float y = 3.f;
 * axSpaw(x, y);
 * \endcode
 * @param[in] x type-unsafe
 * @param[in] y type-unsafe
 * @return void
 */
#define axSwap(x,y) { typeof(x) tmp = (x);(x) = (y);(y) = (tmp); }

/**
 * axGetArrSize() helper function
 */
template<class T, size_t N> T decay_array_to_subtype(T (&a)[N]);
/**
 * returns the size of an array <br>
 * \code
 * char a[15];
 * int b[axGetArrSize(a) + 1];
 * unsigned int j = axGetArrSize(b);    // j = 16
 * // ----------------
 * // NOTE: passing a pointer will not work
 * int ar[21];
 * int* ptr = ar;
 * unsigned int j = axGetArrSize(ptr); // <- will not work
 * \endcode
 * @param[in] x array type-unsafe
 * @return unsigned int
 */
#define axGetArrSize(x) (sizeof(x)/sizeof(decay_array_to_subtype(x)))

/**
 * bit reverse algorithm <br>
 * origin: http://graphics.stanford.edu/~seander/bithacks.html
 * \code
 * unsigned int b = 0x0000000a;
 * unsigned int result = axBitReverse(b); // result = 0xa0000000;
 * \endcode
 * @param[in] x unsigned int
 * @return x unsigned int
 */
__axutils_inline unsigned int axBitReverse(unsigned int v)
{
  unsigned int r = v;
  int s = sizeof(v) * CHAR_BIT - 1;
  for (v >>= 1; v; v >>= 1)
  {   
    r <<= 1;
    r |= v & 1;
    s--;
  }
  r <<= s;
  return r;
}

/**
 * returns a specific bit of an integer
 * \code
 * int x = 0x0f;
 * for(unsigned int i=0; i<32; i++)
 * { cout << axGetBit(x, i); }      // 11110000000000000000000000000000
 * cout << axGetBit(x, 0);          // 1
 * cout << axGetBit(x, 4);          // 0
 * \endcode
 * @param[in] x long int - input value
 * @param[in] bit unsigned int - which bit
 * @return unsigned int - 0x0 / 0x1
 */
#define axGetBit(x, bit) ( 1 & ((x) >> (bit)) )

/**
 * returns a binary representation of an integer as string
 * \code
 * const char* str = axGetBinaryString(-211);
 * cout << str << endl;                        // 10110100111111111111111111111111
 * cout << axGetBinaryString(0xf0, 8) << endl; // 00001111
 * \endcode
 * @param[in] x long int - input value
 * @param[in] bits unsigned int - length (default = 32)
 * @return const char*
 */
/*
// ###### remove or replace with a version that does not use OSS
*/
__deprecated __axutils_inline const char* axGetBinaryString(long int x, unsigned int bits=32)
{
  ostringstream oss;
  for (unsigned int i=0; i<bits; i++) oss << (1 & (x >> i));
  const char* cstr = oss.str().c_str();
  return cstr;
}

// ------------------------------------------------------

/**
 * converts linear value to decibel
 * \code
 * float dc_signal = 1.f;
 * float amp_half = 0.5f;
 * dc_signal *= amp_half;                  // half the amplitude
 * float amp_db = axLin2DB(amp_half);      // amp_db = -6dbFS
 * \endcode
 * @param[in] lin float
 * @return result float
 */
#define axLin2DB(lin) ( LOG2DB*axLogf( (lin) ) )

/**
 * converts decibel value to linear
 * \code
 * float amp_db = 20.f;         // 20 decibels
 * signal *= axDB2Lin(amp_db);  // *= ~10.079
 * \endcode
 * @param[in] dB float
 * @return result float
 */
#define axDB2Lin(dB) ( axExpf( DB2LOG*(dB) ) )

/**
 * sums a set (array) of dBFS values
 * \code
 * float ar[] = {-6.f, -12.f, -18.f, -30.f};    // dBFS values
 * float result = axSumDB(4, ar);               // -0.852114 dBFS
 * \endcode
 * @param[in] n unsigned integer - array length
 * @param[in] ar float* - array of floats
 * @return result float
 */
__axutils_inline float axSumDB(unsigned int n, const float* ar)
{
  float sum = 0.f;
  for (unsigned int i=0; i<n; i++) sum += axDB2Lin(ar[i]);
  return axLogf(sum)*LOG2DB;
}

// ------
bool __AX_SSE3__, __AX_SSSE3__, __AX_FPU__, __AX_CMOV__,  __AX_SSE__, __AX_SSE2__,
__AX_SSE4A__, __AX_SSE5__, __AX_MMX__, __AX_MMXEXT__, __AX_3DNOW__, __AX_3DNOWEXT__;
/**
 * calls the CPUID instruction <br>
 * can be used to target specific CPU architectures:
 * <br>
 * default mode:
 * \code
 * // init stage
 * axCPUID();           // <- makes definitions
 * // ...
 * // processing stage
 * if (__AX_FPU__) cout << "has fpu x87\n";   // FPU code here
 * \endcode
 * list of definitions:
 * \code
 * __AX_SSE3__
 * __AX_SSSE3__
 * __AX_FPU__
 * __AX_CMOV__
 * __AX_SSE__
 * __AX_SSE2__
 * __AX_SSE4A__
 * __AX_SSE5__
 * __AX_MMX__
 * __AX_MMXEXT__
 * __AX_3DNOW__
 * __AX_3DNOWEXT__
 * \endcode
 * specific function call (CPUID FN):
 * \code
 * // variables to be used
 * int eax, ebx, ecx, edx;
 * // pass function call and addresses
 * axCPUID(0x00000001, &eax, &ebx, &ecx, &edx);
 * // you can use axGetBit(value, bit)
 * if (axGetBit(edx, 0) == 1) cout << "has fpu x87\n";
 * \endcode
 * <br>
 * code example: <br>
 * plugins/liteon/_cpuidtest <br>
 * <br>
 * for more information see: <br>
 * http://en.wikipedia.org/wiki/CPUID <br>
 * http://www.amd.com/us-en/assets/content_type/white_papers_and_tech_docs/25481.pdf <br>
 * http://www.intel.com/Assets/PDF/appnote/241618.pdf
 */
__axutils_inline void axCPUID(const int fcall=33139, int* eax=0, int* ebx=0, int* ecx=0, int* edx=0)
{
  // no function call (default)
  // ----------------------------
  if (fcall == 33139)  // 33139 = some default number
  {
    int a, b, c, d;
    // -----------------
    // 0x00000001
    __asmv
    (
      "cpuid;"
      : "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (0x00000001)
    );
    __AX_SSE3__   = axGetBit(c, 0);
    __AX_SSSE3__  = axGetBit(c, 9);
    __AX_FPU__    = axGetBit(d, 0);
    __AX_CMOV__   = axGetBit(d, 15);
    __AX_SSE__    = axGetBit(d, 25);
    __AX_SSE2__   = axGetBit(d, 26);
    // -----------------
    // 0x80000001
    __asmv
    (
      "cpuid;"
      : "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (0x80000001)
    );
    __AX_SSE4A__    = axGetBit(c, 4);
    __AX_SSE5__     = axGetBit(c, 11);
    __AX_MMX__      = axGetBit(d, 23);
    __AX_MMXEXT__   = axGetBit(d, 22);
    __AX_3DNOW__    = axGetBit(d, 31);
    __AX_3DNOWEXT__ = axGetBit(d, 30);
  }
  // user defined call
  // -----------------
  else
  {
    __asmv
    (
      "cpuid;"
      : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (fcall)
    );
  }
}

//-----

#define cc_SSE3     0x0001
#define cc_SSSE3    0x0002
#define cc_FPU      0x0004
#define cc_CMOV     0x0008
#define cc_SSE      0x0010
#define cc_SSE2     0x0020
#define cc_SSE4A    0x0040
#define cc_SSE5     0x0080
#define cc_MMX      0x0100
#define cc_MMXEXT   0x0200
#define cc_3DNOW    0x0400
#define cc_3DNOWEXT 0x0800

int axCpuCaps(void)
{
  int caps = 0;
  axCPUID();
  if (__AX_SSE3__)      caps |= cc_SSE3;
  if (__AX_SSSE3__)     caps |= cc_SSSE3;
  if (__AX_FPU__)       caps |= cc_FPU;
  if (__AX_CMOV__)      caps |= cc_CMOV;
  if (__AX_SSE__)       caps |= cc_SSE;
  if (__AX_SSE4A__)     caps |= cc_SSE4A;
  if (__AX_SSE5__)      caps |= cc_SSE5;
  if (__AX_MMX__)       caps |= cc_MMX;
  if (__AX_MMXEXT__)    caps |= cc_MMXEXT;
  if (__AX_3DNOW__)     caps |= cc_3DNOW;
  if (__AX_3DNOWEXT__)  caps |= cc_3DNOWEXT;
  return caps;
}

static char cpustringbuf[256];

char* axCpuCapsString(void)
{
  axCPUID();
  // ## note: sprintf() is current used as __builtin_spritf()
  sprintf(cpustringbuf,"%s%s%s%s%s%s%s%s%s%s%s%s",
          __AX_SSE3__     ? "sse3 "     : "" ,
          __AX_SSSE3__    ? "ssse3 "    : "" ,
          __AX_FPU__      ? "fpu "      : "" ,
          __AX_CMOV__     ? "cmov "     : "" ,
          __AX_SSE__      ? "sse "      : "" ,
          __AX_SSE2__     ? "sse2 "     : "" ,
          __AX_SSE4A__    ? "sse4a "    : "" ,
          __AX_SSE5__     ? "sse5 "     : "" ,
          __AX_MMX__      ? "mmx "      : "" ,
          __AX_MMXEXT__   ? "mmxext "   : "" ,
          __AX_3DNOW__    ? "3dnow "    : "" ,
          __AX_3DNOWEXT__ ? "3dnowext " : "" );
  return cpustringbuf;
}

//-----

/**
 * conversation from bandwidth (octaves) to q factor
 * @param[in] n float - length in octaves
 * @return q_factor float
 */
__axutils_inline float axOctaves2Q(const float n)
{
  const float _pow2n = axPowf(2.f, n);
  return -axSqrtf(_pow2n) / (1.f - _pow2n);
}

/**
 * conversation from q factor to bandwidth (octaves)
 * @param[in] q float - q factor
 * @return octaves float
 */
__axutils_inline float axQ2Octaves(const float q)
{
  return 1.4426950408889634f * axSinhf(0.5f * (q));
}

/**
 * denormalize
 * @param[in] x - any expression or variable
 */
#define axDenorm(x) { (x) + DENORM }

/**
 * call the 'rdtsc' (Read Time Stamp Counter) instruction
 * usage:
 * \code
 * unsigned long long startValue = axRdtsc();
 * //some code here...
 * unsigned long long endValue = axRdtsc();
 * unsigned long long diff = endValue - startValue;
 * \endcode
 */
// # note:  use CPUID as serializing instruction to prevent out of order
//          execution for cpu cycle measurement.

#ifdef __AX64__
  // 64bit untested 
  static __axutils_inline unsigned long long axRdtsc(void)
  {
    unsigned low, high;
    __asmv ( "cpuid;" "rdtsc;" : "=a" (low), "=d" (high) );
    return ( (low) | ( (unsigned long)(high) << 32 ) );
  }
#endif
#ifdef __AX32__
  static __axutils_inline unsigned long long axRdtsc(void)
  {
    unsigned long long val;
    __asmv ( "cpuid;" "rdtsc;" : "=A" (val) );
    return val;
  }
#endif

/**
 * radix algorithm
 * @param[in] source long*
 * @param[in] dest long*
 * @param[in] N long
 * @param[in] byte int
 */
__axutils_inline void axRadix(long *source, long *dest, unsigned long N, int byte)
{
  unsigned int i;
  long count[256];
  long index[256];
  //axMemset(count, 0, sizeof(count));
  memset(count, 0, sizeof(count));
  for (i=0; i<N; i++)
    count[ ((source[i]) >> (byte*8)) & 0xff ]++;
  index[0] = 0;
  for (i=1; i<256; i++)
    index[i] = index[i-1]+count[i-1];
  for (i=0; i<N; i++)
    dest[ index[ ((source[i])>>(byte*8))&0xff ]++ ] = source[i];
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


//---------------------
#endif
