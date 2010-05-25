#ifndef axDefines_included
#define axDefines_included

/*
TODO:
  - write correct types and limits for __AX64__
  - clean attributes and definitions that give problems
*/

// architechture
// -----------------------------------------------------------------------------
#if defined  (__i386__) || defined (_X86_) || defined (__X86__)
  #define __AX32__
#endif

#if defined (__x86_64) || (__LP64__)  || defined (_WIN64)
  #define __AX64__
#endif

// os
// -----------------------------------------------------------------------------
#ifdef linux
  #define AX_LINUX
#endif

#ifdef WIN32
  #define AX_WIN32
#endif

// ax magic
// -----------------------------------------------------------------------------
// ascii 'AX', 0x4158
// AX_MAGIC+05 -> 41580005,
// hopefully a unique id
// (and it seems most people are using ascii for this)
#define AX_MAGIC 0x41580000
//
// 0x40    0x48    0x50    0x58
// |       |       |       |
// @ABCDEFGHIJKLMNOPQRSTUVWXYZ
//

// constants
// -----------------------------------------------------------------------------
#define DENORM      1.4012984e-45F
#define LOG2DB      8.65617024533F
#define DB2LOG      0.11552453009F
#define E           2.71828182845F
#define LOG2E       1.44269504088F
#define LOG10E      0.43429448190F
#define LN2         0.69314718055F
#define LN10        2.30258509299F
#define PI          3.14159265358F
#define PI2         6.28318530717F
#define PI4         12.5663706143F
#define PI_2        1.57079632679F
#define PI_4        0.78539816339F
#define D1_PI       0.31830988618F
#define D2_PI       0.63661977236F
#define SQRT2       1.41421356237F
#define SQRT1_2     0.70710678118F
#define EPSILON     1.19209290e-7F
#define inv360      0.00277777778F
#define inv16       0.06250000000F
#define inv127      0.00787401575F
#define inv256      0.00390625000F

// internal data types and classes
// -----------------------------------------------------------------------------
#define SPL         float
#define CSPL        const float
#define PTR         char*
#define CPTR        const char*
#define STR         axString
#define REC         axRect
#define COL         axColor
#define AX_PTRCAST  long

// limits
// -----------------------------------------------------------------------------
#define AX_CHAR_BIT       8
#define AX_CHAR_MAX       127
#define AX_SHRT_MAX       32767
#define AX_WCHAR_MAX      65535U

#ifdef __AX32__
  #define AX_INT_MAX        2147483647
  #define AX_LONG_MAX       2147483647L
  #define AX_LONG_LONG_MAX  9223372036854775807LL
  #define AX_INTMAX_MAX     9223372036854775807LL
#endif

#define AX_FLT_MAX        3.40282347e+38F
#define AX_FLT_MIN        1.17549435e-38F
#define AX_FLT_EPSILON    1.19209290e-7F
#define AX_FLT_DENORM     1.40129846e-45F
#define AX_DBL_MAX        1.7976931348623157e+308
#define AX_DBL_MIN        2.2250738585072014e-308
#define AX_DBL_EPSILON    2.2204460492503131e-16
#define AX_DBL_DENORM     4.9406564584124654e-324
#define AX_LDBL_MAX       1.18973149535723176502e+4932L
#define AX_LDBL_MIN       3.36210314311209350626e-4932L
#define AX_LDBL_EPSILON   1.08420217248550443401e-19L
#define AX_LDBL_DENORM    3.64519953188247460253e-4951L

/*
  info on size vs arch:
  http://www.safercode.com/blog/2009/03/10/portable-code-how-to-check-if-a-machine-is-32-bit-or-64-bit.html
*/
// custom data types. TODO: either write these correctly or remove
// -----------------------------------------------------------------------------
#ifdef AX_USE_DATA_TYPES
    #define i8                    char
    #define ci8   const           char
    #define u8          unsigned  char
    #define cu8   const unsigned  char
    #define i16                   short
    #define ci16  const           short
    #define u16         unsigned  short
    #define cu16  const unsigned  short

    #define f32                   float
    #define cf32  const           float
    #define f64                   double
    #define cf64  const           double
    #define lf64                  long double
    #define clf64 const           long double

  #ifdef (__AX32__)
    #define i32                   int
    #define ci32  const           int
    #define u32         unsigned  int
    #define cu32  const unsigned  int
    #define lg                    long
    #define clg   const           long
    #define ulg         unsigned  long
    #define culg  const unsigned  long
    #define i64                   long long
    #define ci64  const           long long
    #define u64         unsigned  long long
    #define cu64  const unsigned  long long
  #endif

  #ifdef (__AX64__)
    //---
  #endif
