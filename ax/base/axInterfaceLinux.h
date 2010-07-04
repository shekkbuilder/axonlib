#ifndef axInterfaceLinux_included
#define axInterfaceLinux_included
//----------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "core/axRect.h"
#include "gui/axWindow.h"

class axInterface : public axInterfaceBase
{
  private:
    axPlatform* mPlatform;
    Display*    mDisplay;
    Window      mRootWindow;

  public:

    axInterface(axPlatform* aPlatform)
    : axInterfaceBase(aPlatform)
      {
        trace("axInterfaceLinux" );
        mPlatform = aPlatform;
        XInitThreads();
        mDisplay = XOpenDisplay(NULL);
        mRootWindow = XDefaultRootWindow(mDisplay);
      }

    virtual ~axInterface()
      {
        XCloseDisplay(mDisplay);
      }

    //----------

    virtual void* getHandle(void)     { return (void*)mDisplay; }
    virtual void* getRootWindow(void) { return &mRootWindow; }

};

//----------------------------------------------------------------------
#endif
