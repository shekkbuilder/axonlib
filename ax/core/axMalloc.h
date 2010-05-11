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
  axMalloc.h

  ==============================================================================
  Axonlib's own memory allocation routines: axMalloc, axRealloc, axFree
  ==============================================================================
  based on code by:
    DJ Delorie (DJGPP)
      http://www.delorie.com/djgpp/malloc/
    where licensing permits use in the case of Axonlib:
      http://www.delorie.com/djgpp/dl/ofc/simtel/v2/copying.dj

  and the following article by Joerg Walter, Doug Lea et al (available in the
  public domain):
    http://www.genesys-e.de/jwalter/mix4win.htm

  original file header by DJ Dolorie from here bellow:
  ==============================================================================
*/

/* Copyright (C) 1997 DJ Delorie, see COPYING.DJ for details */

// end of original header
// =============================================================================

/*
STATISTICS:
    ============================================================================
    speed comparison with stdlib malloc:
      cpu:
        amd athlon 1800+
      compiler (mingw-gcc-tdm-4.4.1):
         -O3 -s -msse -mfpmath=sse,387
      code:
        char* ptr = malloc(2000000); free(ptr);
      n of iterations:
        100000

      stdlib malloc:
        724 ms
      axonlib malloc:
        2 ms !!!
    ----------------------------------------------------------
      binary size overhead:
        axMalloc.h ~= 400 bytes
    ============================================================================
TODO:
  - conclude if more speed optimizations are possible
  - fragmentation level tests
  - thread safety tests (disable the mutex and make the winapi calls directly)
  - check if possible to use implementation with axMutex
  - linux: research on mmap vs sbrk in regard of speed
    ============================================================================
*/

#ifndef axMalloc_included
#define axMalloc_included

#include "axStdlib.h"

#ifdef AX_HOT_INLINE_MALLOC
  #define __axmalloc_inline __hotinline
#else
  #define __axmalloc_inline inline
#endif

#ifdef AX_NO_MALLOC     // no axMalloc - use stdlib.h malloc
  #include "stdlib.h"
  #define axMalloc    malloc
  #define axRealloc   realloc
  #define axFree      free

#else // use axMalloc

