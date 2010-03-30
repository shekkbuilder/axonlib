#ifndef axBitmapLinux_included
#define axBitmapLinux_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "platform/axContext.h"
#include "base/axBitmapBase.h"

class axBitmapLinux : public axBitmapBase
{
  public:

    axBitmapLinux(axContext* aContext)
    : axBitmapBase(aContext)
      {
      }

};

typedef axBitmapLinux axBitmapImpl;

#endif
//----------------------------------------------------------------------

