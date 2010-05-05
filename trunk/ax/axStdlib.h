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

//------------------------------------------------------------------------------

/*
  ==============================================================================
  axStdlib.h
    a collection of optimized methods for memory and string manipulation
    to replace the default ones in C.

    code in open source projects like MINGW, MINIX & DJGPP and used for
    refference.

    for more information remove the "ax" prefix from a method's name
    and use google to search.

  ==============================================================================
  NOTE:
    all methods are tested (mingw32-gcc-tdm-4.4.1) with the examples
    from the website "cplusplus.com" and should work as espected.
    not much error checking going on, so the methods should be used properly
    (by the book), with "low level" programming precautions in mind.

  ==============================================================================
  TODO:
  - add more eccentric str* methods if needed
  - deprecate the use of most (all) stdlib / string.h / memory.h methods across
    axonlib
  - test binary size impact and optimization level when including this header
  - more performance tests. check if methods in here are redundant in regard
    of speed. (slower than the ones in stdlib). so far all have been
    more efficient.
  - axFtoa(), axAtof() for "string <-> float" conversations
  ==============================================================================
*/

#ifndef axStdlib_included
#define axStdlib_included

#include "axDefines.h"
#include "core/axMalloc.h"

#ifdef AX_USE_HOT_INLINE
  #define __axstdlib_inline __hotinline
#else
  #define __axstdlib_inline inline
#endif

/*
  ------------------------------------------------------------------------------
  memory
  ------------------------------------------------------------------------------
*/

/**
 * axMemchr
 */
__axstdlib_inline void* axMemchr (register const void* src, int c,
  unsigned int len)
{
  register const unsigned char* _src = (const unsigned char*) src;
  c &= 0xff;
  while (len--)
  {
    if (*_src == c)
      return (char*) _src;
    _src++;
  }
  return NULL;
}

/**
 * axMemcmp
 */
__axstdlib_inline unsigned int axMemcmp (register const void* m1,
  register const void* m2, register unsigned int n)
{
  register unsigned char* s1 = (unsigned char*) m1;
  register unsigned char* s2 = (unsigned char*) m2;
  while (n--)
  {
    if (*s1 != *s2)
      return *s1 - *s2;
    s1++;
    s2++;
  }
  return 0;
}

/**
 * axMemcpy
 */
__axstdlib_inline void* axMemcpy (register void* dest, register int src,
  register unsigned int len)
{
  register char* _d = (char*) dest;
  register char* _s = (char*) src;
  while (len--)
    *_d++ = *_s++;
  return dest;
}

/**
 * axMemmove
 */
__axstdlib_inline void* axMemmove (register void* dst, register const void* src,
  unsigned int len)
{
  register char* _dst = (char*) dst;
  register const char* _src = (char*) src;
  if (_src < _dst && _dst < _src + len)
  {
    _src += len;
    _dst += len;
    while (len--)
      *--_dst = *--_src;
  }
  else
    while (len--)
      *_dst++ = *_src++;
  return dst;
}

/**
 * axMemset
 */
__axstdlib_inline void* axMemset (register void* dest, register int val,
  register unsigned int len)
{
  register char* _d = (char*) dest;
  while (len--)
    *_d++ = val;
  return dest;
}

/*
  ------------------------------------------------------------------------------
  string
  ------------------------------------------------------------------------------
*/

/**
 * axStrlen
 */
__axstdlib_inline unsigned int axStrlen (register char* str)
{
	register unsigned int num = 0;
	while (*str++)
    num++;
	return num;
}

/**
 * axStrcpy
 */
__axstdlib_inline char* axStrcpy (register char *dest, register const char* src)
{
  while ( (*dest++ = *src++) );
  return dest;
}

/**
 * axStrncpy
 */
__axstdlib_inline char* axStrncpy (register char* dest,
  register const char* src, unsigned int n)
{
  while ( n-- && (*dest++ = *src++) != '\0' )
    continue;
  return dest;
}

/**
 * axStrchr
 */
__axstdlib_inline char* axStrchr (register const char *str,
  register const int c)
{
  while (*str && *str != c)
    str++;
  if (*str != c)
    str = NULL;
  return (char*) str;
}

/**
 * axStrrchr
 */
__axstdlib_inline char* axStrrchr (register const char* s, const int c)
{
  char* p = NULL;
  while (*s++)
    if (*s == c)
      p = (char*) s;
  return p;
}

/**
 * axStrcmp
 */
