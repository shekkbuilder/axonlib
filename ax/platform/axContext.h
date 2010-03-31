#ifndef axContext_included
#define axContext_included
//----------------------------------------------------------------------

#include "axDefines.h"
//#include "extern/vstsdk24/aeffect.h"
//#include "extern/vstsdk24/aeffectx.h"

//----------------------------------------------------------------------
#ifdef AX_LINUX

  #include <X11/Xlib.h>
  struct axContext
  {
    Display*    mDisplay;
    Window      mWindow;//mParent;
    AX_PTRCAST  mAudio;
    axContext() { mDisplay=NULL; mWindow=0; mAudio=0; }
  };

#endif
//----------------------------------------------------------------------
#ifdef AX_WIN32

  #include <windows.h>
  struct axContext
  {
    HINSTANCE   mInstance;
    HWND        mWindow;
    char*       mWinClassName;
    AX_PTRCAST  mAudio;
    axContext() { mInstance=0; mWindow=0; mWinClassName=0; mAudio=0; }
    axContext(int aWindow) { mInstance=0; mWindow=(HWND)aWindow; mWinClassName=0; mAudio=0; }
  };

#endif
//----------------------------------------------------------------------
#endif
