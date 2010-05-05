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
        axStdlib.h + axMalloc.h ~= 400 bytes      
    ============================================================================
TODO:
  - conclude if more speed optimizations are possible  
  - fragmentation level tests
  - windows 7 test (ccernn)
  - thread safety tests (disable the mutex and make the winapi calls directly)
  - check if possible to use implementation with axMutex  
  - linux: research on mmap vs sbrk in regard of speed
    ============================================================================
*/

#ifndef axMalloc_included
#define axMalloc_included

#include "axStdlib.h"

#ifdef AX_USE_HOT_INLINE
  #define __axmalloc_inline __hotinline
#else
  #define __axmalloc_inline inline
#endif

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
  // #include "unistd.h"
#endif

/**
  from here bellow optimized version of DJ Delorie's malloc routines
*/

char *buckets[32] = {0};
int bucket2size[32] = {0};

static __axmalloc_inline int size2bucket(int size)
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
  register unsigned b = 0;
  while (b<32)  
  {
    bucket2size[b] = (1<<b);
    b++;
  }
}

/**
 * axMalloc
 */ 
__axmalloc_inline char *axMalloc (register int size)
{
  register char* rv;
  register int b;
  if (bucket2size[0] == 0)
    init_buckets();
  b = size2bucket(size);
  if (buckets[b])
  {
    rv = buckets[b];
    buckets[b] = *(char**)rv;
    return rv;
  }
  size = bucket2size[b]+4;

  #ifdef linux
    // use sbrk on linux
    // or linux's mmap() from "sys/mman.h" ?
    rv = (char*)sbrk(size);
  #endif
  #ifdef WIN32
    // emulate on windows
    rv = (char*)axMmap(size);
  #endif
  
  *(int*)rv = b;
  rv += 4;
  return rv;
}

/**
 * axFree
 */
__axmalloc_inline void axFree (register char* ptr)
{
  int b = *(int*)(ptr-4);
  *(char**)ptr = buckets[b];
  buckets[b] = ptr;
}

/**
 * axRealloc
 */  
__axmalloc_inline char* axRealloc (register char* ptr,
  register const unsigned int size)
{
  char* newptr;
  const unsigned int oldsize = bucket2size[*(int*)(ptr-4)];
  if (size <= oldsize)
    return ptr;
  newptr = (char*) axMalloc(size);
  // prevent mutual inclusion: axMalloc <-> axStdlib
  // ---------------
  // inline:
  // axMemcpy(ptr, (int)newptr, oldsize);
  register char* _d = ptr;
  register char* _s = newptr;
  register unsigned int _sz = oldsize;
  while (_sz--)
    *_d++ = *_s++;
  // ---------------      
  axFree(ptr);
  return newptr;
}

#endif
