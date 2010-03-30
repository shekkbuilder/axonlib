#ifndef axSurfaceLinux_included
#define axSurfaceLinux_included
//----------------------------------------------------------------------

#include "platform/axContext.h"
#include "axDefines.h"
#include "base/axSurfaceBase.h"

class axSurfaceLinux : public axSurfaceBase
{
  public:
    axSurfaceLinux(axContext* aContext, int aWidth, int aHeight)
    : axSurfaceBase(aContext,aWidth,aHeight)
      {
      }
};

typedef axSurfaceLinux axSurfaceImpl;

#endif
//----------------------------------------------------------------------