/*
  from here bellow adapted version of Joerg Walter's MMAP emulation
  for windows with mutex.
*/
#ifdef WIN32
  #undef _WIN32_WINNT
  #define _WIN32_WINNT 0x0501
  #include "windows.h"

  static long g_sl;

  /*
  // inline these two
  __axmalloc_inline void slwait (long* sl)
    while ( InterlockedCompareExchange( sl, 1, 0) != 0 )
      Sleep(0);
   __axmalloc_inline void slrelease (long* sl)
    InterlockedExchange( sl, 0 );
  */

  __axmalloc_inline long getpagesize (void)
  {
    static long g_pagesize = 0;
    if (! g_pagesize)
    {
      SYSTEM_INFO system_info;
      GetSystemInfo (&system_info);
      g_pagesize = system_info.dwPageSize;
    }
    return g_pagesize;
  }

  __axmalloc_inline long getregionsize (void)
  {
    static long g_regionsize = 0;
    if (! g_regionsize)
    {
      SYSTEM_INFO system_info;
      GetSystemInfo (&system_info);
      g_regionsize = system_info.dwAllocationGranularity;
    }
    return g_regionsize;
  }

  __axmalloc_inline void* axMmap (long size)
  {
    register void* ptr = NULL;
    static long g_pagesize;
    static long g_regionsize;
    //slwait(&g_sl);
    while ( InterlockedCompareExchange( &g_sl, 1, 0) != 0 )
      Sleep(0);
    if (!g_pagesize)
      g_pagesize = getpagesize();
    if (!g_regionsize)
      g_regionsize = getregionsize();
    ptr = VirtualAlloc(ptr, size,
      MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
    //slrelease (&g_sl);
    InterlockedExchange( &g_sl, 0 );
    return ptr;
  }

  /*
  // ### MUNMAP emulation is currently disabled
  __axmalloc_inline long axMunmap (void *ptr,
    __attribute__((unused)) long size)
  {
    static long g_pagesize;
    static long g_regionsize;
    int rc = NULL;
    //slwait(&g_sl);
    while ( InterlockedCompareExchange( sl, 1, 0) != 0 )
      Sleep(0);
    if (!g_pagesize)
      g_pagesize = getpagesize ();
    if (!g_regionsize)
      g_regionsize = getregionsize ();
    if (!VirtualFree (ptr, 0,  MEM_RELEASE) )
      rc = 0;
    //slrelease (&g_sl);
    InterlockedExchange( &g_sl, 0 );
    return rc;
  }
  */

#endif

#ifdef linux
  // may be needed for sbrk()
  //#include "unistd.h"
#endif

/**
  from here bellow optimized and modified version of DJ Delorie's malloc
  routines (malloc1.c)
*/

unsigned char *buckets[32] = {0};
unsigned int bucket2size[32] = {0};

static __axmalloc_inline unsigned int size2bucket(unsigned size)
{
  int rv = 0x1f;
  int bit = ~0x10;
  if (size < 4) size = 4;
  size = (size+3)&~3;
  unsigned int i = 0;
  while (i<5)
  {
    if (bucket2size[rv&bit] >= size)
      rv &= bit;
    bit>>=1;
    i++;
  }
  return rv;
}

static __axmalloc_inline void init_buckets()
{
  register unsigned int b = 0;
  while (b<32)
  {
    bucket2size[b] = (1<<b);
    b++;
  }
}

/**
 * axMalloc
 */
__axmalloc_inline void* axMalloc (register unsigned int size)
{
  if (size <= 0)
    return NULL;
  register unsigned char* rv;
  register unsigned int b;
  if (bucket2size[0] == 0)
    init_buckets();
  b = size2bucket(size);
  if (buckets[b])
  {
    rv = buckets[b];
    buckets[b] = *(unsigned char**)rv;
    return rv;
  }
  size = bucket2size[b]+4;
  // os specific calls
  #ifdef linux
    //rv = (char*)mmap(rv, size);       // #include "sys/mman.h"
    rv = (unsigned char*)sbrk(size);    // sbrk = legacy
  #endif
  #ifdef WIN32
    rv = (unsigned char*)axMmap(size);
  #endif
  *(unsigned int*)rv = b;
  rv += 4;
  return (void*)rv;
}

/**
 * axFree
 */
__axmalloc_inline void axFree (void* _ptr)
{
  if (_ptr != NULL)
  {
    register unsigned char* ptr = (unsigned char*)_ptr;
    unsigned int b = *(unsigned int*)(ptr-4);
    *(unsigned char**)ptr = buckets[b];
    buckets[b] = ptr;
  }
}

/**
 * axRealloc
 */
__axmalloc_inline void* axRealloc (void* _ptr,
  register const unsigned int size)
{
  // case null pointer
  if (_ptr == NULL)
    return axMalloc(size);
  else
  {
    if (size == 0)
      axFree(_ptr);
    else
    {
      register char* newptr;
      register char* ptr = (char*)_ptr;
      unsigned int oldsize = bucket2size[*(unsigned int*)(ptr-4)];
      if (size <= oldsize)
        return ptr;
      newptr = (char*) axMalloc(size * sizeof(_ptr));
      // -- memcpy
      while (oldsize--)
        *ptr++ = *newptr++;
      // --
      axFree(ptr);
      return (void*)newptr;
    }
  }
  return _ptr;
}

#endif // ax_no_malloc

// -----------------------------------------------------------------------------
// enable local debug
// -----------------------------------------------------------------------------
#if defined (AX_DEBUG) && defined (AX_DEBUG_MEM)
  #include <iostream>

  /*
    malloc_usable_size() will be used to determine the real, allocated
    size by the standard methods, since what is passed as _size_ may not be
    the actual size we need to add or substract from the total
    counter (_axMemTotal)
  */
  #ifdef AX_NO_MALLOC
    #include "malloc.h" // malloc_usable_size()
  #endif

    //
    // http://stackoverflow.com/questions/1913853/why-is-used-in-delete-delete-to-free-dynamically-allocated-array
    // "...platform, the _msize (windows), malloc_usable_size (linux) or malloc_size (osx) functions will tell you ..."
    //
    #ifdef AX_WIN32
      #define malloc_usable_size _msize
    #endif

  static unsigned int _axMemTotal = 0;

  // inline _axStrrchr
  __axmalloc_inline char* _axStrrchr (register const char* s, const int c)
  {
    char* p = NULL;
    while (*s++)
      if (*s == c)
        p = (char*) s;
    return p;
  }

  // inline _axGetFileName
  __axmalloc_inline const char* _axGetFileName (const char* path)
  {
    const char *slash, *backslash;
    slash = _axStrrchr(path, '/');
    backslash = _axStrrchr(path, '\\') + 1;
    if (slash) return slash + 1;
      return backslash;
  }

  // malloc debug
  __axmalloc_inline void* axMallocDebug
  (register unsigned int _size, const char* _file, const unsigned int _line)
  {
    #ifdef AX_NO_MALLOC
      void* _ptr = malloc(_size);
      _axMemTotal += malloc_usable_size(_ptr);
    #else
      void* _ptr = axMalloc(_size);
      _axMemTotal += _size;
    #endif
    std::cout << "[" << _axGetFileName(_file) << "|" << _line << "] " <<
    #ifdef AX_NO_MALLOC
      "malloc, " <<
    #else
      "axMalloc, " <<
    #endif
    (void*)&_ptr << ", " << _size << ", " << _axMemTotal << "\n";
    return _ptr;
  }

  // realloc debug
  __axmalloc_inline void* axReallocDebug
  (void* _ptr, register const unsigned int _size, const char* _file, const unsigned int _line)
  {
    #ifdef AX_NO_MALLOC
      unsigned int old_size = malloc_usable_size(_ptr);
      void* _ptr0 = realloc(_ptr, _size);
      _axMemTotal -= old_size;
      _axMemTotal += malloc_usable_size(_ptr0);
    #else
      register char* old_ptr = (char*)_ptr;
      unsigned int old_size = bucket2size[*(unsigned int*)(old_ptr-4)];
      void* _ptr0 = axRealloc(_ptr, _size);
      _axMemTotal -= old_size;
      _axMemTotal += _size;
    #endif
    std::cout << "[" << _axGetFileName(_file) << "|" << _line << "] " <<
    #ifdef AX_NO_MALLOC
      "realloc, " <<
    #else
      "axRealloc, " <<
    #endif
    (void*)&_ptr << ", " << (void*)&_ptr0 <<
    ", " << _size << ", " << _axMemTotal << "\n";
    return _ptr0;
  }

  // free debug
  __axmalloc_inline void axFreeDebug
  (void* _ptr, const char* _file, const unsigned int _line)
  {
    unsigned int _size;
    #ifdef AX_NO_MALLOC
      _size = malloc_usable_size(_ptr);
    #else
      register unsigned char* ptr = (unsigned char*)_ptr;
      _size = bucket2size[*(unsigned int*)(ptr-4)];
    #endif
    _axMemTotal -= _size;
    std::cout << "[" << _axGetFileName(_file) << "|" << _line << "] " <<
    #ifdef AX_NO_MALLOC
      "free, " <<
    #else
      "axFree, " <<
    #endif
    (void*)&_ptr << ", " << _size << ", " << _axMemTotal << "\n";
    #ifdef AX_NO_MALLOC
      free(_ptr);
    #else
      axFree(_ptr);
    #endif
  }

  // clear previous definitions (if any)
  #ifdef AX_NO_MALLOC
    #undef axMalloc
    #undef axRealloc
    #undef axFree
  #endif

  // macro overrides here
  #define axMalloc(s)     axMallocDebug   (s, __FILE__, __LINE__)
  #define axRealloc(p, s) axReallocDebug  (p, s, __FILE__, __LINE__)
  #define axFree(p)       axFreeDebug     (p, __FILE__, __LINE__)

  // same for the standard methods if 'AX_NO_MALLOC'
  #ifdef AX_NO_MALLOC
    #define malloc(s)       axMallocDebug   (s, __FILE__, __LINE__)
    #define realloc(p, s)   axReallocDebug  (p, s, __FILE__, __LINE__)
    #define free(p)         axFreeDebug     (p, __FILE__, __LINE__)
  #endif

#endif // ax_debug && ax_debug_mem

#endif // axMalloc_included
