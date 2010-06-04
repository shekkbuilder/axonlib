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

#ifndef axUtils_included
#define axUtils_included

#include "axDefines.h"
#include "core/axStdlib.h"
#include "core/axMalloc.h"
#include "core/axMath.h"
#include "core/axDebug.h"
#include "stdio.h" // fread, fseek, fopen
#ifdef AX_WIN32
  #include <windows.h>
  #ifdef AX_FORMAT_LIB
    /*
      lii:
      if other dll formats are added for windows a commont entry point    
      could be moved in new header to get gInstance from there 
    */
    #include "format/axFormatVst.h" // gInstance for axGetBasePath()
  #endif
#endif
#ifdef AX_LINUX
  #include <dlfcn.h>
  #include <unistd.h>
#endif

#ifdef AX_HOT_INLINE_UTILS
  #define __axutils_inline __hotinline
#else
  #define __axutils_inline inline
#endif

// -------------------------------------------
#define axStrExpand(x) #x
#define axStr(x) axStrExpand(x)

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
bool __AX_SSE3__, __AX_SSSE3__, __AX_FPU__, __AX_CMOV__,  __AX_SSE__,
  __AX_SSE2__, __AX_SSE4A__, __AX_SSE5__, __AX_MMX__, __AX_MMXEXT__,
  __AX_3DNOW__, __AX_3DNOWEXT__;
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
__axutils_inline void axCPUID(const int fcall=33139, int* eax=0, int* ebx=0,
  int* ecx=0, int* edx=0)
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
    __asmv
    (
      "cpuid;"
      : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (fcall)
    );
}

// axCpuCaps
/*
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
*/

inline unsigned int axCpuCaps(void)
{
  unsigned int caps = 0;
  axCPUID();
  if (__AX_SSE3__)      caps |= 0x0001;
  if (__AX_SSSE3__)     caps |= 0x0002;
  if (__AX_FPU__)       caps |= 0x0004;
  if (__AX_CMOV__)      caps |= 0x0008;
  if (__AX_SSE__)       caps |= 0x0010;
  if (__AX_SSE2__)      caps |= 0x0020;
  if (__AX_SSE4A__)     caps |= 0x0040;
  if (__AX_SSE5__)      caps |= 0x0080;
  if (__AX_MMX__)       caps |= 0x0100;
  if (__AX_MMXEXT__)    caps |= 0x0200;
  if (__AX_3DNOW__)     caps |= 0x0400;
  if (__AX_3DNOWEXT__)  caps |= 0x0800;
  return caps;
}

