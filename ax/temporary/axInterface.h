#ifndef axInterface_included
#define axInterface_included
//----------------------------------------------------------------------

//#define AX_WIN_EMBEDDED   1
//#define AX_WIN_BUFFERED   2
//#define AX_WIN_MSGTHREAD  4
//#define AX_WIN_MSGPROC    8
//#define AX_WIN_MSGDELETE  16

// Interface flags
//#define if_None         0
//#define if_Embedded     1   // else Windowed
//#define if_Buffered     2   // else Unbuffered
//#define if_MsgThread    4   // else MsgProc
//#define if_MsgProc      8   // else MsgProc
//#define if_MsgDelete    16
//#define if_Alpha        32   // else Opaque

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
