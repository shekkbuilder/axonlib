#ifndef axContext_included
#define axContext_included
//----------------------------------------------------------------------

#include "axDefines.h"
//#include "extern/vstsdk24/aeffect.h"
//#include "extern/vstsdk24/aeffectx.h"

//----------------------------------------------------------------------
#ifdef AX_LINUX

  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  //#include <X11/cursorfont.h>
  //#include <pthread.h>


  struct axContext
  {
    Display*    mDisplay;
    Window      mWindow;//mParent;
    AX_PTRCAST  mAudio;

    axContext() {}
    axContext(Display* aDisplay, Window aWindow, AX_PTRCAST aAudio=0)
      {
        mDisplay = aDisplay;
        mWindow  = aWindow;
        mAudio   = aAudio;
      }

  };

#endif
//----------------------------------------------------------------------
#ifdef AX_WIN32

  //#undef _WIN32_WINNT
  #define WINVER 0x0501 // AlphaBlend
  #define _WIN32_WINNT 0x0501
  #include <windows.h>
  struct axContext
  {
    HINSTANCE   mInstance;
    HWND        mWindow;
    char*       mWinClassName;
    AX_PTRCAST  mAudio;

    axContext() {}
    axContext(HINSTANCE aInstance, char* aName, AX_PTRCAST aAudio=0)
      {
        mInstance     = aInstance;
        mWindow       = 0;
        mWinClassName = aName;
        mAudio        = aAudio;
      }
    axContext(HWND aWindow)
      {
        mInstance     = 0;
        mWindow       = (HWND)aWindow;
        mWinClassName = 0;
        mAudio        = 0;
        }
    //axContext(HWND win) { mInstance=0; mWindow=win; mWinClassName=0; mAudio=0; }
  };

#endif
//----------------------------------------------------------------------
#endif
