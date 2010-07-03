#ifndef axInterfaceLinux_included
#define axInterfaceLinux_included
//----------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "core/axRect.h"
#include "gui/axWindow.h"

//void* threadProc(void* data);
//void* timerProc(void* data);
//void  eventProc(XEvent* ev);

class axInterface : public axInterfaceBase
{
  private:
    axPlatform* mPlatform;
    Display*    mDisplay;

  public:

    axInterface(axPlatform* aPlatform)
      {
        mPlatform = aPlatform;
        XInitThreads();
        mDisplay = XOpenDisplay(NULL);
      }

    virtual ~axInterface()
      {
        XCloseDisplay(mDisplay);
      }

    //----------

    virtual void* getHandle(void)
      {
        return (void*)mDisplay;
      }

    //----------

//    virtual void* createWindow(void* aParent, int aWidth, int aHeight)
//      {
//        trace("axInterfaceLinux.createWindow");
//        Window parent;
//        if (aParent==NULL) parent = XDefaultRootWindow(mDisplay);
//        else parent = (Window)/**(Window*)*/aParent;
//        trace("parent = " <<(int)parent);
//        //Window parent = (aParent==NULL) ? XDefaultRootWindow(mDisplay) : (Window)*(Window*)aParent;
//        axWindow* window;
////        window = new axWindow(this,&parent,axRect(0,0,aWidth,aHeight),AX_WIN_DEFAULT);
////        return window;
//        return NULL;
//      }

/*

axFormat -> axInterface -> axInterfaceLinux

axInterfaceLinux.createWindow doesn't know anout axWindow yet ???
(window need to know about axInterface...)

*/


};

//----------------------------------------------------------------------
#endif
