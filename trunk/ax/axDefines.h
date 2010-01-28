/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file axDefines.h
 * \brief a list of definitions
 */

#ifndef axDefines_included
#define axDefines_included
//----------------------------------------------------------------------

/**
 * \brief a list of definitions
 *
 */

// ascii 'AX', 0x4158
// AX_MAGIC+05 -> 41580005,
// hopefully a unique id
// (and it seems most people are using ascii for this)

#define AX_MAGIC 0x41580000

#ifndef NULL
  #define NULL 0
#endif

#define DENORM   10e-30f
#define LOG2DB   8.65617024533f
#define DB2LOG   0.11552453009f

#define E        2.71828182845f
#define LOG2E    1.44269504088f
#define LOG10E   0.43429448190f
#define LN2      0.69314718055f
#define LN10     2.30258509299f
#define PI       3.14159265358f
#define PI2      6.28318530717f
#define PI4      12.5663706143f
#define PI_2     1.57079632679f
#define PI_4     0.78539816339f
#define D1_PI    0.31830988618f
#define D2_PI    0.63661977236f
#define SQRT2    1.41421356237f
#define SQRT1_2  0.70710678118f

#define EPSILON  0.0001f
#define inv360   (1.0f/360.0f)
#define inv16    (1.0f/16.0f)
#define inv127   (1.0f/127.0f)
#define inv256   (1.0f/256.0f)

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

