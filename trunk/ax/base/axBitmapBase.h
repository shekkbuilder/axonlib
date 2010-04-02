#ifndef axBitmapBase_included
#define axBitmapBase_included
//----------------------------------------------------------------------

/*

  user-mem (local) bitmap. let yu access the pixels/ints directly,
  and draw to a canvas with drawImage()

  note that the rgb format is a bit shaky, more work should be done here

  some functions to convert from our 'universal' format,
  to a format accepted by bitblt, XPutImage would be nice.
  or a format specifier variable/flag, so we could check purselves,
  and do what we want..
  considerations: 16/24/32-bit screens, rgba, bgra, rgb, ...

  and/or check capabilities for automatic conversion by the os/platform.
  i think bitblt (win32) is doing some of that, but i don't know about
  xlib's XPutImage/XCopyArea..

*/

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
