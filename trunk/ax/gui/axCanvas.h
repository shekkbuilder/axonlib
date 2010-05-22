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
  //#include "platform/linux/axCanvasLinux.h"
  #include "gui/linux/axCanvasLinux.h"
#endif

#ifdef AX_WIN32
  //#include "platform/win32/axCanvasWin32.h"
  #include "gui/win32/axCanvasWin32.h"
#endif

//----------------------------------------------------------------------

class axCanvas : public axCanvasImpl
{
  public:
    axCanvas(axContext* aContext)
    : axCanvasImpl(aContext)
      {
      }
    virtual ~axCanvas()
      {
      }

};

//----------------------------------------------------------------------
#endif

/*
  some notes about bitmaps & blitting:

  win32:
  http://en.wikipedia.org/wiki/Graphics_Device_Interface
  "In Windows Vista... GDI is no longer hardware-accelerated by the video card driver."
  "Windows 7 includes GDI hardware acceleration for blitting operations"
  "GDI+ will continue to rely on software rendering in Windows 7"

  linux:
  http://en.wikipedia.org/wiki/Xrender
  "..designed to target the 3D graphics capabilities of newer video cards."
*/
