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
 * @file axUtils.h
 * \brief contains various utility methods.
 */

/*
TODO:
  axMemcpy, axMemsey
  axStrCpy,Strdup, etc...
  strcmp
  sprintf (doesn't need to be that advanced..)
  stdlib & std replacements
*/

#ifndef axUtils_included
#define axUtils_included

#include "axMath.h"
#include "axDefines.h"
#include <sstream>
using namespace std;

/**
 * swap the values of two variables <br>
 \code
 * unsigned int x = 5; // the type of the first variable is used as a base
 * int y = 2;
 * axSpaw(x, y);
 * float xf = 5.f;
 * float yf = 3.f;
 * axSpaw(xf, yf);
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
 * fast bit reverse algorithm <br>
 * origin: http://www.jjj.de/fxt/fxtpage.html
 * \code
 * unsigned int b = 0x0000000f;
 * unsigned int result = axBitReverse(b); // result = 0xf0000000;
 * \endcode
 * @param[in] x long int
 * @return x long int
 */
inline long int axBitReverse(long int x)
{
	__asm__ __volatile__ ("":::);
  __asm__
	(
		"movl %0, %%eax;"         "andl $0xaaaaaaaa, %%eax;"		"shrl $1, %%eax;"
		"andl $0x55555555, %0;"		"shll $1, %0;"		            "orl %%eax, %0;"
		"movl %0, %%eax;"		      "andl $0xcccccccc, %%eax;"		"shrl $2, %%eax;"
		"andl $0x33333333, %0;"		"shll $2, %0;"		            "orl %%eax, %0;"
		"movl %0, %%eax;"		      "andl $0xf0f0f0f0, %%eax;"		"shrl $4, %%eax;"
		"andl $0x0f0f0f0f, %0;"		"shll $4, %0;"		            "orl %%eax, %0;"
		"movl %0, %%eax;"		      "andl $0xff00ff00, %%eax;"		"shrl $8, %%eax;"
		"andl $0x00ff00ff, %0;"		"shll $8, %0;"		            "orl %%eax, %0;"
		"movl %0, %%eax;"		      "andl $0xffff0000, %%eax;"		"shrl $16, %%eax;"
		"andl $0x0000ffff, %0;"   "shll $16, %0;"		            "orl %%eax, %0;"
		: "=m" (x)	:		: "eax"
	);
	return x;
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
inline unsigned int axGetBit(long int x, unsigned int bit)
{
  return 1 & (x >> bit);
}

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
inline const char* axGetBinaryString(long int x, unsigned int bits=32)
{
  ostringstream oss;
  for (unsigned int i=0; i<bits; i++) oss << (1 & (x >> i));
  const char* cstr = oss.str().c_str();
  return cstr;
}

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
inline float axLin2DB(const float lin)
{
  return LOG2DB*axLogf(lin);
}

/**
 * converts decibel value to linear
 * \code
 * float amp_db = 20.f;         // 20 decibels
 * signal *= axDB2Lin(amp_db);  // *= ~10.079
 * \endcode
 * @param[in] dB float
 * @return result float
 */
inline float axDB2Lin(const float dB)
{
  return axExpf(DB2LOG*dB);
}

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
inline float axSumDB(unsigned int n, const float* ar)
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
inline void axCPUID(const int fcall=0, int* eax=0, int* ebx=0, int* ecx=0, int* edx=0)
{
  // no function call (default)
  // ----------------------------
  if (fcall == 0)
  {
    int a, b, c, d;    
    // -----------------
    // 0x00000001
    __asm__ __volatile__
    (
      "cpuid;"
      : "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (0x00000001)
    );
    __AX_SSE3__   = 1 & (c >> 0);
    __AX_SSSE3__  = 1 & (c >> 9);
    __AX_FPU__    = 1 & (d >> 0);
    __AX_CMOV__   = 1 & (d >> 15);   
    __AX_SSE__    = 1 & (d >> 25); 
    __AX_SSE2__   = 1 & (d >> 26);
    // ----------------- 
    // 0x80000001    
    __asm__ __volatile__
    (
      "cpuid;"
      : "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (0x80000001)
    );
    __AX_SSE4A__    = 1 & (c >> 4);
    __AX_SSE5__     = 1 & (c >> 11);
    __AX_MMX__      = 1 & (d >> 23);    
    __AX_MMXEXT__   = 1 & (d >> 22);   
    __AX_3DNOW__    = 1 & (d >> 31);
    __AX_3DNOWEXT__ = 1 & (d >> 30); 
  }
  // user defined call
  // -----------------
  else
  {
    __asm__ __volatile__
    (
      "cpuid;"
      : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (fcall)
    );
  }
}

/**
 * radix algorithm
 * @param[in] source long*
 * @param[in] dest long*
 * @param[in] N long
 * @param[in] byte int
 */
inline void axRadix (long *source, long *dest, long N, int byte)
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

//---------------------
#endif
