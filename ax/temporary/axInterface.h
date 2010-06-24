#ifndef axInterface_included
#define axInterface_included
//----------------------------------------------------------------------

// Interface flags
#define if_None         0
#define if_Unbuffered   0
#define if_Windowed     0
#define if_Opaque       0
#define if_Buffered     1
#define if_Embedded     2
#define if_Transparent  4

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

/*

class axInterface
{
  public:
    axInterface(axInstance* aInstance) {}
    virtual ~axInterface() {}
    int       getScreenWidth(void) { return 0; }
    int       getScreenHeight(void) { return 0; }
    int       getScreenDepth(void) { return 0; }
    axWindow* createWindow(int aWidth, int aHeight) { return NULL; }
    axWindow* createEmbeddedWindow(int aParent, int aWidth, int aHeight) { return NULL; }
};

*/
