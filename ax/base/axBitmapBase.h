#ifndef axBitmapBase_included
#define axBitmapBase_included
//----------------------------------------------------------------------

#include "platform/axContext.h"
#include "base/axImage.h"

class axBitmapBase : public axImage
{
  public:
    axBitmapBase(axContext* aContext) {}
    virtual ~axBitmapBase() {}
};

//----------------------------------------------------------------------
#endif
