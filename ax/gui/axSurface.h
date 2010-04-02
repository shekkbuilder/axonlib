#ifndef axSurface_included
#define axSurface_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "platform/axContext.h"
#include "base/axSurfaceBase.h"

//----------

#ifdef AX_LINUX
  #include "platform/linux/axSurfaceLinux.h"
#endif

#ifdef AX_WIN32
  #include "platform/win32/axSurfaceWin32.h"
#endif

//----------------------------------------------------------------------

class axSurface : public axSurfaceImpl
{
  //friend class axWindowLinux;
  public:
    axSurface(axContext* aContext, int aWidth, int aHeight)
    : axSurfaceImpl(aContext,aWidth,aHeight)
      {
      }
//    inline int getWidth(void) { return mWidth; }
//    inline int getHeight(void) { return mHeight; }
//    inline int getDepth(void) { return mDepth; }
    //inline axCanvas* getCanvas(void) { return mCanvas; }
};

//----------------------------------------------------------------------
#endif
