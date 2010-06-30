#ifndef axInterface_included
#define axInterface_included
//----------------------------------------------------------------------

#define DEFAULT_INTERFACE axInterface

//----------

#ifdef AX_FORMAT_VST
  #define AX_WIN_DEFAULT (AX_WIN_BUFFERED|AX_WIN_MSGTHREAD|AX_WIN_EMBEDDED)
#endif

#ifdef AX_FORMAT_EXE
  #define AX_WIN_DEFAULT (AX_WIN_BUFFERED|AX_WIN_MSGDELETE)
#endif

//----------------------------------------------------------------------

#ifdef AX_NOGUI
#include "axInterfaceNone.h"
#else

  #ifdef AX_LINUX
  #include "axInterfaceLinux.h"
  #endif

  #ifdef AX_WIN32
  #include "axInterfaceWin32.h"
  #endif

#endif // !NOGUI

//----------------------------------------------------------------------
#endif
