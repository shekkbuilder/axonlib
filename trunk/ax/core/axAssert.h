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

// g++ -ansi -pedantic -W -Wall -Wextra -s -O3 test.cpp -o test.exe
// g++ -E -Wp,-dD,-dM test.cpp

/*
  ISO C/C++ compatible runtime and static assertion for GCC (4.4.x)
  no external dependencies.
*/

#ifndef axAssert_included
#define axAssert_included

// -----------------------------------------------------------------------------
// run time assertion
bool _axAssert(const bool e, const char* file, const unsigned int line)
#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
  __attribute__(( optimize(0) ))
#endif
  __attribute__(( noinline ));
bool _axAssert(const int e, const char* file, const unsigned int line,
  const char* e_str)
{
  __asm__ __volatile__("");  
  if (!e)
  { __builtin_printf("### axAssert: %s, %u, (%s)\n", file, line, e_str);
    __builtin_exit(0); }
  return 1;
}
#define axAssert(e) _axAssert((e), __FILE__, __LINE__, #e)
#define assert axAssert

// -----------------------------------------------------------------------------

// compile time assertion ( requires GCC 4.4.x )
#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
  bool __axSA(void)
    __attribute__(( error("### axStaticAssert") ))
    __attribute__(( optimize(0) ))
    __attribute__(( noinline ));
  #define axStaticAssert(e) ( (!(e)) ? __axSA(): 1 )
  #define static_assert axStaticAssert
#else
  #define axStaticAssert(e) (void(0))
  #warning "### axStaticAssert() requires GCC 4.4.x"  
#endif

#endif // axAssert_included
