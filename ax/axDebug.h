#ifndef axDebug_included
#define axDebug_included
//----------------------------------------------------------------------

#ifdef AX_DEBUG

  #include <stdio.h>
  #define TRACE printf

#else

  #define NDEBUG
  static void do_nothing(...) {}
  #define TRACE do_nothing

#endif

//----------

#include <assert.h>


//----------------------------------------------------------------------
#endif
