#ifndef axStdlib_included
#define axStdlib_included
//------------------------------------------------------------------------------

#include "axDefines.h"

/*
###########
there are untested methods bellow that may not work

*/

/*
  ------------------------------------------------------------------------------
  memory
  ------------------------------------------------------------------------------
*/

void  _axRealloc (void*, unsigned int)        __alloc_size1(2);
void* _axCalloc  (unsigned int, unsigned int) __alloc_size2(1, 2);

/**
 * axMemset
 */
template <class T>
inline T* axMemset(register T* dest, const unsigned int val, unsigned int len)
{
  while (len--)
    *dest++ = val;
  return dest;
}

/**
 * axMemcpy
 */
inline char* axMemcpy(register char* dest, const char* src, unsigned int len)
{
  while (len--)
    *dest++ = *src++;
  return dest;
}

/**
 * axMalloc
 */
template <class T>
inline T* axMalloc(const unsigned int _size)
{
  const T* _adr = new T [_size];
  return _adr;
}

/**
 * axRealloc
 */
template <class T>
inline T* axRealloc(T* dest, const unsigned int oldlen, const unsigned int newlen)
{
  register T* _dest = new T [newlen];
  unsigned int _len;
  if (oldlen < newlen)
    _len = oldlen;
  else
    _len = newlen;   
  for (unsigned int i = 0; i < _len; i++)
    _dest[i] = dest[i];
  delete[] dest;
  _dest = dest;
  return _dest;
}

/**
 * axCalloc
 */
template <class T>
inline T* axCalloc(const unsigned int _size)
{
  const T* _adr = new T [_size];
  return _adr;
}

/**
 * axFree
 */
template <class T>
inline void axFree(T _adr)
{
  delete[] _adr;
}

/*
  ------------------------------------------------------------------------------
  string
  ------------------------------------------------------------------------------
*/

/**
 * axStrcpy
 */
inline char* axStrcpy(register char* dest, register const char* src)
{
	register char* _str = dest;
  while (*dest++)
    *dest++ = *src++;
  return _str;
}

/**
 * axStrlen
 */
inline unsigned int axStrlen(register char* _str)
{
	register unsigned int _num = 0;
	while (*_str++)
    _num++;
	return _num;
}

/**
 * axStrchr
 */
inline char* axStrchr(const char* _str, unsigned int _idx)
{
  const unsigned char* str = (const unsigned char*) _str;
  unsigned char c = (unsigned int) _idx;
  while (*str && *str != c)
    str++;
  if (*str != c)
    str = NULL;
  return (char*) str;
}

/**
 * axStrrchr
 */
inline char* axStrrchr(const char* str, const unsigned int idx)
{
  const char* last = NULL;
  if (idx)
  {
    while (str)
    {
      str = axStrchr(str, idx);
      last = str;
      str++;
    }
  }
  else
    last = axStrchr(str, idx);
  return (char*) last;
}

//------------------------------------------------------------------------------
#endif
