#ifndef axInterfaceLinux_included
#define axInterfaceLinux_included
//----------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "gui/axWindow.h"

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
        //trace("axInterfaceLinux.constructor");
        mBase = aBase;
        XInitThreads();
        mDisplay = XOpenDisplay(NULL);
        mRootWindow = XDefaultRootWindow(mDisplay);
        //trace("  mDisplay:    " << mDisplay);
        //trace("  mRootWindow: " << mRootWindow);
      }

    virtual ~axInterfaceLinux()
      {
        //trace("axInterfaceLinux.destructor");
        XCloseDisplay(mDisplay);
      }

    virtual void* getHandle(void)
      {
        return (void*)mDisplay;
      }

    virtual void* createWindow(void* aParent, axRect aRect, int aFlags)
      {
        //trace("axInterfaceLinux.createWindow");
        axWindow* window;
        if (aParent) window = new axWindow(mBase,aParent,aRect,aFlags);
        else window = new axWindow(mBase,&mRootWindow,aRect,aFlags);
        //trace("  window " << window);
        return (void*)window;
      }

};

//----------

typedef axInterfaceLinux AX_INTERFACE;

//----------------------------------------------------------------------
#endif
