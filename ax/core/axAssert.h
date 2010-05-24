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
  ISO C/C++ compatible runtime and static assertion for GCC (4.4.x)
  no external dependencies.
*/

#ifndef axAssert_included
#define axAssert_included

#ifdef AX_DEBUG

#include "core/axDebug.h"

/*
  axAssert()
  runtime assertion
*/
unsigned int _axAssert () __attribute__ (( noinline ));
unsigned int _axAssert (const unsigned int e, const char* file,
  const unsigned int line, const char* e_str)
{
  if (!e)
  { _trace("### axAssert: " << file << ", " << line << ", (" << e_str << ")");
    __builtin_exit(0); }
  return 1;
}
#define axAssert(e) _axAssert((e), __FILE__, __LINE__, #e)

/*
  axStaticAssert()
  compile time assertion ( requires GCC 4.4.x )
*/
#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
  unsigned int __axSA (void)
    __attribute__ (( error("### axStaticAssert") ))
    __attribute__ (( optimize(0) ));
  #define axStaticAssert(e) ( (!(e)) ? __axSA(): 1 )
#else
  /* for c89 compat mainly if header is used elsewhere */
  #ifdef __GNUC_STDC_INLINE__
    #define _AXSA_INLINE inline
  #else
    #define _AXSA_INLINE
  #endif
  _AXSA_INLINE const unsigned int __axSA (void) { return 0; }  
  /* prevent some warnings */
  #define axStaticAssert(e) __axSA()
  #warning "### axStaticAssert() requires GCC 4.4.x"
#endif

#endif /* AX_DEBUG */

#endif /* axAssert_included */
