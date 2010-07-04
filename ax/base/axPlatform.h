#ifndef axPlatform_included
#define axPlatform_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------

class axPlatformBase
{
  private:
    axFormatBase* mFormat;
  public:
    axPlatformBase(axFormatBase* aFormat) { mFormat=aFormat; }
    virtual ~axPlatformBase() {}
    virtual axFormatBase* getFormat(void) { return mFormat; }
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
