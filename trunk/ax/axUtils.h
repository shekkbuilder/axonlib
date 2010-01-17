#ifndef axUtils_included
#define axUtils_included

//----------------------------------------------------------------------
// axBitReverse
// http://www.jjj.de/fxt/fxtpage.html
//----------------------------------------------------------------------
inline unsigned int axBitReverse(unsigned int x)
{
	__asm__ volatile
	(
		"movl %0, %%eax;"		"andl $0xaaaaaaaa, %%eax;"		"shrl $1, %%eax;"
		"andl $0x55555555, %0;"		"shll $1, %0;"		"orl %%eax, %0;"
		"movl %0, %%eax;"		"andl $0xcccccccc, %%eax;"		"shrl $2, %%eax;"
		"andl $0x33333333, %0;"		"shll $2, %0;"		"orl %%eax, %0;"
		"movl %0, %%eax;"		"andl $0xf0f0f0f0, %%eax;"		"shrl $4, %%eax;"
		"andl $0x0f0f0f0f, %0;"		"shll $4, %0;"		"orl %%eax, %0;"
		"movl %0, %%eax;"		"andl $0xff00ff00, %%eax;"		"shrl $8, %%eax;"
		"andl $0x00ff00ff, %0;"		"shll $8, %0;"		"orl %%eax, %0;"
		"movl %0, %%eax;"		"andl $0xffff0000, %%eax;"		"shrl $16, %%eax;"
		"andl $0x0000ffff, %0;"		"shll $16, %0;"		"orl %%eax, %0;"
		: "=m" (x)	:		: "eax"
	);
	return x;
}

//----------------------------------------------------------------------
// axRadix
//----------------------------------------------------------------------
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

#endif
