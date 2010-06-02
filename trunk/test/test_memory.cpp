//#define AX_NO_MALLOC
#define AX_DEBUG_AUTO_STD
#define AX_DEBUG_MEM
#define AX_DEBUG_NEW
#define AX_DEBUG_PNG
#define AX_DEBUG_LOG "test_memory.log"

#include "axConfig.h"
#include "axDefines.h"
#include "core/axMalloc.h"
#include "core/axDebug.h"
#include "core/axUtils.h"
#include "core/axAssert.h"
#include "core/axRand.h"

//----------------------------------------------------------------------

int main(void)
{
  axRand(666);
  int i;

  //----------

  int num_malloc = 0;
  void* bufm[1000];

  for (i=0; i<100; i++)
  {
    int n = axRandInt(999)+1;
    bufm[i] = axMalloc(n);
    num_malloc += n;
  }

  for (i=0; i<100; i++) axFree( bufm[i] );

  //----------

  int num_new = 0;
  char* bufn[1000];

  for (i=0; i<100; i++)
  {
    int n = axRandInt(999)+1;
    bufn[i] = new char[n];
    num_new += n;
  }

  for (i=0; i<100; i++) delete bufn[i];

}
