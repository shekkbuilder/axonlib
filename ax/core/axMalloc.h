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
  Axonlib's own memory allocation routines:
  axMalloc, axCalloc, axRealloc, axFree
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
NOTES:
  - the custom memory allocator is currenly disabled due to a conflict with
    picopng.h ( using stdlib malloc() )

TODO:
  - resolve the picopng conflict
  - better error handling
  - fragmentation level tests
    ============================================================================
*/

#ifndef axMalloc_included
#define axMalloc_included

#ifdef AX_HOT_INLINE_MALLOC
  #define __axmalloc_inline __hotinline
#else
  #define __axmalloc_inline inline
#endif

#ifndef AX_USE_MALLOC
  #define AX_NO_MALLOC  // disable the local malloc by default
#endif

#ifdef AX_NO_MALLOC     // no axMalloc - use stdlib.h malloc
  #include "stdlib.h"
  #define axMalloc    malloc
  #define axCalloc    calloc
  #define axRealloc   realloc
  #define axFree      free

  // no debug for these
  #define _axMalloc   malloc
  #define _axCalloc   calloc
  #define _axRealloc  realloc
  #define _axFree     free

#else // use axMalloc

/*
  from here bellow adapted version of Joerg Walter's MMAP emulation
  for windows with mutex.
*/
#ifdef WIN32
  #include "windows.h"

  static __thread long g_sl;

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

  __axmalloc_inline long axMunmap (void *ptr,
    __attribute__((unused)) long size=0)
  {
    static long g_pagesize;
    static long g_regionsize;
    long rc = NULL;
    //slwait(&g_sl);
    while ( InterlockedCompareExchange( &g_sl, 1, 0) != 0 )
      Sleep(0);
    if (!g_pagesize)
      g_pagesize = getpagesize ();
    if (!g_regionsize)
      g_regionsize = getregionsize ();
    if (!VirtualFree (ptr, 0, MEM_RELEASE) )
      rc = 0;
    //slrelease (&g_sl);
    InterlockedExchange( &g_sl, 0 );
    return rc;
  }

#endif

#ifdef linux
  #include "unistd.h" // sbrk()
#endif

/**
  from here bellow optimized and modified version of DJ Delorie's malloc
  routines (malloc1.c)
*/

#define AX_M_MAX_BUCKETS 32

unsigned char* buckets[AX_M_MAX_BUCKETS] = {0};
unsigned int bucket2size[AX_M_MAX_BUCKETS] = {0};

static __axmalloc_inline unsigned int size2bucket(unsigned size)
{
  int rv = 0x1f;
  int bit = ~0x10;
  if (size < 4) size = 4;
  size = (size + 3) & ~3;
  unsigned int i = 0;
  while (i < 5)
  {
    if ((rv & bit) < AX_M_MAX_BUCKETS)
      if (bucket2size[rv & bit] >= size)
        rv &= bit;
    bit >>= 1;
    i++;
  }
  return rv;
}

static __axmalloc_inline void init_buckets()
{
  register unsigned int b = 0;
  while (b < AX_M_MAX_BUCKETS)
  {
    bucket2size[b] = (1 << b);
    b++;
  }
}

// axMalloc
static void* axMalloc (register unsigned int size)
{
  __builtin_printf("%d\n", size);
  if (size <= 0)
    return NULL;
  register unsigned char* rv;
  register unsigned int b;
  if (!bucket2size[0])
    init_buckets();
  b = size2bucket(size);
  if (buckets[b])
  {
    rv = buckets[b];
    buckets[b] = *(unsigned char**)rv; // !!
    return rv;
  }
  size = bucket2size[b] + 4;
  // os specific calls
  #ifdef linux
    //rv = (char*)mmap(rv, size, ...);  // #include "sys/mman.h"
    rv = (unsigned char*)sbrk(size);    // sbrk = legacy
  #endif
  #ifdef WIN32
    rv = (unsigned char*)axMmap(size);
  #endif
  *(unsigned int*)rv = b;
  rv += 4;
  return (void*)rv;
}

// axCalloc
static void* axCalloc (register const unsigned int n,
  register unsigned int size)
{
  //__builtin_printf("<< axCalloc\n");
  size *= n;
  if (size <= 0)
    return NULL;
  else
  {
    void* ptr = axMalloc(size);
    register char* cptr = (char*) ptr;
    while (size--)
      *cptr++ = 0;
    return ptr;
  }
}


// axFree
static void axFree (void* _ptr)
{
  //__builtin_printf("<< axFree\n");
  if (_ptr)
  {
    register unsigned char* ptr = (unsigned char*)_ptr;
    register unsigned int b = *(unsigned int*)(ptr-4);
    if (b < AX_M_MAX_BUCKETS)
    {
      *(unsigned char**)ptr = buckets[b];
      buckets[b] = ptr;
    }
    // compare and free
    axMunmap(_ptr);
  }
}

// axRealloc
static void* axRealloc (void* _ptr,
  register const unsigned int size)
{
  //__builtin_printf("<< axRealloc\n");
  // case null pointer
  if (_ptr == NULL)a lot of what i call "circular dependencies/includes" is coming to light...

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

#define _axMalloc  axMalloc
#define _axCalloc  axCalloc
#define _axFree    axFree
#define _axRealloc axRealloc

#endif // !AX_NO_MALLOC

#include "axMallocDebug.h"

#endif // axMalloc_included
