
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "base/test_Window.h"

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
        trace("- axInterfaceLinux.constructor");
        mBase = aBase;
        XInitThreads();
        mDisplay = XOpenDisplay(NULL);
        mRootWindow = XDefaultRootWindow(mDisplay);
      }

    virtual ~axInterfaceLinux()
      {
        XCloseDisplay(mDisplay);
      }

    virtual void* createWindow(axInstance* aInstance,void* parent)
      {
        return new axWindow(mBase,aInstance,parent);
      }

};

//----------

typedef axInterfaceLinux AX_INTERFACE;

//----------------------------------------------------------------------























//#ifndef axInterfaceLinux_included
//#define axInterfaceLinux_included
////----------------------------------------------------------------------
//
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//
////#include "core/axRect.h"
//
//class axInterface : public axInterfaceBase
//{
//  private:
//    axPlatform* mPlatform;
//    Display*    mDisplay;
//    Window      mRootWindow;
//
//  public:
//
//    axInterface(axPlatform* aPlatform)
//    : axInterfaceBase(aPlatform)
//      {
//        trace("axInterfaceLinux.constructor");
//        mPlatform = aPlatform;
//        XInitThreads();
//        mDisplay = XOpenDisplay(NULL);
//        mRootWindow = XDefaultRootWindow(mDisplay);
//      }
//
//    virtual ~axInterface()
//      {
//        XCloseDisplay(mDisplay);
//      }
//
//    //----------
//
//    virtual void* getHandle(void)     { return (void*)mDisplay; }
//    virtual void* getRootWindow(void) { return &mRootWindow; }
//
//    //----------
//
//};
//
////----------------------------------------------------------------------
//#endif
