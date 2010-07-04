#ifndef axPlatform_included
#define axPlatform_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------

class axPlatformBase
{
  public:
    axPlatformBase(axFormat* aFormat) {}
    virtual ~axPlatformBase() {}
    virtual void* getHandle(void) { return NULL; } // win32:HINSTANCE
};

//----------

#ifdef AX_WIN32
#include "base/axPlatformWin32.h"
#endif

#ifdef AX_LINUX
#include "base/axPlatformLinux.h"
#endif

//----------

#define DEFAULT_PLATFORM axPlatform

//----------------------------------------------------------------------
#endif
