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

 ///////////////
 // TODO: CLEAN

#ifndef axUtils_included
#define axUtils_included

#include "axDefines.h"
#include "axMath.h"

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

//----------

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

//----------------------------------------------------------------------

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

//----------

/// convert to boolean
#define axBool(x)         (!(!(x)))
/// get bit
#define axBitGet(x, bit)  ( 1  &  ((x)  >> (bit)))
/// set bit
#define axBitSet(x, bit)  ((x) |  (1    << (bit)))
/// clear bit
#define axBitClr(x, bit)  ((x) & ~(1    << (bit)))
/// flip bit
#define axBitFlp(x, bit)  ((x) ^  (1    << (bit)))

//----------

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

//----------

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

//----------

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

//----------

/**
 * conversation from q factor to bandwidth (octaves)
 * @param[in] q float - q factor
 * @return octaves float
 */
__axutils_inline float axQ2Octaves(const float q)
{
  return 1.4426950408889634f * axSinhf(0.5f * (q));
}

//----------

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

//----------------------------------------------------------------------


#if 0

  // test or write better algorithm
  
__axutils_inline void axRadix(long *source, long *dest,
  unsigned long N, int byte)
{
  unsigned int i;
  long count[256];
  long index[256];
  
  // ax__Memset(count, 0, sizeof(count));
    
  // inline memset 0
  register unsigned int len = sizeof(count);
  register unsigned char _v = 0;
  register char* _d = (char*) count;
  while (len--)
    *_d++ = _v;
  // ---------------
  
  for (i=0; i<N; i++)
    count[ ((source[i]) >> (byte*8)) & 0xff ]++;
  index[0] = 0;
  for (i=1; i<256; i++)
    index[i] = index[i-1]+count[i-1];
  for (i=0; i<N; i++)
    dest[ index[ ((source[i])>>(byte*8))&0xff ]++ ] = source[i];
}


#endif

//----------------------------------------------------------------------

// check for little endian
__axutils_inline unsigned int axLittleEndian (void)
  { const int i = 1; return  (*(char*)&i); }

//----------

// check for big endian
__axutils_inline unsigned int axBigEndian (void)
  { const int i = 1; return !(*(char*)&i); }

//----------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::
// needs gInstance and trace()
#if 0

// retrieve the path from where the binary (pe / elf) is loaded
__axstdlib_inline const char* axGetBasePath (char* path)
{
  char* path_init = path;
  // windows
  #ifdef WIN32
    char filepath[AX_MAX_PATH] = "";
    #ifdef AX_FORMAT_LIB
      #error there is no gInstance anymore...
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

//----------------------------------------------------------------------

// read file from base path
__axstdlib_inline unsigned char* axFileRead (const char* _file,
  unsigned int* _size, const unsigned int mode = 0)
{
  char filepath[AX_MAX_PATH] = "";
  char _path[AX_MAX_PATH] = "";
  const char* path = axGetBasePath(_path);
  axStrcat(filepath, (char*)path);
  axStrcat(filepath, (char*)_file);

  trace("axFileRead(" << mode << "): " << filepath);
  FILE* f = NULL;
  switch (mode)
  {
    case 0: f = fopen(filepath, "rb"); break;
    case 1: f = fopen(filepath, "r");  break;
  }
  if (!f)
  {
    trace("axFileRead, #ERR open(" << mode << "): " << filepath);
    return 0;
  }
  fseek(f, 0, SEEK_END);
  *_size = ftell(f);
  fseek(f, 0, SEEK_SET);
  if (!*_size)
  {
    trace("axFileRead, #ERR null sized: " << filepath);
    return 0;
  }
  unsigned char* b = (unsigned char*)axMalloc(*_size);
  unsigned int res = fread(b, *_size, 1, f);
  fclose(f);
  if (!res)
  {
    trace("axFileRead, #ERR read: " << filepath);
    return 0;
  }
  return b;
}

//----------

// write file to base path
__axstdlib_inline unsigned int axFileWrite (const char* _file,
  const char* b, const unsigned int len, const unsigned int mode = 0 )
{
  char filepath[AX_MAX_PATH] = "";
  char _path[AX_MAX_PATH] = "";
  const char* path = axGetBasePath(_path);
  axStrcat(filepath, (char*)path);
  axStrcat(filepath, (char*)_file);

  trace("axFileWrite(" << mode << "): " << filepath);
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
    trace("axFileWrite, #ERR open(" << mode << "): " << filepath);
    return 0;
  }
  unsigned int res = fwrite(b, sizeof(b[0]), len, f);
  fclose(f);
  if (!res)
  {
    trace("axFileWrite, #ERR write: " << filepath);
    return 0;
  }
  return 1;
}

#endif //0
//::::::::::::::::::::::::::::::::::::::::::::::::::

//----------------------------------------------------------------------
#endif