#endif

// architecture 'safe' aliases
// -----------------------------------------------------------------------------

#define cchar       const     char
#define uchar       unsigned  char
#define cuchar      const     unsigned  char

#define cshort      const     short
#ifndef AX_LINUX
  #define ushort    unsigned  short
#endif
#define cushort     const     unsigned  short

#define cint        const     int
#ifndef AX_LINUX
  #define uint      unsigned  int
#endif
#define cuint       const     unsigned  int

#define clong       const     long
#ifndef AX_LINUX
  #define ulong     unsigned  long
#endif
#define culong      const     unsigned  long

#define longlong              long      long
#define clonglong   const     long      long
#define ulonglong   unsigned  long      long
#define culonglong  const     unsigned  long long

#define cfloat      const     float
#define cdouble     const     double
#define ldouble     long      double
#define cldouble    const     long      double

// c defaults
// -----------------------------------------------------------------------------
#ifndef   NULL
  #define NULL 0
#endif
#ifndef   CHAR_BIT
  #define CHAR_BIT 8
#endif
#ifndef   TRUE
  #define TRUE 1
#endif
#ifndef   true
  #define true 1
#endif
#ifndef   FALSE
  #define FALSE 0
#endif
#ifndef   false
  #define false 0
#endif

// attributes
// -----------------------------------------------------------------------------
#define __aligned(x)              __attribute__ ((aligned (x)))
#define __alignedmax              __attribute__ ((aligned))
#define __packed                  __attribute__ ((packed))
#define ___shared                 __attribute__ ((shared))
#define __section(name)           __attribute__ ((section (name)))
#define __used                    __attribute__ ((used))
#define __unused                  __attribute__ ((unused))
#define __vector(x)               __attribute__ ((vector_size (x)))
#define __selectany               __attribute__ ((selectany))
#define __transparent_union       __attribute__ ((transparent_union))
#define __may_alias               __attribute__ ((may_alias))
#define __deprecated              __attribute__ ((deprecated))

#define __constructor             __attribute__ ((constructor))
#define __destructor              __attribute__ ((destructor))
#define __alloc_size1(x)          __attribute__ ((alloc_size(x)))
#define __alloc_size2(x, y)       __attribute__ ((alloc_size(x, y)))
#define __alias(name)             __attribute__ ((alias (name)))
#define ___always_inline   inline __attribute__ ((always_inline))
#define __malloc                  __attribute__ ((malloc))
#define __noinline                __attribute__ ((noinline))
#define __noreturn                __attribute__ ((noreturn))
#define __pure                    __attribute__ ((pure))
#define __target(value)           __attribute__ ((target (value)))
#define __weak                    __attribute__ ((weak))
#define __weakref(name)           __attribute__ ((weakref (name)))

// newer version of GCC required
#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
  #define __optimize(level)         __attribute__ ((optimize (level)))
  #define __hot                     __attribute__ ((hot))
  #define __cold                    __attribute__ ((cold))
  #define __error(message)          __attribute__ ((error(message)))
  #define __warning(message)        __attribute__ ((warning(message)))
#else  
  #define __optimize(level)
  #define __hot
  #define __cold
  #define __error
  #define __warning
  // disable TLS & show warning
  #undef __thread
  #define __thread
  #warning "### Thread-local storage requires GCC 4.4.x"
#endif

// aliasing types
typedef ldouble   __may_alias ldouble_a;
typedef double    __may_alias double_a;
typedef float     __may_alias float_a;
typedef longlong  __may_alias longlong_a;
typedef ulonglong __may_alias ulonglong_a;
typedef long      __may_alias long_a;
typedef ulong     __may_alias ulong_a;
typedef int       __may_alias int_a;
typedef uint      __may_alias uint_a;
typedef short     __may_alias short_a;
typedef ushort    __may_alias ushort_a;
typedef char      __may_alias char_a;
typedef uchar     __may_alias uchar_a;

// msvc specific
#ifndef __cdecl
  #define __cdecl     __attribute__ ((cdecl))
#endif
#ifndef __stdcall
  #define __stdcall   __attribute__ ((stdcall)))
#endif

// other
#define __hotinline               ___always_inline __hot __optimize(3)
#define __asmv                    __asm__ __volatile__

#endif // axDefines_included
