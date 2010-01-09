#ifndef axDefines_included
#define axDefines_included
//----------------------------------------------------------------------

#define AX_MAGIC 0x41580000

#ifndef NULL
  #define NULL 0
#endif

//#ifndef PI
  #define PI    (3.1415926535f)
//#endif

#define DENORM	(10e-30)
#define PI2   	(PI*2.0f)
#define EPSILON (0.0001f)
#define inv360  (1.0f/360.0f)
#define inv16   (1.0f/16.0f)
#define inv127  (1.0f/127.0f)
#define inv256  (1.0f/256.0f)

//#define i8    signed char
//#define u8  unsigned char
//#define i16   signed short
//#define u16 unsigned short
//#define i32   signed int
//#define u32 unsigned int
//#define i64   signed long long
//#define u64 unsigned long long
//#define f32   float
//#define f64   double

#include <stdint.h>

//int8_t
//int16_t
//int32_t
//int64_t
//uint8_t
//uint16_t
//uint32_t
//uint64_t

#define SPL float
//#define SPL double


//----------------------------------------------------------------------
#endif

