#ifndef axPlatform_included
#define axPlatform_included
//----------------------------------------------------------------------
/*

  this class is created as the first thing in the AX_ENTRYPOINT.
  the constructor is only called once, when the dll/so is initially loaded
  by a host, and the destructor called (once) when the platform variable
  (in AX_ENTRYPOINT) is deleted (or goes out of scope, if it's static),
  when the dll/so is unloaded.
  we can do one-time platform specific setup things here.

*/

//// platform types
//#define pt_Linux  0
//#define pt_Win32  1

#define DEFAULT_PLATFORM axPlatform

//----------------------------------------------------------------------

class axPlatformBase
{
  public:
    //axPlatformBase(axFormat* aFormat) {}
    //virtual ~axPlatformBase() {}
    virtual void* getHandle(void) { return NULL; } // win32:HINSTANCE
};

//----------


#ifdef AX_WIN32
#include "base/axPlatformWin32.h"
#endif

#ifdef AX_LINUX
#include "base/axPlatformLinux.h"
#endif

//typedef axPlatformBase axPlatform;

//----------------------------------------------------------------------
#endif
