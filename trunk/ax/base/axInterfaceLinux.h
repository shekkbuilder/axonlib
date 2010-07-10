#ifndef axInterfaceLinux_included
#define axInterfaceLinux_included
//----------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xutil.h>

//#include "gui/axWindow.h"
#include "gui/axEditor.h"

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
        mDisplay = XOpenDisplay(NULL);              // må denne være unik for hver instance? i så fall, fløuytt til createWindow
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

    virtual void* createEditor(void* aParent, axRect aRect, int aFlags)
      {
        axEditor* editor;
        void* parent = aParent;
        if (aParent) editor = new axEditor(mBase,aParent,aRect,aFlags);
        else editor = new axEditor(mBase,&mRootWindow,aRect,aFlags);
        //trace("  window " << window);
        return (void*)editor;
      }

};

//----------

typedef axInterfaceLinux AX_INTERFACE;

//----------------------------------------------------------------------
#endif
