#ifndef axInterfaceWin32_included
#define axInterfaceWin32_included
//----------------------------------------------------------------------
/*
  variations:
  - embedded/windowed
  - buffered/unbuffered
  - alpha/noalpha
*/

#include <windows.h>

#include "core/axRect.h"
#include "platform/axContext.h"
#include "gui/axWindow.h"

//  #define MAKESTRING2(s) #s
//  #define MAKESTRING(s) MAKESTRING2(s)
//  #define MAKE_NAME(name) MAKESTRING(name) "_window"
//
//  #define AX_CONTEXT_INIT(name)
//    char* winname  = (char*)MAKE_NAME(name);


class axInterface
{
  private:
    HINSTANCE mWinInstance;
    WNDCLASS  mWinClass;
    char*     mWinClassName;
  public:
    axInterface(axFormat* aFormat)
      {
        axPlatform* platform = aFormat->getPlatform();
        mWinInstance = platform->getHInstance();
        //register windows class
        //char* classname = mWinName.ptr();
        axMemset(&mWinClass,0,sizeof(mWinClass));
        mWinClass.style          = CS_HREDRAW | CS_VREDRAW;
        mWinClass.lpfnWndProc    = &eventProc;
        mWinClass.hInstance      = mWinInstance = platform->getHInstance();
        mWinClass.lpszClassName  = mWinClassName;
        //mWinClass.hCursor        = (HICON)mWinCursor; //LoadCursor(NULL, IDC_ARROW);
        //HICON hIcon = LoadIcon(mHInstance, "axicon");
        //if (hIcon)mWinClass.hIcon = hIcon;
        RegisterClass(&mWinClass);
      }
    virtual ~axInterface()
      {
        //UnregisterClass( mWndClassName, mHInstance);
      }

    int getScreenWidth(void) { return 0; }
    int getScreenHeight(void) { return 0; }
    int getScreenDepth(void) { return 0; }

    axWindow* createWindow(void* parent, int aWidth, int aHeight)
      {
//        trace("axInterfaceWin32.createWindow");
//        axContext context;
//        //context.mInstance     = aFormat->getPlatform()->getHInstance();
//        context.mWinClassName = (char*)"test";//aFormat->getWinClassName();
//        context.mWindow       = (HWND)parent;
//        axWindow* window  = new axWindow(&context,axRect(0,0,aWidth,aHeight),AX_WIN_DEFAULT);
//        return window;
        return NULL;
      }

};

//----------------------------------------------------------------------
#endif