// axCpuCapsString
char cpustringbuf[256];
char* axCpuCapsString(void)
{
  axCPUID();
  cpustringbuf[0] = 0;
  if (__AX_SSE3__)     axStrcat(cpustringbuf,(char*)"sse3 ");
  if (__AX_SSSE3__)    axStrcat(cpustringbuf,(char*)"ssse3 ");
  if (__AX_FPU__)      axStrcat(cpustringbuf,(char*)"fpu ");
  if (__AX_CMOV__)     axStrcat(cpustringbuf,(char*)"cmov ");
  if (__AX_SSE__)      axStrcat(cpustringbuf,(char*)"sse ");
  if (__AX_SSE2__)     axStrcat(cpustringbuf,(char*)"sse2 ");
  if (__AX_SSE4A__)    axStrcat(cpustringbuf,(char*)"sse4a ");
  if (__AX_SSE5__)     axStrcat(cpustringbuf,(char*)"sse5 ");
  if (__AX_MMX__)      axStrcat(cpustringbuf,(char*)"mmx ");
  if (__AX_MMXEXT__)   axStrcat(cpustringbuf,(char*)"mmxext ");
  if (__AX_3DNOW__)    axStrcat(cpustringbuf,(char*)"3dnow ");
  if (__AX_3DNOWEXT__) axStrcat(cpustringbuf,(char*)"3dnowext ");
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
 * denormalize input value
 * @param[in] n float
 * @return n float denormalized value
 */
__axutils_inline float axDenorm(register float n)
{
  register union
  {
    float f;
    unsigned int i;
  } u;
  u.f = n;
  if ( !(u.i & 0x7f800000) ) n = 0.f;
  return n;
}

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
__axutils_inline void axRadix(long *source, long *dest,
  unsigned long N, int byte)
{
  unsigned int i;
  long count[256];
  long index[256];
  axMemset(count, 0, sizeof(count));
  for (i=0; i<N; i++)
    count[ ((source[i]) >> (byte*8)) & 0xff ]++;
  index[0] = 0;
  for (i=1; i<256; i++)
    index[i] = index[i-1]+count[i-1];
  for (i=0; i<N; i++)
    dest[ index[ ((source[i])>>(byte*8))&0xff ]++ ] = source[i];
}

// check for little endian
__axutils_inline unsigned int axLittleEndian (void)
  { const int i = 1; return  (*(char*)&i); }

// check for big endian
__axutils_inline unsigned int axBigEndian (void)
  { const int i = 1; return !(*(char*)&i); }

// retrieve the path from where the binary (pe / elf) is loaded
__axstdlib_inline const char* axGetBasePath (char* path)
{
  char* path_init = path;
  // windows
  #ifdef WIN32
    char filepath[AX_MAX_PATH] = "";
    #ifdef AX_FORMAT_LIB
      GetModuleFileName(gInstance, filepath, MAX_PATH);
    #endif
    #ifdef AX_FORMAT_EXE
      GetModuleFileName(NULL, filepath, MAX_PATH);      
    #endif
    const char* slash = axStrrchr(filepath, '\\') + 1;
    if (slash)
      axStrncpy(path, filepath, (axStrrchr(filepath, '\\') + 1) -
        (char*)filepath);
    else
      axStrcpy(path, (char*)".\\");
  #endif
  // linux
  #ifdef linux
    #ifdef AX_FORMAT_LIB
      Dl_info dli;
      dladdr(__func__, &dli);   // dlfcn.h
      const char* slash = axStrrchr(dli.dli_fname, '/');
      if (slash)
		    axStrncpy(path, dli.dli_fname, (axStrrchr(dli.dli_fname, '/') + 1) -
          (char*)dli.dli_fname);
      else
        axStrcpy(path, (char*)"./");
    #endif
    #ifdef AX_FORMAT_EXE
      char filepath[AX_MAX_PATH] = "";
      unsigned int rl = readlink("/proc/self/exe", filepath, sizeof(filepath));
      if (rl)
      {
        const char* slash = axStrrchr(filepath, '/');
        if (slash)
  		    axStrncpy(path, filepath, (axStrrchr(filepath, '/') + 1) -
            (char*)filepath);
        else
          axStrcpy(path, (char*)"./");
      }
    #endif
  #endif
  return path_init;
}

// read binary file from base path (bin)
__axstdlib_inline unsigned char* axFileRead (const char* _file,
  unsigned int* _size, const unsigned int mode = 0)
{
  char filepath[AX_MAX_PATH] = "";
  char _path[AX_MAX_PATH] = "";
  const char* path = axGetBasePath(_path);
  axStrcat(filepath, (char*)path);
  axStrcat(filepath, (char*)_file);
  
  trace("axFileRead(): " << filepath);
  FILE* f = NULL;
  switch (mode)
  {
    case 0: f = fopen(filepath, "rb"); break;
    case 1: f = fopen(filepath, "r");  break;
  }  
  if (!f)
  {
    trace("axFileRead(), #ERR open(" << mode << "): " << filepath);
    return 0;
  }
  fseek(f, 0, SEEK_END);
  *_size = ftell(f);
  fseek(f, 0, SEEK_SET);
  if (!*_size)
  {
    trace("axFileRead(), #ERR null sized: " << filepath);
    return 0;
  }
  unsigned char* b = (unsigned char*)axMalloc(*_size);
  unsigned int res = fread(b, *_size, 1, f);
  fclose(f);  
  if (!res)
  {
    trace("axFileRead(), #ERR read: " << filepath);
    return 0;
  }  
  return b;
}

// write binary file to base path
__axstdlib_inline unsigned int axFileWrite (const char* _file,
  const char* b, const unsigned int len, const unsigned int mode = 0 )
{
  char filepath[AX_MAX_PATH] = "";
  char _path[AX_MAX_PATH] = "";
  const char* path = axGetBasePath(_path);
  axStrcat(filepath, (char*)path);
  axStrcat(filepath, (char*)_file);
  
  trace("axFileWrite(): " << filepath);
  FILE* f = NULL;
  switch (mode)
  {
    case 0: f = fopen(filepath, "wb"); break;
    case 1: f = fopen(filepath, "w");  break;
    case 2: f = fopen(filepath, "ab"); break;
    case 3: f = fopen(filepath, "a");  break;
  }
  if (!f)
  {
    trace("axFileWrite(), #ERR open(" << mode << "): " << filepath);
    return 0;
  }
  unsigned int res = fwrite(b, sizeof(b[0]), len, f);
  fclose(f);
  if (!res)
  {
    trace("axFileWrite(), #ERR write: " << filepath);
    return 0;
  }  
  return 1;
}

//----------------------------------------------------------------------
#endif
