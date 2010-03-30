#ifndef axBitmap_included
#define axBitmap_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "platform/axContext.h"
#include "base/axBitmapBase.h"

//----------

#ifdef AX_LINUX
  #include "platform/linux/axBitmapLinux.h"
#endif

#ifdef AX_WIN32
  #include "platform/win32/axBitmapWin32.h"
#endif

//----------------------------------------------------------------------

class axBitmap : public axBitmapImpl
{
  public:
    axBitmap(axContext* aContext=NULL)
    : axBitmapImpl(aContext)
      {
        if (!aContext)
        {
          // memory buffer
        }
        else
        {
          //XImage/HBITMAP
        }
      }
};

//----------------------------------------------------------------------
#endif
