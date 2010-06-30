#ifndef axInterfaceWin32_included
#define axInterfaceWin32_included
//----------------------------------------------------------------------
/*
  variations:
  - embedded/windowed
  - buffered/unbuffered
  - alpha/noalpha
*/

class axInterface
{
  public:
    axInterface(axFormat* aFormat)
      {
        //register windows class
      }
    virtual ~axInterface()
      {
      }

    int getScreenWidth(void) { return 0; }
    int getScreenHeight(void) { return 0; }
    int getScreenDepth(void) { return 0; }

    axWindow* createWindow(void* parent, int aWidth, int aHeight)
      {
        aFlags |= (if_Buffered|if_MsgDelete|if_MsgThread);
        axContext context;
        context.mInstance     = mFormat->getInstance();
        context.mWinClassName = mFormat->getWinClassName();
        context.mWindow       = (HWND)parent;
        axWindow* window  = new axWindow(&context,axRect(0,0,aWidth,aHeight),aFlags);
        return window;
        //return NULL;
      }


};

//----------------------------------------------------------------------
#endif


