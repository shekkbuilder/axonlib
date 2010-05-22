#ifndef axSurface_included
#define axSurface_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "platform/axContext.h"
#include "base/axSurfaceBase.h"

//----------------------------------------------------------------------

#ifdef AX_LINUX
  #include "platform/linux/axSurfaceLinux.h"
#endif

#ifdef AX_WIN32
  #include "platform/win32/axSurfaceWin32.h"
#endif

//----------------------------------------------------------------------

class axSurface : public axSurfaceImpl
{
  public:
    axSurface(axContext* aContext, int aWidth, int aHeight, int aDepth)
    : axSurfaceImpl(aContext,aWidth,aHeight,aDepth)
      {
      }
    //virtual ~axSurface() {}
};

//----------------------------------------------------------------------
#endif
