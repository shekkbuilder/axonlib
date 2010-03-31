#ifndef axSurfaceWin32_included
#define axSurfaceWin32_included
//----------------------------------------------------------------------

#include "platform/axContext.h"
#include "axDefines.h"
#include "base/axSurfaceBase.h"

class axSurfaceWin32 : public axSurfaceBase
{
  public:
    axSurfaceWin32(axContext* aContext, int aWidth, int aHeight)
    : axSurfaceBase(aContext,aWidth,aHeight)
      {
      }
};

typedef axSurfaceWin32 axSurfaceImpl;


//----------------------------------------------------------------------
#endif

