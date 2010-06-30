#ifndef axInterfaceLinux_included
#define axInterfaceLinux_included
//----------------------------------------------------------------------
/*
  variations (flags, see axInterface.h)
  - embedded/windowed
  - buffered/unbuffered
  - alpha/noalpha
*/

#include "core/axRect.h"
#include "platform/axContext.h"
#include "gui/axWindow.h"

//void* threadProc(void* data);
//void* timerProc(void* data);
//void  eventProc(XEvent* ev);

class axInterface
{
  private:
    Display*  mDisplay;

  public:

    axInterface(axFormat* aFormat)
      {
        trace("axInterface linux");
        XInitThreads();
        mDisplay = XOpenDisplay(NULL);
      }
    virtual ~axInterface()
      {
        XCloseDisplay(mDisplay);
      }

    //----------

    //int getScreenWidth(void)  { return 0; }
    //int getScreenHeight(void) { return 0; }
    //int getScreenDepth(void)  { return 0; }

    //----------

    virtual axWindow* createWindow(void* aParent, int aWidth, int aHeight/*, int aFlags*/)
      {
        trace("axInterfaceLinux.createWindow");
        //int winflags = (if_Buffered|if_MsgDelete|if_MsgThread);
        axContext context;
        context.mDisplay = mDisplay; // all uses same connection, or one each?
        if (aParent==NULL) context.mWindow = XDefaultRootWindow(mDisplay);
        else context.mWindow = (Window)aParent;
        context.mAudio = NULL;
        axWindow* window = new axWindow(&context,axRect(0,0,aWidth,aHeight),AX_WIN_DEFAULT);
        return window;
        //return NULL;
      }

};

//----------------------------------------------------------------------
#endif
