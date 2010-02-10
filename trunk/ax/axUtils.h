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
 * @file axUtils.h
 * \brief contains various utility methods.
 */

#ifndef axUtils_included
#define axUtils_included

/**
 * swap the values of two variables <br>
 * @param[in] x type-unsafe 
 * @param[in] y type-unsafe
 * \code
 * unsigned int x = 5; // the type of the fist variable is used as a base 
 * int y = 2;
 * axSpaw(x, y); 
 * float xf = 5.f;
 * float yf = 3.f;
 * axSpaw(xf, yf); 
 * \endcode   
 */
#define axSwap(x,y) { typeof(x) tmp = (x);(x) = (y);(y) = (tmp); }

/**
 * returns the size of an array
 * @param[in] x array
 * \code 
 * char a[15];
 * int b[axGetArrSize(a) + 1];
 * unsigned int j = axGetArrSize(b);    // j = 16
 * // ----------------
 * // NOTE: passing a pointer will not work 
 * int ar[21];
 * int* ptr = ar;
 * unsigned int j = axGetArrSize(ptr); // <- will not work     
 * \endcode 
 */
template<class T, size_t N> T decay_array_to_subtype(T (&a)[N]);
#define axGetArrSize(x) (sizeof(x)/sizeof(decay_array_to_subtype(x)))

/**
 * fast bit reverse algorithm
 * origin: http://www.jjj.de/fxt/fxtpage.html
 * @param[in] x int
 * @return x int
 */
inline unsigned int axBitReverse(unsigned int x)
{
	__asm__ __volatile__ ("":::);
  __asm__
	(
		"movl %0, %%eax;"         "andl $0xaaaaaaaa, %%eax;"		"shrl $1, %%eax;"
		"andl $0x55555555, %0;"		"shll $1, %0;"		            "orl %%eax, %0;"
		"movl %0, %%eax;"		      "andl $0xcccccccc, %%eax;"		"shrl $2, %%eax;"
		"andl $0x33333333, %0;"		"shll $2, %0;"		            "orl %%eax, %0;"
		"movl %0, %%eax;"		      "andl $0xf0f0f0f0, %%eax;"		"shrl $4, %%eax;"
		"andl $0x0f0f0f0f, %0;"		"shll $4, %0;"		            "orl %%eax, %0;"
		"movl %0, %%eax;"		      "andl $0xff00ff00, %%eax;"		"shrl $8, %%eax;"
		"andl $0x00ff00ff, %0;"		"shll $8, %0;"		            "orl %%eax, %0;"
		"movl %0, %%eax;"		      "andl $0xffff0000, %%eax;"		"shrl $16, %%eax;"
		"andl $0x0000ffff, %0;"   "shll $16, %0;"		            "orl %%eax, %0;"
		: "=m" (x)	:		: "eax"
	);
	return x;
}

/**
 * radix algorithm
 * @param[in] source long*
 * @param[in] dest long*
 * @param[in] N long
 * @param[in] byte int
 */
inline void axRadix (long *source, long *dest, long N, int byte)
{
  int i;
  long count[256];
  long index[256];
  memset (count, 0, sizeof (count));
  for ( i=0; i<N; i++ ) count[((source[i])>>(byte*8))&0xff]++;
  index[0]=0;
  for ( i=1; i<256; i++ ) index[i]=index[i-1]+count[i-1];
  for ( i=0; i<N; i++ ) dest[index[((source[i])>>(byte*8))&0xff]++] = source[i];
}

//i = 1;
//loop( size-1,
//  cur = buf[i];
//  j = i;
//  buf[j-1] > cur ? (
//    while(
//      buf[j] = buf[j-1];
//      j -= 1;
//      ( (j>0) && (buf[j-1]>cur) )
//    );
//  );
//  i += 1;
//);

/*
TODO:
  axMemcpy, axMemsey
  axStrCpy,Strdup, etc...
  strcmp
  sprintf (doesn't need to be that advanced..)
  stdlib & std replacements
*/


//---------------------



#endif
