#ifndef axInterface_included
#define axInterface_included
//----------------------------------------------------------------------

// Interface flags
#define if_None         0
#define if_Buffered     1 // else Unbuffered
#define if_Embedded     2 // else Windowed
#define if_Transparent  4 // else Opaque
#define if_Unbuffered   0
#define if_Windowed     0
#define if_Opaque       0

// #define IF_VST ( if_Buffered | if_Embedded | if_Opaque )

#define DEFAULT_INTERFACE axInterface

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
