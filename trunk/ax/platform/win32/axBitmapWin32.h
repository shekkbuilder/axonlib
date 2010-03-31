#ifndef axBitmapWin32_included
#define axBitmapWin32_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "platform/axContext.h"
#include "base/axBitmapBase.h"

class axBitmapWin32 : public axBitmapBase
{
  public:

    axBitmapWin32(axContext* aContext)
    : axBitmapBase(aContext)
      {
      }

};

typedef axBitmapWin32 axBitmapImpl;


//----------------------------------------------------------------------
#endif
