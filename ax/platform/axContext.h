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
    AX_AUDIOPTR mAudio;
  };

#endif
//----------------------------------------------------------------------
#ifdef AX_WIN32

  #include <windows.h>
  struct axContext
  {
    //char* mWindowClassName;
  };

#endif
//----------------------------------------------------------------------
#endif
