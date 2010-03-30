#ifndef axCanvas_included
#define axCanvas_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "core/axPoint.h"
#include "core/axRect.h"
#include "base/axCanvasBase.h"

//----------

#ifdef AX_LINUX
  #include "platform/linux/axCanvasLinux.h"
#endif

#ifdef AX_WIN32
  #include "platform/win32/axCanvasWin32.h"
#endif

//----------------------------------------------------------------------

class axCanvas : public axCanvasImpl
{
  public:
    axCanvas(axContext* aContext)
    : axCanvasImpl(aContext)
      {
      }

};

//----------------------------------------------------------------------
#endif
