#ifndef axCanvas_included
#define axCanvas_included
//----------------------------------------------------------------------

// note: it loses its pens,brushes, etc, if you resize the window?
// resources are 'appended' to the canvas, and a new surface, and thus
// also the canvas if you resize the window
// either:
// set default set (or skin) in constructor
// or find a way to copy the resources in resizeBuffer

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
        //createBrush(AX_GREY);
        //createPen(AX_GREY_LIGHT);
        //createFont("default",AX_WHITE);
      }
    virtual ~axCanvas()
      {
      }

};

//----------------------------------------------------------------------
#endif