__axstdlib_inline int axStrcmp (register const char* s1,
  register const char* s2)
{
  while (*s1 != '\0' && *s1 == *s2)
  { s1++; s2++; }
  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

/**
 * axStrncmp
 */
__axstdlib_inline int axStrncmp (register const char* s1,
  register const char* s2, register unsigned int n)
{
  register unsigned char u1, u2;
  while (n--)
  {
    u1 = (unsigned char) *s1++;
    u2 = (unsigned char) *s2++;
    if (u1 != u2)
      return u1 - u2;
    if (u1 == '\0')
      return 0;
  }
  return 0;
}

/**
 * axStrcat
 */
__axstdlib_inline const char* axStrcat (register char* s1, register char *s2)
{
  while (*s1)
    s1++;
  while ((*s1++ = *s2++));
  return s1;
}

/**
 * axStrncat
 */
__axstdlib_inline char* axStrncat (register char* s1, register const char* s2,
  register unsigned int n)
{
  register char* s = s1;
  while (*s1)
    s1++;
  while ( n-- && (*s1++ = *s2++) )
    if (n == 0)
      *s1 = '\0';
  return s;
}

/**
 * axStrstr
 */
__axstdlib_inline char* axStrstr (register const char* s1,
  register const char* s2)
{
  register unsigned int i;
  while (*s1++)
  {
    i = 0;
    while (1)
    {
      if (!s2[i])
        return (char*) s1;
      if (s2[i] != s1[i])
        break;
      i++;
    }
  }
  return (char*) NULL;
}

/**
 * axStrspn
 */
__axstdlib_inline unsigned int axStrspn(register const char* s1,
  register const char* s2)
{
  register const char *_s1, *_s2;
  _s1 = s1;
  while (*_s1)
  {
    _s2 = s2;
    while (*_s2 && *_s2 != *_s1)
      _s2++;
    if (*_s2 == '\0')
      break;
    _s1++;
  }
  return _s1 - s1;
}

/**
 * axStrcspn
 */
__axstdlib_inline unsigned int axStrcspn(register const char* s1,
  register const char* s2)
{
  register const char *_s1, *_s2;
  for (_s1 = s1; *_s1; _s1++)
  {
    for(_s2 = s2; *_s2 != *_s1 && *_s2; _s2++);
    if (*_s2)
      break;
  }
  return _s1 - s1;
}

/**
 * axStrpbrk
 */
__axstdlib_inline char* axStrpbrk(register const char* s1,
  register const char* s2)
{
  register const char* _s1;
  while (*s1++)
  {
    _s1 = s2;
    while (*_s1 && *_s1 != *s1)
      _s1++;
    if (*_s1)
      return (char*) s1;
  }
  return (char*) NULL;
}

/**
 * axStrtok
 */
__axstdlib_inline char* axStrtok(register char *str, const char *spr)
{
  register char *s1, *s2;
  // static buffer = kaboom ? but there is no recursion ...
  static char* _save;
  // --
  if (str == NULL)
  {
    str = _save;
    if (str == NULL)
      return (char*) NULL;
  }
  s1 = str + axStrspn(str, spr);
  if (*s1 == '\0')
    return (char*) (_save = NULL);
  s2 = axStrpbrk(s1, spr);
  if (s2 != NULL)
    *s2++ = '\0';
  _save = s2;
  return s1;
}

/*
  ------------------------------------------------------------------------------
  conversations
  ------------------------------------------------------------------------------
*/

/**
 * axItoa
 */
__axstdlib_inline char* axItoa(int n, register char* str,
  unsigned int base = 10)
{
  char tmp[33];
  char* _tmp = tmp;
  register int i;
  register unsigned int v;
  register char* sp;
  int s;
  if (base > 36 || base <= 1)
    return NULL;
  s = (base == 10 && n < 0);
  if (s)
    v = -n;
  else
    v = (unsigned) n;
  while (v || _tmp == tmp)
  {
    i = v % base;
    v = v / base;
    if (i < 10)
      *_tmp++ = i + '0';
    else
      *_tmp++ = i + 'a' - 10;
  }
  if (str == 0)
    str = (char*) axMalloc( (_tmp - tmp) + s + 1 );
  sp = str;
  if (s)
    *sp++ = '-';
  while (_tmp > tmp)
    *sp++ = *--_tmp;
  *sp = 0;
  return str;
}

/**
 * axAtoi
 */
__axstdlib_inline int axAtoi(register const char* s)
{
  static const char digits[] = "0123456789";
  register unsigned val = 0;
  register int neg = 0;
  while (*s == ' ' || *s == '\t')
    s++;
  if (*s == '-')
  {
    neg=1; s++;
  }
  else if (*s == '+')
    s++;
  while (*s)
  {
    const char *where;
    unsigned digit;
    where = axStrchr(digits, *s);
    if (where == NULL)
      break;
    digit = (where - digits);
    val = val*10 + digit;
    s++;
  }
  if (neg)
    return -val;
  return val;
}

//------------------------------------------------------------------------------
#endif
