#ifndef axInterfaceLinux_included
#define axInterfaceLinux_included
//----------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xutil.h>

//#include "base/test_Window.h"

//----------------------------------------------------------------------

class axInterfaceLinux : public axInterface
{
  private:
    axBase*   mBase;
    Display*  mDisplay;
    Window    mRootWindow;

  public:

    axInterfaceLinux(axBase* aBase) : axInterface(aBase)
      {
        trace("axInterfaceLinux.constructor");
        mBase = aBase;
        XInitThreads();
        mDisplay = XOpenDisplay(NULL);
        mRootWindow = XDefaultRootWindow(mDisplay);
      }

    virtual ~axInterfaceLinux()
      {
        trace("axInterfaceLinux.destructor");
        XCloseDisplay(mDisplay);
      }

    //virtual void* createWindow(axInstance* aInstance,void* parent)
    //  {
    //    return new axWindow(mBase,aInstance,parent);
    //  }

};

//----------

typedef axInterfaceLinux AX_INTERFACE;

//----------------------------------------------------------------------
#endif
