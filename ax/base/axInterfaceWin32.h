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
#include "base/axContext.h"
#include "gui/axWindow.h"

//
//  #define AX_CONTEXT_INIT(name)
//    char* winname  = (char*)MAKE_NAME(name);

class axInterface
{
  private:
    axFormat* mFormat;
    HINSTANCE mWinInstance;
    WNDCLASS  mWinClass;
    char      mWinClassName[256];

  public:
    axInterface(axFormat* aFormat)
      {
        wtrace("axInterfaceWin32");
        mFormat = aFormat;
        axPlatform* platform = aFormat->getPlatform();
        mWinInstance = platform->getWinInstance();
        //char* classname = mWinName.ptr();
        axStrcpy( mWinClassName, mFormat->getName() );
        axStrcat( mWinClassName, "_window" );
        wtrace("mWinClassName: " << mWinClassName);
        axMemset(&mWinClass,0,sizeof(mWinClass));
        mWinClass.style          = CS_HREDRAW | CS_VREDRAW;
        mWinClass.lpfnWndProc    = &eventProc;                    // !!!
        mWinClass.hInstance      = mWinInstance;
        mWinClass.lpszClassName  = mWinClassName;
        //mWinClass.hCursor        = (HICON)mWinCursor;//LoadCursor(NULL, IDC_ARROW);
        //HICON hIcon = LoadIcon(mWinInstance, "axicon");
        //if (hIcon)mWinClass.hIcon = hIcon;
        RegisterClass(&mWinClass);
      }

    virtual ~axInterface()
      {
        UnregisterClass(mWinClassName,mWinInstance);
      }

    //----------------------------------------------------------------------

    //int getScreenWidth(void) { return 0; }
    //int getScreenHeight(void) { return 0; }
    //int getScreenDepth(void) { return 0; }

    axWindow* createWindow(void* parent, int aWidth, int aHeight)
      {
        trace("axInterfaceWin32");
        //axContext context;
        //context.mInstance     = mWinInstance;
        //context.mWinClassName = mWinClassName;;
        //context.mWindow       = (HWND)parent;
        axWindow* window      = new axWindow(&context,axRect(0,0,aWidth,aHeight),AX_WIN_DEFAULT);
        return window;
        return NULL;
      }

};

//----------------------------------------------------------------------
#endif


