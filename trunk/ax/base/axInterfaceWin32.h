#ifndef axInterfaceWin32_included
#define axInterfaceWin32_included
//----------------------------------------------------------------------

//#include <windows.h>
//#include "base/test_Window.h"
//#include "core/axStdlib.h"

//----------------------------------------------------------------------

class axInterfaceWin32 : public axInterface
{
  private:
    axBase*   mBase;
    WNDCLASS  mWinClass;
    char      mWinClassName[256];

  public:

    axInterfaceWin32(axBase* aBase) : axInterface(aBase)
      {
        trace("axInterfaceWin32.constructor");
        mBase = aBase;
        axStrcpy( mWinClassName, "test" );                    // TODO
        axStrcat( mWinClassName, "_window" );
        //trace("mWinClassName: " << mWinClassName);
//        axMemset(&mWinClass,0,sizeof(mWinClass));
//        mWinClass.style          = CS_HREDRAW | CS_VREDRAW;
//        mWinClass.lpfnWndProc    = &eventProc;                    // !!!
//        mWinClass.hInstance      = mWinInstance;
//        mWinClass.lpszClassName  = mWinClassName;
//        //mWinClass.hCursor        = (HICON)mWinCursor;//LoadCursor(NULL, IDC_ARROW);
//        //HICON hIcon = LoadIcon(mWinInstance, "axicon");
//        //if (hIcon)mWinClass.hIcon = hIcon;
//        RegisterClass(&mWinClass);
      }

    virtual ~axInterfaceWin32()
      {
        trace("axInterfaceWin32.destructor");
//        UnregisterClass(mWinClassName,mWinInstance);

      }

    //virtual void* createWindow(axInstance* aInstance,void* parent)
    //  {
    //    return new axWindow(mBase,aInstance,parent);
    //  }

};

//----------

typedef axInterfaceWin32 AX_INTERFACE;

//----------------------------------------------------------------------
#endif
