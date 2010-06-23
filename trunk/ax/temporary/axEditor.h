#ifndef axEditor_included
#define axEditor_included
//----------------------------------------------------------------------

// editor flags
#define ef_None         0
#define ef_Unbuffered   0
#define ef_Windowed     0
#define ef_Opaque       0
#define ef_Buffered     1
#define ef_Embedded     2
#define ef_Transparent  4

//----------------------------------------------------------------------

#ifdef AX_NOGUI
#include "axEditorNone.h"
#else

  #ifdef AX_LINUX
  #include "axEditorLinux.h"
  #endif

  #ifdef AX_WIN32
  #include "axEditorWin32.h"
  #endif

#endif // !NOGUI

//----------------------------------------------------------------------
#endif


